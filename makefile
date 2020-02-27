CC=gcc
CFLAGS=-I $(INCDIR)
CLIBS=-lgmp -lm # -lm only need when executing on Linux

MAKEFLAG=--no-print-directory

BINDIR = bin
INCDIR = inc
SRCDIR = src
OBJDIR = obj

SRCFILES = $(wildcard $(SRCDIR)/*.c)
OBJFILES = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))
INCFILES = inc/automata.h inc/gen.h inc/hash-util.h inc/matrix.h inc/params.h inc/pcp.h inc/population.h inc/util.h

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCFILES)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.c $(INCFILES)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)/main: $(OBJFILES) obj/main.o
	$(CC) -o $@ $^ $(CLIBS)

$(BINDIR)/random-search: $(OBJFILES) obj/random-search.o
	$(CC) -o $@ $^ $(CLIBS)

main: 
	make $(MAKEFLAG) $(BINDIR)/main
random-search: 
	make $(MAKEFLAG) $(BINDIR)/random-search	


.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o
	rm -rf $(BINDIR)/*