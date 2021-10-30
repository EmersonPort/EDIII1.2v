all : 
	gcc main.c ./IO/IO.c ./auxiliar/functions.c -o exe  -g

run : all
	./exe
