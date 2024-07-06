#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <stdint.h>

#define SSID "JM"
#define PASSWORD "JulIsaCasMaxCyp05."
#define LOCAL_PORT 9999

#define PIN_NEO_PIXEL 4
#define NUM_PIXELS 7

#define ADC_PIN A0
#define TIME_BETWEEN_PACKETS 20 // in ms
#define TIME_BETWEEN_PING 1000 // in ms
#define TIME_BETWEEN_READ_UDP 10 // in ms
#define TIME_BETWEEN_ADC_MEASURES 2 // in ms

char packetBuffer[255];
bool masterFound = false;
IPAddress masterIP;
WiFiUDP udp;
Adafruit_NeoPixel strip(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

#define NB_MEASURE_MAX 10
uint8_t g_mesureBuff[NB_MEASURE_MAX];
uint32_t g_mesureIndex = 0;
uint32_t g_nbMeasures = 0;

typedef enum
{
    PACKET_TYPE_INIT = 1,
    PACKET_TYPE_RGB,
    PACKET_TYPE_ADC
} packet_type_t;

void setup()
{
    Serial.begin(115200);
    delay(100);
    Serial.println("\nStart...\n");

    // Initialize the ADC
    pinMode(ADC_PIN, INPUT);

    // Initialize the NeoPixel library.
    strip.begin();
    strip.fill(strip.Color(255, 255, 255));
    strip.show();

    // Connect to WiFi
    Serial.print("Connecting to ");
    Serial.print(SSID);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
        if (millis() > 10000)
        {
            Serial.println("Could not connect to WiFi, restarting...");
            ESP.restart();
        }
    }
    udp.begin(LOCAL_PORT);
    Serial.println("\nWiFi connected, IP address: ");
    Serial.println(WiFi.localIP());

    // Wifi is connected, fill the strip off.
    strip.fill(strip.Color(0, 0, 0));
    strip.show();

}

void loop()
{
    static unsigned long lastReadTime = 0;
    if ( (millis() - lastReadTime > TIME_BETWEEN_READ_UDP))
    {
        lastReadTime = millis();
        int packetSize = udp.parsePacket();
        if (packetSize)
        {
            // Serial.print(" Received packet from : ");
            // Serial.println(udp.remoteIP());
            // Serial.print(" Size : ");
            // Serial.println(packetSize);
            int len = udp.read(packetBuffer, 255);
            if (len > 0)
            {
                switch(packetBuffer[0])
                {
                    case PACKET_TYPE_INIT:
                        Serial.print("Init packet received, master IP:");
                        Serial.println(udp.remoteIP());
                        masterFound = true;
                        masterIP = udp.remoteIP();
                        break;

                    case PACKET_TYPE_RGB:
                        // Serial.println("RGB packet received");
                        if (len == 4)
                        {
                            strip.fill(strip.Color(packetBuffer[1], packetBuffer[2], packetBuffer[3]));
                            strip.show();
                        }
                        break;

                    case PACKET_TYPE_ADC:
                    default:
                        Serial.println("Unknown packet received");
                        break;
                }
                // Serial.println(String("Led strip: ")
                //     + String((int)packetBuffer[0]) + String(" ")
                //     + String((int)packetBuffer[1]) + String(" ")
                //     + String((int)packetBuffer[2]));
            }
        }
    }

    // Update the ADC value in the average buffer
    static unsigned long lastMeasureTime = 0;
    if ( (millis() - lastMeasureTime > TIME_BETWEEN_ADC_MEASURES))
    {
        lastMeasureTime = millis();
        g_mesureBuff[g_mesureIndex] = (analogRead(ADC_PIN) >> 2); // 0-1023 -> 0-255
        g_nbMeasures = (g_nbMeasures < NB_MEASURE_MAX) ? g_nbMeasures + 1 : NB_MEASURE_MAX;
        g_mesureIndex = (g_mesureIndex + 1) % NB_MEASURE_MAX;
    }

    static unsigned long lastPacketTime = 0;
    if ( (millis() - lastPacketTime > TIME_BETWEEN_PACKETS))
    {
        // On calcule le maximum des valeurs mesurées
        uint8_t l_maxValue = 0;
        for (uint32_t i = 0; i < g_nbMeasures; i++)
        {
            if (g_mesureBuff[i] > l_maxValue)
            {
                l_maxValue = g_mesureBuff[i];
            }
        }
        // Send the max ADC value to the master
        char adcPacket[] = {PACKET_TYPE_ADC, (char)(l_maxValue)};
        udp.beginPacket(masterIP, LOCAL_PORT);
        udp.write(adcPacket, 2);
        udp.endPacket();
        lastPacketTime = millis();
    }

    // Ping the master every seconds
    static unsigned long lastPingTime = 0;
    if ( (millis() - lastPingTime > 1000) && (masterFound))
    {
        lastPingTime = millis();
        udp.beginPacket(masterIP, LOCAL_PORT);
        udp.write((char)(PACKET_TYPE_INIT));
        udp.endPacket();
    }
}
