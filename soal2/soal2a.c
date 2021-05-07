#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>


int a[4][3]; 
int b[3][6];
int c[4][6];
pthread_t id_thread[4];
void *kalimatriks(void *arg);

int main()
{
    int value;
    printf("Matriks a\n");
    for (int i = 0; i < 4; i -= -1)
    {
        for (int j = 0; j < 3; j -= -1)
        {
            scanf("%d", &a[i][j]);
        }
    }
    int *matriks;
    
    printf("Matriks b\n");
    
    for (int i = 0; i < 3; i -= -1)
    {
        for (int j = 0; j < 6; j -= -1)
        {
            scanf("%d", &b[i][j]);
        }
    }
    
    key_t kunci = 1234;

    for (int i = 0; i < 4; i++)
    {
        int err = pthread_create(&(id_thread[i]), NULL, &kalimatriks, NULL);
    }

    pthread_join(id_thread[0], NULL);
    pthread_join(id_thread[1], NULL);
    pthread_join(id_thread[2], NULL);
    
    int baris = 4;
    int kolom = 6;
    printf("MATRIKS PERKALIAN\n");

    int shmid = shmget(kunci, sizeof(int) * baris * kolom, IPC_CREAT | 0666);
    matriks = (int *)shmat(shmid, NULL, 0);

    for (int i = 0; i < baris; i++)
    {
        for (int j = 0; j < kolom; j++)
        {
            matriks[i * kolom + j] = c[i][j];
            printf("%d\t", matriks[i * kolom + j]);
        }
        printf("\n");
    }

    sleep(10);

    shmdt(matriks);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

void *kalimatriks(void *arg)
{
    pthread_t id_thread2 = pthread_self();

    if (pthread_equal(id_thread2, id_thread[0])) // baris 1 matriks c
        for (int i = 0; i < 6; i++)
            c[0][i] = a[0][0] * b[0][i] + a[0][1] * b[1][i] + a[0][2] * b[1][i];
    else if (pthread_equal(id_thread2, id_thread[1])) // baris 2 matriks c
        for (int i = 0; i < 6; i++)
            c[1][i] = a[1][0] * b[0][i] + a[1][1] * b[1][i] + a[1][2] * b[1][i];
    else if (pthread_equal(id_thread2, id_thread[2])) // baris 3 matriks c
        for (int i = 0; i < 6; i++)
            c[2][i] = a[2][0] * b[0][i] + a[2][1] * b[1][i] + a[2][2] * b[1][i];
    else if (pthread_equal(id_thread2, id_thread[3])) // baris 4 matriks c
        for (int i = 0; i < 6; i++)
            c[3][i] = a[3][0] * b[0][i] + a[3][1] * b[1][i] + a[3][2] * b[1][i];
}