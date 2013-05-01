all:
	cc aux.c vector.c gethtml.c yextract.c -lcurl -o yextract 
debug:
	cc -g aux.c vector.c gethtml.c yextract.c -lcurl -o yextract 
clean:
	rm yextract
install:
	install yextract /usr/bin

