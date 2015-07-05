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
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM_THREADS     5

/* constantes */
// __declspec(target(mic))
static const double CxMin=-2.5;
// __declspec(target(mic))
static const double CxMax=1.5;
// __declspec(target(mic))
static const double CyMin=-2.0;
// __declspec(target(mic))
static const double CyMax=2.0;
// __declspec(target(mic))
static const int iYmax = 16384;
// __declspec(target(mic))
static const int IterationMax=256;
// __declspec(target(mic))
static const double EscapeRadius=2;
// __declspec(target(mic))
static const int iXmax = 16384;
// __declspec(target(mic))
static const int MaxColorComponentValue=255;
/* variáveis globais */
// __declspec(target(mic))
double ER2;
// __declspec(target(mic))
double PixelWidth;
// __declspec(target(mic))
double PixelHeight;
// __declspec(target(mic))
typedef struct args_str{
    unsigned char *vectorR;
    unsigned char *vectorG;
    unsigned char *vectorB;
    int firstCy, lastCy;
    long tid;
}args;


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

void *innerLoop(void *threadArg){
    /* argumentos de cada thread */
    args *myData = (args *)threadArg; 
    unsigned char *red = myData->vectorR;
    unsigned char *green = myData->vectorG;
    unsigned char *blue = myData->vectorB;
    int inicioCy = myData->firstCy;
    int fimCy = myData->lastCy;
    long t_id = myData->tid;
    /* variáveis locais */
    double Zx, Zy;
    double Zx2, Zy2;
    int iX, iY;
    double Cy, Cx;
    int Iteration;
    long pos = inicioCy*iXmax;

    printf("Thread %ld starting com inicioCy=%d, fimCy=%d\n", t_id, inicioCy, fimCy);

    for(iY=inicioCy;iY<fimCy;iY++)
    {
        Cy=CyMin + iY*PixelHeight;
        if (fabs(Cy)< PixelHeight/2) Cy=0.0; /* Main antenna */


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
                Zy=2*Zx*Zy + Cy;
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
    // printf("Thread %ld done.\n", t_id);
    pthread_exit((void*) t_id);
    
}

int main()
{
    int rc; // returned thread value
    long t; // para o for de threads

    PixelHeight=(CyMax-CyMin)/iYmax;
    ER2=EscapeRadius*EscapeRadius;
    PixelWidth=(CxMax-CxMin)/iXmax;

    /* Minhas variáveis em memória */
    unsigned char *vetorR = (unsigned char *)malloc(iXmax * iYmax * sizeof(char));
    unsigned char *vetorG = (unsigned char *)malloc(iXmax * iYmax * sizeof(char));
    unsigned char *vetorB = (unsigned char *)malloc(iXmax * iYmax * sizeof(char));
    args argumentos[NUM_THREADS];
    void *status;

    pthread_t threads[NUM_THREADS];
    /* setando threads como joinable explicitamente */
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* dividindo número de 'y' que cada thread será responsável */
    int qtdY = iYmax/NUM_THREADS;
    int sobra = iYmax % NUM_THREADS;
    if (sobra == 0)
        sobra = qtdY;
    else
        sobra = qtdY + sobra;
    printf("Chunks: %d  sobra: %d\n", qtdY, sobra);

    //void __attribute__((target(mic))) innerLoop(unsigned char *red, unsigned char *green, unsigned char *blue);
    //#pragma offload target(mic) out(vetorR:length(iXmax * iYmax)) out(vetorG:length(iXmax * iYmax)) out(vetorB:length(iXmax * iYmax))
    //#pragma omp parallel for
    for (t=0; t<(NUM_THREADS-1); t++){
        /* setando argumentos */
        argumentos[t].vectorR = vetorR;
        argumentos[t].vectorG = vetorG;
        argumentos[t].vectorB = vetorB;
        argumentos[t].firstCy = qtdY*t;
        argumentos[t].lastCy = qtdY*(t+1);
        argumentos[t].tid = t;

        rc = pthread_create(&threads[t], &attr, innerLoop, (void *)&argumentos[t]);

        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }

    }
    /* para sobra */
    /* setando argumentos */
    argumentos[t].vectorR = vetorR;
    argumentos[t].vectorG = vetorG;
    argumentos[t].vectorB = vetorB;
    argumentos[t].firstCy = qtdY*t;
    argumentos[t].lastCy = qtdY*t + sobra;
    argumentos[t].tid = t;

    rc = pthread_create(&threads[t], &attr, innerLoop, (void *)&argumentos[t]);

    if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    
    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);
    for(t=0; t<NUM_THREADS-1; t++) {
        rc = pthread_join(threads[t], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            exit(-1);
        }

        printf("Main: completed join with thread %ld\n", t);

      }

    /* write to the file */
    persistent(vetorR, vetorG, vetorB);

    printf("Main: program completed. Exiting.\n");
    pthread_exit(NULL);
    return 0;
}