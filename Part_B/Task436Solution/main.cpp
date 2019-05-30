
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

void Gaussian_Debug(){
	
	uint32_t r0,r1,r2;
	int row, col, rowOffset, colOffset;							
		/*----------------------Gaussian DEBUG------------------------*/	
	
	printf("\n \r ---------------Gaussian DEBUG------------");
		int c =1;
		r2 = 0;
	 for (row=1;row<2;row++) {		 
	  for (col=1;col<20;col++) {

						r0= inp_img[row - 1][col - 1];
						r1= gaussianMask[0][0];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	
			
		
            r0= inp_img[row - 1][col];
						r1= gaussianMask[0][1];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:2  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	


            r0= inp_img[row - 1][col+1];
						r1= gaussianMask[0][2];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:3  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	



            r0= inp_img[row][col - 1];
						r1= gaussianMask[1][0];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:4  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	


            r0= inp_img[row][col];
						r1= gaussianMask[1][1];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:5  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	


            r0= inp_img[row][col+1];
						r1= gaussianMask[1][2];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:6  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	


            r0= inp_img[row + 1][col - 1];
						r1= gaussianMask[0][0];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:7  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	


            r0= inp_img[row + 1][col];
						r1= gaussianMask[0][1];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:8  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	


            r0= inp_img[row + 1][col+1];
						r1= gaussianMask[0][2];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:9  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }		
				
						r2=r2 /16;
						
						printf("\n \r %d, out:%d ", col-1, r2);
						
						r2 =0;
						
						break;
						
							/*				
						if (r2 == out_img[1][col]) {
							printf("\n \r out: %d == %d ",out_img[row][col], r2);
						}							
						else
						printf("\n \r out: %d != %d ",out_img[row][col], r2);
							
						r2 = 0;
						
						*/
			}
		
			
			
		}
 }



void Gaussian_Blur(){
	
	uint32_t r0,r1,r2;
	int row, col;							
		/*----------------------Gaussian Blur------------------------*/	
		
		
		uint16_t (*out_ptr)[N][M] = &out_img;
		
		uint16_t (*imp_ptr)[N][M] = &inp_img;
	
		int newPixel;
				
		int rowOffset =1; int colOffset =1;
		newPixel = 0;
		
						
			int T = (N-1);
			r2 = 0;
		  col = 1;
		int c =1;
		uint16_t a = 0;
		int b =1;
		char t[800]; 
		
		char print[40]; 
		//sprintf(merge, "%d%d,",T % 9, (n-1) % 9);
		
		int mod = T / 9;	
		int section = (N + 6) / 9; 
		
	 for (row = 1; row < N-1; row++) {
		 int n = N-1; 
		 col = 1;
    switch ((N-1) % 9)
     {
				do {
					
					
				case 0:		//0th
						r0= inp_img[row - 1][col - 1];
						r1= gaussianMask[0][0];
						r2 = __smlad(r0,r1,r2);
  if (row == 1 && col > 77) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }
	if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }											
        case 1:   						//1st
            r0= inp_img[row - 1][col];
						r1= gaussianMask[0][1];
						r2 = __smlad(r0,r1,r2);  if   (row == 1 && col > 77) { printf("\r\n --------------------------------");
						printf("\n \r c%d:2  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }
 else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	
        case 2:           //2nd
            r0= inp_img[row - 1][col+1];
						r1= gaussianMask[0][2];
						r2 = __smlad(r0,r1,r2);   if   (row == 1 && col > 77) { printf("\n \r c%d:3  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }					
        case 3:           //four
            r0= inp_img[row][col - 1];
						r1= gaussianMask[1][0];
						r2 = __smlad(r0,r1,r2);   if   (row == 1 && col > 77) { printf("\n \r c%d:4  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }		
        case 4:              //5
            r0= inp_img[row][col];
						r1= gaussianMask[1][1];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:5  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	
        case 5:              //6
            r0= inp_img[row][col+1];
						r1= gaussianMask[1][2];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:6  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	
				case 6:           //7
            r0= inp_img[row + 1][col - 1];
						r1= gaussianMask[0][0];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:7  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }					
				case 7:           //8
            r0= inp_img[row + 1][col];
						r1= gaussianMask[0][1];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:8  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }	
				case 8:   		//9
				//	printf("\n \r 9:%d \t row x, col %d = %d vs r2 %d", b++, col-1, out_img[1][col-1], a );
						r0= inp_img[row + 1][col+1];
						r1= gaussianMask[0][2];
						r2 = __smlad(r0,r1,r2);  if (row == 1 && col > 77) { printf("\n \r c%d:9  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); } else if (row == 2 && col < 4) { printf("\n \r c%d:1  r0:%-5d r1:%-5d r2:%-5d",c, r0,r1,r2); }		
				
						out_img[row][col++] = r2 / 16;
				
						r2 =0;
						//DEBUG
						if (row == 1 && col > 77) {
							printf("\n \r %d, out:%d %d", col-1, out_img[row][col-1], n);
						}
						else if (row == 2 && col < 4) { 
						printf("\n \r %d, out:%d %d", col-1, out_img[row][col-1], n);
						}					
      
						
						if (n % 1000 == 0 || n == 6560) {
						printf("\n \r %d, out:%d %d", col-1, out_img[row][col-1], n);
						}
						//END_DEBUG
			} while (n-- > 0);
		

	
		}
	}
	 printf("%d", section);
 Gaussian_Debug();

}

void Sobel(){
	
	int32_t r0,r1,r2,r3;
	int row, col, rowOffset, Gx, Gy;		
	float thisAngle;				
	uint8_t newAngle;	
		/*---------------------- Sobel ---------------------------------*/
	r2 = 0;
	r3 = 0;
	
	for (row = 1; row < N-1; row++) {
		for (col = 1; col < M-1; col++) {		
			//Calculate the sum of the Sobel mask times the nine surrounding pixels in the x and y direction 
			
						//row left of pixel [row][col]
						r0= out_img[row - 1][col - 1];
						r1= GxMask[0][0];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					

						r1= GyMask[0][0];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);

            r0= out_img[row - 1][col];
						r1= GxMask[0][1];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					
					
						r1= GyMask[0][1];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);

            r0= out_img[row - 1][col + 1];				
						r1= GxMask[0][2];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					
		
						r1= GyMask[0][2];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);
						
						
						//row containing pixel at [row][col]
            r0= out_img[row][col - 1];
						r1= GxMask[1][0];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					

						r1= GyMask[1][0];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);

            r0= out_img[row][col];
						r1= GxMask[1][1];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					
					
						r1= GyMask[1][1];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);

            r0= out_img[row][col + 1];				
						r1= GxMask[1][2];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					
		
						r1= GyMask[1][2];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);		

            //row right of pixel at [row][col]      
            r0= out_img[row + 1][col - 1];
						r1= GxMask[2][0];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					

						r1= GyMask[2][0];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);

                        r0= out_img[row + 1][col];
						r1= GxMask[2][1];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					
					
						r1= GyMask[2][1];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);

                        r0= out_img[row + 1][col + 1];				
						r1= GxMask[2][2];
						r2 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r2);					
		
						r1= GyMask[2][2];
						r3 = __smlad(* (uint32_t *) &r0,* (uint32_t *) &r1,r3);	
			
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
				
			edge_Dir[row][col] = newAngle;

		}
	}
}
	//this function is check whether your code version generates the correct output or not
	//DO NOT AMEND
//returns false/true, when the output image is incorrect/correct, respectively
bool compare_Gaussian_images(){
	
	//bool debug = true;
	
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
			 
			 //DEBUG START!
			// if (row < 2) {
			//	 if (col < 10) {
			// printf("\n\r  row: %d col: %d In val: %d Out val: %d", row ,col , newPixel, out_img[row][col]);
			//	 }
			// }else //DEBUG END
			return false;
			
		 }			 
		}
	}
	
	return true;	
		//return debug;
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
