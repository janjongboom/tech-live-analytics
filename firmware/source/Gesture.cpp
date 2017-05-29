#include "MicroBit.h"
#include "GestureService.h"

MicroBit uBit;

Serial pc(USBTX, USBRX);


void gesture(MicroBitEvent evt) {
    printf("Gesture %d\n", evt.value);

    if (evt.value == GESTURE_SHAKE) {
        uBit.display.print("S");
    }
    else if (evt.value == GESTURE_FREEFALL) {
        uBit.display.print("F");
    }
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_GESTURE, MICROBIT_EVT_ANY, &gesture, MESSAGE_BUS_LISTENER_IMMEDIATE);

    pc.baud(115200);

    new MicroBitGestureService(*uBit.ble);

    printf("Hello world\n");

    while(1)
    {
        uBit.sleep(100);
    }
}
