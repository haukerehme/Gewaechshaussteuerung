/* 
 * File:   Relais.h
 * Author: hauke
 *
 * Created on 27. August 2015, 00:44
 */

#ifndef RELAIS_H
#define	RELAIS_H
#include "gpioWork.h"
class Relais{
public:
    Relais();
    ~Relais();
    
    void setRelayOn(int pin, int sekunden);
    gpioWork * gw;
    
    int fensterAuf;
    int fensterZu;
    int wasserAuf;
    
};


#endif	/* RELAIS_H */

