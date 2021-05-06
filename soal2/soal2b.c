#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/ipc.h>

#define MAX 500

 unsigned long long matriks[4][6], res[4][6];

struct args {
    int a;
    int b;
    int rows;
    int col;
};

    
unsigned long long  hitungfaktorial(int n, int b) {
    int res=1;
    if(n > b){
        for (int i = n; i > n-b; i--)
        {
            res = res * i;
        }  
    return res;      
    }else{
        for (int i = n; i > 0;i--)
        {
            res = res * i;
        }  
    return res; 
    }
}


void* factorial (void *arg){
    struct args* data;
    data = (struct args *) arg;
    unsigned long long fact = 1;
    unsigned long long temp, temp2;
    int n = data->a;
    int m =  data->b;
    int rows = data->rows;
    int col = data->col;
    if(n < m){
        res[rows][col] = hitungfaktorial(n,m);
    }else if(n > m){
        res[rows][col] = hitungfaktorial(n,m);
    }else{
        res[rows][col] = 0;
    }
}

int main () {

    int row = 4;
    int column = 6;
    int *matrix;

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int)*row*column, IPC_CREAT | 0666);
    matrix = (int *)shmat(shmid, NULL, 0);

    printf("MATRIKS PERKALIAN\n");

    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            printf("%d\t", matrix[i*column + j]);
            matriks[i][j] = matrix[i*column + j];
        }
        printf("\n");
    }


    printf("\nMATRIKS B\n");

    pthread_t tid[24];
    int value;
    int mat_b[4][6];
    for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
            // printf("baris %d colom %d\t", i,j);
            scanf("%d", &mat_b[i][j]);
            // mat_b[i][j] = value;
        }
        printf("\n");
    }


    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++){

            struct args* data = (struct args*)malloc(sizeof(struct args));
            data->a = matriks[i][j];
            data->b = mat_b[i][j];
            data->rows = i;
            data->col = j;

            pthread_create(&(tid[i * column + j]), NULL, factorial, (void*)data);
            pthread_join(tid[i * column + j], NULL);
        }
        printf("\n");
    }

    for (int i = 0; i < row; i-=-1)
    {
        for (int j = 0; j < column; j-=-1)
        {
            printf("%d\t", res[i][j] );
        }
        printf("\n");
    }

    shmdt(matrix);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
