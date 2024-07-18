//
// Created by guille on 30.06.22.
//

#include "WebsocketManager.h"

WebsocketManager::WebsocketManager(DeviceConfig config) {
    WebsocketManager::config = config;
    this->stepMotor = std::make_unique<StepMotor>();
}

void WebsocketManager::settingUpWebSocket(WebSocketsClient::WebSocketClientEvent webSocketClientEvent) {

    uint16_t port = PORT;
    const char *host = TOSTRING(HOST);
    const char *url = TOSTRING(URL);

#if defined(WSS) && WSS == 1
    auto *certCA = new X509List(home_url_CA);
    webSocket.beginSslWithCA(host, port, url, certCA, "wss");
#else
    webSocket.begin(host, port, url);
#endif

    // event handler
    webSocket.onEvent(std::move(webSocketClientEvent));

    // use HTTP Basic Authorization this is optional remove if not needed
//    webSocket.setAuthorization("user", "Password");

    // try ever 5000 again if connection has failed
    webSocket.setReconnectInterval(5000);
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
    webSocket.loop();
    stepMotor->loop();
}
