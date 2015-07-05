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

/* constantes */
static const double CxMin=-2.5;
static const double CxMax=1.5;
static const double CyMin=-2.0;
static const double CyMax=2.0;
static const int iYmax = 16384;
static const int IterationMax=256;
static const double EscapeRadius=2;
static const int iXmax = 16384;
static const int MaxColorComponentValue=255;
/* variáveis globais */
// __declspec(target(mic))
double ER2;
// __declspec(target(mic))
double PixelWidth;
// __declspec(target(mic))
double PixelHeight;
// __declspec(target(mic))
//double Cy;

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

void innerLoop(unsigned char *red, unsigned char *green, unsigned char *blue, double CyArg){
    int iX;
    double Cx;
    double Zx, Zy;
    double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
    int Iteration;
    
    long pos = 0;
    for(iX=0;iX<iXmax;iX++)
    {         
        Cx=CxMin + iX*PixelWidth;
        /* initial value of orbit = critical point Z= 0 */
        Zx=0.0;
        Zy=0.0;
        Zx2=Zx*Zx;
        Zy2=Zy*Zy;
        /* */
        for (Iteration=0;Iteration<IterationMax && ((Zx2+Zy2)<ER2);Iteration++)
        {
            Zy=2*Zx*Zy + CyArg;
            Zx=Zx2-Zy2 + Cx;
            Zx2=Zx*Zx;
            Zy2=Zy*Zy;
        };
        /* compute  pixel color (24 bit = 3 bytes) */
        if (Iteration==IterationMax)
        { /*  interior of Mandelbrot set = black */
           red[pos]=0;
           green[pos]=0;
           blue[pos]=0;                           
        }
        else 
        { /* exterior of Mandelbrot set = white */
            red[pos] = ((IterationMax-Iteration) % 8) *  63;  /* Red */
            green[pos] = ((IterationMax-Iteration) % 4) * 127;  /* Green */
            blue[pos] = ((IterationMax-Iteration) % 2) * 255;  /* Blue */
        };
        pos++;
    }
}

int main()
{

    PixelHeight=(CyMax-CyMin)/iYmax;
    ER2=EscapeRadius*EscapeRadius;
    PixelWidth=(CxMax-CxMin)/iXmax;

    /* Minhas variáveis em memória */
    unsigned char *vetorR = (unsigned char *)malloc(iXmax * iYmax * sizeof(char));
    unsigned char *vetorG = (unsigned char *)malloc(iXmax * iYmax * sizeof(char));
    unsigned char *vetorB = (unsigned char *)malloc(iXmax * iYmax * sizeof(char));
    int iY, inc = 0;
    double Cy;

    #pragma omp parallel for
    for(iY=0;iY<iYmax;iY++)
    {
         Cy=CyMin + iY*PixelHeight;
         if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */

         innerLoop(&vetorR[inc], &vetorG[inc], &vetorB[inc], Cy);

         inc += iXmax;
         
    }

    /* write to the file */
    //persistent(vetorR, vetorG, vetorB);

    free(vetorR);
    free(vetorG);
    free(vetorB);

    return 0;
}