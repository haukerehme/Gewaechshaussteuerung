/* 
 * File:   Ausgabe.h
 * Author: hauke
 *
 * Created on 26. August 2015, 00:52
 */

#ifndef AUSGABE_H
#define	AUSGABE_H
#include <string>

using namespace std;
class Ausgabe{
public:
    void ausgabeText(float lufttemperatur, float luftfeuchtigkeit, float wassertemperatur);
    void writeCSV(float lufttemperatur, float luftfeuchtigkeit, float wassertemperatur);
    bool ausgabeprocessAktiv();
    void writeWWW(float lufttemperatur, float luftfeuchtigkeit, float wassertemperatur);
};
#endif	/* AUSGABE_H */

