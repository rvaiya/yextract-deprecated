SRCDIR=src
BINDIR=bin
BIN=yextract
SOURCE=aux.c vector.c gethtml.c yextract.c
CFLAGS=-lcurl -o $(BIN)
DEBUGFLAGS=-g -DDEBUG 
CC=gcc

all:
	mkdir $(BINDIR);cd $(SRCDIR); $(CC) $(SOURCE) $(CFLAGS) && mv $(BIN) ../$(BINDIR)
debug:
	mkdir $(BINDIR);cd $(SRCDIR); $(CC) $(DEBUGFLAGS) $(SOURCE) $(CFLAGS) && mv $(BIN) ../$(BINDIR)
clean:
	rm $(BINDIR)/$(BIN)
install:
	install $(BINDIR)/$(BIN) /usr/bin

