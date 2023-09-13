# Arduino BLE Project

This Arduino BLE project is designed to create a Bluetooth Low Energy (BLE) device with custom services and characteristics. It allows for bidirectional communication of data in JSON format between the BLE device and a connected mobile device.

## Table of Contents

- [Usage](#usage)
- [BLE Services and Characteristics](#ble-services-and-characteristics)
- [Features](#features)
## Usage

1. When the ESP32 is powered up, it will be in "discoverable" mode and waiting for a BLE connection. You will see debug messages on the Arduino serial port.

2. To connect a BLE device, use a phone or other BLE compatible device. Look for the "ESP32_BLE_Example" device in the list of available devices and connect to it.

3. Once connected, you can interact with the following BLE services and characteristics:

### Custom BLE Service

- **Service UUID:** 4fafc201-1fb5-459e-8fcc-c5c9c331914b

### Custom BLE Characteristic

- **Characteristic UUID:** beb5483e-36e1-4688-b7f5-ea07361b26a8

This characteristic allows bidirectional communication of JSON-formatted data. You can send JSON data to the characteristic, and the device will process and display it on the serial port.

4. To disconnect the BLE device, simply close the connection from the device.

## Features

- The BLE device is discoverable and can be connected to from any BLE compatible device.
- Custom BLE services and characteristics for flexible data communication.
- It can receive data in JSON format and display it on the serial port.
- The code is designed to be easily customizable to meet your specific needs.
