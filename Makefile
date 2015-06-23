GCC=../bin/arm-linux-androideabi-gcc-4.8
LIBS=-llog
SRC=LearnRoot.c
DES=LearnRoot

all:
	$(GCC) $(LIBS) $(SRC) -o $(DES)
