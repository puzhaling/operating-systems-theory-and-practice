COMMONSRC = ../wrappers.c ../error.c

both: server client

server: daytimetcpsrv.c
	gcc -Wall -Werror ${COMMONSRC} $^ -I ../ -o $@

client: daytimetcpcli.c
	gcc -Wall -Werror ${COMMONSRC} $^ -I ../ -o $@
