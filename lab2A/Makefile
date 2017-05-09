# NAME: Jeffrey Chan
# EMAIL: jeffschan97@gmail.com
# ID: 004-611-638

.SILENT:

default: build

build:
	gcc -pthread -lrt -o lab2_add -g lab2_add.c
	gcc -pthread -lrt -o lab2_list -g SortedList.c lab2_list.c

tests:
	echo "tests target"

graphs:
	echo "graphs target"

clean:
	rm -f *.tar.gz lab2_add lab2_list

dist:
	tar -czf lab2a-004611638.tar.gz lab2_add.c SortedList.h \
	SortedList.c lab2_list.c Makefile lab2_add.csv lab2_list.csv \
	lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png \
	lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png \
	lab2_list-4.png README 
