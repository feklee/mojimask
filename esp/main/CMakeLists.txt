set(COMPONENT_SRCS "app_main.c" "app_wifi.c" "app_camera.c" "app_httpd.c" "app_display")
set(COMPONENT_ADD_INCLUDEDIRS "include")

set(COMPONENT_REQUIRES
    tarablessd1306
    esp32-camera
    nvs_flash
    esp_http_server
    esp_http_client
    )

register_component()
