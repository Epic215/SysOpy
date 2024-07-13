#include <stdio.h>
#include <stdlib.h>
// wytwarzane zostaja w tym program 3 pliki 10 randomowych liter, 1025 randomowych liter, 2048 randomowych liter
int main() {

    FILE *file1= fopen("random10.txt","w");
    FILE *file2= fopen("random1025.txt","w");
    FILE *file3= fopen("random2048.txt","w");

    char word1[10];
    char word2[1025];
    char word3[2048];
    for(int i=0; i<10;i++){
        int rnd = rand() % 26;
        char new_char = 'A' + rnd;
        word1[i] = new_char;
    }
    for(int i=0; i<1025;i++){
        int rnd = rand() % 26;
        char new_char = 'A' + rnd;
        word2[i] = new_char;
    }
    for(int i=0; i<2048;i++){
        int rnd = rand() % 26;
        char new_char = 'A' + rnd;
        word2[i] = new_char;
    }
    fwrite(word1,1,10,file1);
    fwrite(word1,1,1025,file2);
    fwrite(word1,1,2048,file3);

    fclose(file1);
    fclose(file2);
    fclose(file3);

    return 0;
}