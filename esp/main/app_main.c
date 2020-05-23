#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ssd1306.h"
#include "ssd1306_draw.h"
#include "ssd1306_font.h"
#include "ssd1306_default_if.h"
#include "app_camera.h"
#include "app_wifi.h"
#include "app_httpd.h"

static const int SPIDisplayChipSelect = 15;
static const int SPIDisplayWidth = 128;
static const int SPIDisplayHeight = 64;
static const int SPIResetPin = 2;

struct SSD1306_Device SPIDisplay;

bool default_bus_init() {
    assert(SSD1306_SPIMasterInitDefault() == true);
    assert(SSD1306_SPIMasterAttachDisplayDefault(&SPIDisplay, SPIDisplayWidth, SPIDisplayHeight, SPIDisplayChipSelect, SPIResetPin) == true);
    return true;
}

void draw_line(int x0, int y0, int x1, int y1) {
    SSD1306_DrawLine(&SPIDisplay, x0, y0, x1, y1, SSD_COLOR_WHITE);
}

void draw_emoji(const int lines[][4], const int number_of_lines) {
    for (int i = 0; i < number_of_lines; i++) {
        const int *line = lines[i];
        draw_line(line[0], line[1], line[2], line[3]);
    }

    for (int i = 0; i < number_of_lines; i++) {
        const int *line = lines[i];
        draw_line(127 - line[0], line[1], 127 - line[2], line[3]);
    }
}

void draw_happy() {
    int lines[][4] =
        {{0, 63 - 32, 16, 63 - 48},
         {8, 63 - 40, 20, 63 - 28},
         {20, 63 - 28, 34, 63 - 19},
         {34, 63 - 19, 49, 63 - 14},
         {49, 63 - 14, 64, 63 - 12}};
    draw_emoji(lines, sizeof(lines) / sizeof(int[4]));
}

void draw_sad() {
    int lines[][4] =
        {
         {8, 40, 20, 28},
         {20, 28, 34, 19},
         {34, 19, 49, 14},
         {49, 14, 64, 12}
        };
    draw_emoji(lines, sizeof(lines) / sizeof(int[4]));
}

void draw_neutral() {
    int lines[][4] =
        {
         {5, 28, 5, 36},
         {5, 32, 122, 32}
        };
    draw_emoji(lines, sizeof(lines) / sizeof(int[4]));
}

void delay(
           int x // ms
           ) {
    vTaskDelay(x / portTICK_PERIOD_MS);
}

void app_main(void) {
    if (!default_bus_init()) {
        return;
    }

    SSD1306_Clear(&SPIDisplay, SSD_COLOR_BLACK);
    SSD1306_SetFont(&SPIDisplay, &Font_droid_sans_fallback_24x28);
    SSD1306_FontDrawAnchoredString(&SPIDisplay, TextAnchor_Center,
                                   "MOJIMASK", SSD_COLOR_WHITE);
    SSD1306_Update(&SPIDisplay);

    app_wifi_main();
    app_camera_main();
    app_httpd_main();

    for (unsigned int i = 0;; i++) {
        SSD1306_Clear(&SPIDisplay, SSD_COLOR_BLACK);
        switch (i % 3) {
        case 0:
            draw_happy();
            break;
        case 1:
            draw_neutral();
            break;
        case 2:
            draw_sad();
            break;
        }
        SSD1306_Update(&SPIDisplay);
        delay(1000);
    }
}
