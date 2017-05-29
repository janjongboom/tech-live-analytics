# Tracking physical product engagement with Google Analytics

Demo for Emerce Tech Live! 2017 by Jan Jongboom.

## Setup

1. Flash `firmware/bin/tech-live-nrf51-microbit.hex` onto one or more [BBC micro:bit's](https://developer.mbed.org/platforms/Microbit/).
1. Install [mbed Cloud Bluetooth Devicelink](https://github.com/ARMmbed/cloud-bluetooth-devicelink).
1. Add the micro:bit's in Bluetooth Devicelink with the following read definition:

    ```js
    {
        "deviceinfo/0/model-number": function (m) {
            return m['180a']['2a24'].toString('ascii');
        },
        "deviceinfo/0/serial-number": function (m) {
            return m['180a']['2a25'].toString('ascii');
        },
        "gesture/0/value": function (m) {
            return m['8ec0ffeae50e48ab8b90f7d7622e82b5']['8ec0ffeae50e48ab8b90f7d7622e82b6'][0];
        }
    }
    ```

1. Open `ga-service/server.js` and add:
    * Your mbed Device Connector Access Key.
    * Your Google Analytics account ID (starts with UA-).
1. Run the application via:

    ```
    $ node ga-service/server.js
    ```

1. When you shake one of the micro:bit's events will appear in Google Analytics.

## Re-building the firmware

Load this repository in the mbed Online Compiler and build for the micro:bit target. Alternatively: build locally using mbed CLI and ARMCC (does not build with GCC_ARM).
