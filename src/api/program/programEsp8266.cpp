/**
 * @file programEsp8266.cpp
 * @author Cyprien Ménard
 * @date 2025-08-18
 */

#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "program.hpp"
#include <stdio.h>
#include <sys/time.h>

static const char *TAG = "Program";
static const char *PROGRAM_FILE_PATH = "/spiffs/program.bin";
static const char *NVS_NAMESPACE = "ota_protection";
static const char *BOOT_COUNT_KEY = "boot_count";
static const char *LAST_BOOT_TIME_KEY = "last_boot";

static esp_ota_handle_t s_update_handle = 0;
static const esp_partition_t *s_update_partition = nullptr;
static bool s_ota_in_progress = false;

size_t Program::m_totalSize = 0;
size_t Program::m_currentSize = 0;

void Program::getProgramBinary(Binary *binary)
{
    if (!binary)
    {
        ESP_LOGE(TAG, "Binary pointer is null");
        return;
    }

    FILE *file = fopen(PROGRAM_FILE_PATH, "rb");
    if (!file)
    {
        ESP_LOGE(TAG, "Failed to open program file for reading");
        return;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0)
    {
        ESP_LOGE(TAG, "Invalid file size: %ld", file_size);
        fclose(file);
        return;
    }

    // Allocate memory for binary data
    binary->alloc(file_size);

    // Read file content
    size_t bytes_read = fread(binary->data(), 1, file_size, file);
    fclose(file);

    if (bytes_read != file_size)
    {
        ESP_LOGE(TAG,
                 "Failed to read complete file. Expected: %ld, Read: %d",
                 file_size,
                 bytes_read);
        // Clear the binary by creating a new empty one
        *binary = Binary();
        return;
    }

    ESP_LOGI(TAG,
             "Program binary loaded successfully, size: %d bytes",
             binary->size());
}

bool Program::beginProgramUpdate(size_t total_size)
{
    if (s_ota_in_progress)
    {
        ESP_LOGE(TAG, "OTA update already in progress");
        return false;
    }

    if (total_size == 0)
    {
        ESP_LOGE(TAG, "Total size cannot be zero");
        return false;
    }

    ESP_LOGI(TAG, "Beginning OTA update with total size: %d bytes", total_size);

    s_update_partition = esp_ota_get_next_update_partition(NULL);
    if (!s_update_partition)
    {
        ESP_LOGE(TAG, "Failed to get OTA update partition");
        return false;
    }

    ESP_LOGI(TAG,
             "Writing to partition subtype %d at offset 0x%x",
             s_update_partition->subtype,
             s_update_partition->address);

    esp_err_t err
        = esp_ota_begin(s_update_partition, total_size, &s_update_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ota_begin failed: %s", esp_err_to_name(err));
        s_update_partition = nullptr;
        s_update_handle = 0;
        return false;
    }

    m_totalSize = total_size;

    s_ota_in_progress = true;
    ESP_LOGI(TAG, "OTA update initialized successfully");
    return true;
}

bool Program::updateProgramBinary(const Binary &binary)
{
    if (!s_ota_in_progress)
    {
        ESP_LOGE(TAG,
                 "No OTA update in progress. Call beginProgramUpdate() first");
        return false;
    }

    if (binary.size() == 0)
    {
        ESP_LOGE(TAG, "Binary chunk is empty");
        return false;
    }

    esp_err_t err
        = esp_ota_write(s_update_handle, binary.data(), binary.size());
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ota_write failed: %s", esp_err_to_name(err));
        // Clean up on error
        esp_ota_end(s_update_handle);
        s_ota_in_progress = false;
        s_update_handle = 0;
        s_update_partition = nullptr;
        return false;
    }

    ESP_LOGI(TAG, "Written %d bytes to OTA partition", binary.size());

    m_currentSize += binary.size();
    return true;
}

bool Program::finalizeProgramUpdate()
{
    if (!s_ota_in_progress)
    {
        ESP_LOGE(TAG, "No OTA update in progress");
        return false;
    }

    esp_err_t err = esp_ota_end(s_update_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ota_end failed: %s", esp_err_to_name(err));
        s_ota_in_progress = false;
        s_update_handle = 0;
        s_update_partition = nullptr;
        return false;
    }

    // Note: esp_image_verify is not available in ESP8266 RTOS SDK v3.4
    // Basic validation is done by esp_ota_end()

    err = esp_ota_set_boot_partition(s_update_partition);
    if (err != ESP_OK)
    {
        ESP_LOGE(
            TAG, "esp_ota_set_boot_partition failed: %s", esp_err_to_name(err));
        s_ota_in_progress = false;
        s_update_handle = 0;
        s_update_partition = nullptr;
        return false;
    }

    s_ota_in_progress = false;
    s_update_handle = 0;
    s_update_partition = nullptr;

    ESP_LOGI(TAG,
             "OTA update completed successfully. New firmware will be "
             "validated on next boot.");
    ESP_LOGI(TAG, "Restarting...");
    esp_restart();

    return true; // This line won't be reached due to restart
}

void Program::initAutomaticRollback(uint8_t max_boot_attempts)
{
    ESP_LOGI(TAG,
             "Initializing automatic rollback protection (max attempts: %d)",
             max_boot_attempts);

    // Initialize NVS if not already done
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES
        || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    checkAutomaticRollback();

    // Get current boot count
    nvs_handle_t nvs_handle;
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to open NVS handle: %s", esp_err_to_name(err));
        return;
    }

    uint8_t boot_count = 0;
    size_t required_size = sizeof(boot_count);
    err = nvs_get_blob(nvs_handle, BOOT_COUNT_KEY, &boot_count, &required_size);
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        boot_count = 0;
    }
    else if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read boot count: %s", esp_err_to_name(err));
        nvs_close(nvs_handle);
        return;
    }

    // Increment boot count
    boot_count++;
    ESP_LOGI(TAG, "Boot attempt #%d", boot_count);

    // Save updated boot count and current time (using gettimeofday for ESP8266)
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t current_time
        = tv.tv_sec * 1000 + tv.tv_usec / 1000; // Convert to ms
    nvs_set_blob(nvs_handle, BOOT_COUNT_KEY, &boot_count, sizeof(boot_count));
    nvs_set_blob(
        nvs_handle, LAST_BOOT_TIME_KEY, &current_time, sizeof(current_time));
    nvs_commit(nvs_handle);
    nvs_close(nvs_handle);

    // Check if we exceeded max attempts
    if (boot_count >= max_boot_attempts)
    {
        ESP_LOGE(TAG,
                 "Maximum boot attempts (%d) reached. Initiating automatic "
                 "rollback...",
                 max_boot_attempts);
        rollbackToPreviousFirmware();
        return; // Won't reach here due to restart
    }

    // Check if we have a different partition than boot (simple OTA detection)
    const esp_partition_t *running = esp_ota_get_running_partition();
    const esp_partition_t *boot = esp_ota_get_boot_partition();

    if (running && boot && running != boot)
    {
        ESP_LOGI(TAG,
                 "New firmware detected (boot attempt %d/%d). "
                 "Call validateCurrentFirmware() when ready to confirm.",
                 boot_count,
                 max_boot_attempts);
    }

    ESP_LOGI(TAG, "Automatic rollback protection initialized.");
}

void Program::checkAutomaticRollback()
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK)
    {
        return; // NVS not initialized yet
    }

    // For ESP8266, we use a simple approach: if running partition != boot partition
    // and we have a boot count, we're in a potential rollback situation
    const esp_partition_t *running = esp_ota_get_running_partition();
    const esp_partition_t *boot = esp_ota_get_boot_partition();

    if (!running || !boot || running == boot)
    {
        // Same partition, reset boot count
        uint8_t boot_count = 0;
        nvs_set_blob(
            nvs_handle, BOOT_COUNT_KEY, &boot_count, sizeof(boot_count));
        nvs_commit(nvs_handle);
        nvs_close(nvs_handle);
        return;
    }

    // Check if too much time has passed since last boot attempt
    uint64_t last_boot_time = 0;
    size_t required_size = sizeof(last_boot_time);
    err = nvs_get_blob(
        nvs_handle, LAST_BOOT_TIME_KEY, &last_boot_time, &required_size);
    if (err == ESP_OK)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        uint64_t current_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        uint64_t time_diff = current_time - last_boot_time;

        // If more than 2 minutes since last boot, reset counter (probably intentional restart)
        if (time_diff > 120000)
        {
            ESP_LOGI(
                TAG,
                "Long time since last boot (%llu ms), resetting boot counter",
                time_diff);
            uint8_t boot_count = 0;
            nvs_set_blob(
                nvs_handle, BOOT_COUNT_KEY, &boot_count, sizeof(boot_count));
            nvs_commit(nvs_handle);
        }
    }

    nvs_close(nvs_handle);
}

bool Program::validateCurrentFirmware()
{
    const esp_partition_t *running_partition = esp_ota_get_running_partition();
    if (!running_partition)
    {
        ESP_LOGE(TAG, "Failed to get running partition");
        return false;
    }

    // For ESP8266 SDK v3.4, we use a simplified approach
    // Check if running partition != boot partition (indicates OTA update)
    const esp_partition_t *boot_partition = esp_ota_get_boot_partition();
    if (!boot_partition)
    {
        ESP_LOGE(TAG, "Failed to get boot partition");
        return false;
    }

    if (running_partition != boot_partition)
    {
        // This looks like we're running from OTA partition, mark as validated
        // Reset boot count after successful validation
        nvs_handle_t nvs_handle;
        if (nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle) == ESP_OK)
        {
            uint8_t boot_count = 0;
            nvs_set_blob(
                nvs_handle, BOOT_COUNT_KEY, &boot_count, sizeof(boot_count));
            nvs_commit(nvs_handle);
            nvs_close(nvs_handle);
        }

        ESP_LOGI(TAG, "New firmware validated and marked as valid");
    }
    else
    {
        ESP_LOGI(
            TAG,
            "Current firmware is already validated (same as boot partition)");
    }

    return true;
}

bool Program::rollbackToPreviousFirmware()
{
    const esp_partition_t *running_partition = esp_ota_get_running_partition();
    if (!running_partition)
    {
        ESP_LOGE(TAG, "Failed to get running partition");
        return false;
    }

    const esp_partition_t *boot_partition = esp_ota_get_boot_partition();
    if (!boot_partition)
    {
        ESP_LOGE(TAG, "Failed to get boot partition");
        return false;
    }

    if (running_partition == boot_partition)
    {
        ESP_LOGE(TAG, "Cannot rollback: already running from boot partition");
        return false;
    }

    // Find the other OTA partition (not the current running one)
    const esp_partition_t *rollback_partition = NULL;
    esp_partition_iterator_t it = esp_partition_find(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);

    while (it != NULL)
    {
        const esp_partition_t *part = esp_partition_get(it);
        if (part != running_partition
            && (part->subtype == ESP_PARTITION_SUBTYPE_APP_OTA_0
                || part->subtype == ESP_PARTITION_SUBTYPE_APP_OTA_1))
        {
            rollback_partition = part;
            break;
        }
        it = esp_partition_next(it);
    }

    if (rollback_partition == NULL)
    {
        ESP_LOGE(TAG, "No rollback partition found");
        return false;
    }

    // Set the rollback partition as boot partition
    esp_err_t err = esp_ota_set_boot_partition(rollback_partition);
    if (err != ESP_OK)
    {
        ESP_LOGE(
            TAG, "Failed to set rollback partition: %s", esp_err_to_name(err));
        return false;
    }

    ESP_LOGI(
        TAG,
        "Rollback initiated. System will restart with previous firmware...");
    esp_restart();

    return true; // Won't reach here due to restart
}

uint8_t Program::getCurrentBootAttempt()
{
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK)
    {
        return 0;
    }

    // Check if we're in a potential rollback situation
    const esp_partition_t *running_partition = esp_ota_get_running_partition();
    const esp_partition_t *boot_partition = esp_ota_get_boot_partition();
    if (!running_partition || !boot_partition
        || running_partition == boot_partition)
    {
        nvs_close(nvs_handle);
        return 0; // Not in OTA state
    }

    uint8_t boot_count = 0;
    size_t required_size = sizeof(boot_count);
    err = nvs_get_blob(nvs_handle, BOOT_COUNT_KEY, &boot_count, &required_size);
    nvs_close(nvs_handle);

    if (err != ESP_OK)
    {
        return 0;
    }

    return boot_count;
}

bool Program::isTotalSizeReached()
{
    return m_currentSize >= m_totalSize;
}
