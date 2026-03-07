CC = gcc
CFLAGS = -Wall -Wextra
IMGELIBFLAGS = -lpng -ljpeg
SDLFLAGS = `sdl2-config --cflags --libs`

BIN_DIR = ./bin
TARGET = $(BIN_DIR)/iv

main: $(BIN_DIR) iv.c
	$(CC) $(CFLAGS) iv.c -o $(TARGET) $(IMGELIBFLAGS) $(SDLFLAGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(TARGET)