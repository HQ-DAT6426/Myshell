#include <windows.h>
#include <stdio.h>
#include <string.h>

// linked list start
typedef struct PROCESSNAME{
    char name[20];
    PROCESS_INFORMATION pi;
    struct PROCESSNAME *next;
}PROCESSNAME ;

typedef struct HEADLIST{
    PROCESSNAME *head;
    PROCESSNAME *tail;
} HEADLIST;

PROCESSNAME *makeprocessinfo( char *name,PROCESS_INFORMATION pi){
    PROCESSNAME *newprocess = (PROCESSNAME*)malloc(sizeof(PROCESSNAME));
    if(newprocess == NULL) {
        printf("loi cap phat");
        exit(0);
    }
    strcpy(newprocess->name,name);
    newprocess->pi = pi;
    newprocess->next = NULL;
    return newprocess;
}

HEADLIST *makeheadlist(){
    HEADLIST *newheadlist = (HEADLIST *)malloc(sizeof(HEADLIST));
    if(newheadlist == NULL){
        printf("loi cap phat");
        exit(0);
    }
    newheadlist->head = NULL;
    newheadlist->tail = NULL;
    return newheadlist;
}
//end linked list
void deleteprocess_info(PROCESSNAME *process){
    CloseHandle(process->pi.hProcess);
    CloseHandle(process->pi.hThread);
    free(process);
}
void deletelistprocess(PROCESSNAME *head){
    PROCESSNAME *h;
    while(head!=NULL){
        h = head;
        head = head->next;
        deleteprocess_info(h);
    }
}

void createprocessforeground(char *cmd){
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    BOOL check = CreateProcess(NULL,cmd,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);

    if(check) printf("tao tien trinh foreground thanh cong\n");
    else printf("khong tao duoc\n");
    WaitForSingleObject(pi.hProcess,INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
PROCESS_INFORMATION createprocessbackground(char *cmd){
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    BOOL check = CreateProcess(NULL,cmd,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);

    if(check) printf("tao tien trinh background thanh cong\n");
    else printf("khong tao duoc\n");

    return pi;
}

void checkactive(PROCESSNAME *pn){
    DWORD exitcode;
    if( GetExitCodeProcess(pn->pi.hProcess,&exitcode)){
        if(exitcode == STILL_ACTIVE) printf("hoat dong, exitcode :%ld\n",exitcode);
        else printf("da ngung\n");
    }else{
        printf("khong thanh cong\n");
    }
}

void checkactive(PROCESSNAME *pn);
int main(){
    char cmd[100];
    HEADLIST *headlist;
    headlist = makeheadlist();
    PROCESS_INFORMATION pi;
    int countprocess = 0;
    while(1){
        printf("Myshell>");
        scanf("%s",cmd);
        if(strcmp(cmd,"fore")==0){
            scanf("%s",cmd);
            createprocessforeground(cmd);
        }
        else if(strcmp(cmd,"back")==0){
            scanf("%s",cmd);
            pi = createprocessbackground(cmd);
            PROCESSNAME *h = makeprocessinfo(cmd,pi);
            if(headlist->head == NULL){
                headlist->head = h;
                headlist->tail = h;
            }else{
                headlist->tail->next = h;
                headlist->tail = headlist->tail->next;
            }
            countprocess++;
        }
        /*if(strcmp(cmd,"check_active") == 0 ){
            int j;
            scanf("%d",&j);
            if(j>=n) printf("khong ton tai\n");
            else{
                checkactive(j,list_process);
            }
        }*/
        else if(strcmp(cmd,"list")==0){
            printf("----------------danh sach tien trinh----------------\n");
            printf("  STT    processID     name         status\n");
            PROCESSNAME *h = headlist->head;
            int i = 0;
            while(h!=NULL){
                printf("   %d %10ld %12s %5s",i,h->pi.dwProcessId,h->name,"" );
                checkactive(h);
                i++;
                h = h->next;
            }
        }
        else if(strcmp(cmd,"kill") == 0){
            DWORD id;
            scanf("%ld",&id);
            PROCESSNAME *h,*prevh;
            h = headlist->head;
            if(h->pi.dwProcessId == id){
                headlist->head = h->next;
                TerminateProcess(h->pi.hProcess,0);
                deleteprocess_info(h);
                countprocess--;
            }else{
                while(h != NULL){
                    if(h->pi.dwProcessId == id){
                        prevh->next = h->next;
                        TerminateProcess(h->pi.hProcess,0);
                        deleteprocess_info(h);
                        countprocess--;
                        break;
                    }
                    prevh = h;
                    h = h->next;
                }
            }
        }

        else if(strcmp(cmd,"stop")==0){
            DWORD id;
            scanf("%ld",&id);
            PROCESSNAME *h;
            h = headlist->head;
            while(h != NULL){
                if(h->pi.dwProcessId == id){
                    SuspendThread(h->pi.hThread);
                    break;
                }
                h = h->next;
            }
        }
        else if(strcmp(cmd,"resume")==0){
            DWORD id;
            scanf("%ld",&id);
            PROCESSNAME *h;
            h = headlist->head;
            while(h != NULL){
                if(h->pi.dwProcessId == id){
                    ResumeThread(h->pi.hThread);
                    break;
                }
                h = h->next;
            }
        }

        else if(strcmp(cmd,"help") == 0){
            printf("             WELCOME TO MYSHELL\n");
            FILE *file = fopen("help.txt","r");
            if (file == NULL) {
                printf("khong doc dc file\n");
                break;
            }

            char buffer[100];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                printf("%s", buffer);
            }

            fclose(file);
            printf("\n");
        }
        else if(strcmp(cmd,"exit")==0){
            break;
        }

        else{
            printf("loi nhap lenh, nhap lai\n");
        }
        fgets(cmd,sizeof(cmd),stdin);
    }
    deletelistprocess(headlist->head);
    free(headlist);
    return 0;
}


