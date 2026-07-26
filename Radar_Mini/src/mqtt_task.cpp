#include "mqtt_task.h"
#include "servo_task.h"
#include <WiFi.h>
#include <ArduinoJson.h>

// ---- Cấu hình CoreIoT ----
static const char* MQTT_BROKER    = "app.coreiot.io";
static const int   MQTT_PORT      = 1883;
static const char* ACCESS_TOKEN   = "rldnplkqr61c4xq8601e";
static const char* MQTT_CLIENT_ID = "esp32s3-lab6-coreiot";

// ---- Topic ThingsBoard/CoreIoT----
static const char* TOPIC_RPC_REQUEST = "v1/devices/me/rpc/request/+"; // + là requestId, do server tự sinh mỗi lần bấm nút

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Trích requestId từ topic dạng "v1/devices/me/rpc/request/123" -> "123"
String extractRequestId(const char* topic){
    String t = String(topic);
    int lastSlash = t.lastIndexOf('/');
    return t.substring(lastSlash + 1);
}

void mqttCallback(char* topic, byte* payload, unsigned int length){

    String message;
    for (unsigned int i = 0; i < length; i++){
        message += (char)payload[i];
    }

    Serial.print("[MQTT] RPC request trên topic ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(message);

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, message);

    if (err){
        Serial.print("[MQTT] Lỗi parse JSON: ");
        Serial.println(err.c_str());
        return;
    }

    String method    = doc["method"].as<String>();
    String requestId = extractRequestId(topic);
    
    if (method == "setServoState"){

        bool servoOn = doc["params"].as<bool>();
        const char* cmd = servoOn ? "ON" : "OFF";

        xQueueSend(xQueueServo, &cmd, 0);

        String responseTopic = "v1/devices/me/rpc/response/" + requestId;
        mqttClient.publish(responseTopic.c_str(), "{}");


        Serial.print("[MQTT] handled setServoState = ");
        Serial.println(cmd);
    }
}

void mqtt_reconnect(){

    while (!mqttClient.connected()){

        Serial.print("[MQTT] Đang kết nối tới CoreIoT...");

        // CoreIoT/ThingsBoard: username = access token, password NULL
        if (mqttClient.connect(MQTT_CLIENT_ID, ACCESS_TOKEN, NULL)){
            Serial.println("OK");
            mqttClient.subscribe(TOPIC_RPC_REQUEST);
            Serial.println("[MQTT] Đã subscribe RPC request");
        } else {
            Serial.print("Thất bại, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" - thử lại sau 2s (rc=5 hoặc rc=4 thường do ACCESS_TOKEN sai)");
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }
}

void mqtt_init(){
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setBufferSize(512);
}

void TaskMqtt(void *pvParameter){

    while (WiFi.status() != WL_CONNECTED){
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    mqtt_init();

    while(1){

        if (!mqttClient.connected()){
            mqtt_reconnect();
        }

        mqttClient.loop();

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}