#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NB_THREADS 4 
#define X 7
#define S 30

long num_tasks = 0;

int main(void)
{
  int n;
  long resultSeq, resultPar, resultPar2;

  double start, end, timeSeq, timePar, timePar2, acceleration, efficacite;
  n = 49;
  /* Begin sequential */
  start=omp_get_wtime();
  resultSeq = fibo(n);
  end=omp_get_wtime();
  timeSeq=(end-start);

  printf("Version seq : fibonacci(%d) = %ld\n", n, resultSeq);
  printf ("temps d'execution seq : %g \n",timeSeq);
  /* End sequential */

  /* Begin fiboPar */
  start=omp_get_wtime();
  #pragma omp parallel num_threads(NB_THREADS)
  {
    #pragma omp single
    {
      resultPar = fiboPar(n);
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
  /* End fiboPar */

  /* Begin */
  start=omp_get_wtime();
  #pragma omp parallel num_threads(NB_THREADS)
  {
    #pragma omp single
    {
      resultPar2 = fiboPar3(n, X);
    }
  }

  end=omp_get_wtime();
  timePar2=(end-start);
  acceleration = timeSeq/timePar2;
  efficacite = acceleration/NB_THREADS;

  printf("Version seq : finonacci(%d) = %ld\n", n, resultPar2);
  printf ("temps d'execution par : %g \n",timePar2);
  printf("nombre de taches : %ld\n", num_tasks);

  printf("Accélération %g\n", acceleration);
  printf("Efficacité : %g\n",efficacite);

  start=omp_get_wtime();
  #pragma omp parallel num_threads(NB_THREADS)
  {
    #pragma omp single
    {
      resultPar2 = fiboPar3(n, X);
    }
  }

  end=omp_get_wtime();
  timePar2=(end-start);
  acceleration = timeSeq/timePar2;
  efficacite = acceleration/NB_THREADS;

  printf("Version seq : finonacci(%d) = %ld\n", n, resultPar2);
  printf ("temps d'execution par : %g \n",timePar2);
  printf("nombre de taches : %ld\n", num_tasks);

  printf("Accélération %g\n", acceleration);
  printf("Efficacité : %g\n",efficacite);
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
    if (n<S){
      return fiboPar2(n-1)+fiboPar2(n-2);
    }
    else {
      #pragma omp task shared(fnm1)
      {
        fnm1 = fiboPar2(n-1);
        num_tasks++;
      }
      #pragma omp task shared(fnm2)
      {
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
      return fiboPar3(n-1, -1)+fiboPar3(n-2, -1);
    }     
  } 
}