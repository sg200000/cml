#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define S   30
#define N   30
#define NB_THREADS_MIN 2 
#define NB_THREADS_MAX 8 
#define x_min   3
#define x_max   16

long num_tasks = 0;

long fibo(int);
long fiboPar(int);
long fiboPar2(int);
long fiboPar3(int, int);

int main(){
    int n = N;
    long resultSeq, resultPar;
    double start, end, timeSeq,timePar, acceleration;

    start=omp_get_wtime();
    resultSeq = fibo(n);
    end=omp_get_wtime();
    timeSeq=(end-start);

    FILE * effFile = fopen("traceEff.dat", "w");
    FILE * accelFile = fopen("traceAccel.dat", "w");

    fprintf(effFile, "%d\t", (x_max-x_min)+1);
    fprintf(accelFile, "%d\t", (x_max-x_min)+1);
    for (int i=x_min;i<=x_max;i++){
	    fprintf(effFile, "%d\t", i);
	    fprintf(accelFile, "%d\t", i);
    }
    fprintf(effFile, "\n");
    fprintf(accelFile, "\n");
    for (int th=NB_THREADS_MIN;th<=NB_THREADS_MAX;th++){
	    fprintf(effFile, "%d\t", th);
	    fprintf(accelFile, "%d\t", th);
      for(int x=x_min;x<=x_max;x++){
            start=omp_get_wtime();
            #pragma omp parallel num_threads(th)
            {
                #pragma omp single
                {
                    resultPar = fiboPar3(n, x);
                }
            }
            end=omp_get_wtime();
            timePar=(end-start);
            acceleration = timeSeq/timePar;
	          fprintf(effFile, "%g\t", acceleration/th);
	          fprintf(accelFile, "%g\t", acceleration);
      }
	    fprintf(effFile, "\n");
	    fprintf(accelFile, "\n");
    }
    fclose(effFile);
    fclose(accelFile);
    return 0;
}


long fibo(int n)
{
  long fnm1, fnm2;
  if ( n < 2 )
    return(n);
  else {
    fnm1 = fibo(n-1);
    fnm2 = fibo(n-2);
    return(fnm1 + fnm2);
  }
}

long fiboPar(int n){
  long fnm1, fnm2;
  if ( n < 2 )
    return(n);
  else {
      #pragma omp task shared(fnm1)
      {
        fnm1 = fiboPar(n-1);
        num_tasks++;
      }
      #pragma omp task shared(fnm2)
      {
        fnm2 = fiboPar(n-2);
        num_tasks++;
      }
      #pragma omp taskwait
      return(fnm1 + fnm2);
  }
}

long fiboPar2(int n){
  long fnm1, fnm2;
  if ( n < 2 )
    return(n);
  else {
      if (n<S)
          return fiboPar2(n-1)+fiboPar2(n-2);
      else {
        #pragma omp task shared(fnm1)
        {
          fnm1 = fiboPar2(n-1);
          num_tasks++;
        }
      #pragma omp task shared(fnm2)
      {
        if (n<S)
          fnm2 = fibo(n-2);
        else 
          fnm2 = fiboPar2(n-2);
        num_tasks++;
      }
      #pragma omp taskwait
      return(fnm1 + fnm2);
    }
  }
}

long fiboPar3(int n, int x){
  long fnm1, fnm2;
  if ( n < 2 )
    return(n);
  else {
    if (x>1){
      #pragma omp task shared(fnm1)
      {
        fnm1 = fiboPar3(n-1,x-1);
        num_tasks++;
      }
      #pragma omp task shared(fnm2)
      {
        fnm2 = fiboPar3(n-2, x-1);
        num_tasks++;
      }
      #pragma omp taskwait
      return(fnm1 + fnm2);
    }
    else {
      return fiboPar3(n-2, -1)+fiboPar3(n-1, -1);
    }     
  } 
}
