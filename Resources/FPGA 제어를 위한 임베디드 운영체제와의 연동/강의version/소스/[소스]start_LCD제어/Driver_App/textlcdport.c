//fimename : textlcdport.c

#include <linux/module.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/ioctl.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/delay.h>
#include "textlcdport.h"

void setcommand(unsigned short command){
  command &= 0x00FF;
  outl((command | 0x0000),TEXTLCDPORT_ADDRESS);
	outl((command | 0x0400),TEXTLCDPORT_ADDRESS);
	outl((command & 0x0000),TEXTLCDPORT_ADDRESS);
  udelay(50);
}

void writebyte(char ch)
{
	unsigned short data;
	data = ch & 0x00FF;

	outl((data | 0x100),TEXTLCDPORT_ADDRESS);
  outl((data | 0x500),TEXTLCDPORT_ADDRESS);
  outl((data | 0x100),TEXTLCDPORT_ADDRESS);
  udelay(50);
}

void initialize_textlcd(){
	function_set(2,0); //Function Set:8bit,display 2lines,5x7 mod
	display_control(1,0,0); // Display on, Cursor off
	clear_display(); // Display clear
	entry_mode_set(1,0); // Entry Mode Set : shift right cursor
	return_home(); // go home
	udelay(2000);
}

void write_string(int row, char *str,int length){
	int i;
	unsigned short command = 0x80;
	unsigned short data;

	command = row ? command + 0x40  : command;
	setcommand(command);
	
	for(i=0;i<length;i++) {
		data = (*(str + i) & 0x00FF);
  	outl((data | 0x100),TEXTLCDPORT_ADDRESS);
  	outl((data | 0x500),TEXTLCDPORT_ADDRESS);
  	outl((data | 0x100),TEXTLCDPORT_ADDRESS);
  	udelay(50);
	}
}

//send Function Set command to the text lcd
// rows = 2 : => 2 rows, rows = 1 => 1 rows
// nfonts = 1 : = > 5x10 dot, nfonts = 0 : 5x7 dot
int function_set(int rows, int nfonts){
	unsigned short command = 0x30;

	if(rows == 2) command |= 0x08;
	else if(rows == 1) command &= 0xf7;
	else return -1;

	command = nfonts ? (command | 0x04) : command;
	setcommand(command);
	
	return 1;
}

int display_control(int display_enable, int cursor_enable, int nblink){
	unsigned short command = 0x08;
	command = display_enable ? (command | 0x04) : command;
	command = cursor_enable ? (command | 0x02) : command;
	command = nblink ? (command | 0x01) : command;
	setcommand(command);
	
	return 1;
}

int cusrsor_shit(int set_screen, int set_rightshit){
	unsigned short command = 0x10;
	command = set_screen ? (command | 0x08) : command;
	command = set_rightshit ? (command | 0x04) : command;
	setcommand(command);

	return 1;
}

int entry_mode_set(int increase, int nshift){
	unsigned short command = 0x04;
	command = increase ? (command | 0x2) : command;
	command = nshift ? ( command | 0x1) : command;
	setcommand(command);
	return 1;
}

int return_home(){
	unsigned short command = 0x02;
	setcommand(command);
	
	udelay(2000);
	return 1;
}

int clear_display(){
	unsigned short command = 0x01;
	setcommand(command);
	udelay(2000);
	return 1;
}

int set_ddram_address(int pos){
	unsigned short command = 0x80;
	command += pos;
	setcommand(command);
	return 1;
}

// define functions...
static int textlcdport_open(struct inode *minode, struct file *mfile) {
	if(textlcdport_usage != 0) return -EBUSY;
	MOD_INC_USE_COUNT;
	textlcdport_usage = 1;
	initialize_textlcd();
	return 0;
}

static int textlcdport_release(struct inode *minode, struct file *mfile) {
	MOD_DEC_USE_COUNT;
	textlcdport_usage = 0;
	return 0;
}

static ssize_t textlcdport_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what) {
	struct strcommand_varible strcommand;
	copy_from_user(&strcommand,gdata,32);

  if(strcommand.rows == 0)
		write_string(0,strcommand.buf,strcommand.strlength);
	else if(strcommand.rows == 1)
		write_string(1,strcommand.buf,strcommand.strlength);

	return length;
}

static int textlcdport_ioctl(struct inode *inode, struct file *file,unsigned int cmd,unsigned long gdata) {
	struct strcommand_varible strcommand;

	copy_from_user(&strcommand,(char *)gdata,32);
	switch(cmd){
	case TEXTLCD_COMMAND_SET:
		setcommand(strcommand.command);
		break;
	case TEXTLCD_FUNCTION_SET:
		function_set((int)(strcommand.rows+1),(int)(strcommand.nfonts));
		break;
	case TEXTLCD_DISPLAY_CONTROL:
		display_control((int)strcommand.display_enable,
				(int)strcommand.cursor_enable,(int)strcommand.nblink);
		break;
	case TEXTLCD_CURSOR_SHIFT:
		cusrsor_shit((int)strcommand.set_screen,(int)strcommand.set_rightshit);
		break;
	case TEXTLCD_ENTRY_MODE_SET:
		entry_mode_set((int)strcommand.increase,(int)strcommand.nshift);
		break;
	case TEXTLCD_RETURN_HOME:
		return_home();
		break;
	case TEXTLCD_CLEAR:
		clear_display();
		break;
	case TEXTLCD_DD_ADDRESS:
		set_ddram_address((int)strcommand.pos);
		break;
	case TEXTLCD_WRITE_BYTE:
		writebyte(strcommand.buf[0]);
		break;
	default:
		printk("driver : no such command!\n");
		return -ENOTTY;
	}
	return 0;
}

static struct file_operations textlcd_fops = {
	write : textlcdport_write,
	ioctl : textlcdport_ioctl,
	open : textlcdport_open,
	release : textlcdport_release,
};

int init_module(void){
	int result;
	result = register_chrdev(TEXTLCDPORT_MAJOR,TEXTLCDPORT_NAME,&textlcd_fops);
	
	if(result < 0) {
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}
	
	textlcdport_major = result;
	if(!check_region(TEXTLCDPORT_ADDRESS,TEXTLCDPORT_ADDRESS_RANGE))
			request_region(TEXTLCDPORT_ADDRESS,TEXTLCDPORT_ADDRESS_RANGE,TEXTLCDPORT_NAME);
	else printk(KERN_WARNING"Can't get IO Region 0x%x\n",TEXTLCDPORT_ADDRESS);
	printk("init module, textlcdport major number : %d\n",result);
	return 0;
}

void cleanup_module(void) {
	release_region(TEXTLCDPORT_ADDRESS,TEXTLCDPORT_ADDRESS_RANGE);
	if(unregister_chrdev(textlcdport_major,TEXTLCDPORT_NAME)) 
		printk(KERN_WARNING"%s DRIVER CLEANUP FALLED\n",TEXTLCDPORT_NAME);
}














