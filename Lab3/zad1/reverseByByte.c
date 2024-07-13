#include <stdio.h>
#include <time.h>




void reverseByByte(FILE *file1, FILE *file2){
    fseek(file1,0,SEEK_END);
    long long int lenght = ftell(file1);
    fseek(file1,-1,SEEK_CUR);
    while (lenght>0){
        char c = fgetc(file1);
        fseek(file1,-2,SEEK_CUR);
        fputc(c,file2);
        lenght--;
    }
}


int main( int argc, char *argv[] ) {

    if ( argc != 3 )
    {
        printf("Need 2 files as argument");
        return -1;
    }
    FILE *file1;
    FILE *file2;
    file1= fopen(argv[1],"r");
    file2= fopen(argv[2],"w");
    if (file1 == NULL){
        printf("Open error for %s\n", argv[1]);
        return -1;
    }
    if (file2 == NULL){
        printf("Open error for %s\n", argv[1]);
        return -1;
    }
    double time1, timedif1;
    time1 = (double) clock();
    time1 = time1 / CLOCKS_PER_SEC;
    reverseByByte(file1,file2);
    timedif1 = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;

    FILE *timeFile;
    timeFile= fopen("pomiar_zad_2.txt","a+");
    fprintf(timeFile,"Wynik dla reverseByByte dla pliku %s: %f\n",argv[1],timedif1);

    fclose(file1);
    fclose(file2);
    fclose(timeFile);


    return 0;
}