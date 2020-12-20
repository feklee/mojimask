#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app_display.h"
#include "app_camera.h"
#include "app_wifi.h"
#include "app_httpd.h"

void delay(
           int x // ms
           ) {
    vTaskDelay(x / portTICK_PERIOD_MS);
}

void app_main(void) {
    app_display_main();
    app_wifi_main();
    app_camera_main();
    app_httpd_main();
}
