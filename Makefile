#GRECU STEFANIA 313CB

build:
	gcc -g -o quadtree main.c functii.c -lm
run:
	./quadtree
clean:
	rm quadtree
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./quadtree