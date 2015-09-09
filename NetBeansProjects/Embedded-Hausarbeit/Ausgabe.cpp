#include "Ausgabe.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <time.h> //time stamp
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

void Ausgabe::ausgabeText(float lufttemperatur, float luftfeuchtigkeit,
        float wassertemperatur){
    std::ostringstream ss;
    ss << "LT:"<<lufttemperatur<<"CLF:"<<luftfeuchtigkeit<<
            "%TE:"<<wassertemperatur<<"C" ;
    std::string s(ss.str());
    
    string shellBefehl = "./max7219 ";
    shellBefehl = shellBefehl.append(s);
    system(shellBefehl.c_str());
    cout << shellBefehl << endl;
    int wartezeit = shellBefehl.length();
    sleep(wartezeit);
}

void Ausgabe::writeCSV(float lufttemperatur, float luftfeuchtigkeit, float wassertemperatur){
    time_t sek;
    struct tm *ts;
    sek = time(NULL);
    ts = localtime(&sek);
    std::ostringstream csvTmp;
    csvTmp << ts->tm_mday << "." << ts->tm_mon + 1 << "." << ts->tm_year+1900 
    << " " << ts->tm_hour << ":"<< ts->tm_min <<";" << lufttemperatur <<";"
            << luftfeuchtigkeit<<";"<<wassertemperatur<<"\n";
    std::string csv(csvTmp.str());
    cout << "CSV-Eintrag: " << csv << endl;
    
    fstream f; 
    f.open("Gewaechshaus.csv", ios::out|ios::app); 
    f <<csv;
    f.close(); 
}

void Ausgabe::writeWWW(float lufttemperatur, float luftfeuchtigkeit, float wassertemperatur){
    time_t sek;
    struct tm *ts;
    sek = time(NULL);
    ts = localtime(&sek);
    std::ostringstream csvTmp;
    csvTmp << ts->tm_mday << "." << ts->tm_mon + 1 << "." << ts->tm_year+1900 
    << " " << ts->tm_hour << ":"<< ts->tm_min <<";" << lufttemperatur <<";"
            << luftfeuchtigkeit<<";"<<wassertemperatur<<"\n";
    std::string csv(csvTmp.str());
    
    fstream f; 
    f.open("/var/Gewaechshaus.csv", ios::out); 
    f << csv;
    f.close(); 
}