//
// Created by guillermo on 10/2/26.
//

#ifndef FISH_FEEDER_AUTHMESSAGE_H
#define FISH_FEEDER_AUTHMESSAGE_H

#include <ArduinoJson.h>

struct AuthMessageOut {
    static void buildOutMessage(const char *clientToken, char *json);
};

#endif //FISH_FEEDER_AUTHMESSAGE_H
