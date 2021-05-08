#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#define PORT 1032
char **filename;
char **publisherbuku;
char **tahunpublishbuku;
char **fullpath;
char **ekstensi;
char **nama;
int jumlahData = 0;

void receive_file(char* filepath, int sockfd) {
  FILE* file_fd = fopen(filepath, "w");
  char buffer[4096];
  memset(buffer, 0, 4096);
  int n;
  while (1) {
    n = read(sockfd, buffer, 4096);
    if (strcmp(buffer, "code_eof") == 0) {
      break;
    }
    if (n > 0 && strcmp(buffer, "code_err") == 0) {
      printf("up\n");
      break;
    }

    fprintf(file_fd, "%s", buffer);
    memset(buffer, 0, 4096);
  }
  fclose(file_fd);
}

int checkLogin(char *kredensial)
{
    FILE *fptr;

    fptr = fopen("akun.txt", "r+");
    char line[256];

    while (fgets(line, sizeof(line) - 1, fptr))
    {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        if (strcmp(kredensial, line) == 0)
        {

            return 1;
        }
    }
    fclose(fptr);
    return 0;
}

void registerAkun(char *kredensial)
{

    FILE *fptr;

    fptr = fopen("akun.txt", "a+");

    fprintf(fptr, "%s", kredensial);
    fclose(fptr);
}

void createFolderFile()
{
    printf("happen\n");
    mkdir("FILES", 0777);
}

void write_file(int sockfd, char *fileto_add)
{
    int n;
    FILE *fp;
    char *filename = fileto_add;
    char buffer[1024];

    fp = fopen(filename, "w");
    // while (1)
    // {
    n = recv(sockfd, buffer, 1024, 0);
    // if (n <= 0)
    // {
    //     break;
    //     return;
    // }
    fprintf(fp, "%s", buffer);
    bzero(buffer, 1024);
    // }
    return;
}

void addtoDatabase(char *publisher, char *tahun_publikasi, char *filenamepath)
{

    char tmp[1024] = {0};
    strcpy(tmp, "FILES/");
    strcat(tmp, filenamepath);
    FILE *fp;
    fp = fopen("files.tsv", "a+");
    fprintf(fp, "%s\t%s\t%s\n", publisher, tahun_publikasi, tmp);
    fclose(fp);
}

void readDatabase()
{
    char line[50][256] = {0};
    FILE *fp;
    fp = fopen("files.tsv", "r");
    int i = 0;
    publisherbuku = malloc(55 * sizeof(char *));
    tahunpublishbuku = malloc(55 * sizeof(char *));
    fullpath = malloc(55 * sizeof(char *));
    ekstensi = malloc(55 * sizeof(char *));
    nama = malloc(55 * sizeof(char *));
    char *temp = calloc(100, sizeof(char));
    char *temp2 = calloc(100, sizeof(char));
    while (fgets(line[i], sizeof(line), fp))
    {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        char *token = strtok(line[i], "\t");
        // while (token != NULL)
        // {
        publisherbuku[i] = token;
        token = strtok(NULL, "\t");
        tahunpublishbuku[i] = token;
        token = strtok(NULL, "\t");
        fullpath[i] = token;

        strcpy(temp, fullpath[i]);
        char *token2 = strtok(temp, ".");
        strcpy(temp2, fullpath[i]);
        token2 = strtok(NULL, ".");
        ekstensi[i] = token2;

        char *token3 = strtok(temp, "/");
        token2 = strtok(NULL, "/");
        nama[i] = token2;
        //}
        //printf("%d -> %s -> %s -> %s -> %s -> %s",i, line[i],publisherbuku[i],tahunpublishbuku[i],fullpath[i], ekstensi[i]);
        i++;
        jumlahData++;
    }
    fclose(fp);
}

void see(int new_socket)
{
    char see[1024] = {0};
    char *ap = see;
    for (int i = 0; i < jumlahData; i++)
    {
        ap += sprintf(see, "Nama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File : %sFilepath : %s\n", nama[i], publisherbuku[i], tahunpublishbuku[i], ekstensi[i], fullpath[i]);
    }
    send(new_socket, see, strlen(see), 0);
}
void sendMessage(int new_socket, char *message)
{
    send(new_socket, message, strlen(message), 0);
}
void readMessage(int new_socket)
{
    char readmessage[1024] = {0};
    memset(readmessage, 0, 1024);
    // tahun publikasi
    read(new_socket, readmessage, 1024);
    printf("%s", readmessage);
    memset(readmessage, 0, 1024);
}

void findstring(int new_socket,char * findthisstring){

    printf("Masuk Pak");
}

int main(int argc, char const *argv[])
{
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
    createFolderFile();
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
            char *hello = "Server : Selamat Datang\n1. Login \n2. Register \n3. Quit ";
            send(new_socket, hello, strlen(hello), 0);
            memset(buffer, 0, 1024);
            read(new_socket, buffer, 1024);
            // login
 
            if (strncmp(buffer, "1", 1) == 0)
            {
 
                memset(buffer, 0, 1024);
                // mengirim pesan
                char *message = "Server :[LOGIN] Masukkan id dan password (id:password): ";
                send(new_socket, message, strlen(message), 0);
                // menerima kredensial
                valread = read(new_socket, kredensial, 1024);
                printf("[LOGIN] %s", kredensial);
                // printf("%s", buffer);
                // Check login
                otentikasi = checkLogin(kredensial);
                char isotentikasi[2];
                sprintf(isotentikasi, "%d", otentikasi);
                // kirim otentikasi
                send(new_socket, isotentikasi, strlen(isotentikasi), 0);
                memset(buffer, 0, 1024);
            }
            if (strncmp(buffer, "2", 1) == 0)
            {
                memset(buffer, 0, 1024);
                // mengirim pesan
                char *message = "Server :[REGISTER] Masukkan id dan password (id:password): ";
                send(new_socket, message, strlen(message), 0);
                // menerima kredensial
                valread = read(new_socket, buffer, 1024);
                printf("[Register] %s", buffer);
                registerAkun(buffer);
                memset(buffer, 0, 1024);
 
                message = "Server :[REGISTER] Register Berhasil Dilakukan\n";
                send(new_socket, message, strlen(message), 0);
            }
            else if (strcmp(buffer, "q\n") == 0)
            {
                break;
            }
        }
        if (otentikasi)
        {
            int inputcommand = 1;
            while (inputcommand)
            {
                char *lohe = " Server:[APP] command : add, download, delete, find, see, quit\n";
                send(new_socket, lohe, strlen(lohe), 0);
                char buffer2[1024] = {0};
                read(new_socket, buffer2, 1024);
                if (strcmp(buffer2, "add\n") == 0)
                {
                    memset(buffer2, 0, 1024);
                    // publisher
                    char *message = "Publisher: ";
                    send(new_socket, message, strlen(message), 0);
                    char publisher[1024] = {0};
                    read(new_socket, publisher, 1024);
                    //printf("%s", publisher);
                    publisher[strcspn(publisher, "\n")] = 0;
                    printf("%s\n", publisher);
                    // tahun publikasi
                    message = "Tahun Publikasi: ";
                    send(new_socket, message, strlen(message), 0);
                    char tahun_publikasi[1024] = {0};
                    read(new_socket, tahun_publikasi, 1024);
                    tahun_publikasi[strcspn(tahun_publikasi, "\n")] = 0;
                    printf("%s\n", tahun_publikasi);
                    // tahun publikasi
                    message = "Filepath: ";
                    send(new_socket, message, strlen(message), 0);
                    char temp[4096] = {0};
                    char temp3[4096] = {0};
                    char filenamepath[1024] = {0};
 
                    char *temp2[100];
                    char *slash;
                    int x;
                    while(1)
                    {
                        read(new_socket, temp, 4096);
                        if(strstr(temp, "\n")>0)
                        {
                            break;
                        }
                    }
                    // read(new_socket, temp, 4096);
                    // read(new_socket, temp3, 4096);
                    slash = strtok(temp, "/");
                    // while (slash != NULL)
                    // {
                    //     temp2[x] = slash;
                    //     x++;
                    //     slash = strtok(NULL, "/");
                    // }
                    printf("%s\n", temp);
                    printf("%s\n", temp3);
                    strcpy(filenamepath, temp);
                    memset(temp, 0, 4096);
                    FILE* file_fd = fopen(filenamepath, "w");
                      char bufferz[4096];
                      memset(bufferz, 0, 4096);
                      int n;
                      while (1) {
                        printf("masuk\n");
                        n = read(new_socket, bufferz, 4096);
                        printf("%s\n", bufferz);
                        if (strcmp(bufferz, "code_eof") == 0) {
                            printf("test\n");
                          break;
                        }
                        if (n > 0 && strcmp(bufferz, "code_err") == 0) {
                          printf("up\n");
                          break;
                        }
 
                        fprintf(file_fd, "%s", bufferz);
                        memset(bufferz, 0, 4096);
                      }
                      fclose(file_fd);
                    // int n;
                    // FILE *fp;
                    // char bismillah[1024] = {0};
 
                    // fp = fopen(filenamepath, "w+");
                    // fflush(fp);
                    // while (1)
                    // {
 
                    //     n = recv(new_socket, bismillah, 1024, 0);
                    //     if (n <= 0)
                    //     {
                    //         break;
                    //     }
                    //     fprintf(fp, "%s", bismillah);
                    //     printf("%s", bismillah);
                    // }
                    // bzero(buffer, 1024);
 
                    // fclose(fp);
 
                    //strtok(filenamepath,"\n");
                    //printf("%s -> %s -> %s", publisher, tahun_publikasi, filenamepath);
                    //write_file(new_socket, filenamepath);
                    // memset(publisher, 0, 1024);
                    // memset(tahun_publikasi, 0, 1024);
                    // memset(filenamepath, 0, 1024);
 
                    addtoDatabase(publisher, tahun_publikasi, filenamepath);
                }
                else if (strcmp(buffer2, "delete\n") == 0)
                {
                    /* code */
                }
                else if (strcmp(buffer2, "see\n") == 0)
                {
                    readDatabase();
                    see(new_socket);
                }
                else if (strcmp(buffer2, "find\n") == 0)
                {
                    sendMessage(new_socket, "[APP] String: ");
                    char findthisstring[1024] = {0};
                    // testing
                    read(new_socket, findthisstring, 1024);
                    printf("%s", findthisstring);
 
                    // readDatabase();
                    // findstring(new_socket,findthisstring);
                    memset(findthisstring, 0, 1024);
                }
                else if (strcmp(buffer2, "download\n") == 0)
                {
                    /* code */
                }
                else if (strcmp(buffer2, "quit\n") == 0)
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