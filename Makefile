#obj-m+=ldd.o


obj-m+=ldd.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test.text: test.c
	cat test.c
	cat test.c > test.txt
	cat test.txt


