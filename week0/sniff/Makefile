all: pcap

matcher:
	gcc -g -c matcher.c -lpcre -o matcher.o

pcap: matcher
	gcc -g pcap.c matcher.o -lpcap -lpcre -o pcap

clean:
	rm *.o
	rm pcap
