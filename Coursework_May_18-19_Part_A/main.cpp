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
float sampleStr = 0.1;
float inputFrequency = 10;
bool check = false;
float bufferArray[20];
int arrayCounter = 0;
float sum = 0;
float averageBuffer = 0;
char inputPassword[20];
char inputChange[20];


void arrayAverage(){
	if(arrayCounter == inputFrequency){
		arrayCounter=0;
				printf("%5.3f\n", averageBuffer);
			}
    for(int i = 0; i<sizeof(bufferArray); i++){
        sum = sum + bufferArray[i];
    }
    averageBuffer = sum /20;
    
		sum = 0;
		
}
	
int main() {
	
			while(1){
		
		//Read ADC
		fVin = AIN;
		bufferArray[arrayCounter] = fVin;
		
    arrayCounter++;
    arrayAverage();
        
		
		//printf("Analog input = %5.3f\n", fVin);
		binaryOutput = 2;
		//Wait
		wait(sampleStr);
		binaryOutput = 0;
		
		
	if(SW2 == 1){ 
	
			binaryOutput = 1;
			check = false;
		while(check == false){
			printf("Enter Password: ");
			pc.scanf("%s", &inputPassword);
			
			if(strcmp(inputPassword, "password")==0){
				printf("Enter 'rate' to change sample rate, or 'buffer' to clear buffer: ");
				pc.scanf("%s", &inputChange);
				check = true;
				
				if(strcmp(inputChange, "rate") == 0){
					printf("Enter sampling rate: ");
					pc.scanf("%f", &inputFrequency); 
		
					if(inputFrequency <= 100 && inputFrequency >= 5){
						sampleStr = 1 / inputFrequency;
						check = true;
						
					}
					else{
						check = false;
					}
				}
				
				else if(strcmp(inputChange, "buffer") == 0){
					for(int i = 0; i<sizeof(bufferArray); i++){
						bufferArray[i] = 0;
						
					}
					check = true;
					
				}
			}
			else{
				check = false;
			}
			
		
		}
	}
	
		
		}
}





