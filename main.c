#include <windows.h>
#include <stdio.h>
#include <string.h>

char name[100];

void findfile(char path[]){
    char seach[260];
    WIN32_FIND_DATA fd;
    sprintf(seach,"%s\\*",path);
    HANDLE newFFF = FindFirstFile(seach,&fd);

    if (newFFF == INVALID_HANDLE_VALUE) return;

    do{
        if(strcmp(fd.cFileName,".") == 0 || strcmp(fd.cFileName,"..") == 0) continue;

            char newpath[260];
            sprintf(newpath,"%s\\%s",path,fd.cFileName);
        if(strcmp(fd.cFileName,name)==0)
            printf("%s\n",newpath);
        if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            findfile(newpath);
        }
    }while(FindNextFile(newFFF,&fd));

    FindClose(newFFF);
}

void directory(char *path){
    WIN32_FIND_DATA fd;
    HANDLE FFF = FindFirstFile(path,&fd);
    do{
        printf("%s\n",fd.cFileName);
    }while(FindNextFile(FFF,&fd) );
    FindClose(FFF);
}
void findpath(){
    char pathcurrent[100];
    DWORD len = GetCurrentDirectory(sizeof(pathcurrent),pathcurrent);
    if(len > 0){
        printf("%s\n",pathcurrent);
    }
}

void addpath(char* srcPath,char* dstPath){
    char buffer[4096];
    DWORD readBytes, writtenBytes;

    HANDLE src = CreateFile(srcPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE dst = CreateFile(dstPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    while (ReadFile(src, buffer, sizeof(buffer), &readBytes, NULL) && readBytes > 0) {
        WriteFile(dst, buffer, readBytes, &writtenBytes, NULL);
    }

    CloseHandle(src);
    CloseHandle(dst);

    }

int main(){

    char path[100]="D:\\laptrinhc";

    return 0;
}
