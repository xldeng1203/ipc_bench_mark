
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
    if(argc != 2)
    {
        perror("usage: ./fifo_read <dataSize> ");
        return -1;
    }

    int fifo_fd = 0; 
    int num = 0; 
    int size = atoi(argv[1]); 
    struct timeval start_time, end_time;

    if(access(FIFO_NAME, F_OK)==  -1)  
    {  
        //如果文件不存在,  创建一个fifo
        fifo_fd = mkfifo(FIFO_NAME, 0777); 
        printf("mkfifo FIFO_NAME\n"); 
    } 

    printf("begin open FIFO_NAME\n");

    fifo_fd = open(FIFO_NAME, O_RDONLY); //没有数据到达的时候，会阻塞在这里

    printf("open finished");

    char * buf = malloc(size);
    if(buf == NULL)
    {
        perror("fifo client malloc failed");
        return -1;
    }

    int n = 0;
    int count = 0;
    gettimeofday(&start_time, NULL);
    while(1)
    {   
        printf("Loop ... count:%d\n", count++);
        num = read(fifo_fd, buf, size);
        if(num < 0 )
        {
            printf("Error[%d] fifo_read when reading data into named pipe\n",errno);  
            break;
        }
        else if (num == 0)
        {
            perror("fifo_read read num == 0 ");
            break;
        }
        printf("fifo_read read i = %d\n", (*(int*)buf));
        n += num;
    }
    printf("fifo_read read n =%d \n", n);

    gettimeofday(&end_time, NULL);
    double tm = getCostTime(&start_time, &end_time);
    printf("fifo_read :%.0fMB/s %.0fmsg/s  costtime:%lffs \n",
        count * size * 1.0 / (tm * 1024 * 1024),
        count * 1.0 / tm, 
    tm);

    free(buf);
    buf = NULL;
    close(fifo_fd);  
    return 0;
}
