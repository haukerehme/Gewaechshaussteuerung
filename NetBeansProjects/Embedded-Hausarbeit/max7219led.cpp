/* 
 * File:   main.cpp
 * Author: hauke
 *
 * Created on 16. Juli 2015, 11:58
 */
 
#include <stdlib.h>
#include "gpioWork.h"
 
#include "max7219led.h"
 
void MAX7219::MAX7219_byte(uint8_t data)
{
    gw->setPinDirection(MAX7219_CS,1);
    gw->setPinDirection(MAX7219_CLK,1);
    gw->setPinDirection(MAX7219_DIN,1);
    gw->setPinLow(MAX7219_CS);
    //PORTB &= ~(1 << MAX7219_CS);    // Set to LOW
    for(uint8_t i = 8; i >= 1; i--)
    {
        gw->setPinLow(MAX7219_CLK);
        //PORTB &= ~(1 << MAX7219_CLK);   // Set to LOW
        if ((data & 0x80) != 0){         // Mask the MSB of the data
            gw->setPinHigh(MAX7219_DIN);
            //PORTB |= (1 << MAX7219_DIN);    // Set to HIGH
        }
        else{
            gw->setPinLow(MAX7219_DIN);
            //PORTB &= ~(1 << MAX7219_DIN);   // Set to LOW
        }
        data = data<<1;
        
        gw->setPinHigh(MAX7219_CLK);
        //PORTB |= (1 << MAX7219_CLK);        // Set to HIGH
    }
}
 
void MAX7219::MAX7219_word(uint8_t address, uint8_t data)
{
    gw->setPinLow(MAX7219_CS);
    //PORTB &= ~(1 << MAX7219_CS);    // Set to LOW
    MAX7219_byte(address);          //
    MAX7219_byte(data);             //
    gw->setPinHigh(MAX7219_CS);
    //PORTB |= (1 << MAX7219_CS);     // Set to HIGH
}
 
void MAX7219::MAX7219_init(void)
{
    /*DDRB |= (1 << MAX7219_CLK); // Set port as output
    DDRB |= (1 << MAX7219_CS);  // Set port as output
    DDRB |= (1 << MAX7219_DIN); // Set port as output*/
    gw->setPinDirection(MAX7219_CS,0);
    sleep(1);
    gw->setPinDirection(MAX7219_CS,1);
    gw->setPinDirection(MAX7219_CLK,1);
    gw->setPinDirection(MAX7219_DIN,1);
    usleep(50000);
    
    
    MAX7219_word(0x09, 0x00);   // Decode: BCD
    MAX7219_word(0x0a, 0x03);   // Brightness
    MAX7219_word(0x0b, 0x07);   //
    MAX7219_word(0x0c, 0x01);   //
    MAX7219_word(0x0f, 0x00);   //
}
 
void MAX7219::MAX7219_row(uint8_t address, uint8_t data) {
    gw->setPinLow(MAX7219_CS);
    if (address >= 1 && address <= 8) MAX7219_word(address, data);
    gw->setPinHigh(MAX7219_CS);
}
 
void MAX7219::MAX7219_buffer_out(void) {
    // Output the buffer
    for (uint8_t row = 1; row <= 8; row++)
        MAX7219_row(row, MAX7219_buffer[row - 1]);
}
 
void MAX7219::MAX7219_buffer_set(uint8_t x, uint8_t y) {
    uint8_t sx = 7 - (x & 0b0111);
    uint8_t sy = (y & 0b0111);
    MAX7219_buffer[sy] |= (1 << sx);
}
 
void MAX7219::MAX7219_buffer_clr(uint8_t x, uint8_t y) {
    uint8_t sx = 7 - (x & 0b0111);
    uint8_t sy = (y & 0b0111);
    MAX7219_buffer[sy] &= ~(1 << sx);
}

void MAX7219::deleteAll(){
    for(int x=0;x<8;x++){
        for(int y=0;y<8;y++){            
            MAX7219_buffer_clr(x,y);
        }
    }
}


void MAX7219::displayZeichen(int zweiDimArray[8][8]){
    for(int x=0;x<8;x++){
        for(int y=0;y<8;y++){
            if(zweiDimArray[x][y]==1){
                MAX7219_buffer_set(x,y);
            }
            else{
                MAX7219_buffer_clr(x,y);
            }
        }
    }
}



void MAX7219::displayText(){
    //printf("Start displayText()\n");
    vector<MAX7219::Punkt> ausgabe = MAX7219::text;
    //printf("vector übergeben\n");
    int displayIndex=0;
    int maxX = findMax(MAX7219::text);
    //printf("maxX: %d\n", maxX);
    
    //printf("Start while\n");
    while(displayIndex-2 < maxX)
    {
        deleteAll();
        MAX7219_buffer_out();
        int zweiDimArray[8][8] = {0};
        
        //printf("displayIndex: %d\n", displayIndex);
        
        //zweiDimArray[8][8] = { 0 };
        for(int i = 0; i < ausgabe.size();i++){
            if(ausgabe.at(i).x<displayIndex+8 && ausgabe.at(i).x>=displayIndex)
            {
                zweiDimArray[ausgabe.at(i).x - displayIndex][ausgabe.at(i).y] = 1;
            }
        }
        displayIndex++;
        //printf("Print Led\n");
        displayZeichen(zweiDimArray);
        MAX7219_buffer_out(); // Output the buffer
        //usleep(200000);
        usleep(200000);
    }
    //printf("Ende displayText()\n");
}

void MAX7219::buchstabenHinzu(int buchstabe[8][8]){
    //printf("Start buchstabenHinzu()");
    //höchstes x finden
    int maxX = findMax(MAX7219::text);
    for(int x=0;x<8;x++){
        for(int y=0;y<8;y++){
           if(buchstabe[x][y]==1){
            struct Punkt pkt;
            pkt.x = x + maxX+2;
            pkt.y = y;
            MAX7219::text.push_back(pkt);
           }
        }
    }
    //printf("Ende buchstabenHinzu()");
}

int MAX7219::findMax(vector<Punkt> ausgabe){
    if(ausgabe.empty())
    {return 0;}
    int maxX = 0;
    for(int i = 0; i<ausgabe.size();i++){
        if(ausgabe.at(i).x>maxX)
        {
            maxX = ausgabe.at(i).x;
        }
    }
    return maxX;
}

MAX7219::MAX7219(){
    MAX7219::zeichenT[2][1] = 1;
    MAX7219::zeichenT[3][1] = 1;
    MAX7219::zeichenT[4][1] = 1;
    MAX7219::zeichenT[4][2] = 1;
    MAX7219::zeichenT[4][3] = 1;
    MAX7219::zeichenT[4][4] = 1;
    MAX7219::zeichenT[4][5] = 1;
    MAX7219::zeichenT[4][6] = 1;
    MAX7219::zeichenT[5][1] = 1;
    MAX7219::zeichenT[6][1] = 1;
    
    //E:
    MAX7219::zeichenE[2][1] = 1;
    MAX7219::zeichenE[2][2] = 1;
    MAX7219::zeichenE[2][3] = 1;
    MAX7219::zeichenE[2][4] = 1;
    MAX7219::zeichenE[2][5] = 1;
    MAX7219::zeichenE[2][6] = 1;
    MAX7219::zeichenE[3][1] = 1;
    MAX7219::zeichenE[3][4] = 1;
    MAX7219::zeichenE[3][6] = 1;
    MAX7219::zeichenE[4][1] = 1;
    MAX7219::zeichenE[4][4] = 1;
    MAX7219::zeichenE[4][6] = 1;
    MAX7219::zeichenE[5][1] = 1;
    MAX7219::zeichenE[5][6] = 1;

    //F:
    MAX7219::zeichenF[2][1] = 1;
    MAX7219::zeichenF[2][2] = 1;
    MAX7219::zeichenF[2][3] = 1;
    MAX7219::zeichenF[2][4] = 1;
    MAX7219::zeichenF[2][5] = 1;
    MAX7219::zeichenF[2][6] = 1;
    MAX7219::zeichenF[3][1] = 1;
    MAX7219::zeichenF[3][4] = 1;
    MAX7219::zeichenF[4][1] = 1;
    MAX7219::zeichenF[4][4] = 1;
    MAX7219::zeichenF[5][1] = 1;

    //L:
    MAX7219::zeichenL[2][1] = 1;
    MAX7219::zeichenL[2][2] = 1;
    MAX7219::zeichenL[2][3] = 1;
    MAX7219::zeichenL[2][4] = 1;
    MAX7219::zeichenL[2][5] = 1;
    MAX7219::zeichenL[2][6] = 1;
    MAX7219::zeichenL[3][6] = 1;
    MAX7219::zeichenL[4][6] = 1;
    MAX7219::zeichenL[5][6] = 1;

    //C:
    MAX7219::zeichenC[2][1] = 1;
    MAX7219::zeichenC[2][2] = 1;
    MAX7219::zeichenC[2][3] = 1;
    MAX7219::zeichenC[2][4] = 1;
    MAX7219::zeichenC[2][5] = 1;
    MAX7219::zeichenC[2][6] = 1;
    MAX7219::zeichenC[3][1] = 1;
    MAX7219::zeichenC[3][6] = 1;
    MAX7219::zeichenC[4][1] = 1;
    MAX7219::zeichenC[4][6] = 1;
    MAX7219::zeichenC[5][1] = 1;
    MAX7219::zeichenC[5][2] = 1;
    MAX7219::zeichenC[5][5] = 1;
    MAX7219::zeichenC[5][6] = 1;

    //,:
    MAX7219::zeichenKomma[1][6] = 1;
    MAX7219::zeichenKomma[1][7] = 1;

    //°:
    MAX7219::zeichenGrad[2][1] = 1;
    MAX7219::zeichenGrad[2][2] = 1;
    MAX7219::zeichenGrad[2][3] = 1;
    MAX7219::zeichenGrad[3][1] = 1;
    MAX7219::zeichenGrad[3][3] = 1;
    MAX7219::zeichenGrad[4][1] = 1;
    MAX7219::zeichenGrad[4][2] = 1;
    MAX7219::zeichenGrad[4][3] = 1;

    //%:
    MAX7219::zeichenProzent[2][1] = 1;
    MAX7219::zeichenProzent[2][5] = 1;
    //MAX7219::zeichenProzent[2][6] = 1;
    MAX7219::zeichenProzent[3][4] = 1;
    MAX7219::zeichenProzent[4][3] = 1;
    //MAX7219::zeichenProzent[5][1] = 1;
    MAX7219::zeichenProzent[5][2] = 1;
    MAX7219::zeichenProzent[5][6] = 1;

        MAX7219::zeichen0[2][1] = 1;
    MAX7219::zeichen0[2][2] = 1;
    MAX7219::zeichen0[2][3] = 1;
    MAX7219::zeichen0[2][4] = 1;
    MAX7219::zeichen0[2][5] = 1;
    MAX7219::zeichen0[2][6] = 1;
    MAX7219::zeichen0[3][1] = 1;
    MAX7219::zeichen0[3][6] = 1;
    MAX7219::zeichen0[4][1] = 1;
    MAX7219::zeichen0[4][6] = 1;
    MAX7219::zeichen0[5][1] = 1;
    MAX7219::zeichen0[5][2] = 1;
    MAX7219::zeichen0[5][3] = 1;
    MAX7219::zeichen0[5][4] = 1;
    MAX7219::zeichen0[5][5] = 1;
    MAX7219::zeichen0[5][6] = 1;

    MAX7219::zeichen1[2][3] = 1;
    MAX7219::zeichen1[3][2] = 1;
    MAX7219::zeichen1[4][1] = 1;
    MAX7219::zeichen1[4][2] = 1;
    MAX7219::zeichen1[4][3] = 1;
    MAX7219::zeichen1[4][4] = 1;
    MAX7219::zeichen1[4][5] = 1;
    MAX7219::zeichen1[4][6] = 1;

    MAX7219::zeichen2[2][2] = 1;
    MAX7219::zeichen2[3][1] = 1;
    MAX7219::zeichen2[4][1] = 1;
    MAX7219::zeichen2[5][1] = 1;
    MAX7219::zeichen2[5][2] = 1;
    MAX7219::zeichen2[4][3] = 1;
    MAX7219::zeichen2[3][4] = 1;
    MAX7219::zeichen2[2][5] = 1;
    MAX7219::zeichen2[2][6] = 1;
    MAX7219::zeichen2[3][6] = 1;
    MAX7219::zeichen2[4][6] = 1;
    MAX7219::zeichen2[5][6] = 1;

    MAX7219::zeichen3[2][1] = 1;
    MAX7219::zeichen3[2][6] = 1;
    MAX7219::zeichen3[3][1] = 1;
    MAX7219::zeichen3[3][6] = 1;
    MAX7219::zeichen3[4][1] = 1;
    MAX7219::zeichen3[4][3] = 1;
    MAX7219::zeichen3[4][4] = 1;
    MAX7219::zeichen3[4][6] = 1;
    MAX7219::zeichen3[5][1] = 1;
    MAX7219::zeichen3[5][2] = 1;
    MAX7219::zeichen3[5][4] = 1;
    MAX7219::zeichen3[5][5] = 1;
    MAX7219::zeichen3[5][6] = 1;

    MAX7219::zeichen4[2][1] = 1;
    MAX7219::zeichen4[2][2] = 1;
    MAX7219::zeichen4[2][3] = 1;
    MAX7219::zeichen4[2][4] = 1;
    MAX7219::zeichen4[3][4] = 1;
    MAX7219::zeichen4[4][3] = 1;
    MAX7219::zeichen4[4][4] = 1;
    MAX7219::zeichen4[4][5] = 1;
    MAX7219::zeichen4[4][6] = 1;
    MAX7219::zeichen4[5][4] = 1;

    MAX7219::zeichen5[2][1] = 1;
    MAX7219::zeichen5[2][2] = 1;
    MAX7219::zeichen5[2][3] = 1;
    MAX7219::zeichen5[2][6] = 1;
    MAX7219::zeichen5[3][1] = 1;
    MAX7219::zeichen5[3][3] = 1;
    MAX7219::zeichen5[3][6] = 1;
    MAX7219::zeichen5[4][1] = 1;
    MAX7219::zeichen5[4][3] = 1;
    MAX7219::zeichen5[4][4] = 1;
    MAX7219::zeichen5[4][6] = 1;
    MAX7219::zeichen5[5][1] = 1;
    MAX7219::zeichen5[5][4] = 1;
    MAX7219::zeichen5[5][5] = 1;
    MAX7219::zeichen5[5][6] = 1;

    MAX7219::zeichen6[2][3] = 1;
    MAX7219::zeichen6[2][4] = 1;
    MAX7219::zeichen6[2][5] = 1;
    MAX7219::zeichen6[2][6] = 1;
    MAX7219::zeichen6[3][2] = 1;
    MAX7219::zeichen6[3][4] = 1;
    MAX7219::zeichen6[3][6] = 1;
    MAX7219::zeichen6[4][1] = 1;
    MAX7219::zeichen6[4][4] = 1;
    MAX7219::zeichen6[4][6] = 1;
    MAX7219::zeichen6[5][1] = 1;
    MAX7219::zeichen6[5][4] = 1;
    MAX7219::zeichen6[5][5] = 1;
    MAX7219::zeichen6[5][6] = 1;

    MAX7219::zeichen7[2][1] = 1;
    MAX7219::zeichen7[2][5] = 1;
    MAX7219::zeichen7[2][6] = 1;
    MAX7219::zeichen7[3][1] = 1;
    MAX7219::zeichen7[3][4] = 1;
    MAX7219::zeichen7[4][1] = 1;
    MAX7219::zeichen7[4][3] = 1;
    MAX7219::zeichen7[5][1] = 1;
    MAX7219::zeichen7[5][2] = 1;

    MAX7219::zeichen8[2][1] = 1;
    MAX7219::zeichen8[2][2] = 1;
    MAX7219::zeichen8[2][3] = 1;
    MAX7219::zeichen8[2][4] = 1;
    MAX7219::zeichen8[2][5] = 1;
    MAX7219::zeichen8[2][6] = 1;
    MAX7219::zeichen8[3][1] = 1;
    MAX7219::zeichen8[3][3] = 1;
    MAX7219::zeichen8[3][4] = 1;
    MAX7219::zeichen8[3][6] = 1;
    MAX7219::zeichen8[4][1] = 1;
    MAX7219::zeichen8[4][3] = 1;
    MAX7219::zeichen8[4][4] = 1;
    MAX7219::zeichen8[4][6] = 1;
    MAX7219::zeichen8[5][1] = 1;
    MAX7219::zeichen8[5][2] = 1;
    MAX7219::zeichen8[5][3] = 1;
    MAX7219::zeichen8[5][4] = 1;
    MAX7219::zeichen8[5][5] = 1;
    MAX7219::zeichen8[5][6] = 1;

    MAX7219::zeichen9[2][1] = 1;
    MAX7219::zeichen9[2][2] = 1;
    MAX7219::zeichen9[2][3] = 1;
    MAX7219::zeichen9[2][6] = 1;
    MAX7219::zeichen9[3][1] = 1;
    MAX7219::zeichen9[3][3] = 1;
    MAX7219::zeichen9[3][6] = 1;
    MAX7219::zeichen9[4][1] = 1;
    MAX7219::zeichen9[4][3] = 1;
    MAX7219::zeichen9[4][6] = 1;
    MAX7219::zeichen9[5][1] = 1;
    MAX7219::zeichen9[5][2] = 1;
    MAX7219::zeichen9[5][3] = 1;
    MAX7219::zeichen9[5][4] = 1;
    MAX7219::zeichen9[5][5] = 1;
    MAX7219::zeichen9[5][6] = 1;

    MAX7219::zeichenDoppelpunkt[1][3] = 1;
    MAX7219::zeichenDoppelpunkt[1][5] = 1;
    gw = new gpioWork();
    for(int i = 0; i < 8; i++){
        MAX7219::MAX7219_buffer[i] =(uint8_t) 0x00;
    }
}
MAX7219::~MAX7219(){
    gw->~gpioWork();
}