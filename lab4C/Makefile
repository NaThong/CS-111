# NAME: Jeffrey Chan
# EMAIL: jeffschan97@gmail.com
# ID: 004-611-638

.SILENT:

default:
	gcc -lmraa -lm -g lab4c_tcp.c -o lab4c_tcp
	gcc -lmraa -lm -lssl -lcrypto -g lab4c_tls.c -o lab4c_tls

clean:
	rm -f *.tar.gz *.txt lab4c_tcp lab4c_tls

dist:
	tar -czf lab4c-004611638.tar.gz Makefile README lab4c_tcp.c lab4c_tls.c
