#include <Arduino.h>
#include "../lib/network/NetworkManager.h"
#include "../lib/websocket/WebsocketManager.h"

// Network credentials
char *ssidNetwork = TOSTRING(NETWORK_SSID);
char *passwordNetwork = TOSTRING(NETWORK_PASSWORD);

// device static configuration
char *ID = TOSTRING(DEVICE_ID);
char *name = TOSTRING(DEVICE_NAME);
char *type = TOSTRING(DEVICE_TYPE);

NetworkManager networkManager;
WebsocketManager websocketManager = WebsocketManager({ID, type, name});

void sendStatus() {
    websocketManager.sendCurrentStatus("current_status_mid", "QUERY");
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {

    switch (type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            Serial.printf("[WSc] Connected to url: %s\n", payload);

            // send message to server when Connected
            websocketManager.sendCurrentStatus("", "QUERY");
            break;
        case WStype_TEXT:
            Serial.printf("[WSc] get text: %s\n", payload);

            // send message to server
            // webSocket.sendTXT("message here");
            websocketManager.messageReceived(MessageIn::parseObject(payload));
            break;
        case WStype_PING:
            // pong will be sent automatically
            Serial.printf("[WSc] get ping\n");
            break;
        case WStype_PONG:
            // answer to a ping we send
            Serial.printf("[WSc] get pong\n");
            break;
    }
}

void setup() {
    Serial.begin(9600);

    networkManager.connectToNetwork(ssidNetwork, passwordNetwork);

    websocketManager.settingUpWebSocket(webSocketEvent);
    websocketManager.stepMotor->setSendStatusEvent(sendStatus);
}

void loop() {
    websocketManager.loop();
}
