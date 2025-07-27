//
// Created by guille on 30.06.22.
//

#ifndef SMART_HOME_WEBSOCKETMANAGER_H
#define SMART_HOME_WEBSOCKETMANAGER_H

#define CONNECTION_LOST_TIMEOUT 60000

#include <SocketIOclient.h>
#include "../communication/MessageIn.h"
#include "../flash/FlashManager.h"
#include "../motor/StepMotor.h"

typedef std::function<void(bool status)> UpdateStatusEvent;
typedef std::function<void(void)> ResetFunction;

struct DeviceConfig {
	const char *ID;
	const char *type;
	const char *name;
};

class WebsocketManager {
	WebSocketsClient webSocket;
	UpdateStatusEvent updateStatusEvent;

	unsigned long lastConnection = 0;

public:
	FlashManager *flashManager = nullptr;
	std::unique_ptr<WiFiServer> server;
	ResetFunction rf;
	std::unique_ptr<StepMotor> stepMotor;
	DeviceConfig config{};

	bool isConnectionAlive();

	void settingUpWebSocket(const WebSocketsClient::WebSocketClientEvent &webSocketClientEvent,
							uint16_t port,
							const char *host,
							const char *url);

	void messageReceived(MessageIn msg);

	void sendCurrentStatus(const char *mid, const char *messageType);

	void loop();
};


#endif //SMART_HOME_WEBSOCKETMANAGER_H
