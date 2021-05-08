#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 1033

void send_file(char *filepath, int sockfd)
{
  FILE *file_fd = fopen(filepath, "r");
  char buffer[4096] = {0};
  if (file_fd != NULL)
  {
    printf("Sending file: %s\n", filepath);
  }
  else
  {
    printf("Failed to open file: %s\n", filepath);
    //* Sending dummy file to server to cancel receiving file
    send(sockfd, "code_err", 20, 0);
    return;
  }

  while (fgets(buffer, 4096, file_fd) != NULL)
  {
    if (send(sockfd, buffer, sizeof(buffer), 0) == -1)
    {
      perror("[-]Error in sending file.");
      exit(1);
    }
    memset(buffer, 0, 4096);
  }
  send(sockfd, "code_eof", 10, 0);
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

int main(int argc, char const *argv[])
{
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};
  char buff[1024] = {0};

  int otentikasi = 0;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return -1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConnection Failed \n");
    return -1;
  }
  printf("oke bos\n");
  // login form
  while (otentikasi == 0)
  {
    // selamat datang
    read(sock, buffer, 1024);
    printf("%s\n", buffer);
    memset(buffer, 0, 1024);
    // input mode
    fgets(buff, 1024, stdin);
    send(sock, buff, strlen(buff), 0);

    if (strcmp(buff, "1\n") == 0)
    {
      char buff2[1024] = {0};
      // bersihkan buffer
      memset(buffer, 0, 1024);
      memset(buff, 0, 1024);
      // pesan dari server id
      read(sock, buffer, 1024);
      printf("%s\n", buffer);
      memset(buffer, 0, 1024);
      // Masukkan kredensial
      fgets(buff, 1024, stdin);
      // kirim kredensial
      send(sock, buff, strlen(buff), 0);
      memset(buff, 0, 1024);
      puts("lalal");
      // password
      read(sock, buffer, 1024);
      printf("%s\n", buffer);
      memset(buffer, 0, 1024);
      // Masukkan kredensial
      fgets(buff2, 1024, stdin);
      // kirim kredensial
      send(sock, buff2, strlen(buff2), 0);
      memset(buff2, 0, 1024);
      // menerima otentikasi
      char isotentikasi[2] = {0};
      read(sock, isotentikasi, 2);
      // sprintf(otentikasi, "%d", isotentikasi);
      otentikasi = atoi(isotentikasi);
      memset(isotentikasi, 0, 2);
    }
    else if (strcmp(buff, "2\n") == 0)
    {
      // bersihkan buffer
      memset(buffer, 0, 1024);
      memset(buff, 0, 1024);
      // pesan dari server
      read(sock, buffer, 1024);
      printf("%s\n", buffer);
      memset(buffer, 0, 1024);
      // Masukkan kredensial
      fgets(buff, 1024, stdin);
      // kirim kredensial
      send(sock, buff, strlen(buff), 0);
      memset(buff, 0, 1024);
      read(sock, buffer, 1024);
      printf("%s\n", buffer);
      memset(buffer, 0, 1024);
    }
    else if (strcmp(buff, "3\n") == 0)
    {
      printf("Good Bye\n");
      return 0;
    }
    if (otentikasi)
    {
      int inputcommand = 1;
      while (inputcommand)
      {
        char pesan[300] = {0};
        char buffer2[4096] = {0};
        memset(buffer2, 0, 4096);
        read(sock, pesan, 300);
        printf("%s", pesan);
        memset(buffer2, 0, 4096);
        fgets(buffer2, 4096, stdin);
        send(sock, buffer2, strlen(buffer2), 0);
        if (strcmp(buffer2, "add\n") == 0)
        {
          FILE *fp;
          memset(buffer2, 0, 4096);
          char *tes;
          // publisher
          read(sock, buffer2, 4096);
          printf("%s", buffer2);
          memset(buffer2, 0, 4096);
          fgets(buffer2, 4096, stdin);
          printf("%s", buffer2);
          send(sock, buffer2, strlen(buffer2), 0);
          printf("%s", buffer2);
          memset(buffer2, 0, 4096);
          // tahun publikasi
          read(sock, buffer2, 4096);
          printf("%s", buffer2);
          memset(buffer2, 0, 4096);
          fgets(buffer2, 4096, stdin);
          send(sock, buffer2, strlen(buffer2), 0);
          memset(buffer2, 0, 4096);
          // filepath
          read(sock, buffer2, 4096);
          printf("%s", buffer2);
          memset(buffer2, 0, 4096);
          char filenamepath[4096] = {0};
          memset(filenamepath, 0, 4096);
          gets(tes);
          fgets(buffer2, 4096, stdin);
          printf("buf%s\n", buffer2);
          strcpy(filenamepath, buffer2);
          memset(buffer2, 0, 4096);
          strtok(filenamepath, "\n");
          send(sock, filenamepath, strlen(filenamepath), 0);
          printf("tse%s\n", filenamepath);
          fp = fopen(filenamepath, "r+");
          char bufferz[4096] = {0};
          memset(bufferz, 0, 4096);
          if (!fp)
          {
            printf("Sending file: %s\n", filenamepath);
          }
          else
          {
            printf("Failed to open file: %s\n", filenamepath);
            //* Sending dummy file to server to cancel receiving file
            send(sock, "code_err", 20, 0);
            // return;
          }
          memset(bufferz, 0, 4096);
          while (fgets(bufferz, 4096, fp) != NULL)
          {
            printf("tes");
            if (send(sock, bufferz, sizeof(bufferz), 0) == -1)
            {
              perror("[-]Error in sending file.");
              exit(1);
            }
            memset(bufferz, 0, 4096);
          }
          fclose(fp);
          send(sock, "code_eof", 10, 0);
          // send_file(filenamepath, sock);
          //buffer2[strcspn(buffer2, "\n")] = 0;
          // printf("%stes",filenamepath);

          //printf("the file was sent successfully");
          // fp = fopen(filenamepath, "r");
          // if (fp == NULL)
          // {
          //     perror("[-]Error in reading file.");
          //     return 0;
          // }
          // int n;
          // char data[1024] = {0};

          // while (fgets(data, 1024, fp) != NULL)
          // {
          //     if (send(sock, data, sizeof(data), 0) == -1)
          //     {
          //         perror("[-]Error in sending file.");
          //         exit(1);
          //     }
          // }
          // bzero(data, 1024);

          //send_file(fp, sock);
          //printf("[+]File data sent successfully.\n");
        }
        else if (strcmp(buffer2, "delete\n") == 0)
        {
          /* code */
        }
        else if (strcmp(buffer2, "see\n") == 0)
        {
          char see[1024] = {0};
          memset(see, 0, 1024);
          // tahun publikasi
          read(sock, see, 1024);
          printf("%s", see);
        }
        else if (strcmp(buffer2, "find\n") == 0)
        {
          readMessage(sock);
          // input string
          char inputString[1024] = {0};
          fgets(inputString, 1024, stdin);

          send(sock, inputString, strlen(inputString), 0);
        }
        else if (strcmp(buffer2, "download\n") == 0)
        {
          /* code */
        }
        else if (strcmp(buffer2, "quit\n") == 0)
        {
          printf("Good Bye\n");
          return 0;
        }
      }
    }
  }

  printf("Good Bye\n");
  // valread = read( sock , buffer, 1024);

  return 0;
}
