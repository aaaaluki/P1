LDLIBS += -lm

all: p1 p1_hamming

p1: p1.o pav_analysis.o fic_wave.o
p1_hamming: p1_hamming.o pav_analysis_hamming.o fic_wave.o

p1.o: p1.c pav_analysis.h fic_wave.h
p1_hamming.o: p1_hamming.c pav_analysis_hamming.h fic_wave.h
fic_wave.o: fic_wave.c fic_wave.h
pav_analysis.o: pav_analysis.c pav_analysis.h
pav_analysis_hamming.o: pav_analysis_hamming.c pav_analysis_hamming.h

clean:
	rm -f *.o
