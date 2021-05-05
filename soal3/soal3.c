#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

char cwd[100]; // Inisialisasi array menampung current working dir
pthread_t id_thread[3]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread
char *arr[4], *arr2[20], arr3[100], abc[100], *fding[100];
int n = 0, m = 0;
int length = 5; //inisialisasi jumlah untuk looping
int end;
sem_t sem1, sem2, sem3;

void *eksekusi(void *arg)
{
    strcpy(abc, arg);
    char *potongtitik, *potongslash;

    unsigned long i = 0;
    pthread_t id = pthread_self();

    potongslash = strtok(abc, "/");
    while (potongslash != NULL)
    {
        arr2[m] = potongslash;
        m++;
        potongslash = strtok(NULL, "/");
    }
    strcpy(arr3, arr2[m - 1]);

    potongtitik = strtok(arr2[m - 1], ".");
    while (potongtitik != NULL)
    {
        arr[n] = potongtitik;
        n++;
        potongtitik = strtok(NULL, ".");
    }

    char ekstensiFile[100];
    strcpy(ekstensiFile, arr[n - 1]);
    for (int i = 0; ekstensiFile[i]; i++)
    {
        ekstensiFile[i] = tolower(ekstensiFile[i]);
    }

    DIR *folder, *folderopen;
    struct dirent *entry;
    char tempat2[100], tempat3[100];
    folder = opendir(cwd);
    int available = 0;

    if (n > 1)
    {
        // printf("n > 1");
        if (folder == NULL)
        {
            printf("error");
        }
        while ((entry = readdir(folder)))
        {
            if (strcmp(entry->d_name, ekstensiFile) == 0 && entry->d_type == 4)
            {
                available = 1;
                break;
            }
        }

        if (available == 0)
        {
            // printf("bisa\n");
            sprintf(tempat2, "%s/%s", cwd, ekstensiFile);
                mkdir(tempat2, 0777);
        }
    }
    else
    {
        sprintf(tempat2, "%s/Unknown", cwd);
        mkdir(tempat2, 0777);
    }

    char source[1024], target[1024];
    sprintf(source, "%s", arg);

    if (n == 1)
    {
        sprintf(target, "%s/Unknnown", cwd);
    }
    else
    {
        sprintf(target, "%s/%s", cwd, ekstensiFile);
    }
    strcat(target, "/");
    strcat(target, arr3);
    rename(source, target);
    n = 0;
    m = 0;

    return NULL;
}

int main(int argc, char *argv[])
{
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    int i = 0, j = 0;
    int err;
    int status[100];
    memset(status, 0, sizeof status);
    char *arg_path[100], *arg_name[100];
    if (strcmp(argv[1], "-f") == 0)
    {
        for (j = 2; j < argc; j++)
        {
            int err;
            end = argc - 2;
            pthread_create(&(id_thread[i]), NULL, eksekusi, argv[j]);
            pthread_join(id_thread[i], NULL);
            i++;
        }

        sleep(10);

        DIR *dir;
        struct dirent *enter;
        for (int count = 0; count < argc - 2; count++)
        {
            char *potongtitik, *potongslash;
            char temp_file[1024];
            int m = 0;
            int n = 0;
            potongslash = strtok(argv[count + 2], "/");
            while (potongslash != NULL)
            {
                arg_path[m] = potongslash;
                m++;
                potongslash = strtok(NULL, "/");
            }
            strcpy(temp_file, arg_path[m - 1]);

            potongtitik = strtok(arg_path[m - 1], ".");
            while (potongtitik != NULL)
            {
                arg_name[n] = potongtitik;
                n++;
                potongtitik = strtok(NULL, ".");
            }

            char ekstensiFile[100];
            strcpy(ekstensiFile, arr[n - 1]);
            for (int i = 0; ekstensiFile[i]; i++)
            {
                ekstensiFile[i] = tolower(ekstensiFile[i]);
            }
            char it[1024];
            sprintf(it, "%s/%s", cwd, ekstensiFile);
            dir = opendir(it);
            if (dir == NULL)
            {
                printf("error");
            }
            while ((enter = readdir(dir)))
            {
                char temp[1024];
                strcpy(temp, temp_file);
                if (strcmp(enter->d_name, temp) == 0)
                {
                    status[count] = 1;
                }
            }
            (void)closedir(dir);
        }

        for (int count = 0; count < argc - 2; count++)
        {
            if (status[count] == 1)
            {
                printf("file %d : Berhasil dikategorikan\n", count + 1);
            }
            else
            {
                printf("File %d : Sad, gagal\n", count + 1);
            }
        }
    }

    else if (strcmp(argv[1], "*") == 0 && argc == 2)
    {
        i = 0;
        DIR *fd, *fdo;
        struct dirent *masuk;
        char tempata[100], tempatb[100];
        fd = opendir(cwd);
        int available = 0;

        if (fd == NULL)
        {
            printf("error\n");
        }
        while ((masuk = readdir(fd)))
        {
            if (!strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, ".."))
                continue;
            printf("%s %d\n", masuk->d_name, masuk->d_type);

            int err;
            strcpy(tempata, cwd);
            strcat(tempata, "/");
            strcat(tempata, masuk->d_name);
            if (masuk->d_type == 8)
            {
                pthread_create(&(id_thread[i]), NULL, eksekusi, tempata); //membuat thread
                pthread_join(id_thread[i], NULL);
                i++;
            }
        }
    }
    else if (strcmp(argv[1], "-d") == 0 && argc == 3)
    {
        i = 0;
        DIR *fd, *fdo;
        struct dirent *masuk;
        char tempata[100], tempatb[100];
        fd = opendir(argv[2]);
        int available = 0;

        if (fd == NULL)
        {
            printf("error\n");
        }
        while ((masuk = readdir(fd)))
        {
            if (!strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, ".."))
                continue;
            printf("%s %d\n", masuk->d_name, masuk->d_type);

            int err;
            strcpy(tempata, argv[2]);
            strcat(tempata, "/");
            strcat(tempata, masuk->d_name);
            if (masuk->d_type == 8)
            {
                pthread_create(&(id_thread[i]), NULL, eksekusi, tempata); //membuat thread
                pthread_join(id_thread[i], NULL);
                i++;
            }
        }
    }

    return 0;
}
