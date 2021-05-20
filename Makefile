CC=gcc-10
FILES=server.c csv.c php.c
OUTDIR=out
all: server
server:
	$(CC) $(FILES) -o $(OUTDIR)/server
clean:
	rm -rf $(OUTDIR)
