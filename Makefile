main : TP2.c test.c
	gcc -o TP2 TP2.c -fopenmp
	gcc -o test test.c -fopenmp
part4: part3.c
	gcc -o part4 part3.c -fopenmp