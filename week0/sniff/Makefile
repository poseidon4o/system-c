all: pcap

matcher:
	gcc -c matcher.c -lpcre -o matcher.o

pcap: matcher
	gcc pcap.c matcher.o -lpcap -lpcre -o pcap

clean:
	rm *.o
	rm pcap
