//
// Created by guille on 22.06.22.
//

#include "MessageIn.h"

MessageIn MessageIn::parseObject(unsigned char *payload) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
//        Serial.print(F("deserializeJson() failed: "));
//        Serial.println(error.f_str());
        return {};
    }

    const char *messageId = "";
    MessagePayload messagePayload = {};
    if (doc.containsKey("mid")) {
        messageId = doc["mid"];
    }
    if (doc.containsKey("payload")) {
        const char *messageType = "";
        MessagePayload::MessageCommand command = {};
        if (doc["payload"].containsKey("messageType")) {
            messageType = doc["payload"]["messageType"];
        }
        if (doc["payload"].containsKey("command")) {
            bool start = false;
            if (doc["payload"]["command"].containsKey("start")) {
                start = doc["payload"]["command"]["start"];
            }
            command = {start};
        }
        messagePayload = {messageType, command};
    }

    return {messageId, messagePayload};
}
