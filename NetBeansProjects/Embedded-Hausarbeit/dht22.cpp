/* 
 * File:   dht22.cpp
 * Author: hauke
 * 
 * Created on 16. Juli 2015, 11:45
 */

#include "dht22.h"
#include "gpioWork.h"

#define MaxDurchlaeufe 100

//Konstruktor.
dht22::dht22() {
    gw = new gpioWork();
}

dht22::~dht22() {
    gw->~gpioWork();
}

int dht22::readDHT(int pin, float *temperatur, float *luftfeuchtigkeit)
{
    int counter = 0;
    int tmpStatus = gw->HIGH;
    int j = 0;
    int data[5] = {0};   
    
    gw->setPinDirection(pin,1);
    gw->setPinHigh(pin);
    usleep(50000);  
    //Pin Low setzen
    gw->setPinLow(pin);
    usleep(20000);  
    gw->setPinHigh(pin);
    //GPIO als INPUT setzen
    gw->setPinDirection(pin,0);
    
    //Warten auf dht
    //Read GPIO INPUT
    while (gw->getPinValue(pin) == 1) {
                //usleep(1);
    }
    
    // Auslesen der Daten beginnt
    for (int i = 0; i < MaxDurchlaeufe; i++) {
        counter = 0;
        
        while ( gw->getPinValue(pin) == tmpStatus) {
            counter++;
            if (counter == 1000){
                break;
            }
        }
        //Read GPIO INPUT
        tmpStatus = gw->getPinValue(pin);
        
        if (counter == 1000){
            break;
        }
        if ((i>3) && (i%2 == 0)) {
        //if(true){
            data[j/8] <<= 1;
            if (counter > 200)
                data[j/8] |= 1;
            j++;
        }
    }
    
    if (j >= 39) {
        int checksumme = (data[0] + data[1] + data[2] + data[3]) & 0xFF;
        
        if (data[4] == checksumme) {
            *luftfeuchtigkeit = data[0] * 256 + data[1];
            *luftfeuchtigkeit /= 10;
            
            *temperatur = (data[2] & 0x7F)* 256 + data[3];
            *temperatur /= 10.0;
            
            //Schauen ob erstes Bit = 1
            //Wenn erstes Bit = 1 ist die Temperatur negativ
            if(data[2] & 0x80){
                *temperatur *= -1;
            }
            //return 0 für erfolgreiches Lesen der Daten
            return 0;
        }
        return -2;
    }
    return -1;
}