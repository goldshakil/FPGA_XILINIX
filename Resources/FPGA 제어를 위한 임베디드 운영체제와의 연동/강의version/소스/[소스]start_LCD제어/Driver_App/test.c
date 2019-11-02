#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define TEXTLCDPORT_BASE          0xbc
#define TEXTLCD_COMMAND_SET     _IOW(TEXTLCDPORT_BASE,0,32)
#define TEXTLCD_FUNCTION_SET    _IOW(TEXTLCDPORT_BASE,1,32)
#define TEXTLCD_DISPLAY_CONTROL _IOW(TEXTLCDPORT_BASE,2,32)
#define TEXTLCD_CURSOR_SHIFT    _IOW(TEXTLCDPORT_BASE,3,32)
#define TEXTLCD_ENTRY_MODE_SET  _IOW(TEXTLCDPORT_BASE,4,32)
#define TEXTLCD_RETURN_HOME     _IOW(TEXTLCDPORT_BASE,5,32)
#define TEXTLCD_CLEAR           _IOW(TEXTLCDPORT_BASE,6,32)
#define TEXTLCD_DD_ADDRESS      _IOW(TEXTLCDPORT_BASE,7,32)
#define TEXTLCD_WRITE_BYTE      _IOW(TEXTLCDPORT_BASE,8,32)


struct strcommand_varible {
    char rows;
    char nfonts;
    char display_enable;
    char cursor_enable;
    char nblink;
    char set_screen;
    char set_rightshit;
    char increase;
    char nshift;
    char pos;
    char command;
    char strlength;
    char buf[20];
};

int main(int argc, char **argv)
{
    int i,j,dev;    
    char indata[80] = {'\0'};
    char indata2[80] = {'\0'};

    struct strcommand_varible strcommand;
    
    strcommand.rows = 0;
    strcommand.nfonts = 0;
    strcommand.display_enable = 1;
    strcommand.cursor_enable = 0;
    strcommand.nblink = 0;
    strcommand.set_screen = 0;
    strcommand.set_rightshit= 1;
    strcommand.increase = 1;
    strcommand.nshift = 0;
    strcommand.pos = 10;
    strcommand.command = 1;
    strcommand.strlength = 16;

    // Device Loading
    dev = open("/dev/textlcdport", O_WRONLY|O_NDELAY );
    
    if (dev == -1)
    {
        printf( "application : Device Open ERROR!\n");
        exit(-1);
    }
    
    write(dev,&strcommand,32);

    ioctl(dev,TEXTLCD_CLEAR,&strcommand,32);
    strcommand.pos = 0;
    ioctl(dev,TEXTLCD_DD_ADDRESS,&strcommand,32);
	fprintf(stdout, "types sentence : ");
	fgets(indata, 80, stdin);	indata[strlen(indata)-1] = '\0';
	if((j=strlen(indata)) > 16)
	{
		memcpy(&strcommand.buf[0], &indata[0], 16);
		strcommand.strlength = 16;
		write(dev, &strcommand, 32);
		memset(strcommand.buf, '\0', sizeof(strcommand.buf));
		strncpy(strcommand.buf, &indata[16], j-16);
		strcommand.rows = 1;
		strcommand.strlength = j-16;
		write(dev, &strcommand, 32);
	}
		
	fprintf(stdout, "types sentence : ");
	fgets(indata2, 80, stdin); indata2[strlen(indata2)-1] = '\0';
	strncat(strcommand.buf, indata2, strlen(indata2));
	strcommand.strlength = strlen(strcommand.buf);
	write(dev, &strcommand, 32);
	
    close(dev);
    
    return(0);
}
