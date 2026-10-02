//
// Created by guille on 22.06.22.
//

#ifndef SMART_HOME_AUTHMESSAGEIN_H
#define SMART_HOME_AUTHMESSAGEIN_H

#include <ArduinoJson.h>

struct AuthMessageIn {
    bool status;

    static AuthMessageIn parseObject(unsigned char *payload);
};


#endif //SMART_HOME_AUTHMESSAGEIN_H
