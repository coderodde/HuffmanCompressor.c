huffc: *.c
	gcc -g *.c -o huffc
	#gcc -O3 -march=native -flto -funroll-loops -fomit-frame-pointer -DNDEBUG *.c -o huffc

