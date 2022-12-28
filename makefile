all: numerology.h numerology.c ut.c
	gcc -o ut numerology.c ut.c

clean:
	rm -rf ut *.o
