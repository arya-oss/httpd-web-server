CC=gcc
CFLAGS= -Wall -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
LIBS=-I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lpthread
SRC=src
BUILD=build
OPTS= -c
server.out: $(BUILD)/tcp.o $(BUILD)/utils.o
	$(CC) $(CFLAGS) $(BUILD)/tcp.o $(BUILD)/utils.o $(SRC)/main.c -o $(BUILD)/server.out $(LIBS)

$(BUILD)/tcp.o: $(SRC)/tcp.c
	$(CC) $(OPTS) $(SRC)/tcp.c -o $(BUILD)/tcp.o 

$(BUILD)/utils.o: $(SRC)/utils.c
	$(CC) $(CFLAGS) $(OPTS) $(SRC)/utils.c -o $(BUILD)/utils.o $(LIBS)

clean:
	$(BUILD)/%.o $(BUILD)/%.out
	