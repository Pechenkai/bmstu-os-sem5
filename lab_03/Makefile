CC := gcc

daemon.out : main.o
	$(CC) main.o -o daemon.out

main.o :
	$(CC) main.c -c

run : daemon.out
	sudo ./daemon.out

log :
	sudo cat /var/log/syslog | grep daemon.out

jlog :
	sudo cat /var/log/syslog

gid:
	sudo cat /var/run/daemon.pid

conf:
	sudo cat /etc/daemon.conf

clean :
	rm -f *.out *.o