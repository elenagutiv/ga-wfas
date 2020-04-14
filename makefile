CC=gcc
CFLAGS=-I $(INCDIR)
CLIBS=-lgmp -lm # -lm only need when executing on Linux

MAKEFLAG=--no-print-directory

BINDIR = bin
INCDIR = inc
SRCDIR = src
OBJDIR = obj
PLOTSDIR = plots
RESDIR = res

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

$(BINDIR)/bf-search: $(OBJFILES) obj/bf-search.o
	$(CC) -o $@ $^ $(CLIBS)

main: 
	make $(MAKEFLAG) $(BINDIR)/main
random-search: 
	make $(MAKEFLAG) $(BINDIR)/random-search	
bf-search: 
	make $(MAKEFLAG) $(BINDIR)/bf-search	

env:
	mkdir bin
	mkdir obj
	mkdir res
	cd res; mkdir bf-search; mkdir ga; mkdir random-search;\
	cd ga/; mkdir random; mkdir paren;\
	cd random/; mkdir exp-bf-search; mkdir exp-comparison; mkdir exp-memoization;\
	cd exp-memoization/; mkdir memo/; mkdir no-memo/;\
	cd ../../paren/; mkdir exp-paren;\
	cd ../../random-search/; mkdir random;\
	cd random/; mkdir exp-comparison;\
	cd ../../bf-search/; mkdir random;\
	cd random/; mkdir exp-bf-search;\
	cd ../../../; mkdir plots;\
	cd plots/; mkdir random; mkdir paren;\
	cd random; mkdir exp-comparison; mkdir exp-bf-search;\
	cd ../paren/; mkdir exp-paren	

.PHONY: clean

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -rf $(PLOTSDIR)

clean-all:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -rf $(PLOTSDIR)
	rm -rf $(RESDIR)