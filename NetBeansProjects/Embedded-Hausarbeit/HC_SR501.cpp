/* 
 * File:   HC_SR501.cpp
 * Author: hauke
 * 
 * Created on 16. Juli 2015, 21:20
 */

#include "HC_SR501.h"

HCSR501::HCSR501() {
    gw = new gpioWork();
}
HCSR501::~HCSR501() {
    gw->~gpioWork();
}
int HCSR501::readHCSR501(){
    gw->setPinDirection(18,0);
    return gw->getPinValue(18);
}
