#include "ssd1306.h"
#include "ssd1306_draw.h"
#include "ssd1306_font.h"
#include "ssd1306_default_if.h"

static const int SPIDisplayChipSelect = 15;
static const int SPIDisplayWidth = 128;
static const int SPIDisplayHeight = 64;
static const int SPIResetPin = 2;

static struct SSD1306_Device SPIDisplay;

static bool default_bus_init() {
    assert(SSD1306_SPIMasterInitDefault() == true);
    assert(SSD1306_SPIMasterAttachDisplayDefault(&SPIDisplay, SPIDisplayWidth, SPIDisplayHeight, SPIDisplayChipSelect, SPIResetPin) == true);
    return true;
}

static void draw_line(int x0, int y0, int x1, int y1) {
    SSD1306_DrawLine(&SPIDisplay, x0, y0, x1, y1, SSD_COLOR_WHITE);
}

static void draw_emoji(const int lines[][4], const int number_of_lines) {
    SSD1306_Clear(&SPIDisplay, SSD_COLOR_BLACK);

    for (int i = 0; i < number_of_lines; i++) {
        const int *line = lines[i];
        draw_line(line[0], line[1], line[2], line[3]);
    }

    for (int i = 0; i < number_of_lines; i++) {
        const int *line = lines[i];
        draw_line(127 - line[0], line[1], 127 - line[2], line[3]);
    }

    SSD1306_Update(&SPIDisplay);
}

void display_draw_happy() {
    int lines[][4] =
        {{0, 63 - 32, 16, 63 - 48},
         {8, 63 - 40, 20, 63 - 28},
         {20, 63 - 28, 34, 63 - 19},
         {34, 63 - 19, 49, 63 - 14},
         {49, 63 - 14, 64, 63 - 12}};
    draw_emoji(lines, sizeof(lines) / sizeof(int[4]));
}

void display_draw_sad() {
    int lines[][4] =
        {
         {8, 40, 20, 28},
         {20, 28, 34, 19},
         {34, 19, 49, 14},
         {49, 14, 64, 12}
        };
    draw_emoji(lines, sizeof(lines) / sizeof(int[4]));
}

void display_draw_neutral() {
    int lines[][4] =
        {
         {5, 28, 5, 36},
         {5, 32, 122, 32}
        };
    draw_emoji(lines, sizeof(lines) / sizeof(int[4]));
}

void app_display_main()
{
    if (!default_bus_init()) {
        return;
    }

    SSD1306_Clear(&SPIDisplay, SSD_COLOR_BLACK);
    SSD1306_SetFont(&SPIDisplay, &Font_droid_sans_fallback_24x28);
    SSD1306_FontDrawAnchoredString(&SPIDisplay, TextAnchor_Center,
                                   "MOJIMASK", SSD_COLOR_WHITE);
    SSD1306_Update(&SPIDisplay);
}
