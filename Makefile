EXE=fetchmail
SOURCES = main.c connect.c login.c retrieve.c parse.c serverCommunication.c mime.c list.c
CFLAGS = -Wall
CC = gcc

$(EXE): main.c
	$(CC) $(CFLAGS) -o $(EXE) $(SOURCES) -lm -g

clean:
	rm -f $(EXE) *.o

format:
	clang-format -style=file -i *.c
