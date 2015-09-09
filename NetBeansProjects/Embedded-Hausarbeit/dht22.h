/* 
 * File:   dht22.h
 * Author: hauke
 *
 * Created on 16. Juli 2015, 11:45
 */

#ifndef DHT22_H
#define	DHT22_H
#include "gpioWork.h"
class dht22 {
public:    
    dht22();
    int readDHT(int pin, float *temp_p, float *hum_p);
    ~dht22();
    gpioWork *gw;
};

#endif	/* DHT22_H */

