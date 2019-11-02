
# Device Driver Makefile

CC     = arm-linux-gcc

KERNELDIR = /usr/local/linux-2.4.19-rmk4-pax2-empx1
INCLUDEDIR = -I$(KERNELDIR)/include -I./

CFLAGS = -D__KERNEL__ -DMODULE -Wall -O2 -I$(INCLUDEDIR)

MODULE_OBJS = segmentport.o
MODULE_SRCS = segmentport.c

DEMO_TARGET = demo
DEMO_OBJS = demo.o
DEMO_SRCS = demo.c

TEST_TARGET = test
TEST_OBJS = test.o
TEST_SRCS = test.c

all: $(MODULE_OBJS) $(TEST_TARGET) $(DEMO_TARGET)

$(MODULE_OBJS) :
	$(CC) $(CFLAGS) -c $(MODULE_SRCS)

$(DEMO_TARGET) : $(DEMO_OBJS)
	$(CC) -o $@ $(DEMO_OBJS) 

$(TEST_TARGET) : $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS) 

clean:
	rm -f *.o
	rm -f $(TEST_TARGET)
	rm -f $(DEMO_TARGET)
