#include "webserver_task.h"
#include "led_task.h"

WebServer server(80);

void handleOn(){
    
    const char* cmd = "ON";

    xQueueSend(xQueueLed, &cmd, 0);

    server.send(200, "text/html", "<h1>LED is ON</h1><a href=\"/\">Back</a>");
}

void handleOff(){
    const char* cmd = "OFF";

    xQueueSend(xQueueLed, &cmd, 0);

    server.send(200, "text/html", "<h1>LED is OFF</h1><a href=\"/\">Back</a>");
}

void handleRoot(){
    String html = "<h1>ESP32 LED Control</h1>"
                    "<p><a href=\"/on\">Turn ON</a></p>"
                    "<p><a href=\"/off\">Turn OFF</a></p>";

                    server.send(200, "text/html", html);
}

void handleNotFound(){
    server.send(404, "text/html", "<h1>404 Not Found</h1><a href=\"/\">Back</a>");
}

void webserver_init(){
    Serial.println("[WebServer] Initializing...");

    server.on("/", handleRoot);
    server.on("/on", handleOn);
    server.on("/off", handleOff);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("[WebServer] Started on port 80");
}

void TaskWebServer(void *pvParameter){

    webserver_init();

    while(1){
        server.handleClient(); // Handle request of HTTP
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

