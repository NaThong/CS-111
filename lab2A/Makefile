# NAME: Jeffrey Chan
# EMAIL: jeffschan97@gmail.com
# ID: 004-611-638

#.SILENT:

default: build

build:
	gcc -pthread -lrt -o lab2_add -g lab2_add.c
	gcc -pthread -lrt -o lab2_list -g SortedList.c lab2_list.c

tests: clean build
	./lab2_list --threads=1  --iterations=10	      >> lab2_list.csv
	./lab2_list --threads=1  --iterations=100	      >> lab2_list.csv
	./lab2_list --threads=1  --iterations=1000	      >> lab2_list.csv
	./lab2_list --threads=1  --iterations=10000	      >> lab2_list.csv
	./lab2_list --threads=1  --iterations=20000	      >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1              >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=10             >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=100            >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=1000           >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1              >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=10             >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=100            >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1000           >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1              >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=10             >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=100            >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1000           >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1              >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=10             >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=100            >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1000           >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=4   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=8   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=16  --yield=i  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=32  --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=2   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=8   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=16  --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=2   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=4   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=16  --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8   --yield=i  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16  --yield=i  >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=4   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=8   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=16  --yield=d  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=32  --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=2   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=8   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=16  --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=2   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=4   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=16  --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8   --yield=d  >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16  --yield=d  >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1   --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=4   --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=8   --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=16  --yield=il >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=32  --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1   --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=2   --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=8   --yield=il >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=16  --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1   --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=2   --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=4   --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=il >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=16  --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1   --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2   --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4   --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8   --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16  --yield=il >> lab2_list.csv

	-./lab2_list --threads=2  --iterations=1   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=4   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=8   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=16  --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=32  --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=1   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=2   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=8   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=16  --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=1   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=2   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=4   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=16  --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=4   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=8   --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=16  --yield=dl >> lab2_list.csv

	./lab2_list --threads=12 --iterations=32 --yield=i  --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=d  --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=i  --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=d  --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=32 --yield=dl --sync=s >> lab2_list.csv

	./lab2_list --threads=1  --iterations=1000          >> lab2_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

graphs: clean tests
	export PATH=/usr/local/cs/bin:$PATH
	gnuplot ./lab2_add.gp
	gnuplot ./lab2_list.gp

clean:
	rm -f *.tar.gz lab2_add lab2_list

dist:
	tar -czf lab2a-004611638.tar.gz lab2_add.c SortedList.h \
	SortedList.c lab2_list.c Makefile lab2_add.csv lab2_list.csv \
	lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png \
	lab2_add-5.png lab2_list-1.png lab2_list-2.png lab2_list-3.png \
	lab2_list-4.png README 
