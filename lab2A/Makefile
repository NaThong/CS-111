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
	tar -czf lab2a-004611638.tar.gz Makefile
