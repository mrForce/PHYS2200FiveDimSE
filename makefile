
GRAPH = gnuplot

CC = clang
CFLAGS = -Wall -O3 
LFLAGS = -O3
LIBS = -lgsl -lgslcblas -lm 

all: final_part_one

final_part_one: final_part_one.o
	${CC} $(LFLAGS) -o $@ $^ $(LIBS)

results.png: results.gpl results.txt
	$(GRAPH) results.gpl
clean:
	final_part_one
	rm -f *.o *~

veryclean: clean
	rm -f final_part_one.o
