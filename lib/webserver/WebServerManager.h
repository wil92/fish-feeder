//
// Created by guillermo on 11/18/24.
//

#ifndef WEBSERVERMANAGER_H
#define WEBSERVERMANAGER_H

#include <WiFiServer.h>
#include <WiFiClient.h>
#include "../flash/FlashManager.h"
#include "../network/NetworkManager.h"

#define CLIENT_TIMEOUT 2000

typedef std::function<void(void)> ResetFunction;

class WebServerManager {
    bool checkEndpoints(WiFiClient *client, const String &message) const;

    [[nodiscard]] String prepareBaseHeaders() const;

    [[nodiscard]] String prepareHeaders() const;

    [[nodiscard]] String prepareHtmlPage() const;

    [[nodiscard]] String prepareJSONHeaders(int contentLength) const;

    [[nodiscard]] String prepareSSIDsJson() const;

    void checkForConnections();

    void setParameter(const String &key, const String &value) const;

    static String urlDecode(String &src);

public:
    FlashManager *flashManager = nullptr;
    NetworkManager *networkManager = nullptr;
    std::unique_ptr<WiFiServer> server;
    ResetFunction rf;

    void setup();

    void run() const;

    void loop();
};


#endif //WEBSERVERMANAGER_H
