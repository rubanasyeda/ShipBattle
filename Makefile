#Rubana Syeda
#Tony Kusalik CMPT 214 (03)


.PHONY: clean librn all
CFLAGS = -Wall -Wextra
LDGLAGS = -lrn


all: shipbattle librn
	
# rule to make ship battle
shipbattle: gameplay.o gameboard.o shipbattle.o

#rules to build object files needed for shipbattle
gameplay.o: gameplay.c gameplay.h gameboard.h

gameboard.o: gameboard.c gameboard.h gameplay.h rand_num.h

shipbattle.o: shipbattle.c rand_num.h gameplay.h
	

# rule to build both the shared and static library 
librn: librn.so librn.a
	
#rule to build the object file from rand_num.c
rand_num.o: rand_num.c 
	gcc $(CFLAGS) -c rand_num.c
	
#rule to build the shared dynamic library 	
librn.so: rand_num.o
	gcc -shared -fpic -o librn.so rand_num.o

#rule to build the static library
librn.a: rand_num.o
	ar -r -c -s librn.a rand_num.o

# removes all makefile targets
clean: 
	rm -f *.o librn* shipbattle