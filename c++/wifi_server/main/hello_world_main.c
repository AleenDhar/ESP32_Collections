// /*
//  * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
//  *
//  * SPDX-License-Identifier: CC0-1.0
//  */

// #include <stdio.h>
// // #include <inttypes.h>
// // #include "sdkconfig.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/gpio.h"
// // #include "freertos/task.h"
// // #include "esp_chip_info.h"
// // #include "esp_flash.h"
// // #include "esp_system.h"

// void app_main(void)
// {
//     gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);  // Built-in LED on GPIO 0
//     while(1){
//         gpio_set_level(GPIO_NUM_2,0);
//         vTaskDelay(100);
//         gpio_set_level(GPIO_NUM_2,1);
//         vTaskDelay(100);
//     } 

//     // printf("Hello world!\n");
//     // int i = 0;
//     // printf("Loop Starting...\n");
//     // while(1){
//     //     printf("loop %d \n",i);
//     //     i++;
//     //     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     // }

//     /* Print chip information */
//     // esp_chip_info_t chip_info;
//     // uint32_t flash_size;
//     // esp_chip_info(&chip_info);
//     // printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
//     //        CONFIG_IDF_TARGET,
//     //        chip_info.cores,
//     //        (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
//     //        (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
//     //        (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
//     //        (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

//     // unsigned major_rev = chip_info.revision / 100;
//     // unsigned minor_rev = chip_info.revision % 100;
//     // printf("silicon revision v%d.%d, ", major_rev, minor_rev);
//     // if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
//     //     printf("Get flash size failed");
//     //     return;
//     // }

//     // printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
//     //        (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

//     // printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

//     // for (int i = 10; i >= 0; i--) {
//     //     printf("Restarting in %d seconds...\n", i);
//     //     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     // }
//     // printf("Restarting now.\n");
//     // fflush(stdout);
//     // esp_restart();
// }

// WiFi connection and POST command

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "my_data.h"
#include "driver/gpio.h"

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("WiFi connecting WIFI_EVENT_STA_START ... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi connected WIFI_EVENT_STA_CONNECTED ... \n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi lost connection WIFI_EVENT_STA_DISCONNECTED ... \n");
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("WiFi got IP ... \n\n");
        break;
    default:
        break;
    }
}

void wifi_connection()
{
    nvs_flash_init();
    esp_netif_init();                    
    esp_event_loop_create_default();     
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); 
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = SSID,
            .password = PASS
            }
        };
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_connect();
}

// static esp_err_t post_handler(httpd_req_t *req)
// {
//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//     httpd_resp_send(req, "URI POST Response ... from ESP32", HTTPD_RESP_USE_STRLEN);
//     return ESP_OK;
// }


static esp_err_t get_handlerath(httpd_req_t *req)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    httpd_resp_send(req, "<a href=\"/L\">OFF</a>", HTTPD_RESP_USE_STRLEN);
    gpio_set_level(GPIO_NUM_2,1);
    printf("GPIO 2 is ON\n");
    return ESP_OK;
}
static esp_err_t get_handleratl(httpd_req_t *req)
{
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    httpd_resp_send(req, "<a href=\"/H\">ON</a>", HTTPD_RESP_USE_STRLEN);
    gpio_set_level(GPIO_NUM_2,0);
    printf("GPIO 2 is OFF\n");
    return ESP_OK;
}

void server_initiation()
{
    httpd_config_t server_config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server_handle = NULL;
    httpd_start(&server_handle, &server_config);
    httpd_uri_t uri_getl = {
        .uri = "/L",
        .method = HTTP_GET,
        .handler = get_handleratl,
        .user_ctx = NULL};
    httpd_register_uri_handler(server_handle, &uri_getl);
    httpd_uri_t uri_geth = {
        .uri = "/H",
        .method = HTTP_GET,
        .handler = get_handlerath,
        .user_ctx = NULL};
    httpd_register_uri_handler(server_handle, &uri_geth);
}

void app_main(void)
{
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);  // Built-in LED on GPIO 0
    wifi_connection();
    server_initiation();
}
