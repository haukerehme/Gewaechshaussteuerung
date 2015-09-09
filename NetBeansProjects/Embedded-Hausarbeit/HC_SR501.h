/* 
 * File:   HC_SR501.h
 * Author: hauke
 *
 * Created on 16. Juli 2015, 21:20
 */

#ifndef HCSR501_H
#define	HCSR501_H
#include "gpioWork.h"



class HCSR501 {
public:
    HCSR501();
    ~HCSR501();
    int readHCSR501();
    gpioWork *gw;
};

#endif	/* HCSR501_H */

