# Feeder project

**Run the project**

With platformIO installed you can use the next commands in the project:

```
// build and upload project (in ESP8266-ESP01S hardware)
pio run -e esp01 -t upload

// build and upload project (in Nodemcu hardware)
pio run -e nodemcuv2 -t upload

// see arduino logs
pio device monitor

// run tests
pio test -e native

// build local
URL=/ws/ HOST=home.guilledev.com PORT=3000 WSS=1 NETWORK_SSID="" NETWORK_PASSWORD= pio run -e nodemcuv2 -t upload --upload-port /dev/ttyUSB0
```

> Note: `pio` is the short of `platformio`.
