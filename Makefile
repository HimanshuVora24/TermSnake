CC = gcc 
CFLAGS = -g 
CFILES = main.c
HFILES = snake.h
OFILES = main.o
OBJNAME = main
LINKS = -lncurses

%.o: %.c $(HFILES)
	$(CC) -c $< -o $@

main: $(OFILES)
	@ $(CC) $(OFILES) -o $(OBJNAME) $(LINKS)
clean:
	rm -f *.o
	rm -f main 