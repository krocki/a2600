all: t1.bin

%.bin : %.asm
	 dasm $< -f3 -o$@

clean:
	rm -rf *.bin *.lst
