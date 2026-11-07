#include <Arduino.h>

#include "AuthMessageIn.h"
#include "../lib/network/NetworkManager.h"
#include "../lib/flash/FlashManager.h"
#include "../lib/websocket/WebsocketManager.h"
#include "../lib/webserver/WebServerManager.h"

#define RESET_CONFIG_PIN 16

#ifndef DEBUG
#define DEBUG 0
#endif

NetworkManager networkManager;
FlashManager flashManager;
WebServerManager webServer;
WebsocketManager websocketManager;

void resetFunc() {
    ESP.restart();
};

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

            // send client token auth
            websocketManager.authorized = false;
            websocketManager.sendClientTokenAuth();

            // send message to server when Connected
            websocketManager.sendCurrentStatus("", "QUERY");
            break;
        case WStype_TEXT:
            Serial.printf("[WSc] get text: %s\n", payload);

            if (!websocketManager.authorized) {
                // check if authorized
                if (const auto [status] = AuthMessageIn::parseObject(payload); status) {
                    websocketManager.authorized = true;
                    Serial.printf("[WSc] Authorized\n");
                }
            } else {
                // send message to server
                websocketManager.messageReceived(MessageIn::parseObject(payload));
            }

            break;
        case WStype_PING:
        case WStype_PONG:
        case WStype_ERROR:
        case WStype_BIN:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
        default:
            break;
    }
}

void resetConfigWhenButtonPress() {
    if (digitalRead(RESET_CONFIG_PIN) == LOW) {
        Serial.println("Reset configuration executed");
        flashManager.resetSetup();

        while (digitalRead(RESET_CONFIG_PIN) == LOW) {
            delay(100);
        }

        resetFunc();
    }
}

// todo: for testing (remove)
unsigned long heapLogsTime = 0;

void setup() {
    Serial.begin(9600);
    EEPROM.begin(512);

    // todo: remove
    heapLogsTime = millis();

    flashManager.loadSetup();

    pinMode(RESET_CONFIG_PIN, INPUT_PULLUP);

    // reset configuration if button is pressed when arduino starts
    resetConfigWhenButtonPress();

    if (flashManager.isSetup) {
        // connect to network
        networkManager.connectToNetwork(flashManager.ssidNetwork.c_str(), flashManager.passwordNetwork.c_str());

        websocketManager.flashManager = &flashManager;
        websocketManager.config = {
            flashManager.ID.c_str(),
            flashManager.type.c_str(),
            flashManager.name.c_str(),
            flashManager.clientToken.c_str()
        };

#if DEBUG
        Serial.println("Configuring network");
        Serial.println(
            "Host: " + flashManager.host + "\nPort: " + String(flashManager.port) + "\nUrl: " + flashManager.url);
#endif

        websocketManager.settingUpWebSocket(webSocketEvent, flashManager.port, flashManager.host.c_str(),
                                            flashManager.url.c_str());
        websocketManager.stepMotor->setSendStatusEvent(sendStatus);
    } else {
        networkManager.scanNetworks();

        // create hotpot
        if (!networkManager.createHotpot(flashManager.ssid, flashManager.password)) {
            // reset arduino if hotpot fails to start
            resetFunc();
        }
        delay(100);

        // start web server
        webServer.flashManager = &flashManager;
        webServer.networkManager = &networkManager;
        webServer.rf = resetFunc;
        webServer.setup();
        webServer.run();
    }
}

void loop() {
    // check if action pin is pressed
    if (flashManager.isSetup) {
        websocketManager.loop();

        // check if websocket is connected or reset arduino
        // if (!websocketManager.isConnectionAlive()) {
        //     Serial.println("Connection lost");
        //     resetFunc();
        // }

        resetConfigWhenButtonPress();
    } else {
        // run web server
        webServer.loop();
    }

#if DEBUG
    if (millis() - heapLogsTime >= 5000) {
        // todo: remove
        heapLogsTime = millis();
        Serial.println("Heap logs time: " + String(heapLogsTime));
        Serial.println("Free heap: " + String(ESP.getFreeHeap()));
        Serial.println("Reset info: " + String(ESP.getResetInfo()));
    }
#endif
}
