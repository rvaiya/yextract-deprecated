all:
	cc gethtml.c yextract.c -lcurl -o yextract 
debug:
	cc -g gethtml.c yextract.c -lcurl -o yextract 
clean:
	rm yextract
install:
	install yextract /usr/bin

