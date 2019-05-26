#include "mbed.h"
#include "rtos.h"
#include "string.h"
#include <stdio.h>
#include <ctype.h>



//Serial Interface
Serial pc(USBTX, USBRX);
//Global Objects
BusOut binaryOutput(D5, D6, D7);
DigitalIn SW1(D3);
DigitalIn SW2(D4);

//Analog input
AnalogIn AIN(A0);

//Global Variable
float fVin = 0.0;


int main() {
	while(1){
		
		//Read ADC
		fVin = AIN;
		
		//Write to terminal
		printf("Analog input = %5.3f\n", fVin);
		
		//Wait
		wait(0.1);
	}
}



