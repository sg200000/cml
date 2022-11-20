main : TP2.c test.c
	gcc -o TP2 TP2.c -fopenmp
	gcc -o test test.c -fopenmp
