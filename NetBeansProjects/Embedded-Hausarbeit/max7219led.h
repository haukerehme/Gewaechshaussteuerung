/* 
 * File:   max7219led.h
 * Author: hauke
 *
 * Created on 16. Juli 2015, 23:48
 */

#ifndef MAX7219LED_H
#define	MAX7219LED_H

using namespace std; 
#include <stdint.h>
#include <vector>
#include "gpioWork.h"
// ---------------------    // Vcc, Pin 1 on LED8x8 Board
// ---------------------    // GND, Pin 2 on LED8x8 Board
#define MAX7219_DIN     10 // DI,  Pin 3 on LED8x8 Board
#define MAX7219_CS      8 // CS,  Pin 4 on LED8x8 Board
#define MAX7219_CLK     11 // CLK, Pin 5 on LED8x8 Board


class MAX7219{
public:
    int zeichenT[8][8];
    int zeichenE[8][8];
    int zeichenF[8][8];
    int zeichenL[8][8];
    int zeichenC[8][8];
    int zeichenKomma[8][8];
    int zeichenGrad[8][8];
    int zeichenProzent[8][8];
    int zeichenDoppelpunkt[8][8];
    
    int zeichen0[8][8];
    int zeichen1[8][8];
    int zeichen2[8][8];
    int zeichen3[8][8];
    int zeichen4[8][8];
    int zeichen5[8][8];
    int zeichen6[8][8];
    int zeichen7[8][8];
    int zeichen8[8][8];
    int zeichen9[8][8];
    
    struct Punkt {int x; int y;};
    vector<Punkt> text;
    uint8_t MAX7219_buffer[8];
    
    MAX7219();
    ~MAX7219();
    void deleteAll();
    void MAX7219_byte(uint8_t data);
    void MAX7219_word(uint8_t address,uint8_t dat);
    void MAX7219_init(void);
    void MAX7219_row(uint8_t address,uint8_t dat);
    void MAX7219_buffer_out(void);
    void MAX7219_buffer_set(uint8_t x, uint8_t y);
    void MAX7219_buffer_clr(uint8_t x, uint8_t y);
    void displayZeichen(int zweiDimArray[8][8]);
    void displayText();
    int findMax(vector<Punkt> ausgabe);
    void buchstabenHinzu(int buchstabe[8][8]);
    gpioWork *gw;
};
#endif	/* MAX7219LED_H */

