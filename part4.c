#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NB_THREADS 4 
#define N 40
#define X 5

long num_tasks = 0;

long fibo(int n);
long fiboPar3(int n, int x);


int main(void)
{
  int n, x;
  long resultSeq, resultPar;

  double start, end, timeSeq, timePar, acceleration, efficacite;
  n = N;
  x = X;
  /* Begin sequential */
  start=omp_get_wtime();
  resultSeq = fibo(n);
  end=omp_get_wtime();
  timeSeq=(end-start);
  /* End sequential */

  /* Begin fiboPar2 */
  start=omp_get_wtime();
  #pragma omp parallel num_threads(NB_THREADS)
  {
    #pragma omp single
    {
      resultPar = fiboPar3(n, x);
    }
  }

  end=omp_get_wtime();
  timePar=(end-start);
  acceleration = timeSeq/timePar;
  efficacite = acceleration/NB_THREADS;

  printf("Version seq : finonacci(%d) = %ld\n", n, resultPar);
  printf ("temps d'execution par : %g \n",timePar);
  printf("nombre de taches : %ld\n", num_tasks);

  printf("Accélération %g\n", acceleration);
  printf("Efficacité : %g\n",efficacite);
  /* End fiboPar3 */

  /* begin seuil test */
  for (int i=0; i<5; i++){
        x += 5;
        start=omp_get_wtime();
        #pragma omp parallel num_threads(NB_THREADS)
        {
            #pragma omp single
            {
                resultPar = fiboPar3(n,x);
            }
        }

        end=omp_get_wtime();
        timePar=(end-start);
        acceleration = timeSeq/timePar;
        efficacite = acceleration/NB_THREADS;
        printf("x = %d\n", x);
        printf("nombre de taches : %ld\n", num_tasks);
        printf("Accélération %g\n", acceleration);
        printf("Efficacité : %g\n\n",efficacite);
    }
    /* End seuil test */

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
      return fiboPar3(n-1, -1)+fiboPar3(n-2, -1);
    }     
  } 
}