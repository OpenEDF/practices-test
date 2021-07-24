/*****************************************************************************************
 * ESP HTTP Client To Get Github Hotmap
 *
 * Author: Macro
 * Email: makermuyi@gmail.com
 *
 * V 1. can get the basic data
 *
*****************************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"
#include "esp_http_client.h"

#define MAX_HTTP_RESP_BUFFER   1024
#define MAX_POST_DATA_SIZE 	   256
#define MAX_GITHUB_TOKEN_SIZE  126
#define DEBUG_LOG
static const char *TAG = "github";

/* Github API token */
const char token[MAX_GITHUB_TOKEN_SIZE] = "Bearer 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";

/* POST query data */
const char post_data[MAX_POST_DATA_SIZE] = "{\"query\":\"query {user(login: \\\"OpenEDF\\\") {contributionsCollection(from: \\\"2021-07-23T00:00:01+08:00\\\", to: \\\"2021-07-23T23:59:59+08:00\\\") {contributionCalendar {weeks {contributionDays {color date contributionCount }}}}}}\"}";

/* http response state */
typedef enum {ENABLE = 0, DISABLE = !ENABLE }FlagState;

/* http client struct */
typedef struct {
	char http_post_data[MAX_POST_DATA_SIZE];
	char http_token[MAX_GITHUB_TOKEN_SIZE];
	char http_response_buffer[MAX_HTTP_RESP_BUFFER];
	uint32_t http_response_len;
	FlagState http_flag;
}http_client_str;

/* global data for github http client */
http_client_str github_http_client;

/* github gttp client data initalization */
void github_http_cilent_init(void)
{
	/* initalization the default data */
	memcpy(github_http_client.http_post_data, post_data, strlen(post_data));
	memcpy(github_http_client.http_token, token, strlen(token));
	memset(github_http_client.http_response_buffer, 0x00, MAX_HTTP_RESP_BUFFER);
	github_http_client.http_response_len = 0;
	github_http_client.http_flag = DISABLE;
}

/* control system according the wifi work state */
esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
	if (evt == NULL)	/* check data  */
		return ESP_ERR_INVALID_ARG;

    switch(evt->event_id) {
		case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
			if (!esp_http_client_is_chunked_response(evt->client)) {
                github_http_client.http_response_len = evt->data_len;
                memcpy(github_http_client.http_response_buffer, evt->data, github_http_client.http_response_len);
				github_http_client.http_flag = ENABLE;
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            break;
		default:
			ESP_LOGD(TAG, "WIFI IS NOT WORK!");
    }
    return ESP_OK;
}

/* Https connect and get data */
static void https_with_url(void)
{
	uint32_t respon_state = 404;
	uint32_t respon_len;

	/* config https post information */
    esp_http_client_config_t config = {
        .url = "https://api.github.com/graphql",
		.host = "api.github.com",
        .event_handler = http_event_handler,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

#ifdef DEBUG_LOG
	ESP_LOGI(TAG, "HTTP POST DATA: %s\n", github_http_client.http_post_data);
#endif

	/* POST url configuration */
    esp_http_client_handle_t client = esp_http_client_init(&config);
	esp_http_client_set_method(client, HTTP_METHOD_POST);
	esp_http_client_set_header(client, "Content-Type", "application/json");
	esp_http_client_set_header(client, "Authorization", github_http_client.http_token);

	/* write post query and execute client */
	esp_http_client_set_post_field(client, post_data, strlen(github_http_client.http_post_data));
    esp_err_t err= esp_http_client_perform(client);

    if (ESP_OK != err) {
        ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
		goto label;
	}

	/* check the execute reault */
	respon_len = esp_http_client_get_content_length(client);
	respon_state = esp_http_client_get_status_code(client);
    ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %d", respon_state, respon_len);

label:
	/* disconnect and clean client hander resourse */
    esp_http_client_cleanup(client);
}

static void http_test_task(void *pvParameters)
{
	/* get the data from github */
    https_with_url();
    
	/* show the data */
#ifdef DEBUG_LOG
	if (github_http_client.http_flag == ENABLE) {
		ESP_LOGI(TAG, "HTTP GET DATA: %s", github_http_client.http_response_buffer);
		github_http_client.http_flag = DISABLE;
	}
#endif

	ESP_LOGI(TAG, "Finish http example");
    vTaskDelete(NULL);
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());
    ESP_LOGI(TAG, "Connected to AP, begin http example");

	/* github http data init */
	github_http_cilent_init();
    xTaskCreate(&http_test_task, "http_test_task", 8192, NULL, 5, NULL);
}
