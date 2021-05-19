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

char *arr[4];
char *arr2[20];
int length = 5;
int akhir, status = 0;
char gantiDir[100];
void *eksekusi(void *arg);
pthread_t id_thread[3];
char arr3[100];
char inputanPath[100];
char *fding[100];
int n = 0;
int m = 0;

int main(int argc, char *argv[])
{
    if (getcwd(gantiDir, sizeof(gantiDir)) != NULL)
    {
        printf("Current working dir: %s\n", gantiDir);
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
            akhir = argc - 2;
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

            char ekstensiFile[100];
            if (strncmp(temp_file, ".", 1) == 0)
            {
                strcpy(ekstensiFile, "hidden");
                n++;
                // printf("up1 %s\n", ekstensiFile);
            }
            else
            {
                potongtitik = strtok(arg_path[m - 1], ".");
                while (potongtitik != NULL)
                {
                    arg_name[n] = potongtitik;
                    // printf("%s\n", arg_name);
                    n++;
                    potongtitik = strtok(NULL, ".");
                }
                if (n > 2)
                {
                    // printf("lebih dari\n");
                    sprintf(ekstensiFile, "%s.%s", arg_name[n - 2], arg_name[n - 1]);
                }
                else
                {
                    // printf("else\n");
                    strcpy(ekstensiFile, arg_name[n - 1]);
                }

                for (int i = 0; ekstensiFile[i]; i++)
                {
                    ekstensiFile[i] = tolower(ekstensiFile[i]);
                }

                if (n == 1)
                {
                    strcpy(ekstensiFile[i], "Unknown");
                }
            }
            char it[1024];
            sprintf(it, "%s/%s", gantiDir, ekstensiFile);
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
        struct dirent *lebihmasuk;
        char tempata[100], tempatb[100];
        fd = opendir(gantiDir);
        int available = 0;

        if (fd == NULL)
        {
            printf("error\n");
        }
        while ((masuk = readdir(fd)))
        {
            if (!strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, ".."))
                continue;
            // printf("%s %d\n", masuk->d_name, masuk->d_type);
            if (masuk->d_type == 4)
            {
                DIR *dalemlagi;
                char tempdalem[100];
                sprintf(tempdalem, "%s/%s", gantiDir, masuk->d_name);
                dalemlagi = opendir(tempdalem);
                while (lebihmasuk = readdir(dalemlagi))
                {
                    if (!strcmp(lebihmasuk->d_name, ".") || !strcmp(lebihmasuk->d_name, ".."))
                        continue;
                    sprintf(tempata, "%s/%s/%s", gantiDir, masuk->d_name, lebihmasuk->d_name);
                }
            }
            else
            {
                sprintf(tempata, "%s/%s", gantiDir, masuk->d_name);
            }
            pthread_create(&(id_thread[i]), NULL, eksekusi, tempata); //membuat thread
            pthread_join(id_thread[i], NULL);
            i++;
        }

        if (status == 0)
        {
            printf("Yah, gagal disimpan :(\n");
        }
        else
        {
            printf("Direktori sukses disimpan!\n");
        }
    }
    else if (strcmp(argv[1], "-d") == 0 && argc == 3)
    {
        i = 0;
        DIR *fd, *fdo;
        struct dirent *masuk;
        struct dirent *lebihmasuk;
        char tempata[100], tempatb[100];
        fd = opendir(argv[2]);
        int available = 0;

        if (fd == NULL)
        {
            printf("Yah, gagal disimpan :(\n");
        }
        while ((masuk = readdir(fd)))
        {
            if (!strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, ".."))
                continue;
            if (masuk->d_type == 4)
            {
                DIR *dalemlagi;
                char tempdalem[100];
                sprintf(tempdalem, "%s/%s", argv[2], masuk->d_name);
                dalemlagi = opendir(tempdalem);
                while (lebihmasuk = readdir(dalemlagi))
                {
                    if (!strcmp(lebihmasuk->d_name, ".") || !strcmp(lebihmasuk->d_name, ".."))
                        continue;
                    sprintf(tempata, "%s/%s/%s", argv[2], masuk->d_name, lebihmasuk->d_name);
                }
            }
            else
            {
                sprintf(tempata, "%s/%s", argv[2], masuk->d_name);
            }

            pthread_create(&(id_thread[i]), NULL, eksekusi, tempata); //membuat thread
            pthread_join(id_thread[i], NULL);
            i++;
        }
        if (status == 0)
        {
            printf("Yah, gagal disimpan :(\n");
        }
        else
        {
            printf("Direktori sukses disimpan!\n");
        }
    }

    return 0;
}

void *eksekusi(void *arg)
{
    strcpy(inputanPath, arg);
    char *potongtitik, *potongslash;

    unsigned long i = 0;
    pthread_t id = pthread_self();

    potongslash = strtok(inputanPath, "/");
    while (potongslash != NULL)
    {
        arr2[m] = potongslash;
        m++;
        potongslash = strtok(NULL, "/");
    }
    strcpy(arr3, arr2[m - 1]);
    // printf("%s\n", arr3);
    // printf("%s\n", &arr3[0]);
    char ekstensiFile[100];

    if (strncmp(arr3, ".", 1) == 0)
    {
        strcpy(ekstensiFile, "hidden");
        n = 2;
        // printf("up %s\n", ekstensiFile);
    }
    else
    {
        potongtitik = strtok(arr2[m - 1], ".");
        while (potongtitik != NULL)
        {
            arr[n] = potongtitik;
            n++;
            // printf("%s\n", arr[n-1]);
            // printf("%d\n", n);
            potongtitik = strtok(NULL, ".");
        }
        if (n > 2)
        {
            // printf("lebih dari\n");
            sprintf(ekstensiFile, "%s.%s", arr[n - 2], arr[n - 1]);
            // printf("%s\n", ekstensiFile);
        }
        else
        {
            // printf("else\n");
            strcpy(ekstensiFile, arr[n - 1]);
            // printf("%s\n", ekstensiFile);
        }

        for (int i = 0; ekstensiFile[i]; i++)
        {
            ekstensiFile[i] = tolower(ekstensiFile[i]);
        }
        // printf("do %s\n", ekstensiFile);
    }

    DIR *folder, *folderopen;
    struct dirent *entry;
    char tempat2[100], tempat3[100];
    folder = opendir(gantiDir);
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
                // printf("%d\n", available);
                break;
            }
        }

        if (available == 0)
        {
            // printf("bisa\n");
            sprintf(tempat2, "%s/%s", gantiDir, ekstensiFile);
            mkdir(tempat2, 0777);
        }
    }
    else
    {
        sprintf(tempat2, "%s/Unknown", gantiDir);
        // printf("mkdir unknowm%s\n", ekstensiFile);
        mkdir(tempat2, 0777);
    }

    char source[1024], target[1024];
    sprintf(source, "%s", arg);

    if (n == 1)
    {
        sprintf(target, "%s/Unknown", gantiDir);
    }
    else
    {
        sprintf(target, "%s/%s", gantiDir, ekstensiFile);
    }
    strcat(target, "/");
    strcat(target, arr3);
    int thisre = rename(source, target);
    if (thisre == 0)
    {
        status = 1;
    }
    else
    {
        status = 0;
    }
    n = 0;
    m = 0;

    return NULL;
}