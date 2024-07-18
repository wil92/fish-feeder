//
// Created by guille on 30.06.22.
//

#ifndef SMART_HOME_WEBSOCKETMANAGER_H
#define SMART_HOME_WEBSOCKETMANAGER_H

#define STRINGIGY(v) #v
#define TOSTRING(v) STRINGIGY(v)

#include <SocketIOclient.h>
#include "certs.h"
#include "../communication/MessageIn.h"
#include "../communication/MessageOut.h"
#include "../motor/StepMotor.h"

typedef std::function<void(bool status)> UpdateStatusEvent;

struct DeviceConfig {
    const char *ID;
    const char *type;
    const char *name;
};

class WebsocketManager {
private:
    WebSocketsClient webSocket;
    UpdateStatusEvent updateStatusEvent;
    DeviceConfig config;

public:
    std::unique_ptr<StepMotor> stepMotor;

    WebsocketManager(DeviceConfig config);

    void settingUpWebSocket(WebSocketsClient::WebSocketClientEvent webSocketClientEvent);

    void messageReceived(MessageIn msg);

    void sendCurrentStatus(const char *mid, const char *messageType);

    void loop();
//
//    void onUpdateStatusEvent(UpdateStatusEvent updateStatusEvent);
};


#endif //SMART_HOME_WEBSOCKETMANAGER_H
