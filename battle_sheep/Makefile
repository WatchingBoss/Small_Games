CC = gcc
CLFLAGS = -lncurses

OBJ = game.o
OUT = go

$(OUT): $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(CLFLAGS)

clean:
	$(RM) $(OUT) $(OBJ)
