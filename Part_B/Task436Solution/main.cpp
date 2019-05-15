
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
  
	
printf("\n\r Programmed started \n\r");

	initialization();
	
	printf("\n\rTimer started for Gaussian Blur!\n");
	timer.start();
    
	//YOU WILL OPTIMIZE THE FOLLOWING function
//--------------------------------
	Gaussian_Blur();	
//------------------------------------------------------------------------------
    timer.stop();
    printf("\n\rThe time taken for gaussian Blur was %f seconds\n", timer.read());
	  timer.reset();
	  snprintf(message,sizeof(message)-1,"Gaussian Blur");
    print_message( message,compare_Gaussian_images());


	//--------------------------------------------------------
	//--------------------------------------
	
	  printf("\n\rTimer started for Sobel!\n");
		timer.start();
    	
	//YOU WILL OPTIMIZE THE FOLLOWING function 
	Sobel();
	//-------------------
	  timer.stop();
    printf("\n\rThe time taken for Sobel was %f seconds\n", timer.read());
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
	
		int row, col, rowOffset, colOffset;							
	int newPixel;	
	
		/*---------------------- Gaussian Blur ---------------------------------*/
	 for (row = 1; row < N-1; row++) {
		for (col = 1; col < M-1; col++) {
			newPixel = 0;
			for (rowOffset=-1; rowOffset<=1; rowOffset++) {
				for (colOffset=-1; colOffset<=1; colOffset++) {
					
                   newPixel += inp_img[row+rowOffset][col+colOffset] * gaussianMask[1 + rowOffset][1 + colOffset];
				}
			        }
		out_img[row][col] = newPixel / 16;
		}
	}
}



void Sobel(){
	
			int row, col, rowOffset, colOffset,Gx,Gy;	
				
	float thisAngle;				
	uint8_t newAngle;
	
		/*---------------------- Sobel ---------------------------------*/
		for (row = 1; row < N-1; row++) {
		for (col = 1; col < M-1; col++) {

			Gx = 0;
			Gy = 0;

			//Calculate the sum of the Sobel mask times the nine surrounding pixels in the x and y direction 
			for (rowOffset=-1; rowOffset<=1; rowOffset++) {
				for (colOffset=-1; colOffset<=1; colOffset++) {
					
					Gx += out_img[row+rowOffset][col+colOffset] * GxMask[rowOffset + 1][colOffset + 1];
					Gy += out_img[row+rowOffset][col+colOffset] * GyMask[rowOffset + 1][colOffset + 1];
				}
			}

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
				
			edge_Dir[row][col] = newAngle;		


		}
	}

}



//returns false/true, when the output image is incorrect/correct, respectively
bool compare_Gaussian_images(){
	
	int row, col, rowOffset, colOffset;	
	int newPixel;
	
			
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
		printf("\n\n\r ----- %s output is correct -----\n\r",s);
	else 
		printf("\n\n\r -----%s output is INcorrect -----\n\r",s);
		
	}

