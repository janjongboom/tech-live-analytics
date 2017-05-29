/**
  * Class definition for the custom MicroBit Gesture Service.
  * Provides a BLE service to remotely read the state of the Gesture, and configure its behaviour.
  */
#include "MicroBitConfig.h"
#include "ble/UUID.h"

#include "GestureService.h"

/**
  * Constructor.
  * Create a representation of the GestureService
  * @param _ble The instance of a BLE device that we're running on.
  */
MicroBitGestureService::MicroBitGestureService(BLEDevice &_ble) :
        ble(_ble)
{
    // Create the data structures that represent each of our characteristics in Soft Device.
    GattCharacteristic  gestureDataCharacteristic(MicroBitGestureServiceDataUUID, (uint8_t *)gestureDataCharacteristicBuffer, 0,
    sizeof(gestureDataCharacteristicBuffer), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);

    // Initialise our characteristic values.
    gestureDataCharacteristicBuffer[0] = 0;

    // Set default security requirements
    gestureDataCharacteristic.requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

    GattCharacteristic *characteristics[] = {&gestureDataCharacteristic};
    GattService         service(MicroBitGestureServiceUUID, characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));

    ble.addService(service);

    gestureDataCharacteristicHandle = gestureDataCharacteristic.getValueHandle();

    ble.gattServer().write(gestureDataCharacteristicHandle,(uint8_t *)gestureDataCharacteristicBuffer, sizeof(gestureDataCharacteristicBuffer));

    if (EventModel::defaultEventBus)
        EventModel::defaultEventBus->listen(MICROBIT_ID_GESTURE, MICROBIT_EVT_ANY, this, &MicroBitGestureService::gestureUpdate,  MESSAGE_BUS_LISTENER_IMMEDIATE);
}

/**
  * Gesture update callback
  */
void MicroBitGestureService::gestureUpdate(MicroBitEvent evt)
{
    if (ble.getGapState().connected)
    {
        gestureDataCharacteristicBuffer[0] = static_cast<uint8_t>(evt.value);

        ble.gattServer().notify(gestureDataCharacteristicHandle,(uint8_t *)gestureDataCharacteristicBuffer, sizeof(gestureDataCharacteristicBuffer));
    }
}

const uint8_t  MicroBitGestureServiceUUID[] = {
    0x8e, 0xc0, 0xff, 0xea, 0xe5, 0x0e, 0x48, 0xab, 0x8b, 0x90, 0xf7, 0xd7, 0x62, 0x2e, 0x82, 0xb5
};

const uint8_t  MicroBitGestureServiceDataUUID[] = {
    0x8e, 0xc0, 0xff, 0xea, 0xe5, 0x0e, 0x48, 0xab, 0x8b, 0x90, 0xf7, 0xd7, 0x62, 0x2e, 0x82, 0xb6
};

