CC=clang++
STANDARD=c++11
FLAGS=-Weverything -O3 -Wno-c++98-compat -std=$(STANDARD)

%.o:%.cpp
	$(CC) $(FLAGS) -o $@ -c $<

all: chord spectrum tonegen tempo

chord: chord.o fourier.o
spectrum: spectrum.o fourier.o
tempo: tempo.o fourier.o
tonegen: tonegen.o

clean:
	rm -f chord spectrum tonegen *.o

# Analyse mic input
live: chord
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./chord"

live2: spectrum
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./spectrum"

live3: tempo
	watch -c -t -n .01 "arecord -q -f S16_LE -c1 -r 2000 | ./tempo"

# Analyse generated tone
demo: tonegen chord spectrum
	./tonegen 220 275 330 | ./chord
	./tonegen 11 21 32 | ./spectrum | head -40

# Wait for a cpp to be updated and build
wait:
	while :; do inotifywait -qe modify *.cpp; make; done

progress: tonegen chord
	./tonegen 196.00 246.94 293.66 | aplay -q
	./tonegen 196.00 246.94 293.66 | ./chord
	./tonegen 261.63 329.63 392.00 | aplay -q
	./tonegen 261.63 329.63 392.00 | ./chord
	./tonegen 261.63 311.13 392.00 | aplay -q
	./tonegen 261.63 311.13 392.00 | ./chord

cppcheck:
	cppcheck --enable=all .
