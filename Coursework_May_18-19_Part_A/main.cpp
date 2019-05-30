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
int secondCounter = 0;
float sum = 0;
float averageBuffer = 0;
char inputPassword[20];



void arrayAverage(){
	//If data is taken the same number of times as the frequency
	// then 1 second has passed, so the average is calculated
	if(secondCounter == inputFrequency){
		secondCounter=0;
				
			
		//Get the sum of array
    for(int i = 0; i<sizeof(bufferArray); i++){
        sum = sum + bufferArray[i];
    }
		//Divide sum by size
    averageBuffer = sum /20;
    printf("Average input = %5.3f\n", averageBuffer);
		sum = 0;
	}
}
	
int main() {
	
			while(1){
		
		//Read ADC
		fVin = AIN;
		//Add to circular buffer
		bufferArray[arrayCounter] = fVin;
		//If array is full, set counter back to 0
		if(arrayCounter== 19){
			arrayCounter=0;
		}
		
		//Increment counters
    arrayCounter++;
		secondCounter++;
		
		//Go to the arrayFuntion to check if second has passed
		//	and also calculate average.
    arrayAverage();
        
		
		//printf("Analog input = %5.3f\n", fVin);
		//Turn on green LED
		binaryOutput = 1;
		//Wait 1/frequency seconds
		wait(sampleStr);
		//Turn off green LED
		binaryOutput = 0;
		
	//If switch is pressed...
	if(SW2 == 1){
			//Turn on red LED
			binaryOutput = 4;
		
			check = false;

					while(check == false){

									printf("Enter Frequency: ");
									pc.scanf("%f", &inputFrequency);
									
								//Checks frequency between 5 and 100Hz	
								if(inputFrequency <= 100 && inputFrequency >= 5){
									//Changes from hertz to seconds
									sampleStr = 1 / inputFrequency;
									secondCounter = 0;
									sum = 0;
									check = true;
								}
								
								else{
									check = false;
								}
						
				}
	
}
}
			}




