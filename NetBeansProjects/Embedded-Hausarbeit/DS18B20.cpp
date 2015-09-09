#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "DS18B20.h"

DS18B20::DS18B20(){
    char path[] = "/sys/bus/w1/devices/28-0000061571d2/w1_slave";
    DIR *dir = opendir (path);
    fd = open(path, O_RDONLY); 
}

DS18B20::~DS18B20(){
    close(fd);
}

float DS18B20::getTemp(){
    char buf[256];
    char tmpData[6];
    if(fd == -1){
      perror ("Fehler beim öffnen"); 
      return -1;
    }
    read(fd, buf, 256);
    strncpy(tmpData, strstr(buf, "t=") + 2, 5); 
    float tempC = strtof(tmpData, NULL);
    tempC = tempC / 1000;
    return tempC; 
}