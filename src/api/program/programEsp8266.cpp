/**
 * @file programEsp8266.cpp
 * @author Cyprien Ménard
 * @date 2025-08-18
 */

#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "program.hpp"
#include <stdio.h>

static const char *TAG = "Program";
static const char *PROGRAM_FILE_PATH = "/spiffs/program.bin";

static esp_ota_handle_t s_update_handle = 0;
static const esp_partition_t *s_update_partition = nullptr;
static bool s_ota_in_progress = false;

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

    ESP_LOGI(TAG, "OTA update completed successfully. Restarting...");
    esp_restart();

    return true; // This line won't be reached due to restart
}
