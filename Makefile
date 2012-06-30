all:
	cc gethtml.c yextract.c -o yextract `pkg-config --libs --cflags libcurl` 
debug:
	cc -g gethtml.c yextract.c -o yextract `pkg-config --libs --cflags libcurl` 
clean:
	rm yextract
install:
	install yextract /usr/bin

