#include <Arduino.h>

#include "../lib/network/NetworkManager.h"
#include "../lib/flash/FlashManager.h"
#include "../lib/websocket/WebsocketManager.h"
#include "../lib/webserver/WebServerManager.h"

#define RESET_CONFIG_PIN 16

NetworkManager networkManager;
FlashManager flashManager;
WebServerManager webServer;
WebsocketManager websocketManager;

void (*resetFunc)(void) = 0;

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
			websocketManager.messageReceived(MessageIn::parseObject(payload));
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
		digitalWrite(RESET_CONFIG_PIN, LOW);
		Serial.println("Reset configuration executed");
		flashManager.resetSetup();
		resetFunc();
	}
}

void setup() {
	Serial.begin(9600);
	EEPROM.begin(512);

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
			flashManager.name.c_str()
		};
		websocketManager.settingUpWebSocket(webSocketEvent, flashManager.port, flashManager.host.c_str(),
		                                    flashManager.url.c_str());
		websocketManager.stepMotor->setSendStatusEvent(sendStatus);
	} else {
		// create hotpot
		if (!networkManager.createHotpot(flashManager.ssid, flashManager.password)) {
			// reset arduino if hotpot fails to start
			resetFunc();
		}
		delay(100);

		// start web server
		webServer.flashManager = &flashManager;
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
		if (!websocketManager.isConnectionAlive()) {
			Serial.println("Connection lost");
			resetFunc();
		}
	} else {
		// scan networks
		webServer.loop();
	}
}
