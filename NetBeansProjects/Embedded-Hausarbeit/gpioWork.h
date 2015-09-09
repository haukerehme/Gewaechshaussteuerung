/* 
 * File:   gpioWork.h
 * Author: hauke
 *
 * Created on 16. Juli 2015, 11:04
 */

#ifndef GPIOWORK_H
#define	GPIOWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class gpioWork{
    
private:
    volatile unsigned int *getMapRegister(unsigned long baseAddr);
    volatile unsigned int *gpio; 
    //Konstanten
public: 
    static const unsigned int registerLaenge =   0xB4;
    //GPIO Addresse (0x3f200000 für den Rasp2)
    static const unsigned int GPIO_VERZEICHNIS = 0x20200000;
    
    static const unsigned int INPUT = 0;
    static const unsigned int OUTPUT = 1;
    
    static const unsigned int LOW = 0;
    static const unsigned int HIGH = 1;
    
    /*static const unsigned int GPFSET0 = 7;
    static const unsigned int GPFCLR0 = 10;
    static const unsigned int GPFLEV0 = 13;
    static const unsigned int GPFSEL0 = 0;
    static const unsigned int GPFSEL1 = 1;
    static const unsigned int GPFSEL2 = 2;
    static const unsigned int GPFSEL3 = 3;*/
    
    gpioWork();
    ~gpioWork();
    
    void setPinDirection(unsigned int pin, const unsigned &direction);
    
    unsigned int getPinValue(unsigned int pin);
        
    void inline setPinHigh(unsigned int pin){*(this->gpio + 7) = (1 << pin);}
 
    void inline setPinLow(unsigned int pin){*(this->gpio + 10) = (1 << pin);}
};

#endif