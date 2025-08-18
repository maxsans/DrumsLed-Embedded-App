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

void Program::updateProgramBinary(const Binary &binary)
{
    if (binary.size() == 0)
    {
        ESP_LOGE(TAG, "Binary is empty, cannot update");
        return;
    }

    ESP_LOGI(
        TAG, "Starting OTA update with binary size: %d bytes", binary.size());

    // Get the next OTA partition
    const esp_partition_t *update_partition
        = esp_ota_get_next_update_partition(NULL);
    if (!update_partition)
    {
        ESP_LOGE(TAG, "Failed to get OTA update partition");
        return;
    }

    ESP_LOGI(TAG,
             "Writing to partition subtype %d at offset 0x%x",
             update_partition->subtype,
             update_partition->address);

    // Begin OTA update
    esp_ota_handle_t update_handle = 0;
    esp_err_t err
        = esp_ota_begin(update_partition, binary.size(), &update_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ota_begin failed: %s", esp_err_to_name(err));
        return;
    }

    // Write binary data to flash
    err = esp_ota_write(update_handle, binary.data(), binary.size());
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ota_write failed: %s", esp_err_to_name(err));
        esp_ota_end(update_handle);
        return;
    }

    // Finalize the OTA update
    err = esp_ota_end(update_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ota_end failed: %s", esp_err_to_name(err));
        return;
    }

    // Set the new partition as boot partition
    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK)
    {
        ESP_LOGE(
            TAG, "esp_ota_set_boot_partition failed: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "OTA update completed successfully. Restarting...");

    // Restart the system to boot from the new partition
    esp_restart();
}
