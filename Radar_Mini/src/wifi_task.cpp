#include "wifi_task.h"

static const char* WIFI_SSID = "Huynh Phat";
static const char* WIFI_PASS = "0984242477";

// void wifi_init_ap(){

//     Serial.println("[WiFi] Starting access point...");

//     WiFi.mode(WIFI_MODE_AP);
//     WiFi.setSleep(false);

//     bool ok = WiFi.softAP(WIFI_SSID, WIFI_PASS);

//     if (!ok){
//         Serial.println("[WiFi] Failed to start AP");
//         return;
//     }

//     Serial.print("[WiFi] AP IP Address: ");
//     Serial.println(WiFi.softAPIP());
// }

void wifi_init_sta(){

    Serial.println("[WiFi] Connecting to WiFi (STA mode)...");

    WiFi.mode(WIFI_STA);    // Set mode
    WiFi.setSleep(false);   // false -> keep always active
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    if (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.println(".");
    }

    Serial.println("[WiFi] Connected! IP Address: ");
    Serial.println(WiFi.localIP());
}


void TaskWifi(void *pvParameter){

    //wifi_init_ap();
    wifi_init_sta();

    while(1){
        // For AP MODE
        // int clients = WiFi.softAPgetStationNum();
        // Serial.print("[WiFi] Clients connected: ");
        // Serial.println(clients);

        // For STA MODE
        if (WiFi.status() != WL_CONNECTED){
            Serial.println("[WiFi] Disconnected, trying to reconnect to WIFi again");
            WiFi.reconnect();
        }


        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
