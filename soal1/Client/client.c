#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
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

#define PORT 1037
char cwd[100];

int main(int argc, char const *argv[])
{
  getcwd(cwd, sizeof(cwd));
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
  while (otentikasi == 0)
  {
    puts("otentikasi 0");
    bzero(buffer, sizeof(buffer));
    valread = read(sock, buffer, 1024);
    if (valread < 1)
    {
      printf("1eror recv\n");
    }
    printf("%s\n", buffer);
    bzero(buffer, sizeof(buffer));
    fgets(buff, 1024, stdin);
    send(sock, buff, strlen(buff), 0);

    //
    if (strncmp(buff, "1", 1) == 0)
    {
      bzero(buffer, sizeof(buffer));
      bzero(buff, sizeof(buff));
      puts("login form");
      char buff2[1024] = {0};
      valread = read(sock, buffer, 1024);
      if (valread < 1)
      {
        printf("2eror recv\n");
      }
      printf("%s\n", buffer);
      bzero(buffer, sizeof(buffer));
      fgets(buff, 1024, stdin);
      send(sock, buff, strlen(buff), 0);
      bzero(buff, sizeof(buff));
      puts("lalal");
      valread = read(sock, buffer, 1024);
      if (valread < 1)
      {
        printf("3eror recv\n");
      }
      printf("%s\n", buffer);
      bzero(buffer, sizeof(buffer));
      fgets(buff2, 1024, stdin);
      send(sock, buff2, strlen(buff2), 0);
      bzero(buff2, sizeof(buff2));
      char isotentikasi[2] = {0};
      valread = read(sock, isotentikasi, 2);
      if (valread < 1)
      {
        printf("4eror recv\n");
      }
      printf("auth %s\n", isotentikasi);
      otentikasi = atoi(isotentikasi);
      bzero(isotentikasi, sizeof(isotentikasi));
    }
    if (strncmp(buff, "2", 1) == 0)
    {
      // bersihkan buffer
      bzero(buffer, sizeof(buffer));
      bzero(buff, sizeof(buff));
      // id
      valread = read(sock, buffer, 1024);
      if (valread < 1)
      {
        printf("5eror recv\n");
      }
      printf("%s\n", buffer);
      bzero(buffer, sizeof(buffer));
      // Masukkan kredensial
      fgets(buff, 1024, stdin);
      // kirim kredensial
      send(sock, buff, strlen(buff), 0);
      bzero(buff, sizeof(buff));

      valread = read(sock, buffer, 1024);
      if (valread < 1)
      {
        printf("6eror recv\n");
      }
      printf("%s\n", buffer);
      bzero(buffer, sizeof(buffer));
      fgets(buff, 1024, stdin);
      send(sock, buff, strlen(buff), 0);
      bzero(buff, sizeof(buff));
    }
    else if (strcmp(buff, "3\n") == 0)
    {
      printf("Good Bye\n");
      return 0;
    }
    if (otentikasi != 0)
    {
      puts("logged in");
      int inputcommand = 1;
      while (inputcommand)
      {
        char pesan[300] = {0};
        char buffer2[4096] = {0};
        bzero(buffer2, sizeof(buffer2));
        valread = read(sock, pesan, 300);
        if (valread < 1)
        {
          printf("7eror recv\n");
        }
        printf("%s", pesan);
        bzero(buffer2, sizeof(buffer2));
        fgets(buffer2, 4096, stdin);
        send(sock, buffer2, strlen(buffer2), 0);
        if (strcmp(buffer2, "add\n") == 0)
        {
          FILE *fp;
          char input[4096];
          bzero(buffer2, sizeof(buffer2));
          char *tes;
          // publisher
          valread = read(sock, buffer2, 4096);
          if (valread < 1)
          {
            printf("8eror recv\n");
          }
          printf("%s", buffer2);
          bzero(buffer2, sizeof(buffer2));
          fgets(input, 4096, stdin);
          // printf("%s", input);
          send(sock, input, strlen(input), 0);
          printf("%s", input);
          bzero(input, sizeof(input));

          // tahun publikasi
          valread = read(sock, buffer2, 4096);
          if (valread < 1)
          {
            printf("9eror recv\n");
          }
          printf("%s", buffer2);
          bzero(buffer2, sizeof(buffer2));
          fgets(input, 4096, stdin);
          send(sock, input, strlen(input), 0);
          bzero(input, sizeof(input));
          // filepath
          valread = read(sock, buffer2, 4096);
          if (valread < 1)
          {
            printf("10eror recv\n");
          }
          printf("%s", buffer2);
          bzero(buffer2, sizeof(buffer2));
          char filenamepath[4096] = {0};
          bzero(filenamepath, sizeof(filenamepath));
          // gets(tes);
          fgets(input, 4096, stdin);
          printf("buf%s\n", input);
          strcpy(filenamepath, input);
          bzero(input, sizeof(input));
          strtok(filenamepath, "\n");
          printf("%s\n", filenamepath);
          // filenamepath[strcspn(filenamepath, "\n")] = 0;
          send(sock, filenamepath, strlen(filenamepath), 0);

          ssize_t len;
          struct stat file_stat;
          char file_size[256];
          char file_length[1024];

          fp = fopen(filenamepath, "r+");
          printf("%x\n", fp);
          char bufferz[1024];
          bzero(bufferz, sizeof(bufferz));
          if (fp > 0)
          {
            printf("Sending file: %s\n", filenamepath);
          }
          else
          {
            printf("Failed to open file: %s\n", filenamepath);
            send(sock, "error", 20, 0);
            // return;
          }
          bzero(bufferz, sizeof(bufferz));
          while (fgets(bufferz, 1024, fp) != NULL)
          {
            // printf("tes");
            if (send(sock, bufferz, sizeof(bufferz), 0) == -1)
            {
              fprintf(stderr, "System error (%d): %s\n", errno, strerror(errno));
              exit(1);
            }
            bzero(bufferz, sizeof(bufferz));
          }
          fclose(fp);
          send(sock, "EOF", 10, 0);
          printf("[+]File data sent successfully.\n");
        }
        if (strncmp(buffer2, "delete", 6) == 0)
        {
          char *namafile;
          char *tmp;
          char *tmp2[50];
          tmp = strtok(buffer2, " ");
          int n;
          n = 0;
          while (tmp != NULL)
          {
            tmp2[n] = tmp;
            n++;
            tmp = strtok(NULL, " ");
          }
          strtok(buffer2, "delete ");
          printf("%s\n", tmp2[n - 1]);
          send(sock, tmp2[n - 1], sizeof(tmp2[n - 1]), 0);
          printf("%s\n", tmp2[n - 1]);
          // printf("under maintanance\n");
        }
        if (strncmp(buffer2, "see", 3) == 0)
        {
          bzero(buffer2, sizeof(buffer2));
          char see[1024];
          bzero(see, sizeof(see));
          valread = read(sock, see, 1024);
          if (valread < 1)
          {
            printf("eror recv\n");
          }
          printf("%s", see);
          bzero(see, sizeof(see));
        }
        if (strncmp(buffer2, "find", 4) == 0)
        {
          printf("under maintain\n");
        }
        if (strncmp(buffer2, "download", 8) == 0)
        {
          char *namafile;
          char *tmp;
          char *tmp2[50];
          tmp = strtok(buffer2, " ");
          int n;
          n = 0;
          while (tmp != NULL)
          {
            tmp2[n] = tmp;
            n++;
            tmp = strtok(NULL, " ");
          }
          strtok(buffer2, "delete ");
          printf("%s\n", tmp2[n - 1]);
          send(sock, tmp2[n - 1], sizeof(tmp2[n - 1]), 0);
          printf("%s\n", tmp2[n - 1]);
          char filenamepath[100];
          strcpy(filenamepath, tmp2[n - 1]);
          strtok(filenamepath, "\n");
          printf("file %s\n", filenamepath);
          FILE *file_fd = fopen(filenamepath, "a+");
          char bufferz[1024];
          bzero(bufferz, sizeof(bufferz));
          int pors;
          while (1)
          {
            // printf("lool\n");
            pors = read(sock, bufferz, 1024);
            if (strcmp(bufferz, "EOF") == 0)
            {
              break;
            }
            if (pors > 0 && strcmp(bufferz, "error") == 0)
            {
              printf("testint erorr\n");
              // fprintf(stderr, "System error (%d): %s\n", errno, strerror(errno));
              break;
            }

            fprintf(file_fd, "%s", bufferz);
            bzero(bufferz, sizeof(bufferz));
          }
          fclose(file_fd);
          printf("done\n");
          // printf("under maintanance\n");
        }
        if (strncmp(buffer2, "quit", 4) == 0)
        {
          printf("Good Bye\n");
          return 0;
        }
      }
    }
  }

  return 0;
}