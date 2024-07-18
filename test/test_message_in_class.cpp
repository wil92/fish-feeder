#include <unity.h>
#include <iostream>
#include <search.h>
#include "../lib/communication/MessageIn.h"
#include "../lib/communication/MessageOut.h"

void test_decode_message_from_json() {
    unsigned char payload[] = "{\n  \"mid\": \"LmyWgTCcZp\",\n  \"payload\": {\n    \"messageType\": \"QUERY\",\n    \"command\": {\n      \"start\": true\n    }\n  }\n}\n";
    MessageIn msg = MessageIn::parseObject(&payload[0]);

    std::cout << "++++++++++++++++++++++++++++++++\n";
    std::cout << msg.mid << '\n';
    std::cout << "++++++++++++++++++++++++++++++++\n";
    TEST_ASSERT_EQUAL(strcmp("LmyWgTCcZp", msg.mid), 0);
    TEST_ASSERT_EQUAL(strcmp("QUERY", msg.payload.messageType), 0);
    TEST_ASSERT_TRUE(msg.payload.command.start);
}

void test_encode_message_to_json() {
    const char *expected = "{\"mid\":\"LmyWgTCcZp\",\"messageType\":\"QUERY\",\"payload\":{\"id\":\"123456789\",\"type\":\"action.devices.types.PETFEEDER\",\"name\":{\"name\":\"n1\"},\"isRunning\":false}}";

    char json[300];
    MessageOut::buildOutMessage("LmyWgTCcZp", "QUERY", "123456789", "action.devices.types.PETFEEDER", "n1", false, json);

    TEST_ASSERT_EQUAL(strcmp(expected, json), 0);
}

#ifdef ARDUINO

void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_decode_message_from_json);

    UNITY_END();
}

void loop() {}

#else

int main(int argc, char **argv) {
    UNITY_BEGIN();

    RUN_TEST(test_decode_message_from_json);
    RUN_TEST(test_encode_message_to_json);

    UNITY_END();
}

#endif
