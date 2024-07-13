#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

double f(double x){
    return 4.0/(x*x+1);
}
double integral(double n, double i){
    double dx=(1/n);
    return dx*f(i*dx);
}

int main(int argc, char *argv[]) {


    if (argc!=3){
        printf("Niepoprawna ilosc argumentow powinny być 2 argumenty\n");
        return -1;
    }
    double width=strtod(1[argv],NULL);
    double n=strtod(2[argv],NULL);
    if(1/width<n){
        printf("Mniej przedzialow niz procesow\n");
        return -1;
    }
    int N = (int) n;
    int number_of_intervals=ceil(1.0/width);
    int for_one_process=number_of_intervals/N;
    int ends[N];

    double time2, timedif2;
    time2 = (double) clock();
    time2 = time2 / CLOCKS_PER_SEC;


    for (int i=0;i<N;i++){
        ends[i]=for_one_process;
    }
    if (for_one_process*N!=number_of_intervals){
        for (int i=0;for_one_process*N+i<number_of_intervals;i++){
            ends[i]++;
        }
    }
    for (int i=0;i<N-1;i++){
        ends[i+1]+=ends[i];
    }
    int fdT[N][2];
    for(int i=0; i<N;i++){

        pipe(fdT[i]);

        int pid=fork();
        if(pid==0){
            close(fdT[i][0]);
            double iresult=0;
            int k=1;
            for (int j = (i == 0 ? 0 : ends[i - 1]); j < ends[i]; j++) {
                iresult += integral(n, (j+1));
                k++;
            }
            write(fdT[i][1],&iresult,sizeof(double));
            return 0;
        }

        close(fdT[i][1]);

    }

    double result=0.0;
    for(int i=0; i<N;i++){
        double el;
        read(fdT[i][0],&el,sizeof(double));
        result+=el;
    }
    timedif2 = ( ((double) clock()) / CLOCKS_PER_SEC) - time2;
    FILE *timeFile;
    timeFile= fopen("pomiar_zad_1.txt","a+");
    fprintf(timeFile,"Dokladnosc obliczen: %.9f, liczba procesow potomnych:  %d, czas trwania: %f\n",width,N,timedif2);

    printf("Calka wynosi: %lf\n",result);
    printf("Dokladnosc obliczen: %.9f, liczba procesow potomnych:  %d, czas trwania: %f\n",width,N,timedif2);
    fclose(timeFile);
    return 0;
}
