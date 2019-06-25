GCC=gcc
INCLUDES=-I./6502/
#C_FLAGS=-Wpedantic -fmax-errors=3 -Wall -std=gnu99 -g3 -fPIC ${INCLUDES}

C_FLAGS=-Wpedantic -ferror-limit=3 -Wall -std=gnu11 -g3 -fPIC ${INCLUDES}
#C_FLAGS=-Ofast -fPIC ${INCLUDES}

OS:=$(shell uname)

ifeq ($(OS),Darwin) #OSX
  GL_FLAGS=-lglfw -framework OpenGL -lpthread
else # Linux or other
  GL_FLAGS=-lglfw -lGL -lpthread
endif

objs=main.o io.o a26.o lcd.o pia.o tia.o mmu.o 6502/6502.o
all: a26

a26: ${objs}
	${GCC} $^ ${C_FLAGS} ${GL_FLAGS} -o $@

%.o: %.c %.h Makefile
	${GCC} ${C_FLAGS} -c $< -o $@

%.o: %.c Makefile
	${GCC} ${C_FLAGS} -c $< -o $@

clean:
	rm -rf ${objs} a26
