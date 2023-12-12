# Feeder project

## Run the project

With platformIO installed you can use the next commands in the project:

```
// build aquarium feeder
DEVICE_ID= \
 DEVICE_NAME= \
 DEVICE_TYPE=action.devices.types.OUTLET \
 NETWORK_SSID= \
 NETWORK_PASSWORD= \
 pio run -e nodemcuv2
 

// build/upload aquarium feeder
DEVICE_ID= \
 DEVICE_NAME= \
 DEVICE_TYPE=action.devices.types.OUTLET \
 NETWORK_SSID= \
 NETWORK_PASSWORD= \
 pio run -e nodemcuv2 -t upload

// see arduino logs
pio device monitor

// run tests
pio test -e native
```

> Note: `pio` is the short of `platformio`.

## Get CA from server url

```
openssl s_client -showcerts -verify 5 -connect home.guilledev.com:443
```
