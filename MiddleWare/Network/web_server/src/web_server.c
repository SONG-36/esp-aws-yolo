// #include "esp_log.h"
// #include "esp_http_server.h"
// #include "wifi_ap_sta.h"
// #include "esp_spiffs.h"
// #include <string.h>

// static const char *TAG = "web_server";

// esp_err_t index_get_handler(httpd_req_t *req)
// {
//     httpd_resp_set_type(req, "text/html");
//     return httpd_resp_sendfile(req, "/spiffs/index.html");
// }

// esp_err_t wifi_post_handler(httpd_req_t *req)
// {
//     char content[100];
//     int ret = httpd_req_recv(req, content, sizeof(content));
//     if (ret <= 0) return ESP_FAIL;

//     content[ret] = '\0';
//     ESP_LOGI(TAG, "Received: %s", content);

//     char ssid[32] = {0}, password[64] = {0};
//     sscanf(content, "ssid=%31[^&]&password=%63s", ssid, password);
    
//     wifi_connect_sta(ssid, password);
    
//     httpd_resp_sendstr(req, "Connecting to WiFi...");
//     return ESP_OK;
// }

// httpd_handle_t start_webserver(void)
// {
//     httpd_config_t config = HTTPD_DEFAULT_CONFIG();
//     httpd_handle_t server = NULL;

//     if (httpd_start(&server, &config) == ESP_OK)
//     {
//         httpd_uri_t uri_index = {
//             .uri = "/",
//             .method = HTTP_GET,
//             .handler = index_get_handler,
//             .user_ctx = NULL
//         };
//         httpd_register_uri_handler(server, &uri_index);

//         httpd_uri_t uri_post = {
//             .uri = "/wifi_config",
//             .method = HTTP_POST,
//             .handler = wifi_post_handler,
//             .user_ctx = NULL
//         };
//         httpd_register_uri_handler(server, &uri_post);
//     }

//     return server;
// }

// void init_webserver_fs(void)
// {
//     esp_vfs_spiffs_conf_t conf = {
//         .base_path = "/spiffs",
//         .partition_label = NULL,
//         .max_files = 5,
//         .format_if_mount_failed = true
//     };
//     esp_vfs_spiffs_register(&conf);
// }
