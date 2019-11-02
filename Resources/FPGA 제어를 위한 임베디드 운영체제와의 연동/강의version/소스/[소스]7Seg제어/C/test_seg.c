/* ************************************************************ */
/*    Test program of 7 segment Digits(4 digits) display        */
/*    Hanback Electronics                                       */
/*    Date: March 2003                                           */
/* ************************************************************ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <linux/delay.h>

int main()
{
    int     dev;
    int     i,j,dir;
    int     index =0;
    unsigned short buff;
    int seg_data[16] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71 };
    //                     0     1    2      3     4     5      6    7     8     9     A    b     C     d     E     F
    int digit[6] = { 0x0100, 0x0200, 0x0400, 0x0800,0x1000,0x2000 };
    
    // Device Open..    
    dev = open("/dev/segmentport", O_WRONLY);
    if(dev < 0) {
        printf("application : segment driver open fails!\n");
        return -1;
    }
    
    // 1번째 7세그먼트에 A 출력
    nindex = 10;
    buff = (seg_data[j+index] | digit[0]);
    write(dev,&buff,1); 
    for(i=0;i<20000;i++);

    // 2번째 7세그먼트에 9 출력 
    nindex = 9;  
    buff = (seg_data[j+index] | digit[1]);
    write(dev,&buff,1); 
    for(i=0;i<20000;i++);

    // 3번째 7세그먼트에 4 출력 
    nindex = 4;  
    buff = (seg_data[j+index] | digit[1]);
    write(dev,&buff,1); 
    for(i=0;i<20000;i++);

    close(dev);
  return 0;
}


