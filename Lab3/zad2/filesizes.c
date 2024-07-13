#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

int main() {
    DIR* dir =opendir(".");
    if (dir==NULL){
        return -1;
    }
    struct dirent* entity;
    struct stat entityInfo;
    long long int totalSize=0;
    entity= readdir(dir);
    while(entity!=NULL){
        if (!stat(entity->d_name,&entityInfo))
        {
            if (!S_ISDIR(entityInfo.st_mode))
            {
                printf("File Name:    %s\n",entity->d_name);
                printf("File Size:     %ld bytes\n",entityInfo.st_size);
                totalSize+=entityInfo.st_size;
            }
        }
        entity= readdir(dir);
    }
    closedir(dir);
    printf("Total Size of Files:     %lld bytes\n",totalSize);
    return 0;
}
