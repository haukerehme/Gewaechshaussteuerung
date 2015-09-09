#include "gpioWork.h"

gpioWork::gpioWork(){
    this->gpio = getMapRegister(gpioWork::GPIO_VERZEICHNIS);
}

gpioWork::~gpioWork(){
    if(munmap((void*)gpio, gpioWork::registerLaenge)!=0){
        printf("munmap Error");}
    gpio = NULL;
    //free((void*)gpio);
}

void gpioWork::setPinDirection(unsigned int pin, const unsigned& direction){
	if (direction == OUTPUT){
		switch(pin/10) {	
			case 0:
				*(this->gpio + 0) &= ~(7<<(((pin)%10)*3));
				*(this->gpio + 0) |=  (1<<(((pin)%10)*3));
				break;
			case 1:
				*(this->gpio + 1) &= ~(7<<(((pin)%10)*3));
				*(this->gpio + 1) |=  (1<<(((pin)%10)*3));
				break;
			case 2:
				*(this->gpio + 2) &= ~(7<<(((pin)%10)*3));
				*(this->gpio + 2) |=  (1<<(((pin)%10)*3));
				break;
			default:
				break;
		}
	
	}
	else{
		switch(pin/10) {	
			case 0:
				*(this->gpio + 0) &= ~(7<<(((pin)%10)*3));
				break;
			case 1:
				*(this->gpio + 1) &= ~(7<<(((pin)%10)*3));
				break;
			case 2:
				*(this->gpio + 2) &= ~(7<<(((pin)%10)*3));
				break;
			default:
				break;
		}
	}
}

unsigned int gpioWork::getPinValue(unsigned int pin){
	unsigned int retVal = 0;
	
	if ((*(this->gpio + 13) & (1 << pin)) != 0)
		retVal = 1;
	
	return retVal;
}

volatile unsigned int* gpioWork::getMapRegister(unsigned long baseAddress){
  int mem_fd = 0;
  void *mapRegister = MAP_FAILED;
  
  if (!mem_fd) {
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
     perror("/dev/mem kann nicht geöffnet werden");
      exit (1);
    }
  }
  
  mapRegister = mmap(
      NULL,
      gpioWork::registerLaenge,
      PROT_READ|PROT_WRITE,//|PROT_EXEC,
      MAP_SHARED|MAP_LOCKED,
      mem_fd,
      baseAddress
  );
    
  if (mapRegister == MAP_FAILED) {
	perror("mmap error");
	close(mem_fd);
	exit (1);
  }
  
  if(close(mem_fd) < 0){
        perror("/dev/mem filediskriptor kann nicht geschlossen werden");
        exit(1);
  }	
  return (volatile unsigned *)mapRegister;
}