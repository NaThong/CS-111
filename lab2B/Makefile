# NAME: Jeffrey Chan
# EMAIL: jeffschan97@gmail.com
# ID: 004-611-638

#.SILENT:

default: build

build:
	gcc -pthread -lrt -o lab2_list -g SortedList.c lab2_list.c

tests: clean build
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

	-./lab2_list --threads=1  --iterations=1   --yield=id --lists=4  >> lab2_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=id --lists=4  >> lab2_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=id --lists=4  >> lab2_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=id --lists=4  >> lab2_list.csv
	-./lab2_list --threads=16  --iterations=16  --yield=id --lists=4  >> lab2_list.csv

	./lab2_list --threads=1  --iterations=10  --yield=id --lists=4 --sync=s  >> lab2_list.csv
	./lab2_list --threads=4  --iterations=20   --yield=id --lists=4 --sync=s  >> lab2_list.csv
	./lab2_list --threads=8  --iterations=40   --yield=id --lists=4 --sync=s  >> lab2_list.csv
	./lab2_list --threads=12  --iterations=80   --yield=id --lists=4 --sync=s  >> lab2_list.csv

	./lab2_list --threads=1  --iterations=10  --yield=id --lists=4 --sync=m  >> lab2_list.csv
	./lab2_list --threads=4  --iterations=20   --yield=id --lists=4 --sync=m  >> lab2_list.csv
	./lab2_list --threads=8  --iterations=40   --yield=id --lists=4 --sync=m  >> lab2_list.csv
	./lab2_list --threads=12  --iterations=80   --yield=id --lists=4 --sync=m  >> lab2_list.csv


	# ./lab2_list --threads=1  --iterations=1000 --sync=m >> lab2_list.csv
	# ./lab2_list --threads=2  --iterations=1000 --sync=m >> lab2_list.csv
	# ./lab2_list --threads=4  --iterations=1000 --sync=m >> lab2_list.csv
	# ./lab2_list --threads=8  --iterations=1000 --sync=m >> lab2_list.csv
	# ./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2_list.csv
	# ./lab2_list --threads=1  --iterations=1000 --sync=s >> lab2_list.csv
	# ./lab2_list --threads=2  --iterations=1000 --sync=s >> lab2_list.csv
	# ./lab2_list --threads=4  --iterations=1000 --sync=s >> lab2_list.csv
	# ./lab2_list --threads=8  --iterations=1000 --sync=s >> lab2_list.csv
	# ./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2_list.csv

profile:
	echo "profile target"

graphs:
	gnuplot ./lab2_list.gp

clean:
	rm -f *.tar.gz lab2_list lab2_list.csv lab2b_1.png lab2b_2.png \
	lab2b_3.png lab2b_4.png lab2b_5.png

dist:
	tar -czf lab2b-004611638.tar.gz Makefile README
