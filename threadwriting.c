#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define SIZE_NAME 5000
#define SIZE_INFO 1000000

pthread_mutex_t mtx;
pthread_cond_t cond;
int flag = 0;


typedef struct Results {
    char file_name[SIZE_NAME];
    char file_contents[SIZE_INFO];
} Results;

void *writefile1(void *result2) {

    pthread_mutex_lock(&mtx);
    
    Results *res = (Results *)malloc(sizeof(Results));
    strncpy(res->file_name, (char *)result2, SIZE_NAME - 5);
    strcat(res->file_name, ".txt");
    FILE *fp = fopen(res->file_name, "w");
    
    fclose(fp);

    flag = 1;

    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&mtx);

    pthread_exit(res);
}

void *writefile2(void *result1) {

    pthread_mutex_lock(&mtx);

    while (flag == 0) {
        pthread_cond_wait(&cond, &mtx);
    }

    Results *res = (Results *)result1;

    
    char writefinal[SIZE_NAME + 4]; 
    strncpy(writefinal, res->file_name, SIZE_NAME);
    strncat(writefinal, ".txt", SIZE_NAME);

    FILE *fp = fopen(writefinal, "w");
    fprintf(fp, "%s\n", res->file_contents);

    fclose(fp);

    pthread_mutex_unlock(&mtx);

    pthread_exit(res);
}


void init_Threads() {

    FILE *fp;

    Results *fileinfo = (Results *)malloc(sizeof(Results));

    pthread_t t_id0, t_id1;



    printf("Enter the File name you wish to enter: ");
    fgets(fileinfo->file_name, SIZE_NAME, stdin);
    strtok(fileinfo->file_name, "\n");

    printf("This is the file name: %s\n", fileinfo->file_name);

    printf("%s\n", fileinfo->file_contents);
    printf("Now, enter the information you wish to enter: ");
    fgets(fileinfo->file_contents, SIZE_INFO, stdin);

    printf("This is the file's content: %s\n", fileinfo->file_contents);

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cond, NULL);

    Results *file1name;
    Results *file1contents;

    pthread_create(&t_id0, NULL, &writefile1, fileinfo);
    pthread_create(&t_id1, NULL, &writefile2, fileinfo);

    pthread_join(t_id0, (void **)&file1name);

    printf("This is the file's name: %s\n", file1name->file_name);

    pthread_join(t_id1, (void **)&file1contents);

    printf("This is the file content: %s\n", file1contents->file_contents);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&mtx);

    free(fileinfo);
    free(file1name);
    free(file1contents);
    
}

int main() {

    init_Threads();

    return 0;
}