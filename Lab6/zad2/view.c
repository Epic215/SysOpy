#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main() {
    double start;
    double stop;
    int number;
    double result;
    int Write=open("PIPE1",O_WRONLY);
    int Read=open("PIPE2",O_RDONLY);
    printf("Integral range start: \n");
    scanf("%lf", &start);
    printf("Integral range stop: \n");
    scanf("%lf", &stop);
    printf("Integral number of intervals: \n");
    scanf("%d", &number);
    write(Write,&start,sizeof(double));
    write(Write,&stop,sizeof(double));
    write(Write,&number,sizeof(int));


    read(Read,&result,sizeof(double));
    printf("Calculated result: %lf\n", result);



    return 0;
}
