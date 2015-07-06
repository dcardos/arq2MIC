all:
	echo compilando
    icc mandelbrotNP.c -o mandelbrotNP.out
	
	echo executando
	mandelbrotNP.out