//fimename : segmentport.c

#include <linux/module.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/ioctl.h>
#include <linux/ioport.h>
#include <linux/delay.h>

#define SEGMENTPORT_MAJOR   0
#define SEGMENTPORT_NAME    "SEGMENT PORT"
#define SEGMENTPORT_MODULE_VERSION "SEGMENT PORT V0.1"
#define SEGMENTPORT_ADDRESS 0xf1840000
#define SEGMENTPORT_ADDRESS_RANGE 1

unsigned int *addr;

//Global variable
static int segmentport_usage = 0;
static int segmentport_major = 0;

// define functions...
int segmentport_open(struct inode *minode, struct file *mfile) {

	if(segmentport_usage != 0) return -EBUSY;
	MOD_INC_USE_COUNT;
	segmentport_usage = 1;
	addr = (unsigned int *)(SEGMENTPORT_ADDRESS);	
	return 0;
}

int segmentport_release(struct inode *minode, struct file *mfile) {
	MOD_DEC_USE_COUNT;
	segmentport_usage = 0;
	return 0;
}

ssize_t segmentport_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what) 
{
	unsigned int val;
	get_user(val,(unsigned short *)gdata);
	*addr = val;
	return length;
}

struct file_operations segment_fops = {
	write: segmentport_write,
	open: segmentport_open,
	release: segmentport_release,
};

int init_module(void) {

	int result;
	result = register_chrdev(SEGMENTPORT_MAJOR,SEGMENTPORT_NAME,&segment_fops);

	if(result < 0) {
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}
	
	segmentport_major = result;
	
	if(!check_region(SEGMENTPORT_ADDRESS,SEGMENTPORT_ADDRESS_RANGE))	{
		request_region(SEGMENTPORT_ADDRESS,SEGMENTPORT_ADDRESS_RANGE,SEGMENTPORT_NAME);
		printk("init module, segmentport major number : %d\n",result);
	}
	else printk("driver : unable to register this!\n");
	
	return 0;
}

void cleanup_module(void) {
		release_region(SEGMENTPORT_ADDRESS,SEGMENTPORT_ADDRESS_RANGE);
	if(!unregister_chrdev(segmentport_major,SEGMENTPORT_NAME))
		printk("driver : %s DRIVER CLEANUP Ok\n",SEGMENTPORT_NAME);
	else
		printk("driver : %s DRIVER CLEANUP FALLED\n",SEGMENTPORT_NAME);
}














