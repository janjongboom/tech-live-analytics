#ifndef MICROBIT_GESTURE_SERVICE_H
#define MICROBIT_GESTURE_SERVICE_H

#include "MicroBitConfig.h"
#include "ble/BLE.h"
#include "EventModel.h"

// UUIDs for our service and characteristics
extern const uint8_t  MicroBitGestureServiceUUID[];
extern const uint8_t  MicroBitGestureServiceDataUUID[];
extern const uint8_t  MicroBitGestureServicePeriodUUID[];


class MicroBitGestureService
{
    public:

    /**
      * Constructor.
      * Create a representation of the GestureService
      * @param _ble The instance of a BLE device that we're running on.
      */
    MicroBitGestureService(BLEDevice &_ble);


    private:



    /**
     * Gesture update callback
     */
    void gestureUpdate(MicroBitEvent e);

    // Bluetooth stack we're running on.
    BLEDevice               &ble;

    // memory for our 8 bit control characteristics.
    uint8_t            gestureDataCharacteristicBuffer[1];

    // Handles to access each characteristic when they are held by Soft Device.
    GattAttribute::Handle_t gestureDataCharacteristicHandle;
};


#endif

