/* 
 * File:   main.cpp
 * Author: hauke
 *
 * Created on 16. Juli 2015, 11:58
 */

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "dht22.h"
#include "HC_SR501.h"
#include "max7219led.h"
#include "Ausgabe.h"
#include "DS18B20.h"
#include "Relais.h"
using namespace std;

/*
 * Programmablauf:
 * Die Werte der verschiedenen Sensoren (Lufttemperatur, Wassertemperatur und 
 * Luftfeuchtigkeit) werden jede Minute gemessen und in eine 
 * CSV-Datei geschrieben. Außerdem sollen die Werte auch in eine HTML-Datei 
 * geschrieben werden. Diese HTML-Datei ist aus dem Internet abrufbar.
 * Wenn der Bewegungssensor eine Bewegung registriert, 
 * werden die aktuellen Werte auf dem Max7219 Display ausgegeben.
 */
int main(int argc, char** argv) {
    float luftfeuchtigkeit,lufttemperatur, wassertemperatur;
    lufttemperatur = 1.0;
    luftfeuchtigkeit = 1.0;
    dht22 *dht;
    HCSR501 *hc = new HCSR501();
    Ausgabe *aus;
    DS18B20 *ds;
    Relais *re = new Relais();
    time_t sek;
    struct tm *ts;
    int altTime;
    bool offenesFenster = false;
    aus = new Ausgabe();
        dht = new dht22();
        ds = new DS18B20();
    while(true){
        
        if(hc->readHCSR501() == 1){
            printf("Bewegung");
            while(true){
                int i = dht->readDHT(17,&lufttemperatur,&luftfeuchtigkeit);
                if(i == 0 ){
                    break;
                }
                printf("DHT nächster Versuch");
            }
            wassertemperatur = ds->getTemp();
            cout << "Temp: " << lufttemperatur << endl;
            aus->ausgabeText(lufttemperatur,luftfeuchtigkeit,wassertemperatur);         
        }
        sek = time(NULL);
        ts = localtime(&sek);
        
        if(ts->tm_min % 5 == 0 && ts->tm_min!=altTime){
            altTime = ts->tm_min;
            while(true){
                int i = dht->readDHT(17,&lufttemperatur,&luftfeuchtigkeit);
                if(i == 0 ){
                    break;
                }
            }
            wassertemperatur = ds->getTemp();
            /*printf("Wassertemp: %.1f\n", wassertemperatur);
            printf("Temperatur: %.2f `C\n",lufttemperatur);
            printf("Luftfeuchtigkeit: %.1f %\n",luftfeuchtigkeit); */
                        
            //aus->ausgabeText(lufttemperatur,luftfeuchtigkeit,wassertemperatur);
            aus->writeCSV(lufttemperatur,luftfeuchtigkeit,wassertemperatur);
            aus->writeWWW(lufttemperatur,luftfeuchtigkeit,wassertemperatur); 
            
            /*if(lufttemperatur>30 && !offenesFenster){
                re->setRelayOn(re->fensterAuf,20);
                offenesFenster = true;
            }
            if(lufttemperatur<23 && offenesFenster){
                re->setRelayOn(re->fensterZu,20);
                offenesFenster = false;
            }*/
            
            sleep(1);
            
        }
        else{usleep(300);}
        
        /*if(ts->tm_min == 0 && ts->tm_hour){
            re->setRelayOn(re->wasserAuf, 60);
        }*/
        
        
    }
        dht->~dht22();
        ds->~DS18B20();
    return 0;
}

