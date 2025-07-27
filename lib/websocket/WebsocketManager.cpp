//
// Created by guille on 30.06.22.
//

#include "WebsocketManager.h"
#include "certs.h"
#include "MessageOut.h"

bool WebsocketManager::isConnectionAlive() {
	return millis() - lastConnection <= CONNECTION_LOST_TIMEOUT ||
	       webSocket.isConnected();
}

void WebsocketManager::settingUpWebSocket(const WebSocketsClient::WebSocketClientEvent &webSocketClientEvent,
                                          uint16_t port,
                                          const char *host,
                                          const char *url) {
	if (flashManager->isSecure) {
		webSocket.beginSslWithCA(host, port, url, home_url_CA, "wss");
	} else {
		webSocket.begin(host, port, url);
	}

	// event handler
	webSocket.onEvent(webSocketClientEvent);

	// use HTTP Basic Authorization this is optional remove if not needed
	//    webSocket.setAuthorization("user", "Password");

	// try ever 5000 again if connection has failed
	webSocket.setReconnectInterval(5000);

	// initialize lastConnectionValue
	lastConnection = millis();

	// Initialize step motor
	stepMotor = std::make_unique<StepMotor>();
}

void WebsocketManager::messageReceived(MessageIn msg) {
	if (!std::strcmp(msg.payload.messageType, "EXECUTE")) {
		if (msg.payload.command.start) {
			stepMotor->startRotation();
		}
	}
	sendCurrentStatus(msg.mid, msg.payload.messageType);
}

void WebsocketManager::sendCurrentStatus(const char *mid, const char *messageType) {
	char json[400];
	MessageOut::buildOutMessage(
		mid,
		messageType,
		config.ID,
		config.type,
		config.name,
		stepMotor->isRunning(),
		json);
	webSocket.sendTXT(json);
}

void WebsocketManager::loop() {
	if (webSocket.isConnected()) {
		lastConnection = millis();
	}

	webSocket.loop();
	stepMotor->loop();
}
