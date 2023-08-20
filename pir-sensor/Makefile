CC ?= $(CROSS_COMPILE)gcc
SRC = pir-demo.c
TARGET ?= pir-demo 
OBJS = $(SRC:.c=.o)
LDFLAGS ?= -pthread -lpthread -lrt
all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LDFLAGS)

clean:
	-rm -f *.o $(TARGET) *.elf *.map
