#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define SSID "JM"
#define PASSWORD "JulIsaCasMaxCyp05."
#define LOCAL_PORT 9999

#define PIN_NEO_PIXEL 4
#define NUM_PIXELS 200

#define ADC_PIN A0
#define TIME_BETWEEN_PACKETS 100

char packetBuffer[255];
IPAddress masterIP;
WiFiUDP udp;
Adafruit_NeoPixel strip(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

typedef enum
{
    PACKET_TYPE_INIT,
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
    // Fill the entire strip with white while wifi isn't connected.
    strip.fill(strip.Color(50, 50, 50), 0, NUM_PIXELS);
    strip.show();

    // Connect to WiFi
    Serial.print("Connecting to ");
    Serial.print(SSID);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
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
                    masterIP = udp.remoteIP();
                    // Respond to the master
                    udp.beginPacket(masterIP, LOCAL_PORT);
                    udp.write((char)(PACKET_TYPE_INIT));
                    udp.endPacket();
                    break;

                case PACKET_TYPE_RGB:
                    // Serial.println("RGB packet received");
                    if (len == 4)
                    {
                        strip.fill(strip.Color(packetBuffer[1], packetBuffer[2], packetBuffer[3]));
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
    strip.show();

    static unsigned long lastPacketTime = 0;
    if (millis() - lastPacketTime > TIME_BETWEEN_PACKETS)
    {
        // Read the ADC value
        int adcValue = analogRead(ADC_PIN);
        // Serial.println(adcValue);
        char adcPacket[] = {PACKET_TYPE_ADC, (char)(adcValue*256/1024)};
        udp.beginPacket(masterIP, LOCAL_PORT);
        udp.write(adcPacket, 2);
        udp.endPacket();
        lastPacketTime = millis();
    }
}
