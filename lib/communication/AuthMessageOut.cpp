//
// Created by guillermo on 10/2/26.
//

#include "AuthMessageOut.h"


void AuthMessageOut::buildOutMessage(const char *clientToken, char *json) {
    DynamicJsonDocument doc(256);
    doc["token"] = clientToken;

    serializeJson(doc, json, 256);
}
