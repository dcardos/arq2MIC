/* 
Modified from: http://rosettacode.org/wiki/Mandelbrot_set#PPM_non_interactive

c program:
--------------------------------
1. draws Mandelbrot set for Fc(z)=z*z +c
using Mandelbrot algorithm ( boolean escape time )
-------------------------------         
2. technique of creating ppm file is  based on the code of Claudio Rocchini
http://en.wikipedia.org/wiki/Image:Color_complex_plot.jpg
create 24 bit color graphic file ,  portable pixmap file = PPM 
see http://en.wikipedia.org/wiki/Portable_pixmap
to see the file use external application ( graphic viewer)
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

__declspec(target(mic))
static const double CxMin=-2.5;
__declspec(target(mic))
static const double CxMax=1.5;
__declspec(target(mic))
static const double CyMin=-2.0;
__declspec(target(mic))
static const double CyMax=2.0;
__declspec(target(mic))
static const int iYmax = 16384;
__declspec(target(mic))
static const int IterationMax=256;
__declspec(target(mic))
static const double EscapeRadius=2;
__declspec(target(mic))
static const int iXmax = 16384;
__declspec(target(mic))
static const int MaxColorComponentValue=255; 

void innerLoop(int Cy_arg, double PixelWidth_arg, int pos, unsigned char *vetorR, unsigned char *vetorG, unsigned char *vetorB)
{
    /* bail-out value , radius of circle ;  */
    
    double ER2=EscapeRadius*EscapeRadius;

    int Iteration;
    double Zx, Zy;
    double Zx2, Zy2;
    int iX_arg;
    for(iX_arg=0;iX_arg<iXmax;iX_arg++)
    {         
        double Cx_arg=CxMin + iX_arg*PixelWidth_arg;
        /* initial value of orbit = critical point Z= 0 */
        Zx=0.0;
        Zy=0.0;
        Zx2=Zx*Zx;
        Zy2=Zy*Zy;
        /* */
        for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
        {
            Zy=2*Zx*Zy + Cy_arg;
            Zx=Zx2-Zy2 + Cx_arg;
            Zx2=Zx*Zx;
            Zy2=Zy*Zy;
        };
        /* compute  pixel color (24 bit = 3 bytes) */
        if (Iteration==IterationMax)
        { /*  interior of Mandelbrot set = black */
           vetorR[pos]=0;
           vetorG[pos]=0;
           vetorB[pos]=0;                           
        }
        else 
        { /* exterior of Mandelbrot set = white */
            vetorR[pos] = ((IterationMax-Iteration) % 8) *  63;  /* Red */
            vetorG[pos] = ((IterationMax-Iteration) % 4) * 127;  /* Green */
            vetorB[pos] = ((IterationMax-Iteration) % 2) * 255;  /* Blue */
        };
        pos++;
    }
}

void persistent(unsigned char *vetorR, unsigned char *vetorG, unsigned char *vetorB)
{
    FILE * fp;
    char *filename="mandelbrot.ppm";
    fp= fopen(filename,"wb");
    fprintf(fp,"P6\n %d\n %d\n %d\n",iXmax,iYmax,MaxColorComponentValue);
    int i=0;
    for (i; i<iYmax*iXmax; i++)
    {
        fwrite(vetorR,1,1,fp);
        vetorR++;
        fwrite(vetorG,1,1,fp);
        vetorG++;
        fwrite(vetorB,1,1,fp);
        vetorB++;
    }
    fclose(fp);
}

int main()
{
    int iY;
     
    double Cy;
    
    double PixelWidth=(CxMax-CxMin)/iXmax;
    double PixelHeight=(CyMax-CyMin)/iYmax;

    /* Minhas variáveis em memória */
	unsigned char *colorR = (unsigned char *)malloc(iXmax * iYmax * sizeof(char));
    unsigned char *colorG = (unsigned char *)malloc(iXmax * iYmax * sizeof(char));
    unsigned char *colorB = (unsigned char *)malloc(iXmax * iYmax * sizeof(char));
	int inc = 0;

    void __attribute__((target(mic))) innerLoop(int Cy_arg, double PixelWidth_arg, int pos, unsigned char *v1, unsigned char *v2, unsigned char *v3);
    #pragma offload target(mic) inout(colorR:length(iXmax * iYmax)) inout(colorG:length(iXmax * iYmax)) inout(colorB:length(iXmax * iYmax))
    for(iY=0;iY<iYmax;iY++)
    {
        Cy=CyMin + iY*PixelHeight;
        if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */
        
        innerLoop(Cy, PixelWidth, inc, colorR, colorG, colorB);

        inc += iXmax;
    }

    //persistent(colorR, colorG, colorB);

    return 0;
}
