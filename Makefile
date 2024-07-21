ssi: ssInsertion.c
	gcc -fopenmp ssInsertion.c -o ssi.exe -lm

mmi: mmInsertion.c
	gcc -fopenmp mmInsertion.c -o mmi.exe -lm

ssomp: ompssInsertion.c
	gcc -fopenmp ompssInsertion.c -o ssomp.exe -lm

mmomp: ompmmInsertion.c
	gcc -fopenmp ompmmInsertion.c -o mmomp.exe -lm

issomp: ompssInsertion.c
	icc -qopenmp ompssInsertion.c -o issomp.exe -lm

immomp: ompmmInsertion.c
	icc -qopenmp ompmmInsertion.c -o immomp.exe -lm
