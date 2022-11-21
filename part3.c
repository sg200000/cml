#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NB_THREADS 4 
#define N 48
#define S 30

long num_tasks = 0;

long fibo(int n);
long fiboPar2(int n, int s);


int main(void)
{
  int n, s;
  long resultSeq, resultPar;

  double start, end, timeSeq, timePar, acceleration, efficacite;
  n = N;
  s = S;
  /* Begin sequential */
  start=omp_get_wtime();
  resultSeq = fibo(n);
  end=omp_get_wtime();
  timeSeq=(end-start);

  printf("Version seq : fibonacci(%d) = %ld\n", n, resultSeq);
  printf ("temps d'execution seq : %g \n",timeSeq);
  /* End sequential */

  /* Begin fiboPar2 */
  start=omp_get_wtime();
  #pragma omp parallel num_threads(NB_THREADS)
  {
    #pragma omp single
    {
      resultPar = fiboPar2(n, s);
    }
  }

  end=omp_get_wtime();
  timePar=(end-start);
  acceleration = timeSeq/timePar;
  efficacite = acceleration/NB_THREADS;

  printf("Version sequentiel : finonacci(%d) = %ld\n", n, resultSeq);
  printf("Version parallèle  : finonacci(%d) = %ld\n", n, resultPar);
  printf ("temps d'execution par : %g \n",timePar);
  printf("nombre de taches : %ld\n", num_tasks);

  printf("Accélération %g\n", acceleration);
  printf("Efficacité : %g\n",efficacite);
  /* End fiboPar2 */

  /* begin seuil test */
  for (int i=S; i<5; i++){
        s += 5;
        start=omp_get_wtime();
        #pragma omp parallel num_threads(NB_THREADS)
        {
            #pragma omp single
            {
                resultPar = fiboPar2(n,s);
            }
        }

        end=omp_get_wtime();
        timePar=(end-start);
        acceleration = timeSeq/timePar;
        efficacite = acceleration/NB_THREADS;

        printf("Version sequentiel : finonacci(%d) = %ld\n", n, resultSeq);
        printf("Version parallèle  : finonacci(%d) = %ld\n", n, resultPar);
        printf ("temps d'execution par : %g \n",timePar);
        printf("nombre de taches : %ld\n", num_tasks);

        printf("Accélération %g\n", acceleration);
        printf("Efficacité : %g\n",efficacite);
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

long fiboPar2(int n, int s){
  long fnm1, fnm2;
  if ( n < 2 )
    return(n);
  else {
    if (n<s){
      return fiboPar2(n-1, s)+fiboPar2(n-2, s);
    }
    else {
      #pragma omp task shared(fnm1)
      {
        fnm1 = fiboPar2(n-1, s);
        num_tasks++;
      }
      #pragma omp task shared(fnm2)
      {
        fnm2 = fiboPar2(n-2, s);
        num_tasks++;
      }
      #pragma omp taskwait
      return(fnm1 + fnm2);
    }
      
  }
}