/* 
 * File:   DS18B20.h
 * Author: hauke
 *
 * Created on 26. August 2015, 23:11
 */

#ifndef DS18B20_H
#define	DS18B20_H
class DS18B20{
public:
    float getTemp();
    DS18B20();
    ~DS18B20();
    int fd;
};


#endif	/* DS18B20_H */

