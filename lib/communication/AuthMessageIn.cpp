//
// Created by guillermo on 10/2/26.
//

#include "AuthMessageIn.h"


AuthMessageIn AuthMessageIn::parseObject(unsigned char *payload) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return {};
    }
    bool status = false;
    if (doc["status"].is<bool>()) {
        status = doc["status"].as<bool>();
    }
    return {status};
}
