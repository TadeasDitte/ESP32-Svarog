#include "Rfid.h"

void Rfid::begin()
{
    pinMode(RX_PIN, INPUT);
    pinMode(TX_PIN, OUTPUT);
}
