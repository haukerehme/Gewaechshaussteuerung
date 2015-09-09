
#include "Relais.h"
#include "gpioWork.h"
#include "gpioWork.h"

void Relais::setRelayOn(int pin, int sekunden){
    gw->setPinDirection(pin,1);
    gw->setPinLow(pin);
    sleep(sekunden);
    gw->setPinHigh(pin);
    gw->~gpioWork();
}

Relais::Relais(){
    gw = new gpioWork(); 
    Relais::fensterAuf = 19;
    Relais::fensterZu = 26;
    Relais::wasserAuf = 21;
}

Relais::~Relais(){
    gw->~gpioWork();
}