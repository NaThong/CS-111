# NAME: Jeffrey Chan
# EMAIL: jeffschan97@gmail.com
# ID: 004-611-638

.SILENT:

default: client server
    
client:
	gcc -g -lmcrypt lab1b-client.c -o lab1b-client
    
server:
	gcc -g -lmcrypt lab1b-server.c -o lab1b-server

clean:
	rm -f *.tar.gz lab1b-client lab1b-server *.txt

dist:
	tar -czvf lab1b-004611638.tar.gz Makefile README my.key \
	lab1b-client.c lab1b-server.c
