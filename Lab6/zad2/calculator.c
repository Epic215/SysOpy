#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

double f(double x){
    return 4.0/(x*x+1);
}
double integral(double n, double i,double start,double stop){
    double dx=((stop-start)/n);
    return dx*f(i*dx);
}

int main() {
    double start;
    double stop;
    int n;
    int Read=open("PIPE1",O_RDONLY);
    int Write=open("PIPE2",O_WRONLY);
    read(Read,&start,sizeof(double));
    read(Read,&stop,sizeof(double));
    read(Read,&n,sizeof(int));
    double result=0.0;
    for (int i=0; i<n;i++){
        result+=integral(n,(i+1),start,stop);
    }

    write(Write,&result,sizeof(double));

    return 0;
}