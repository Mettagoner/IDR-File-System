CC = gcc  # The compiler being used

# Compiler flags:
# -g    adds debugging information to the executable file
# -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# The build target executable:
TARGET = fsinterface

all: $(TARGET)

$(TARGET): FSInterface/fsinterface.o FSCore/fscore.o DiskLib/disklib.o
	$(CC) $(CFLAGS) -o $(TARGET) FSInterface/fsinterface.o FSCore/fscore.o DiskLib/disklib.o

FSInterface/fsinterface.o: FSInterface/fsinterface.c FSCore/fscore.h
	$(CC) $(CFLAGS) -c FSInterface/fsinterface.c -o FSInterface/fsinterface.o

FSCore/fscore.o: FSCore/fscore.c FSCore/fscore.h DiskLib/disklib.h
	$(CC) $(CFLAGS) -c FSCore/fscore.c -o FSCore/fscore.o

DiskLib/disklib.o: DiskLib/disklib.c DiskLib/disklib.h
	$(CC) $(CFLAGS) -c DiskLib/disklib.c -o DiskLib/disklib.o

clean:
	$(RM) $(TARGET) FSInterface/*.o FSCore/*.o DiskLib/*.o *~
