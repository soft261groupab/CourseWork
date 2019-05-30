
#include "mbed.h"
#include "string.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define N 82
#define M 82

//Serial Interface
Serial pc(USBTX, USBRX);

Timer timer;
char message[20];


uint16_t inp_img[N][M],out_img[N][M],edge_Dir[N][M];
short int gradient[N][M];

/* Declare Gaussian mask */
uint16_t gaussianMask[3][3] = {
	{1, 2, 1},  {2, 4, 2},  {1, 2, 1}
};	

int16_t GxMask[3][3] = {
	{-1, 0, 1},  {-2, 0, 2},  {-1, 0, 1}
};	

int16_t GyMask[3][3] = {
	{-1, -2, -1},  {0, 0, 0},  {1, 2, 1}
};	


void initialization();
bool compare_Gaussian_images();
bool compare_Sobel_images();
void print_message(char *s, bool outcome);
void Gaussian_Blur();
void Sobel();

// Main program function.
int main() {
  
	
//printf("\n\r Programmed started \n\r");

	initialization();
	
	printf("\n\rTimer started for Gaussian Blur!\n");
	timer.start();
    
	//YOU WILL OPTIMIZE THE FOLLOWING function
//--------------------------------
	Gaussian_Blur();	
//------------------------------------------------------------------------------
		timer.stop();
    float gausTime;
		gausTime	= timer.read();
		//printf("\n\rGaussian Blur: executed in %f seconds", timer.read());
	  timer.reset();
	  snprintf(message,sizeof(message)-1,"Gaussian Blur");
	
		bool outcome;
		timer.start();
		outcome =	compare_Gaussian_images();
		timer.stop();
    print_message( message,outcome);
		
		printf("\n\rGaussian Blur: executed in %f seconds, vs %f", gausTime, timer.read());

	//--------------------------------------------------------
	//--------------------------------------
	
	  //printf("\n\rTimer started for Sobel!\n");
		timer.start();
    	
	//YOU WILL OPTIMIZE THE FOLLOWING function 
	//Sobel();
	Sobel();
	//-------------------
	  timer.stop();
    printf("\n\rSobel executed in %f seconds", timer.read());
		snprintf(message,sizeof(message)-1,"Sobel");
    print_message(message,compare_Sobel_images());

	return 0;
}


void initialization(){
	int i,j;
	
//This is a greyscale image with random values
// your flash memory doesn't support fopen() function	
	for (i=0;i<N;i++)
	 for (j=0;j<M;j++)
	  inp_img[i][j]=rand()%255;
	
		for (i=0;i<N;i++)
	   for (j=0;j<M;j++)
	    out_img[i][j]=0;
	
		for (i=0;i<N;i++)
	   for (j=0;j<M;j++)
	    edge_Dir[i][j]=0;
	
		for (i=0;i<N;i++)
	   for (j=0;j<M;j++)
	    gradient[i][j]=0;
}



void Gaussian_Blur(){
	
		uint32_t r0,r1,r2;
		int row, col;							
		/*----------------------Gaussian Blur------------------------*/					

		r2 = 0;
		col =1;	
	/*
	*
	*		register blocking applied using Duffs Device
	*
	*/
	
	
	 for (row = 1; row < N-1; row++) {
		 int n = N-1; 
		 col = 1;
    switch ((N-1) % 3)
     {
				do {										
				case 0:		//0th
									
						//GaussMask == 0, used __sadd16 to add values from imp_img via vector
						r0 = inp_img[row - 1][col - 1];						
						r2 = inp_img[row - 1][col - 1];
						
						r0 = inp_img[row + 1][col+1];
						r2 = __sadd16(r0,r2);
				
						r0= inp_img[row + 1][col - 1];
						r2 = __sadd16(r0,r2);

						r0= inp_img[row - 1][col+1];
						
						r2 = __sadd16(r0,r2);

				//for case 1 and 2 __smlad is used for vectorised multiplication
        case 1:   	//1st
						//
						r1= gaussianMask[0][1];			
            r0= inp_img[row - 1][col];				
						r2 = __smlad(r0,r1,r2); 	
				
            r0= inp_img[row][col - 1];					
						r2 = __smlad(r0,r1,r2); //if (row == 1 ) { if (c <10)   { printf("\n \r case:%d:3  \t   m:%-5d",c++, r1); } } 				
						
            r0= inp_img[row][col+1];						
						r2 = __smlad(r0,r1,r2);// if (row == 1 ) { if (c <10)   { printf("\n \r case:%d:5  \t   m:%-5d",c++, r1); } }  

            r0= inp_img[row + 1][col];			
						r2 = __smlad(r0,r1,r2);// if (row == 1 ) { if (c <10)   { printf("\n \r case:%d:6  \t   m:%-5d",c++, r1); } } 

					case 2:     //2nd		
 			
						r0= inp_img[row][col];
						r1= gaussianMask[1][1];
						r2 = __smlad(r0,r1,r2); // if (row == 1 ) { if (c <10)   { printf("\n \r case:3 %d:4  \t   m:%-5d",c++, r1); } }  	
							
						out_img[row][col++] = r2 / 16;
				
						r2 =0;						
					
			} while (n-- > 0);		
		}
	} 
}


void Sobel(){
	
	int32_t r0,r1,r2,r3;
	int row, col, Gx, Gy;		
	float thisAngle;				
	uint8_t newAngle;	
		/*---------------------- Sobel ---------------------------------*/	
		/*
	*
	*		register blocking applied using Duffs Device
	*
	*/
	
			r3 = 0;
			r2 = 0;
		  col=1;
	
	 for (row = 1; row < N-1; row++) {
		 int n = N-1; 
		 col = 1;
    switch ((N-1) % 3)
     {
				do {										
				case 0:		//0th									
//0th			
						//row left of pixel [row][col]						
						r0= out_img[row - 1][col - 1];
						r1= GxMask[0][0];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					

						//r1= GyMask[0][0];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);	
//1
						//Uneeded task adding nill value
            //r0=  out_img[row - 1][col];
						//r1= 0; //GxMask[0][1];
						//r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);
				
						r0= out_img[row - 1][col];
						r1= GyMask[0][1];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);		
//2
            r0= out_img[row - 1][col + 1];				
						r1= GxMask[0][2];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					
		
						r1= GyMask[0][2];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);	
//4						//uneeded taks removed here											
        case 1: 
//3
						//row containing pixel at [row][col]
            r0= out_img[row][col - 1];
						r1= GxMask[1][0];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);
//5
            r0= out_img[row][col + 1];				
						r1= GxMask[1][2];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);	
				case 2: 
					
            //row right of pixel at [row][col] 
//6					
            r0= out_img[row + 1][col - 1];
						r1= GxMask[2][0];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					

						r1= GyMask[2][0];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);
//7

            r0= out_img[row + 1][col + 1];				
						//r1= 1; //GxMask[2][2];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					
		
						//r1= 1; // GyMask[2][2];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);		
					
						r0= out_img[row + 1][col];
						r1= GyMask[2][1];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);
//8
			
			Gx = r2;
			r2 = 0;
			
			Gy = r3;
			r3 = 0;
			
			gradient[row][col] = abs(Gx) + abs(Gy);	// Calculate gradient strength		
			thisAngle = (atan2((float) Gx, (float) Gy)/3.14159f) * 180.0f;		// Calculate actual direction of edge [-180, +180]
			
			// Convert actual edge direction to approximate value 
			if ( ( (thisAngle >= -22.5f) && (thisAngle <= 22.5f) ) || (thisAngle >= 157.5f) || (thisAngle <= -157.5f) )
				newAngle = 0;
			if ( ( (thisAngle > 22.5f) && (thisAngle < 67.5f) ) || ( (thisAngle > -157.5f) && (thisAngle < -112.5f) ) )
				newAngle = 45;
			if ( ( (thisAngle >= 67.5f) && (thisAngle <= 112.5f) ) || ( (thisAngle >= -112.5f) && (thisAngle <= -67.5f) ) )
				newAngle = 90;
			if ( ( (thisAngle > 112.5f) && (thisAngle < 157.5f) ) || ( (thisAngle > -67.5f) && (thisAngle < -22.5f) ) )
				newAngle = 135;
				
			edge_Dir[row][col++] = newAngle;
						
					
			} while (n-- > 0);	
		}		
	}
}


	//this function is check whether your code version generates the correct output or not
	//DO NOT AMEND
//returns false/true, when the output image is incorrect/correct, respectively
bool compare_Gaussian_images(){
	
		
	int row, col, rowOffset, colOffset;	
	int newPixel;	
		/*----------------------COMPARE------------------------*/		
		
		for (row = 1; row < N-1; row++) {
		for (col = 1; col < M-1; col++) {
			newPixel = 0;
			for (rowOffset=-1; rowOffset<=1; rowOffset++) {
				for (colOffset=-1; colOffset<=1; colOffset++) {
					
          newPixel += inp_img[row+rowOffset][col+colOffset] * gaussianMask[1 + rowOffset][1 + colOffset];					
				}
			}
		 newPixel /= 16;

		 if (newPixel != out_img[row][col]){			 
			
			return false;			
		 }			 
		}
	}	
	return true;			
	}

	//this function is check whether your code version generates the correct output or not
	//DO NOT AMEND
	bool compare_Sobel_images(){
	
	int row, col, rowOffset, colOffset;	
	int Gx,Gy,temp;
	float thisAngle;
	uint8_t newAngle;
	/*----------------------COMPARE------------------------*/	
	 for (row = 1; row < N-1; row++) {
		for (col = 1; col < M-1; col++) {

			Gx = 0;
			Gy = 0;

			/* Calculate the sum of the Sobel mask times the nine surrounding pixels in the x and y direction */
			for (rowOffset=-1; rowOffset<=1; rowOffset++) {
				for (colOffset=-1; colOffset<=1; colOffset++) {
					
					Gx += out_img[row+rowOffset][col+colOffset] * GxMask[rowOffset + 1][colOffset + 1];
					Gy += out_img[row+rowOffset][col+colOffset] * GyMask[rowOffset + 1][colOffset + 1];
				}
			}

			temp = abs(Gx) + abs(Gy);	/* Calculate gradient strength		*/	
			if (temp != gradient[row][col]){
				
				return false;
			}
			thisAngle = (atan2((float) Gx, (float) Gy)/3.14159f) * 180.0f;		/* Calculate actual direction of edge [-180, +180]*/
			
			/* Convert actual edge direction to approximate value */
			if ( ( (thisAngle >= -22.5f) && (thisAngle <= 22.5f) ) || (thisAngle >= 157.5f) || (thisAngle <= -157.5f) )
				newAngle = 0;
			if ( ( (thisAngle > 22.5f) && (thisAngle < 67.5f) ) || ( (thisAngle > -157.5f) && (thisAngle < -112.5f) ) )
				newAngle = 45;
			if ( ( (thisAngle >= 67.5f) && (thisAngle <= 112.5f) ) || ( (thisAngle >= -112.5f) && (thisAngle <= -67.5f) ) )
				newAngle = 90;
			if ( ( (thisAngle > 112.5f) && (thisAngle < 157.5f) ) || ( (thisAngle > -67.5f) && (thisAngle < -22.5f) ) )
				newAngle = 135;			
			
			if (newAngle != edge_Dir[row][col])
					return false;
		}
	}	 		
	return true;			
	}
	
	void print_message(char *s, bool outcome){
		
	if (outcome==true)
		printf("\n\n\r ----- %s output is correct - SUCCESS!-----\n\r",s);
	else 
		printf("\n\n\r -----%s output is INcorrect - ERROR!-----\n\r",s);
	
	}
