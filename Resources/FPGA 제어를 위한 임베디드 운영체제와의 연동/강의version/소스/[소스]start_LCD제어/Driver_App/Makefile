
# Device Driver Makefile

CC     = /usr/cross-tools/bin/arm-linux-gcc

KERNELDIR = /usr/local/linux-2.4.19-rmk4-pax2-empx1
INCLUDEDIR = -I$(KERNELDIR)/include -I./

CFLAGS = -D__KERNEL__ -DMODULE -Wall -O2 -I$(INCLUDEDIR)

MODULE_OBJS = textlcdport.o
MODULE_SRCS = textlcdport.c

TEST_TARGET = test
TEST_OBJS = test.o
TEST_SRCS = test.c

all: $(MODULE_OBJS) $(TEST_TARGET)

$(MODULE_OBJS) :
	$(CC) $(CFLAGS) -c $(MODULE_SRCS)

$(TEST_TARGET) : 
	$(CC)  -o $(TEST_TARGET) $(TEST_SRCS)

clean:
	rm -f *.o
	rm -f $(TEST_TARGET)
