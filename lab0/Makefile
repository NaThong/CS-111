# NAME: Jeffrey Chan
# EMAIL: jeffschan97@gmail.com
# ID: 004611638

.SILENT:

default:
	gcc -g lab0.c -o lab0

check: default successTest unrecognizedTest inputTest outputTest segfaultTest catchTest cmpTest
	if [[ -s errorlog.txt ]]; then \
	    cat errorlog.txt; \
	    rm errorlog.txt; \
	else \
	    echo "Passed all tests"; \
	fi

clean:
	rm -f lab0 *.o *.txt *.tar.gz

dist:
	tar -czvf lab0-004611638.tar.gz lab0.c Makefile \
	backtrace.png breakpoint.png README

successTest:
	echo "CS 111" > input.txt; \
	./lab0 --input=input.txt --output=output.txt &> /dev/null; \
	if [[ $$? -ne 0 ]]; then \
	    echo "error: program didn't exit with code 0" >> errorlog.txt; \
	fi; \
	rm -f *.txt

unrecognizedTest:
	./lab0 --jeffrey &> /dev/null; \
	if [[ $$? -ne 1 ]]; then \
	    echo "error: unrecognized argument doesn't exit with code 1" >> errorlog.txt; \
	fi

inputTest:
	echo "CS 111" > input.txt; \
	chmod -r input.txt; \
	./lab0 --input=input.txt --output=output.txt &> /dev/null; \
	if [[ $$? -ne 2 ]]; then \
	    echo "error: unreadable input doesn't exit with code 2" >> errorlog.txt; \
	fi; \
	rm -f *.txt

outputTest:
	echo "CS111" > input.txt; \
	touch output.txt; \
	chmod -w output.txt; \
	./lab0 --input=input.txt --output=output.txt &> /dev/null; \
	if [[ $$? -ne 3 ]]; then \
	    echo "error: unwritable output doesn't exit with code 3" >> errorlog.txt; \
	fi; \
	rm -f *.txt

segfaultTest:
	./lab0 --segfault &> /dev/null; \
	if [[ $$? -ne 139 ]]; then \
	    echo "error: segfault option didn't exit with code 139" >> errorlog.txt; \
	fi

catchTest:
	./lab0 --segfault --catch &> /dev/null; \
	if [[ $$? -ne 4 ]]; then \
	    echo "error: catch test didn't exit with code 4" >> errorlog.txt; \
	fi

cmpTest:
	echo "CS 111" > input.txt; \
	./lab0 --input=input.txt --output=output.txt &> /dev/null; \
	cmp input.txt output.txt; \
	if [[ $$? -ne 0 ]]; then \
	    echo "error: output file isn't copy of input file" >> errorlog.txt; \
	fi; \
	rm -f *.txt
