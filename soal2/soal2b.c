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

 unsigned long long matriks[4][6];
 unsigned long long hasil[4][6];

struct args {
    int a;
    int b;
    int baris;
    int kolom;
};

    
unsigned long long  hitungfaktorial(int n, int b) {
    int hasil=1;
    if(n > b){
        for (int i = n; i > n-b; i--)
        {
            hasil = hasil * i;
        }  
    return hasil;      
    }else{
        for (int i = n; i > 0;i--)
        {
            hasil = hasil * i;
        }  
    return hasil; 
    }
}


void* faktorial (void *arg);

int main () {

    int barisMatriks = 4;
    int kolomMatriks = 6;
    int *matrix;

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int)*barisMatriks*kolomMatriks, IPC_CREAT | 0666);
    matrix = (int *)shmat(shmid, NULL, 0);

    printf("MATRIKS PERKALIAN\n");

    for (int i = 0; i < barisMatriks; i++){
        for (int j = 0; j < kolomMatriks; j++){
            printf("%d\t", matrix[i*kolomMatriks + j]);
            matriks[i][j] = matrix[i*kolomMatriks + j];
        }
        printf("\n");
    }


    printf("\nMATRIKS B\n");

    pthread_t tid[24];
    int value;
    int mat_b[4][6];
    for (int i = 0; i < barisMatriks; i++){
        for (int j = 0; j < kolomMatriks; j++){
            // printf("baris %d colom %d\t", i,j);
            scanf("%d", &mat_b[i][j]);
            // mat_b[i][j] = value;
        }
        printf("\n");
    }


    for (int i = 0; i < barisMatriks; i++) {
        for (int j = 0; j < kolomMatriks; j++){

            struct args* data = (struct args*)malloc(sizeof(struct args));
            data->a = matriks[i][j];
            data->b = mat_b[i][j];
            data->baris = i;
            data->kolom = j;

            pthread_create(&(tid[i * kolomMatriks + j]), NULL, faktorial, (void*)data);
            pthread_join(tid[i * kolomMatriks + j], NULL);
        }
        printf("\n");
    }

    for (int i = 0; i < barisMatriks; i-=-1)
    {
        for (int j = 0; j < kolomMatriks; j-=-1)
        {
            printf("%d\t", hasil[i][j] );
        }
        printf("\n");
    }

    shmdt(matrix);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}


void* faktorial (void *arg){
    struct args* data;
    data = (struct args *) arg;
    unsigned long long fact = 1;
    unsigned long long temp, temp2;
    int n = data->a;
    int m =  data->b;
    int baris = data->baris;
    int kolom = data->kolom;
    if(n < m){
        hasil[baris][kolom] = hitungfaktorial(n,m);
    }else if(n > m){
        hasil[baris][kolom] = hitungfaktorial(n,m);
    }else{
        hasil[baris][kolom] = 0;
    }
}
