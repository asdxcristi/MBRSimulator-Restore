CC=gcc
TARGET = mbr
build: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) -o $(TARGET) $(TARGET).c 

clean:
	$(RM) $(TARGET)
