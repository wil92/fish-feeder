//
// Created by guillermo on 11/18/24.
//

#ifndef FLASHMANAGER_H
#define FLASHMANAGER_H

#include <EEPROM.h>
#include <WString.h>

#define STRINGIGY(v) #v
#define TOSTRING(v) STRINGIGY(v)

#define IS_ENABLE_FLAG 7
#define CONFIG_STATUS_ADDRESS 0
#define CONFIG_DATA_ADDRESS 1


class FlashManager {
	bool cameraStatus = false;

	void saveString(String value, int *index);

	String loadString(int *index);

public:
	bool isSetup = false;

	// Hotpot. todo: add this variables to the env
	const char *ssid = "SmartHomeDevice_FishFeeder";
	const char *password = "smarthome";

	// Network
	String ssidNetwork;
	String passwordNetwork;

	// Server config
	uint16_t port = 443;
	String host;
	String url;
	bool isSecure = true;

	// Device configuration
	String ID = "";
	String name = "Camera";
	String type = "action.devices.types.PETFEEDER";

	void loadSetup();

	void saveSetup();

	void resetSetup();

	void setVariable(const String &key, const String &value);
};


#endif //FLASHMANAGER_H
