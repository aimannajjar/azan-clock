#include "systime.h"
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "esp_log.h"
#include "esp_event.h"
#include "esp_sleep.h"
#include "esp_netif.h"
#include "esp_netif_sntp.h"
#include "esp_sntp.h"
#include "azan_clock.h"
#include "clock.h"
#include "weather.h"
#include "prayers.h"
#include "settings.h"

static const char *TAG = "SYSTIME";
extern lv_obj_t *ui_Main_Screen;
extern lv_obj_t *ui_Loading_Status_Text;

#ifndef INET6_ADDRSTRLEN
#define INET6_ADDRSTRLEN 48
#endif

static void print_servers(void);

void systime_init(void)
{
    take_ui_mutex("systime_init");
    lv_label_set_text(ui_Loading_Status_Text, "Synchronizing System Time...");
    give_ui_mutex("systime_init");
    xTaskCreate(systime_task, "systime_task", 4096, NULL, 5, NULL);
}

void notify_systime(void) {
    xTaskCreate(systime_task, "systime_task", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "Updating systime");
}

void systime_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Initializing and starting SNTP");
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG_MULTIPLE(5,
                                ESP_SNTP_SERVER_LIST("time.nist.gov", "time.cloudflare.com", "time.aws.com", "pool.ntp.org", "time.google.com" ) );

    // TODO: Move to settings init
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();

    // sync with sntp
    esp_netif_sntp_init(&config);
    print_servers();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 30;
    while (esp_netif_sntp_sync_wait(1000 / portTICK_PERIOD_MS) == ESP_ERR_TIMEOUT && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    }
    time(&now);
    localtime_r(&now, &timeinfo);

    esp_netif_sntp_deinit();

    if (!is_settings_initialized()) {
        // We are going through initializtion sequence, proceed to settings_init
        settings_init();
    } else {
        // otherwise, we can skip settings init and skip directly to notifying prayers of systime change
        notify_prayers();
    }

    // time(&now);


    // if (sntp_get_sync_mode() == SNTP_SYNC_MODE_SMOOTH) {
    //     struct timeval outdelta;
    //     while (sntp_get_sync_status() == SNTP_SYNC_STATUS_IN_PROGRESS) {
    //         adjtime(NULL, &outdelta);
    //         ESP_LOGI(TAG, "Waiting for adjusting time ... outdelta = %jd sec: %li ms: %li us",
    //                  (intmax_t)outdelta.tv_sec, (long)outdelta.tv_usec / 1000, (long)outdelta.tv_usec % 1000);
    //         vTaskDelay(2000 / portTICK_PERIOD_MS);
    //     }
    // }

    vTaskDelete(NULL); // Delete the task once time synchronization is done
}

static void print_servers(void)
{
    ESP_LOGI(TAG, "List of configured NTP servers:");

    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i){
        if (esp_sntp_getservername(i)){
            ESP_LOGI(TAG, "server %d: %s", i, esp_sntp_getservername(i));
        } else {
            // we have either IPv4 or IPv6 address, let's print it
            char buff[INET6_ADDRSTRLEN];
            ip_addr_t const *ip = esp_sntp_getserver(i);
            if (ipaddr_ntoa_r(ip, buff, INET6_ADDRSTRLEN) != NULL)
                ESP_LOGI(TAG, "server %d: %s", i, buff);
        }
    }
}
