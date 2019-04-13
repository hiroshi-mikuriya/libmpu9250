CC		:= gcc
CFLAGS	:= -Wall -std=gnu99 -fPIC
TARGET	:= libmpu9250.so
HEADER	:= mpu9250.h
SRCS	:= $(wildcard ./*.c)
OBJS	:= $(SRCS:.c=.o)

.PHONY: all
.PHONY: clean
.PHONY: install
.PHONY: uninstall

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -shared -o $@ $(OBJS)

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	$(RM) -f $(TARGET) *.o

install: $(TARGET)
	install $(TARGET) /usr/local/lib
	install $(HEADER) /usr/local/include

uninstall:
	rm /usr/local/lib/$(TARGET)
	rm /usr/local/include/$(HEADER)
