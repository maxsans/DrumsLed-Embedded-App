#include "addrLed.h"
#include "esp8266/gpio_struct.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define WS2812B_GPIO_NUM 18

static int g_num_leds = 0;
static addrLedColor_t *g_pixels = NULL;

void delay_ns(uint32_t ns)
{
    // Fonction de délai pour les timings WS2812B
    for (uint32_t i = 0; i < ns; i++) {
        asm volatile("nop");
    }
}

void send_bit(bool bit)
{
    if (bit) {
        GPIO.out_w1ts = (1 << WS2812B_GPIO_NUM);
        delay_ns(800); // T1H
        GPIO.out_w1tc = (1 << WS2812B_GPIO_NUM);
        delay_ns(450); // T1L
    } else {
        GPIO.out_w1ts = (1 << WS2812B_GPIO_NUM);
        delay_ns(400); // T0H
        GPIO.out_w1tc = (1 << WS2812B_GPIO_NUM);
        delay_ns(850); // T0L
    }
}

void send_byte(uint8_t byte)
{
    for (int i = 0; i < 8; i++) {
        send_bit(byte & (1 << (7 - i)));
    }
}

void send_color(addrLedColor_t color)
{
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    send_byte(g); // Green
    send_byte(r); // Red
    send_byte(b); // Blue
}

void addrLed_init(int gpio_num, int num_leds)
{
    if (g_pixels != NULL) {
        free(g_pixels);
    }
    g_pixels = (addrLedColor_t *)malloc(num_leds * sizeof(addrLedColor_t));
    g_num_leds = num_leds;
    gpio_set_direction((gpio_num_t)WS2812B_GPIO_NUM, GPIO_MODE_OUTPUT);
}

void addrLed_set_color(addrLedIndex_t index, addrLedColor_t color)
{
    // Stocker les couleurs dans un tableau pour les envoyer plus tard
    if (index < g_num_leds) {
        g_pixels[index] = color;
    }
}

void addrLed_refresh()
{
    // Envoyer les couleurs stockées aux LEDs
    for (int i = 0; i < g_num_leds; i++) {
        send_color(g_pixels[i]);
    }
}
