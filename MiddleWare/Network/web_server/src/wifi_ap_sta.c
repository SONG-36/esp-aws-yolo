#include "wifi_ap_sta.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "esp_mac.h"
#include "math.h"
#include <string.h>

static const char *TAG = "wifi_config";

#define EXAMPLE_MAX_STA_CONN       (3)

esp_netif_t *wifi_sta_netif = NULL;
esp_netif_t *wifi_ap_netif = NULL;

void wifi_config_start_softap(void)
{
    wifi_mode_t wifi_mode = WIFI_MODE_AP;

    ESP_LOGI(TAG, "Initializing ESP-Netif");
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_sta_netif = esp_netif_create_default_wifi_sta();
    wifi_ap_netif  = esp_netif_create_default_wifi_ap();

    ESP_LOGI(TAG, "Initializing Wi-Fi");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 动态生成 SSID（包含 MAC 尾部）
    uint8_t mac[6];
    char ssid_str[20];
    esp_read_mac(mac, ESP_MAC_WIFI_SOFTAP);
    snprintf(ssid_str, sizeof(ssid_str), "esp-sparkbot-%02X%02X", mac[4], mac[5]);

    ESP_ERROR_CHECK(esp_wifi_set_mode(wifi_mode));

    if (wifi_mode == WIFI_MODE_AP || wifi_mode == WIFI_MODE_APSTA) {
        wifi_config_t wifi_ap_config = {
            .ap = {
                .ssid = { 0 },
                .password = "",
                .channel = 8,
                .max_connection = EXAMPLE_MAX_STA_CONN,
                .authmode = WIFI_AUTH_OPEN,
            },
        };
        strncpy((char *)wifi_ap_config.ap.ssid, ssid_str, sizeof(wifi_ap_config.ap.ssid) - 1);
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config));
    }

    ESP_LOGI(TAG, "Starting Wi-Fi (SoftAP Mode)");
    ESP_ERROR_CHECK(esp_wifi_start());
}
