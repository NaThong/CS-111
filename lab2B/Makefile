# NAME: Jeffrey Chan
# EMAIL: jeffschan97@gmail.com
# ID: 004-611-638

.SILENT:

default: build

build:
	gcc -pthread -lrt -o lab2_list -g SortedList.c lab2_list.c

tests: clean build
	# PLOT 1 AND 2 TEST CASES
	./lab2_list --threads=1  --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv

	# PLOT 3 TEST CASES
	-./lab2_list --threads=1  --iterations=1   --yield=id --lists=4  >> lab2b_list.csv
	-./lab2_list --threads=2  --iterations=2   --yield=id --lists=4  >> lab2b_list.csv
	-./lab2_list --threads=4  --iterations=4   --yield=id --lists=4  >> lab2b_list.csv
	-./lab2_list --threads=8  --iterations=8   --yield=id --lists=4  >> lab2b_list.csv
	-./lab2_list --threads=16  --iterations=16  --yield=id --lists=4  >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=10  --yield=id --lists=4 --sync=s  >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=20   --yield=id --lists=4 --sync=s  >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=40   --yield=id --lists=4 --sync=s  >> lab2b_list.csv
	./lab2_list --threads=12  --iterations=80   --yield=id --lists=4 --sync=s  >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=10  --yield=id --lists=4 --sync=m  >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=20   --yield=id --lists=4 --sync=m  >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=40   --yield=id --lists=4 --sync=m  >> lab2b_list.csv
	./lab2_list --threads=12  --iterations=80   --yield=id --lists=4 --sync=m  >> lab2b_list.csv

	# PLOT 4 TEST CASES
	# ./lab2_list --threads=1  --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	# ./lab2_list --threads=2  --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	# ./lab2_list --threads=4  --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	# ./lab2_list --threads=8  --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	# ./lab2_list --threads=12  --iterations=1000 --sync=m --lists=1 >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	./lab2_list --threads=12  --iterations=1000 --sync=m --lists=4 >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	./lab2_list --threads=12  --iterations=1000 --sync=m --lists=8 >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv
	./lab2_list --threads=12  --iterations=1000 --sync=m --lists=16 >> lab2b_list.csv

	# PLOT 5 TEST CASES
	# ./lab2_list --threads=1  --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	# ./lab2_list --threads=2  --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	# ./lab2_list --threads=4  --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	# ./lab2_list --threads=8  --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	# ./lab2_list --threads=12  --iterations=1000 --sync=s --lists=1 >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	./lab2_list --threads=12  --iterations=1000 --sync=s --lists=4 >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	./lab2_list --threads=12  --iterations=1000 --sync=s --lists=8 >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv
	./lab2_list --threads=12  --iterations=1000 --sync=s --lists=16 >> lab2b_list.csv

profile: lab2_list Makefile
	-rm -f ./raw.gperf
	LD_PRELOAD=/u/ee/ugrad/jeffreyc/lib/libprofiler.so.0 CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text ./lab2_list ./raw.gperf > profile.out
	pprof --list=listOperations ./lab2_list ./raw.gperf >> profile.out
	rm -f ./raw.gperf

graphs:
	gnuplot ./lab2_list.gp

clean:
	rm -f *.tar.gz lab2_list lab2b_list.csv lab2b_1.png lab2b_2.png \
	lab2b_3.png lab2b_4.png lab2b_5.png

dist:
	tar -czf lab2b-004611638.tar.gz Makefile README SortedList.h \
	SortedList.c lab2_list.c lab2b_list.csv profile.out lab2b_1.png \
	lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png lab2_list.gp
