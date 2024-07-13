#include <stdio.h>
#include <time.h>

int BUFFER_SIZE=1024;



void reverseByBlocks(FILE *file1, FILE *file2){
    fseek(file1,0,SEEK_END);
    long long int lenght = ftell(file1);
    int toCopy=BUFFER_SIZE;
    if (lenght<toCopy){
        toCopy=lenght;
    }
    fseek(file1,-toCopy,SEEK_CUR);
    while (lenght>0){
        char buffer[BUFFER_SIZE];
        fread(buffer,1,toCopy,file1);
        for (int i=0;i<toCopy/2;i++){
            char c=buffer[i];
            buffer[i]=buffer[toCopy-i-1];
            buffer[toCopy-i-1]=c;
        }

        fwrite(buffer,1,toCopy,file2);
        lenght-=BUFFER_SIZE;
        if (lenght<toCopy){
            toCopy=lenght;
        }
        fseek(file1,-2*toCopy,SEEK_CUR);
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

    double time2, timedif2;
    time2 = (double) clock();
    time2 = time2 / CLOCKS_PER_SEC;
    reverseByBlocks(file1,file2);
    timedif2 = ( ((double) clock()) / CLOCKS_PER_SEC) - time2;
    FILE *timeFile;
    timeFile= fopen("pomiar_zad_2.txt","a+");
    fprintf(timeFile,"Wynik dla reverseByBlocks dla pliku %s: %f\n",argv[1],timedif2);

    fclose(file1);
    fclose(file2);
    fclose(timeFile);

    return 0;
}
