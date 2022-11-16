#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N   10
#define NB_THREADS_MIN  4
#define NB_THREADS_MAX  16
#define x_min   3
#define x_max   10

accel[NB_THREADS_MAX][x_max];
eff[NB_THREADS_MAX][x_max];

long num_tasks = 0;

long fibo(int);
long fiboPar(int);
long fiboPar2(int);
long fiboPar3(int, int);

int main(){
    int n;
    long resultSeq, resultPar, resultPar2, resultPar3;

    double start, end, timeSeq, timePar, timePar2, timePar3, acceleration, efficacite;
    n = N;
    for (int th=NB_THREADS_MIN;th<=NB_THREADS_MAX;th++){
        for(int x=x_min;x<=x_max;x++){
            start=omp_get_wtime();
            resultSeq = fibo(n);
            end=omp_get_wtime();
            timeSeq=(end-start);

            start=omp_get_wtime();
            #pragma omp parallel num_threads(th)
            {
                #pragma omp single
                {
                    resultPar3 = fiboPar3(n, x);
                }
            }
            end=omp_get_wtime();
            timePar3=(end-start);
            accel[th][x] = timeSeq/timePar3;
            eff[th][x] = acceleration/th;

            printf("accel[%d][%d] = %ld\n");
            printf("eff[%d][%d] = %ld\n");
        }

    }
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
      #pragma omp task shared(fnm1)
      {
        if (n<30)
          fnm1 = fibo(n-1);
        else
          fnm1 = fiboPar2(n-1);
        num_tasks++;
      }
      #pragma omp task shared(fnm2)
      {
        if (n<30)
          fnm2 = fibo(n-2);
        else 
          fnm2 = fiboPar2(n-2);
        num_tasks++;
      }
      #pragma omp taskwait
      return(fnm1 + fnm2);
  }
}

long fiboPar3(int n, int x){
  long fnm1, fnm2;
  if ( n < 2 )
    return(n);
  else {
    if (x>0){
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
      return fibo(n-1);
    }     
  } 
}