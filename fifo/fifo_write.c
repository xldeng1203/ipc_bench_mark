
/* 
    name:FIFO
    auth:neil deng 
    data:2019-11-28
    desc:FIFO(First In First Out)，又称为有名管道，弥补了管道只能用于有共同祖先的进程间通信的不足，相对于pipe，fifo主要有以下几个优点：
        1. 可以用于任意进程之间通信
        2. 可以有多个读/写进程同时对管道进行操作
        3. 可以像变通文件一样管理管道的权限
        4. 可以使用标准文件读写方式来操作管道(打开，读/写，关闭)  
*/

#include "fifo.h"

int main(int argc, char*argv[])
{
    if(argc != 3)
    {
        perror("usage: ./fifo_write <dataSize> <count>");
        return -1;
    }

    int fifo_fd = 0; 
    int num = 0;
    int size = atoi(argv[1]);
    int count = atoi(argv[2]);  
    int i = 0;
    struct timeval start_time, end_time;
    
    if(access(FIFO_NAME, F_OK)==  -1)  
    {  
        //如果文件不存在,  创建一个fifo
        fifo_fd = mkfifo(FIFO_NAME, 0777);  
    } 

    printf("begin open FIFO_NAME\n");
    fifo_fd = open(FIFO_NAME, O_WRONLY);  //没有数据读取的时候，会阻塞到这里  

    printf("open finished\n");
    char * buf = malloc(size);
    if(buf == NULL)
    {
        perror("fifo write malloc failed");
        return -1;
    }

   gettimeofday(&start_time, NULL);
    for(i = 0; i < count; ++i)
    {
        *(int*)buf = i;
        num = write(fifo_fd, buf, size);  
        if(num == -1)
        {
             printf("Error[%d] when reading data into named pipe\n",errno);  
        }
    }
    gettimeofday(&end_time, NULL);

    double tm = getCostTime(&start_time, &end_time);
    printf("fifo_write :%.0fMB/s %.0fmsg/s  costtime:%lffs \n",
        count * size * 1.0 / (tm * 1024 * 1024),
        count * 1.0 / tm, 
    tm);

    close(fifo_fd);
    free(buf);
    buf = NULL;
    return 0;
}
