#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
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

#define PORT 1036
char *filename[1024], *info[4096], *fullpath[1024], *ekstensi[1024], *nama[1024], *publisher[1024], *year[1024];
char cwd[100];
int jumlahData = 0;

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
        printf("%s\n", line);
        printf("%d\n", strlen(line));
        if (strcmp(kredensial, line) == 0)
        {
            printf("true\n");
            return 1;
        }
        p = strtok(NULL, "\n");
    }
    fclose(fptr);
    printf("false\n");
    return 0;
}

void registerAkun(char *kredensial)
{

    FILE *fptr = fopen("akun.txt", "a+");
    fprintf(fptr, "%s", kredensial);
    fclose(fptr);
}

void readDatabase()
{
    bzero(publisher, sizeof(publisher));
    bzero(year, sizeof(year));

    FILE *fp = fopen("files.tsv", "r");
    char file_content[1024];
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);
    fread(file_content, 1, fsize, fp);
    int i;
    i = 0;
    char *tok, *token2, *temp, *temp2;
    char *baris[1024];

    char *p = strtok(file_content, "\n");
    while (p != NULL)
    {
        printf("%s\n", p);
        baris[i] = strtok(file_content, "\n");
        int n = 0;
        tok = strtok(baris[i], "< >");
        while (tok != NULL)
        {
            info[n] = tok;
            printf("info %s\n", info[n]);
            n++;
            tok = strtok(NULL, "< >");
        }
        printf("losss\n");
        strcpy(publisher[i], info[0]);
        strcpy(year[i], info[1]);
        printf("%s\n", publisher[i]);
        printf("%s\n", year[i]);
        strcpy(temp, info[n - 1]);
        printf("%s\n", temp);
        token2 = strtok(temp, ".");
        ekstensi[i] = token2;
        printf("%s\n", token2);
        char *token3 = strtok(temp, "/");
        token2 = strtok(NULL, "/");
        nama[i] = token2;
        printf("%d -> %s -> %s -> %s -> %s -> %s", i, baris[i], publisher[i], year[i], fullpath[i], ekstensi[i]);
        i++;
        jumlahData++;
        p = strtok(NULL, "\n");
    }
    fclose(fp);
}

void tambahDB(char *publisher, char *tahun_publikasi, char *filenamepath, int n)
{
    char tmp[1024];
    bzero(tmp, sizeof(tmp));
    FILE *fp = fopen("files.tsv", "a+");
    fprintf(fp, "%s< >%s< >%s\n", publisher, tahun_publikasi, filenamepath);
    fclose(fp);
}

void see(int new_socket, int n)
{
    char result[4096];
    bzero(result, sizeof(result));
    n = 0;
    while (n < jumlahData)
    {
        int x = 1;
        strcpy(result, "Nama: ");
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
                // mengirim pesan
                char *message = "user id : ";
                send(new_socket, message, strlen(message), 0);
                // menerima kredensial

                valread = read(new_socket, id, 1024);
                if (valread < 1)
                {
                    printf("eror recv\n");
                }
                printf("%s\n", id);
                id[strcspn(id, "\n")] = 0;
                // bzero(buffer, sizeof(buffer));

                message = "password : ";
                send(new_socket, message, strlen(message), 0);
                // menerima kredensial
                valread = read(new_socket, password, 1024);
                if (valread < 1)
                {
                    printf("eror recv\n");
                }
                printf("%s\n", password);
                // password[strcspn(password, "\n")] = 0;
                sprintf(buffer, "%s:%s", id, password);
                printf("[Register] %s", buffer);
                registerAkun(buffer);
                bzero(buffer, sizeof(buffer));

                message = "Server :[REGISTER] Register Berhasil Dilakukan\n";
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
                             "Server: command : "
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

                    sprintf(filenamepath, "FILES/%s\n", temp2[x - 1]);
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
                        n = read(new_socket, bufferz, 1024);
                        if (strcmp(bufferz, "EOF") == 0)
                        {
                            break;
                        }
                        if (n > 0 && strcmp(bufferz, "code_err") == 0)
                        {
                            printf("up\n");
                            break;
                        }

                        fprintf(file_fd, "%s", bufferz);
                        bzero(bufferz, sizeof(bufferz));
                    }
                    fclose(file_fd);

                    tambahDB(publisher, tahun_publikasi, filenamepath, 0);
                }
                else if (strncmp(buffer2, "delete", 6) == 0)
                {
                    // bzero(buffer2, sizeof(buffer2));
                    printf("%s\n", buffer2);
                    // strtok(buffer2, "delete ");
                    // // strtok(buffer2, "\n");
                    // printf("%s\n", buffer2);
                    int status;
                    char namafile[1024];
                    char old[1024];
                    char ano[1024];
                    strcpy(namafile, buffer2);
                    bzero(namafile, sizeof(namafile));
                    valread = read(new_socket, namafile, 1024);
                    if (valread < 1)
                    {
                        printf("recv err\n");
                    }
                    printf("nama file %s\n", namafile);
                    printf("%s\n", cwd);
                    sprintf(old, "%s/FILES/old-%s", cwd, namafile);
                    sprintf(ano, "%s/FILES/%s", cwd, namafile);
                    printf("%s\n", old);
                    printf("%s\n", ano);
                    status = rename(ano, old);
                    while (status != 0)
                    {
                        status = rename(ano, old);
                        if (status == 0)
                        {
                            printf("File renamed successfully");
                        }
                        else
                        {
                            printf("Error: unable to rename the file");
                        }
                    }

                    printf("done\n");
                }
                if (strncmp(buffer2, "see", 3) == 0)
                {
                    readDatabase();
                    see(new_socket, 0);
                }
                if (strncmp(buffer2, "find", 4) == 0)
                {
                    printf("under maintain\n");
                }
                if (strncmp(buffer2, "download", 8) == 0)
                {
                    printf("under maintan\n");
                }
                if (strncmp(buffer2, "quit", 4) == 0)
                {
                    break;
                }
            }
        }
    }

    printf("Login Berhasil\n");
    close(new_socket);
    return 0;
}