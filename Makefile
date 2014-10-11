all: pcap

matcher:
	gcc -c week0/matcher.c -lpcre -o matcher.o

pcap: matcher
	gcc week0/pcap.c matcher.o -lpcap -lpcre -o pcap

clean:
	rm *.o
	rm pcap
