#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#define DEVICE_GPIODRV "/dev/buzzer"
int main()
{
          int fd;
          int tmp;
         
          char *data;
          if((fd=open(DEVICE_GPIODRV,O_RDONLY | O_NONBLOCK))<0)
          {
                      printf("open device: %s\n",DEVICE_GPIODRV);
                      perror("can not open device");
                      exit(1);
          }
          printf("Content-type: text/html\n\n");
          printf("<html>\n");
          printf("<head><title>CGI BUZZER MODE</title></title>\n");
          printf("<body>\n");
          printf("<h1>It's success!</h1>\n");
          printf("</body>\n");
          data=getenv("QUERY_STRING");
          sscanf(data, "tmp=%ld",&tmp);//获得输入的数据
          ioctl(fd,tmp);
          close(fd);
          printf("</html>\n");
          exit(0);
}



