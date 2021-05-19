#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define PORT 1040
char filename[256][1024], info[256][4096], fullpath[100][1024], ekstensi[100][1024], nama[100][1024], publisher[100][1024], year[100][1024];
char cwd[100];
int jumlahData = 0;

void runninglog(char *log)
{
    FILE *file_log;
    char file_content[1024];
    file_log = fopen("runing.log", "a+");
    fseek(file_log, 0, SEEK_END);
    long fsize = ftell(file_log);
    rewind(file_log);
    fread(file_content, 1, fsize, file_log);
    fprintf(file_log, "%s\n", log);
    fclose(file_log);
}

int checkLogin(char *kredensial)
{
    FILE *fptr;

    fptr = fopen("akun.txt", "r+");
    char line[256];
    char file_content[1024];
    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    rewind(fptr);
    fread(file_content, 1, fsize, fptr);
    char *p = strtok(file_content, "\n");
    while (p != NULL)
    {
        strcpy(line, p);
        // printf("%s\n", line);
        // printf("%d\n", strlen(line) );
        if (strcmp(kredensial, line) == 0)
        {
            printf("true\n");
            return 1;
        }
        p = strtok(NULL, "\n");
        // printf("akun %s\n", p);
    }
    fclose(fptr);
    printf("false\n");
    return 0;
}

void registerAkun(char *kredensial)
{

    FILE *filereg = fopen("akun.txt", "a+");
    fprintf(filereg, "%s", kredensial);
    fclose(filereg);
}

void readDatabase()
{

    FILE *filedb = fopen("files.tsv", "r");
    char file_content[1024];
    fseek(filedb, 0, SEEK_END);
    long fsize = ftell(filedb);
    rewind(filedb);
    fread(file_content, 1, fsize, filedb);
    int i;
    i = 0;
    char another_buffer[100][1024];
    char token2[100];
    char temp[100], temp2[100];
    char baris[100][1024];
    if (fsize == 0)
    {
        return;
    }
    printf("file con %s\n", file_content);
    printf("%ld\n", fsize);
    // printf("end of file\n");
    char *strmax;
    char *xyz = strtok_r(file_content, "\n", &strmax);
    // while (xyz != NULL)
    // {
    //     strcpy(baris[i], xyz);
    //     xyz = strtok(NULL, "\n");
    //     printf("new xyz %s\n", xyz);
    //     i++;
    //     jumlahData++;
    // }
    int x = 0;
    strcpy(baris[i], xyz);

    while (xyz != NULL)
    {

        // printf("baris %s\n", baris[x]);
        int n = 0;
        char *tok = strtok(baris[x], "< >");
        while (tok != NULL)
        {
            strcpy(info[n], tok);
            // printf("info %s\n", info[n]);
            n++;
            tok = strtok(NULL, "< >");
        }
        // printf("losss\n" );
        if (info[0] > 0)
        {
            strcpy(publisher[x], info[0]);
        }
        else
        {
            strcpy(publisher[x], "error");
        }

        if (info[1] > 0)
        {
            strcpy(year[x], info[1]);
        }
        else
        {
            strcpy(year[x], "error");
        }

        if (info[n - 1] > 0)
        {
            strcpy(temp, info[n - 1]);
        }
        else
        {
            strcpy(temp, "error");
        }

        // printf("publisher %s\n", publisher[x]);
        // printf("year %s\n", year[x]);

        // printf("file %s\n", temp);
        strcpy(fullpath[x], temp);

        int another_counter = 0;
        char *potongtitik;
        potongtitik = strtok(temp, ".");
        while (potongtitik != NULL)
        {
            strcpy(another_buffer[another_counter], potongtitik);
            // printf("potong titik %s\n", another_buffer[another_counter]);
            another_counter++;

            potongtitik = strtok(NULL, ".");
        }
        // strcpy(token2, strtok(temp, "."));
        // printf("tok2 %s\n", token2);
        // // strcpy(ekstensi[x], token2);
        // strcpy(token2, strtok(NULL, "."));
        if (another_buffer[1] > 0)
        {
            strcpy(ekstensi[x], another_buffer[1]);
        }
        else
        {
            strcpy(ekstensi[x], "no ekstensi");
        }

        // ekstensi[x] = token2;
        // printf("token 2%s\n", token2);
        char *token3 = strtok(temp, "/");
        strcpy(token2, strtok(NULL, "/"));
        // nama[x] = token2;
        strcpy(nama[x], token2);
        // printf("%s\n", nama[x]);
        printf("%d -> %s -> %s -> %s -> %s -> %s \n", x, xyz, publisher[x], year[x], fullpath[x], ekstensi[x]);
        jumlahData++;
        x++;
        i++;
        xyz = strtok_r(NULL, "\n", &strmax);
        // printf("new xyz %s\n", xyz);
        if (xyz != NULL)
            strcpy(baris[x], xyz);
    }
    fclose(filedb);
}

void tambahDB(char *temp_publish, char *tahun_publikasi, char *filenamepath, char *fileekstensi, int n)
{
    char tmp[1024];
    bzero(tmp, sizeof(tmp));
    FILE *fp = fopen("files.tsv", "a+");
    fprintf(fp, "%s< >%s< >%s\n", temp_publish, tahun_publikasi, filenamepath);
    fclose(fp);

    strcpy(publisher[jumlahData], temp_publish);
    strcpy(year[jumlahData], tahun_publikasi);
    strcpy(fullpath[jumlahData], filenamepath);

    char another_buffer[100][1024];
    char token2[1024];
    int another_counter = 0;
    char *potongtitik;
    potongtitik = strtok(filenamepath, ".");
    while (potongtitik != NULL)
    {
        strcpy(another_buffer[another_counter], potongtitik);
        // printf("potong titik %s\n", another_buffer[another_counter]);
        another_counter++;

        potongtitik = strtok(NULL, ".");
    }
    // strcpy(token2, strtok(temp, "."));
    // printf("tok2 %s\n", token2);
    // // strcpy(ekstensi[x], token2);
    // strcpy(token2, strtok(NULL, "."));
    if (another_buffer[1] > 0)
    {
        strcpy(ekstensi[jumlahData], another_buffer[1]);
    }
    else
    {
        strcpy(ekstensi[jumlahData], "no ekstensi");
    }

    // ekstensi[x] = token2;
    // printf("token 2%s\n", token2);
    char *token3 = strtok(filenamepath, "/");
    strcpy(token2, strtok(NULL, "/"));
    // nama[x] = token2;
    strcpy(nama[jumlahData], token2);

    jumlahData++;
    sprintf(tmp, "tambah: %s\n", fileekstensi);
    runninglog(tmp);
}

void see(int new_socket, int n)
{
    char result[4096];
    bzero(result, sizeof(result));
    n = 0;
    strcpy(result, "");
    while (n < jumlahData - 1)
    {
        printf("n %d\n", n);
        strcat(result, "Nama: ");
        strcat(result, nama[n]);
        strcat(result, "\nPublisher: ");
        strcat(result, publisher[n]);
        strcat(result, "\nTahun Publik: ");
        strcat(result, year[n]);
        strcat(result, "\nEkstensi file: ");
        strcat(result, ekstensi[n]);
        strcat(result, "\nFilepath: ");
        strcat(result, fullpath[n]);
        strcat(result, "\n");
        printf("result %s\n", result);
        n++;
    }
    send(new_socket, result, strlen(result), 0);
}

int main(int argc, char const *argv[])
{
    getcwd(cwd, sizeof(cwd));
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char buff[1024] = {0};
    char iduser[1024] = {0};
    char password[1024] = {0};
    char kredensial[1024] = {0};
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    mkdir("FILES", 0777);
    FILE *fp;
    fp = fopen("files.tsv", "a+");
    fclose(fp);
    printf("testing\n");
    readDatabase();
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // login form
        int otentikasi = 0;
        while (otentikasi == 0)
        {
            puts("otentikasi 0");
            char *hello = "Server :\n1. Login \n2. Register \n3. Quit ";
            send(new_socket, hello, strlen(hello), 0);
            bzero(buffer, sizeof(buffer));
            read(new_socket, buffer, 1024);
            // login

            if (strncmp(buffer, "1", 1) == 0)
            {
                puts("log");
                char password[1024];
                char login[1024];
                // bzero(buffer, sizeof(buffer));
                // mengirim pesan
                char *message = "User ID :";
                send(new_socket, message, strlen(message), 0);
                // menerima kredensial
                valread = read(new_socket, kredensial, 1024);
                if (valread < 1)
                {
                    printf("eror recv\n");
                }
                printf("cred%s\n", kredensial);
                kredensial[strcspn(kredensial, "\n")] = 0;
                message = "password :";
                send(new_socket, message, strlen(message), 0);
                valread = read(new_socket, password, 1024);
                if (valread < 1)
                {
                    printf("eror recv\n");
                }
                printf("pass %s\n", password);
                sprintf(login, "%s:%s", kredensial, password);
                printf("[LOGIN] %s", login);
                strtok(login, "\n");
                printf("len %d\n", strlen(login));
                // printf("%s", buffer);
                // Check login
                otentikasi = checkLogin(login);
                char isotentikasi[2];
                memset(isotentikasi, 0, 2);
                sprintf(isotentikasi, "%d", otentikasi);
                // kirim otentikasi
                printf("auth %d\n", otentikasi);
                printf("auth %d\n", isotentikasi);
                send(new_socket, isotentikasi, strlen(isotentikasi), 0);
                // bzero(buffer, sizeof(buffer));
                bzero(buffer, sizeof(buffer));
            }
            if (strncmp(buffer, "2", 1) == 0)
            {
                char id[1024];
                char password[1024];
                bzero(buffer, sizeof(buffer));
                char *message = "user id : ";
                send(new_socket, message, strlen(message), 0);
                valread = read(new_socket, id, 1024);
                if (valread < 1)
                {
                    printf("eror recv\n");
                }
                printf("%s\n", id);
                strtok(id, "\n");
                char *message2 = "password : ";
                send(new_socket, message2, strlen(message2), 0);
                valread = read(new_socket, password, 1024);
                if (valread < 1)
                {
                    printf("eror recv\n");
                }
                printf("%s\n", password);
                sprintf(buffer, "%s:%s", id, password);
                printf("[Register] %s", buffer);
                registerAkun(buffer);
                bzero(buffer, sizeof(buffer));

                message = "Server :Registrasi Berhasil Dilakukan\n";
                send(new_socket, message, strlen(message), 0);
            }
            else if (strcmp(buffer, "3\n") == 0)
            {
                break;
            }
        }
        if (otentikasi)
        {
            int inputcommand = 1;
            while (inputcommand)
            {
                puts("loged in");
                char *this = ""
                             "\nServer: command : \n"
                             "add\n"
                             "download\n"
                             "delete\n"
                             "find\n"
                             "see\n"
                             "quit\n";
                send(new_socket, this, strlen(this), 0);
                char buffer2[4096];
                memset(buffer2, 0, 4096);
                valread = read(new_socket, buffer2, 4096);
                if (valread < 1)
                {
                    printf("eror recv\n");
                }
                if (strncmp(buffer2, "add", 3) == 0)
                {
                    bzero(buffer2, sizeof(buffer2));
                    // publisher
                    char *message = "   Publisher: ";
                    send(new_socket, message, strlen(message), 0);
                    char publisher[1024] = {0};
                    valread = read(new_socket, publisher, 1024);
                    if (valread < 1)
                    {
                        printf("eror recv\n");
                    }
                    //printf("%s", publisher);
                    publisher[strcspn(publisher, "\n")] = 0;
                    printf("%s\n", publisher);
                    // tahun publikasi
                    char *message2 = "Tahun Publikasi: ";
                    send(new_socket, message2, strlen(message2), 0);
                    char tahun_publikasi[1024] = {0};
                    valread = read(new_socket, tahun_publikasi, 1024);
                    if (valread < 1)
                    {
                        printf("eror recv\n");
                    }
                    tahun_publikasi[strcspn(tahun_publikasi, "\n")] = 0;
                    printf("%s\n", tahun_publikasi);
                    // tahun publikasi
                    char *message3 = "Filepath: ";
                    send(new_socket, message3, strlen(message3), 0);
                    char temp[4096] = {0};
                    char temp3[4096] = {0};
                    char filenamepath[1024] = {0}, *temp2[100], *slash;
                    bzero(filenamepath, sizeof(filenamepath));

                    valread = read(new_socket, temp, 4096);
                    if (valread < 1)
                    {
                        printf("eror recv\n");
                    }
                    printf("te %s\n", temp);
                    int x = 0;
                    slash = strtok(temp, "/");
                    while (slash != NULL)
                    {
                        // printf("slashing\n");
                        temp2[x] = slash;
                        // printf("mp2 %s\n", temp2[x]);
                        x++;
                        slash = strtok(NULL, "/");
                    }

                    sprintf(filenamepath, "FILES/%s", temp2[x - 1]);
                    printf("file %s\n", filenamepath);
                    char bufferz[1024];
                    bzero(bufferz, sizeof(bufferz));
                    FILE *received_file;
                    ssize_t len;
                    int remain_data = 0;
                    int file_size;
                    char file_length[1024] = {0};
                    long fsize;
                    char file_content[1024] = {0};
                    FILE *file_fd = fopen(filenamepath, "w");

                    int n;
                    while (1)
                    {
                        printf("...");
                        n = read(new_socket, bufferz, 1024);
                        if (strcmp(bufferz, "EOF") == 0)
                        {
                            break;
                        }
                        if (n > 0 && strcmp(bufferz, "error") == 0)
                        {
                            fprintf(stderr, "System error (%d): %s\n", errno, strerror(errno));
                            break;
                        }

                        fprintf(file_fd, "%s", bufferz);
                        bzero(bufferz, sizeof(bufferz));
                    }
                    fclose(file_fd);

                    tambahDB(publisher, tahun_publikasi, filenamepath, temp2[x - 1], 0);
                }
                if (strncmp(buffer2, "delete", 6) == 0)
                {
                    // bzero(buffer2, sizeof(buffer2));
                    printf("%s\n", buffer2);
                    // strtok(buffer2, "delete ");
                    // // strtok(buffer2, "\n");
                    // printf("%s\n", buffer2);
                    int status, avail = 0, pcc;
                    char namafile[1024];
                    char old[1024];
                    char ano[1024];
                    char filedb_temp[1024];
                    strcpy(namafile, buffer2);
                    bzero(namafile, sizeof(namafile));
                    valread = read(new_socket, namafile, 1024);
                    if (valread < 1)
                    {
                        printf("recv err\n");
                    }
                    printf("jumlah data %d\n", jumlahData);
                    printf("nama file %s\n", namafile);
                    printf("%s\n", cwd);
                    sprintf(old, "%s/FILES/old-%s", cwd, namafile);
                    sprintf(ano, "%s/FILES/%s", cwd, namafile);
                    strtok(ano, "\n");
                    strtok(old, "\n");
                    sprintf(filedb_temp, "FILES/%s", namafile);
                    strtok(filedb_temp, "\n");
                    for (int i = 0; i < jumlahData; i++)
                    {
                        if (strcmp(fullpath[i], filedb_temp) == 0)
                        {

                            avail = 1;
                            pcc = i;
                            break;
                        }
                    }

                    if (avail == 0)
                    {
                        char *msg404 = "Maaf file tidak ditemukan";
                        send(new_socket, msg404, strlen(msg404), 0);
                    }
                    else
                    {
                        printf("%s\n", old);
                        printf("%s\n", ano);
                        status = rename(ano, old);

                        if (status == 0)
                        {
                            printf("File renamed successfully");

                            FILE *file_new_db;
                            for (int i = 0; i <= jumlahData - 1; i++)
                            {
                                // printf("%d\n", i );
                                if (i == pcc)
                                {
                                    // printf("pcc %d\n", i );
                                    continue;
                                }
                                else
                                {

                                    file_new_db = fopen("temp.tsv", "a+");
                                    // printf("input");
                                    // printf("pub: %s\n", publisher[i]);
                                    // printf("year: %s\n", year[i]);
                                    // printf("path: %s\n", fullpath[i]);
                                    fprintf(file_new_db, "%s< >%s< >%s\n", publisher[i], year[i], fullpath[i]);
                                }
                            }
                            printf("keluar\n");
                            fclose(file_new_db);
                            char tsv_temp[1024];
                            char tsv_new[1024];
                            int re_status;
                            printf("%s\n", cwd);
                            sprintf(tsv_temp, "%s/files.tsv", cwd);
                            sprintf(tsv_new, "%s/temp.tsv", cwd);
                            re_status = rename(tsv_temp, tsv_new);
                            if (re_status == 0)
                            {
                                char log[1024];
                                sprintf(log, "hapus: %s", namafile);
                                runninglog(log);
                                printf("Database change!\n");
                            }
                            else
                            {
                                printf("Database error\n");
                                fprintf(stderr, "System error (%d): %s\n", errno, strerror(errno));
                            }
                            char *del_success = "Delete file success!";
                            send(new_socket, del_success, strlen(del_success), 0);
                        }
                        else
                        {
                            printf("Error: unable to rename the file");
                            fprintf(stderr, "System error (%d): %s\n", errno, strerror(errno));
                        }
                        printf("done\n");
                    }
                }
                if (strncmp(buffer2, "see", 3) == 0)
                {
                    see(new_socket, 0);
                }
                if (strncmp(buffer2, "find", 4) == 0)
                {
                    int status = 0, index[100];
                    int n = 0;
                    char namafile[1024];
                    char old[1024];
                    char *ano;
                    char show_find[1024];
                    strcpy(namafile, buffer2);
                    bzero(namafile, sizeof(namafile));
                    valread = read(new_socket, namafile, 1024);
                    if (valread < 1)
                    {
                        printf("recv err\n");
                    }
                    printf("nama file %s\n", namafile);
                    strtok(namafile, "\n");
                    for (int i = 0; i < jumlahData; i++)
                    {
                        printf("%d\n", i);
                        printf("%s\n", fullpath[i]);
                        printf("nama file %s\n", namafile);
                        // printf("db %d",strlen(fullpath[i]));
                        // printf("old %d",strlen(old));
                        ano = strstr(fullpath[i], namafile);
                        if (ano)
                        {
                            status = 1;
                            index[n] = i;
                            n++;
                        }
                    }

                    if (status == 0)
                    {
                        char *msg = "file tidak ditemukan";
                        send(new_socket, msg, strlen(msg), 0);
                    }
                    else
                    {
                        char *msg = "file ditemukan";
                        send(new_socket, msg, strlen(msg), 0);
                    }

                    strcpy(show_find, "");
                    for (int i = 0; i < n; i++)
                    {
                        strcat(show_find, "Nama: ");
                        strcat(show_find, nama[index[i]]);
                        strcat(show_find, "\nPublisher: ");
                        strcat(show_find, publisher[index[i]]);
                        strcat(show_find, "\nTahun Publik: ");
                        strcat(show_find, year[index[i]]);
                        strcat(show_find, "\nEkstensi file: ");
                        strcat(show_find, ekstensi[index[i]]);
                        strcat(show_find, "\nFilepath: ");
                        strcat(show_find, fullpath[index[i]]);
                        strcat(show_find, "\n");
                    }
                    if (n > 0)
                        send(new_socket, show_find, strlen(show_find), 0);
                }
                if (strncmp(buffer2, "download", 8) == 0)
                {
                    printf("%s\n", buffer2);
                    int status, avail = 0, pcc;
                    char namafile[4096];
                    char old[1024];
                    char ano[1024];
                    // strcpy(namafile, buffer2);
                    bzero(namafile, sizeof(namafile));
                    valread = read(new_socket, namafile, 4096);
                    if (valread < 1)
                    {
                        printf("recv err\n");
                    }
                    printf("nama file %s\n", namafile);
                    // printf("%s\n", cwd);

                    sprintf(old, "FILES/%s", namafile);
                    printf("full %s\n", old);
                    strtok(old, "\n");
                    sprintf(ano, "%s/FILES/%s", cwd, namafile);
                    strtok(ano, "\n");
                    printf("%s\n", ano);
                    for (int i = 0; i < jumlahData; i++)
                    {
                        // printf("%s\n", fullpath[i]);
                        // printf("db %d",strlen(fullpath[i]));
                        // printf("old %d",strlen(old));
                        if (strcmp(fullpath[i], old) == 0)
                        {

                            avail = 1;
                            pcc = i;
                            break;
                        }
                    }

                    if (avail == 0)
                    {
                        printf("fail\n");
                        char *msg404lg = "File tidak dapat ditemukan";
                        send(new_socket, msg404lg, strlen(msg404lg), 0);
                    }
                    else
                    {
                        printf("ada\n");
                        char *msg404lg = "File sedang dikirim...";
                        send(new_socket, msg404lg, strlen(msg404lg), 0);
                        // bzero(msg404lg, sizeof(msg404lg));

                        FILE *fp = fopen(ano, "r+");
                        // printf("%x\n", fp);
                        char bufferz[1024];
                        memset(bufferz, 0, 1024);
                        if (fp > 0)
                        {
                            printf("Sending file: %s\n", ano);
                        }
                        else
                        {
                            printf("Failed to open file: %s\n", ano);
                            send(new_socket, "error", 20, 0);
                            // return;
                        }
                        bzero(bufferz, sizeof(bufferz));
                        while (fgets(bufferz, 1024, fp) != NULL)
                        {
                            printf("...");
                            if (send(new_socket, bufferz, sizeof(bufferz), 0) == -1)
                            {
                                fprintf(stderr, "System error (%d): %s\n", errno, strerror(errno));
                                exit(1);
                            }
                            bzero(bufferz, sizeof(bufferz));
                        }
                        fclose(fp);

                        send(new_socket, "EOF", 10, 0);
                        printf("done\n");
                    }
                }
                if (strncmp(buffer2, "quit", 4) == 0)
                {
                    printf("byebye\n");
                    otentikasi = 0;
                    inputcommand = 0;
                    // close(new_socket);
                    // return 0;
                }
            }
        }
    }

    printf("bye bye\n");
    close(new_socket);
    return 0;
}