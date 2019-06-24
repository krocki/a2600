GCC=gcc

C_FLAGS=-g -Ofast -fPIC -I./6502/
#C_FLAGS=-Ofast -fPIC -I./6502/
OS:=$(shell uname)

ifeq ($(OS),Darwin) #OSX
  GL_FLAGS=-lglfw -framework OpenGL -lpthread
else # Linux or other
  GL_FLAGS=-lglfw -lGL -lpthread
endif

all: a26

a26: mmu.o 6502/6502.o 6502/6502.h Makefile a26.o
	${GCC} mmu.o a26.o 6502/6502.o ${C_FLAGS} ${GL_FLAGS} -o $@

%.o: %.c
	${GCC} ${C_FLAGS} -c $< -o $@

clean:
	rm -rf *.o ./6502/*.o a26
