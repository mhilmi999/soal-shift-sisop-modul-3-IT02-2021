## soal-shift-sisop-modul-3-IT02-2021
---

Penjelasan dan penyelesaian Soal Shift 3 Sistem Operasi 2021\
Kelompok IT02

1. Muhammad Hilmi Ramadhan (05311940000044)
2. Lambang Akbar Wijayadi (05311940000006)
3. Mulki Kusumah (05311940000043)

---

## Daftar Isi

- [Soal 1](#soal-1)

  - [Soal 1.a.](#soal-1a)
  - [Soal 1.b.](#soal-1b)
  - [Soal 1.c.](#soal-1c)
  - [Soal 1.d.](#soal-1d)
  - [Soal 1.e.](#soal-1e)
  - [Soal 1.f.](#soal-1f)
  - [Soal 1.g.](#soal-1g)
  - [Soal 1.h.](#soal-1h)

- [Soal 2](#soal-2)

  - [Soal 2.a.](#soal-2a)
  - [Soal 2.b.](#soal-2b)
  - [Soal 2.c.](#soal-2c)

- [Soal 3](#soal-3)
  - [Soal 3.a.](#soal-3a)
  - [Soal 3.b.](#soal-3b)
  - [Soal 3.c.](#soal-3c)
  - [Soal 3.d.](#soal-3d)
  - [Soal 3.e.](#soal-3e)

---

<br>


## Soal 1
---

Source Code tersedia pada : [client.c](./soal1/Client/client.c)
Source Code tersedia pada : [server.c](./soal1/Server/server.c)

## **Analisa Soal**

Secara umum, kami menangkap bahwa program yang harus dibuat merupakan sebuah penerapan dari socket programming dengan bantuan dari thread. Dimana server bisa menerima tidak hanya 1 client request saja. Soal ini akan membuat sebuah database file buku, dimana nanti akan diperlukan file tsv untuk databasenya bukunya, file akun.txt untuk verifikasi loginya, log penambahan dan pengurangan data dalam database, dan directory untuk file file yang disimpan dalam server-side. Selain itu juga sebagai client bisa mendownload file yang tersimpan di server, mendelete file pada server, menambahkan file atau database dan juga melihat dan mencari isi dalam database

Hal-hal yang perlu diperhatikan diantaranya :
1. Dikerjakan dalam socket programming dengan sisi server yang dapat menerima request lebih dari 1 client dalam waktu yang berbeda.
2. Terdapat akun.txt sebagai database akun yang teregistrasi di server-side.
3. Terdapat file files.tsv sebagai database di server-side.
4. Terdapat directory FILES pada sisi server dimana sebagai path penyimpanan file.
5. Dilarang menggunakan system() dan execv().
6. Untuk download dan upload silahkan menggunakan file teks dengan ekstensi dan isi bebas (yang ada isinya bukan touch saja dan tidak kosong) dan requirement untuk benar adalah percobaan dengan minimum 5 data.


<br>

**Cara pengerjaan**
---
Dalam menyelesaikan program yang diminta oleh [soal3](#soal-3), pertama-tama yang diperlukan adalah melakukan *import* library yang digunakan dalam socket programming :
```cpp
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
```
- `<pthread.h>` library untuk mendapatkan ID dari pemanggilan sebuah thread(e.g. `pthread_self()`)
- `<stdio.h>` library untuk fungsi input-output (e.g. `scanf()`)
- `<sys/types.h>` library digunakan untuk identifikasi sebuah thread (e.g `pthread_t id_thread[3]`)
- `<sys/socket.h>` library untuk membuat socket.
- `<stdlib.h>` library untuk fungsi umum (e.g. `NULL`)
- `<errno.h>` library untuk memberikan tambahan error pada sistem yang sesuai dengan IEEE Std 1003.1-2001  (e.g. `ECHILD`)
- `<string.h>` library untuk melnampilkan pesan error apa saat gagal membuat thread dalam *development side* (e.g. `strerror()`)
- `<arpa/inet.h>` library untuk melakukan koneksi socket
- `<unistd.h>` library untuk mendapatkan lokasi direktori saat bekerja dimana (e.g. `getcwd()`)
- `<netinet/in.h>` library untuk melakukan koneksi socket
- `<sys/stat.h>` library untuk melakukan pemanggilan fungsi dalam pembuatan sebuah direktori baru (e.g. `mkdir()`)
- `<fcntl.h>` library untuk handling file (e.g `open()`)
- `<sys/sendfile.h>` library untuk mengirimkan data antar file descriptor

Selain itu kami sangat terbantu dengan template socket programming yang terdapat dalam modul 3 sistem operasi. Kurang lebih dasar pengerjaan kami adalah modul tersebut yang selanjutnya dikembangkan.

```cpp
int main(int argc, char const *argv[])
{
    getcwd(cwd, sizeof(cwd));
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
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
```
_Code untuk memulai socket programming dalam server_

```cpp
int main(int argc, char const *argv[])
{
  getcwd(cwd, sizeof(cwd));
  struct sockaddr_in address;
  int sock = 0, valread;
  struct sockaddr_in serv_addr;

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
```
_Code untuk memulai socket programming dalam client_
<br>
 Untuk melakukan transfer data antara client dan server kami menggunakan potongan program dibawah ini. Untuk mengirim dan menerima pesan dari server dan client kami menggunakan bantuan fungsi `send` untuk melakukan pengiriman buffer data,  `read` untuk menerima dan menyimpan variabel dari data yang dikirimkan, `bzero` untuk me-NULL-kan variabel buffer yang berguna untuk tempat pengiriman ataupun penerimaan data. 

```cpp
    char *message = "user id : ";
    send(new_socket, message, strlen(message), 0);
```
_Code pengiriman data, argumen untuk fungsi `send` ada 4 yaitu `new_socket` sebagai variabel koneksi socket, `message` sebagai pesan yang dikirimkan, `strlen()` sebagai panjangnya data yang dikirimkan , dan 0 sebagai flags default dari fungsi `send`_

```cpp
    int valread;
    char id[1024];
    valread = read(new_socket, id, 1024);
    if (valread < 1)
    {
        printf("eror recv\n");
    }
    printf("%s\n", id);
    strtok(id, "\n");
```
_Code penerimaan data, dimana variabel valread nantinya akan menjadi status keberhasilan penerimaan data, jika fungsi `read` mengembalikan nilai -1 maka akan memunculkan pesan error recv. Fungsi `read` memiliki 3 argument yang pertama `new_socket` berarti file descriptor yang merupakan variabel connect ke server/client, `id` merupakan variabel buffer yang akan menerima pesan yang dikirimkan, dan argumen ketiga yaitu `1024` sebagai panjang file yang mampu ditampung fungsi read._
<br>

## Soal 1.a.
## **Analisa Soal**
Pada soal 1.a sebagai permulaan dari soal ini terdapat beberapa kriteria utama yaitu server bisa multi-connection. Client bisa melakukan login dan register, client yang melakukan register akan memasukan id dan password yang nantinya disimpan dalam file akun.txt di server-side. Client yang memilih login akan memasukan id dan password, jika id dan passwordnya ada di dalam file akun.txt, client akan diarahkan ke fitur fitur program, namun jika id dan password tidak ada maka client akan diarahkan pada pilihan login, register, dan quit secara berulang.

**Cara Pengerjaan**
---
Dalam menjawab soal 1.a. kami membuat server yang bisa terkoneksi secara multi-connection dengan menambahkan argumen pada fungsi listen(), yaitu listen(server_fd, 3), 3 disini sebagai batas maksimal client yang dapat terkoneksi dengan server.

```cpp
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
```
_Code server_

```cpp

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
```
_Code Client_
<br>
Untuk selanjutnya diperlukan fitur login dan register. Disini menggunakan session pada variabel otentikasi, dimana jika sudah login maka akan diganti nilai otentikasinya menjadi 1. Jika client memilih 1 maka akan diarahkan dalam fitur login dimana client akan mendapat tampilan untuk memasukan id dan password kemudian server akan me-concat inputan dari client supaya sesuai dengan database akun.txt. Jika client memilih 2 maka client akan memasukan id dan password yang nantinya dimasukan server kedalam file akun.txt pada sisi server.

```cpp
int otentikasi = 0;
while (otentikasi == 0)
{
    puts("otentikasi 0");
    char *hello = "Server :\n1. Login \n2. Register \n3. Quit ";
    send(new_socket, hello, strlen(hello), 0);
    bzero(buffer, sizeof(buffer));
    read(new_socket, buffer, 1024);
    if (strncmp(buffer, "1", 1) == 0)
    {
        puts("log");
        char password[1024];
        char login[1024];
        char *message = "User ID :";
        send(new_socket, message, strlen(message), 0);
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
        otentikasi = checkLogin(login);
        char isotentikasi[2];
        memset(isotentikasi, 0, 2);
        sprintf(isotentikasi, "%d", otentikasi);
        printf("auth %d\n", otentikasi);
        printf("auth %d\n", isotentikasi);
        send(new_socket, isotentikasi, strlen(isotentikasi), 0);
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

```
_Code server_

```cpp
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
      puts("...");
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
```
_Code client_
<br>




## Soal 1.b.
## **Analisa Soal**
Pada soal 1.b kita diharuskan untuk mempunyai database dengan nama files.tsv, yang akan menyimpan data publisher, tahun publikasi, dan path nama file yang ada di server. Selain itu juga harus terdapat folder FILES, yang akan menyimpan file file yang ditambahkan pada server.

**Cara Pengerjaan**
---
Guna menyelesaikan permasalahan b, kami mengikuti arahan dari Tips dengan menggunakan fungsi mkdir dan juga menggunakan fopen dengan tag `a+` dimana akan membuka file, dan jika file tersebut tidak ditemukan maka akan otomatis membuat filenya. Code ini dijalankan sebelum server terkoneksi dengan client, jadi ketika server masih baru dijalankan akana dibuat file `files.tsv` dan folder FILES.

```cpp
....
    mkdir("FILES", 0777);
    FILE *fp;
    fp = fopen("files.tsv", "a+");
    fclose(fp);
....
```
_Code server_

```cpp
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
    char *strmax;
    char *xyz = strtok_r(file_content, "\n", &strmax);
    int x = 0;
    strcpy(baris[i], xyz);

    while (xyz != NULL)
    {
        int n = 0;
        char *tok = strtok(baris[x], "< >");
        while (tok != NULL)
        {
            strcpy(info[n], tok);
            n++;
            tok = strtok(NULL, "< >");
        }
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
        strcpy(fullpath[x], temp);

        int another_counter = 0;
        char *potongtitik;
        potongtitik = strtok(temp, ".");
        while (potongtitik != NULL)
        {
            strcpy(another_buffer[another_counter], potongtitik);
            another_counter++;

            potongtitik = strtok(NULL, ".");
        }
        if (another_buffer[1] > 0)
        {
            strcpy(ekstensi[x], another_buffer[1]);
        }
        else
        {
            strcpy(ekstensi[x], "no ekstensi");
        }
        char *token3 = strtok(temp, "/");
        strcpy(token2, strtok(NULL, "/"));
        strcpy(nama[x], token2);
        printf("%d -> %s -> %s -> %s -> %s -> %s \n", x, xyz, publisher[x], year[x], fullpath[x], ekstensi[x]);
        jumlahData++;
        x++;
        i++;
        xyz = strtok_r(NULL, "\n", &strmax);
        if (xyz != NULL)
            strcpy(baris[x], xyz);
    }
    fclose(filedb);
}
```
_code membaca database tiap memulai program server_
Fungsi diatas akan selalu dijalankan terlebih dahulu ketika memulai server, dimana code diatas akan membaca file database dan menyimpannya dalam variabel variabel global untuk membantu menjalankan fungsi pada fitur fitur selanjutnya.

<br>

## Soal 1.c.
## **Analisa Soal**
Pada soal ini kita akan membuat code untuk fitur add pada soal. Fitur add ini nanti membuat client dapat menambahkan data ke dalam server yaitu data publisher, tahun publikasi dan path sebuah file yang akan dikirimkan oleh user ke server. Server sendiri akan menyimpan inputan dari client dan dimasukan ke dalam database, selain itu server juga bisa menerima file yang diupload dari client.

**Cara Pengerjaan**
---
Dalam menjawab soal 1.c. kami menyelesaikannya dengan bantuan dari send dan read pada socket programming seperti dijelaskan dalam soal poin sebelumnya. Server akan mengirimkan form untuk publisher, tahun publikasi, filepath yang akan di upload, dan client akan mengirimkan datanya satu persatu. Jika client sudah menginputkan ketiga form yang dikirimkan server maka dari client akan melakukan upload file sesuai dengan filepath yang diinputkan dan server akan menerimanya _byte-per-byte_. Setelah file berhasil dikirimkan ke server, server akan memasukan data data yang sudah didapatkan dari client kedalam database `files.tsv`.

```cpp
....
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
        temp2[x] = slash;
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
....
```
_Code server dalam penerimaan data dari client_

```cpp
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
        another_counter++;

        potongtitik = strtok(NULL, ".");
    }
    if (another_buffer[1] > 0)
    {
        strcpy(ekstensi[jumlahData], another_buffer[1]);
    }
    else
    {
        strcpy(ekstensi[jumlahData], "no ekstensi");
    }
    char *token3 = strtok(filenamepath, "/");
    strcpy(token2, strtok(NULL, "/"));
    strcpy(nama[jumlahData], token2);

    jumlahData++;
    sprintf(tmp, "tambah: %s\n", fileekstensi);
    runninglog(tmp);
}
```
_Code server untuk memasukan input user ke dalam database_

```cpp
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
```
_Code client dalam fitur add_
<br>

## Soal 1.d.

## **Analisa Soal**
Pada soal ini kita diminta untuk client bisa melakukan fitur download, yaitu mengambil file yang disimpan dalam database server. Disini prosesnya adalah client mengirimkan request file apa yang diminta kemudian server akan melakukan pengecekan apakah file tersebut ada di server atau tidak, jika ada maka file akan dikirimkan dari server ke client, jika tidak akan diberikan pesan gagal ke client.

**Cara Pengerjaan**
---
Dalam pengerjaan soal 1.d ini kami menggunakan snipet code yang pada soal sebelum juga sudah tersedia seperti send dan read message dari client ataupun server, proses pengiriman dan penerimaan file yang tadinya juga sudah terdapat pada fitur add. Pada soal ini kami hanya menambahkan pengecekan database terlebih dahulu memastikan file yang apakah benar terdapat dalam sisi server atau tidak. 

```cpp
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
    printf("%s\n", tmp2[n - 1]);
    send(sock, tmp2[n - 1], sizeof(tmp2[n - 1]), 0);
    printf("%s\n", tmp2[n - 1]);
    char filenamepath[100];
    strcpy(filenamepath, tmp2[n - 1]);
    strtok(filenamepath, "\n");
    printf("file %s\n", filenamepath);
    bzero(buffer2, sizeof(buffer2));

    valread = read(sock, buffer2, 4096);
    if (valread < 1)
    {
    printf("10eror recv\n");
    }
    printf("%s\n", buffer2);
    if (strncmp(buffer2, "File tidak dapat", 16) != 0)
    {
    FILE *file_fd = fopen(filenamepath, "a+");
    char bufferz[1024];
    bzero(bufferz, sizeof(bufferz));
    int pors;
    while (1)
    {
        printf("...");
        pors = read(sock, bufferz, 1024);
        if (strcmp(bufferz, "EOF") == 0)
        {
        break;
        }
        if (pors > 0 && strcmp(bufferz, "error") == 0)
        {
        fprintf(stderr, "System error (%d): %s\n", errno, strerror(errno));
        break;
        }

        fprintf(file_fd, "%s", bufferz);
        bzero(bufferz, sizeof(bufferz));
    }
    fclose(file_fd);
    printf("done\n");
    }
    else
    {
    printf("file gagal dikirim\n");
    }
    bzero(buffer2, sizeof(buffer2));
}
```
_Code Client dalam fitur download_

Dalam sisi client akan mengirimkan command seperti `download rop.py` , rop.py adalah nama file yang ingin di-download oleh client, Dari command client yang berbentuk seperti itu, Code akan memotong perintah client menjadi dua bagian, yang pertama adalah `download` yang kedua `rop.py` atau nama file yang diminta client. Perintah pertama ini akan menjadi penanda command apa yang diberikan client dan potongan kedua menjadi nama file yang akan didownload oleh client. Server akan mengembalikan apakah file tersebut terdapat di server atau tidak, jika iya client akan melakukan proses penerimaan file jika tidak menerima pesan bahwa file gagal dikirim.


```cpp
if (strncmp(buffer2, "download", 8) == 0)
{
    printf("%s\n", buffer2);
    int status, avail = 0, pcc;
    char namafile[4096];
    char old[1024];
    char ano[1024];
    bzero(namafile, sizeof(namafile));
    valread = read(new_socket, namafile, 4096);
    if (valread < 1)
    {
        printf("recv err\n");
    }
    printf("nama file %s\n", namafile);
    sprintf(old, "FILES/%s", namafile);
    printf("full %s\n", old);
    strtok(old, "\n");
    sprintf(ano, "%s/FILES/%s", cwd, namafile);
    strtok(ano, "\n");
    printf("%s\n", ano);
    for (int i = 0; i < jumlahData; i++)
    {
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
        FILE *fp = fopen(ano, "r+");
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
```
_Code sisi server untuk fitur download_

Pada sisi server setelah menerima perintah `download` maka server akan menerima nama file yang diminta. Server akan melakukan pengecekan pada variabel global yang sudah di-assign pada saat menjalankan server. Jika terdapat nama file yang sama dari yang diminta dan disimpan dalam database maka akan diproses untuk pengiriman dari server ke client. 

<br>

## Soal 1.e.

## **Analisa Soal**
Pada soal ini client dapat melakukan delete file yang terdapat pada server. Namun file harus dicek terlebih dahulu apakah memang benar tersedia atau tidak dalam database. Selain itu sebenarnya server tidak menghapus file namun merubah namanya menjadi old-namafile.ekstensi dan menghapusnya dari list file database. 

**Cara Pengerjaan**
---
Dalam pengerjaannya kami akan menggunakan fungsi-fungsi yang sudah dibuild sebelumnya seperti pemotongan antara command dan nama file pada fungsi download, begitu juga dengan penerimaan dan pengiriman data melalui socket programming. Selain itu kami juga menggunakan bantuan dari fungsi rename, karena juga perlu diperhatikan bahwasanya tidak boleh menggunakan fungsi system dan execve untuk mengganti nama file yang hendak dihapus. Untuk menghapus list file dalam database kami akan membuat sebuah temporary database dan menuliskan ulang semua database kecuali untuk path file yang akan di-delete.

```cpp
if (strncmp(buffer2, "delete", 6) == 0)
{
    char *namafile;
    char *tmp;
    char *tmp2[50];
    char buff[1024];
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
    valread = read(sock, buff, 1024);
    if (valread < 1)
    {
    printf("delete eror recv\n");
    }
    printf("%s\n", buff);
}
```
_Code client saat mengirimkan request delete_

Dalam sisi client akan mengirimkan command seperti `delete rop.py` , rop.py adalah nama file yang ingin di-delete oleh client, Dari command client yang berbentuk seperti itu, Code akan memotong perintah client menjadi dua bagian, yang pertama adalah `delete` yang kedua `rop.py` atau nama file yang diminta client. Perintah pertama ini akan menjadi penanda command apa yang diberikan client dan potongan kedua menjadi nama file yang akan di-delete oleh client. Server akan mengembalikan apakah file tersebut terdapat di server atau tidak.

```cpp
if (strncmp(buffer2, "delete", 6) == 0)
{
    printf("%s\n", buffer2);
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
                if (i == pcc)
                {
                    continue;
                }
                else
                {

                    file_new_db = fopen("temp.tsv", "a+");
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
```
_Code server untuk handling command `delete`_

Dalam sisi server sendiri akan melakukan iterasi pengecekan apakah filenya terdapat di database atau tidak, seperti pada fungsi selanjutnya. Jika filenya terdapat di database maka akan melakukan proses rename file, jika file berhasil di-rename maka akan dilakukan penulisan database ulang tanpa menuliskan database yang dihapuskan.

<br>

## Soal 1.f.

## **Analisa Soal**
Pada soal ini kita diminta untuk bisa mengembalikan data didalam database dengan format :
```
Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 

Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 

```
Dari format yang diberikan, soal ini lebih mengarah ke penanganan string untuk melakukan slicing dari value dalam database dan dibagi menjadi publisher, tahun publikasi, ekstensi file, nama file, dan filepath pada server.
**Cara Pengerjaan**
---
untuk mengerjakan poin 1.f kami sangat terbantu dengan fungsi yang sudah di-build sebelumnya, yaitu fungsi `readDatabase()` yanng dijalankan setiap memulai server. Karena pada fungsi tersebut sekaligus kami melakukan slicing untuk publisher, tahun publikasi, ekstensi file, nama file, dan filepath yang di-assign dalam global variabel. Jadi dalam fitur see ini kami akan melakukan looping sebanyak jumlah data dalam database dan melakuna concat-ing untuk semua datanya sesuai format yang diberikan dan mengirimkanya ke client.

```cpp
if (strncmp(buffer2, "see", 3) == 0)
{
    bzero(buffer2, sizeof(buffer2));
    char see[4096];
    bzero(see, sizeof(see));
    valread = read(sock, see, 4096);
    if (valread < 1)
    {
    printf("eror recv\n");
    }
    printf("%s", see);
    bzero(see, sizeof(see));
}
```
_Code client side untuk fitur see_

Pada susu client seperti biasa jika memasukan command see akan menerima data yang dikirimkan server dan menampilkanya namun jika terdapat error dalam penerimaan data akan dimunculkan `error recv`.

```cpp
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
```
_Code dari sisi server untuk handling command see yang dikirimkan client_

Pada sisi server karena tadinya sudah mendapatkan command see dari client maka server akan mengambil semua data dari variabel `nama[]` yang berisi nama file, `publisher[]` yang berisi nama publishernya, `year[]` yang berisi taun publikasi, `ekstensi[]` yang berisi ekstensi file yang disimpan, dan `fullpath[]` yang berisi Path file yang disimpan dalam server. Variabel `jumlahData` menandakan jumlah data yang terdapat di database. Program akan melakukan concatenasi dengan bantuan fungsi `strcat` untuk setiap blok format dan mengirimkanya secara utuh jika semua data di global variabel sudah dibaca.
<br>


## Soal 1.g.

## **Analisa Soal**
Pada soal ini kami akan melakukan pengecekan apakah sebuah potongan string yang dikirimkan oleh client terdapat dalam nama file yang ada di server. Perlu diperhatikan bahwasanya bisa saja client hanya mengirimkan 2 char dan server harus bisa mendapatkan data secara utuh dan mengembalikanya ke client dengan format : 

```
Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 

```

**Cara Pengerjaan**
---
Dalam proses pengerjaan poin ini kami menggunakan bantuan dari fungsi `strstr` yang akan bekerja sebagai filter apakaha dalam variabel `fullpath[]` terdapat string yang diminta. Fungsi strstr akan mengembalikan substring yang ditemukan dalam variabel atau NULL jika tidak menemukanya.

```cpp
if (strncmp(buffer2, "find", 4) == 0)
{
    char *namafile;
    char *tmp;
    char buff[1024];
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
    strtok(buffer2, "find ");
    printf("%s\n", tmp2[n - 1]);
    send(sock, tmp2[n - 1], sizeof(tmp2[n - 1]), 0);
    printf("%s\n", tmp2[n - 1]);
    valread = read(sock, buff, 4096);
    if (valread < 1)
    {
    printf("find eror recv\n");
    }
    printf("%s\n", buff);

    if (strncmp(buff, "file ditemukan", 14) == 0)
    {
    bzero(buff, sizeof(buff));
    valread = read(sock, buff, 4096);
    if (valread < 1)
    {
        printf("find eror recv\n");
    }
    printf("%s\n", buff);
    }
}
```
_Code fitur see dari sisi client_
Client akan mengirimkan command `see ro` misalnya, kemudian akan di-slice antara command dan string yang akan dicari dimana pada contoh string yang dicari adalah ro. Server akan memproses pengiriman client ini dan akan mengembalikan sesuai hasil yang didapatkan.

```cpp
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
```
_Code pada sisi server untuk handle fitur find_
Dalam server sendiri telah menerima string yang dicari dan disimpan dalam variabel `namaFile`, kemudian dari variabel `namaFile` ini dengan bantuan fungsi `strstr` akan dilakukan iterasi sebanyak jumlah dari variabel `fullpath[]`, jika didapatkan maka akan melakukan penambahan dari counter yang nantinya digunakan dalam proses iterasi pembuatan pesan yang akan dikirimkan ke client. Ketika sudah didapatkan berapa jumlah nama file yang mengandung string tersebut akan melakukan concat-ing seperti fitur see dan mengirimkanya ke client. Kami juga memanfaatkan variabel global yang sudah dijelaskan dalam fitur see untuk mengirimkan sesuai dengang format messagenya.
<br>

## Soal 1.h.

## **Analisa Soal**
Pada soal ini, server diminta untuk membuat sebuah file `running.log` dimana file ini akan digunakan sebagai log, jika terdapat penambahan ataupun penghapusan file. File log ini mempunyai format yang sudah ditentukan yaitu seperti ini:

```
Tambah : File1.ektensi (id:pass)
Hapus : File2.ektensi (id:pass)
```

**Cara Pengerjaan**
---
Poin soal ini hanya akan memengaruhi code dalam sisi server saja, Dimana saya membuat fungsi `runninglog()` dengan argument pesan yang akan ditambahkan dalam file ini. Fungsinya cukup sederhana, dengan bantuan file descriptor kita akan membuak file dengan tag a+ dimana ini akan meng-append dan juga membuat file tersebut jika masih belum tersedia, dan selanjutnya akan mengisi filenya dengan bantuan `fprintf()`.
```cpp
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
```
_Code runninglog dalam server_

Fungsi ini akan dipanggil ketika menggunakan fitur add dan delete, berikut untuk snippet codenya.

```cpp
void tambahDB(char *temp_publish, char *tahun_publikasi, char *filenamepath, char *fileekstensi, int n)
{
    char tmp[1024];
    ....
    sprintf(tmp, "tambah: %s\n", fileekstensi);
    runninglog(tmp);
}
```
_Code menambahakan log pada saat menggunakan fitur add_

```cpp
if (strncmp(buffer2, "delete", 6) == 0)
{
....
        re_status = rename(tsv_temp, tsv_new);
        if (re_status == 0)
        {
            char log[1024];
            sprintf(log, "hapus: %s", namafile);
            runninglog(log);
            printf("Database change!\n");
        }
....
}
    
```
_Code menambahkan log pada saat menggunakan fitur delete, Code ini dijalankan ketika sudah berhasil membuat database baru_
<br>

**Screenshoot Hasil Run Program Soal Nomor 1 Client**\
![hasil](./img/soal1/client/...png)
---

<br>

**Screenshoot Hasil Run Program Soal Nomor 1 Server**\
![hasil](./img/soal1/server/...png)
---

<br>


<br>
<br>

## Soal 2
---

Source Code [soal2a.c](./soal2/soal2a.c)\
Source Code [soal2b.c](./soal2/soal2b.c)\
Source Code [soal2c.c](./soal2/soal2c.c)

## **Analisa Soal**

Dari kami merasa soal ini lebih utamanya meminta membuat sebuah program perkalian matriks dan menampilkan hasilnya. Selanjutnya program ini menggunakan shared memory yang mana akan dilakukan terhadap perhitungan matriks yang baru (inputan user). Perhitungan terjadi pada tiap-tiap sel yang berasal dari matriks A agar menjadi faktorial, lalu sel dari matriks B menjadi batas maksimal faktorialnya. Detil dari program yang diminta adalah sebagai berikut :
1. Program merupakan untuk perkalian matriks `4x3` dengan `3x6` dan menampilkan hasilnya. Matriks hanya berisi angka 1 - 20 `(tidak perlu membuat filter angka)`.
2. Program yang menerima inputan output dari [soal2a.c](./soal2/soal2a.c) dengan shared memory yang selanjutnya akan melakukan kalkulasi matriks baru (inputan baru user) dengan detil nantinya pada [Soal 2.b.](#soal-2b). 
3. Program yang ketiga dibuat untuk melakukan *cross check* dalam 5 proses teratas yang memakan *reource* komputernya dengan command <br> `"ps aux | sort -nrk 3,3 | head -5"`.
4. Semua matriks berasal dari input ke program.
5. Dilarang menggunakan `system()`.


<br>

**Cara pengerjaan**
---
Dalam menyelesaikan program yang diminta oleh [soal2](#soal-2), pertama-tama yang diperlukan adalah melakukan *import* library yang digunakan dalam pembuatan matrix dengan `thread` :
```c
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
```
- `<pthread.h>` library untuk mendapatkan ID dari pemanggilan sebuah thread(e.g. `pthread_self()`)
- `<stdio.h>` library untuk fungsi input-output (e.g. `scanf()`)
- `<stdlib.h>` library untuk fungsi umum (e.g. `NULL`)
- `<string.h>` library untuk melnampilkan pesan error apa saat gagal membuat thread dalam *development side* (e.g. `strerror()`)
- `<sys/ipc.h>` library digunakan untuk struktur akses dalam komunikasi antarproses (e.g `IPC_RMID, IPC_CREAT`)
- `<sys/shm.h>` library digunakan untuk melakukan operasi *shared memori*  (e.g `shmat, shmdt, shmctl, shmget`)
- `<sys/types.h>` library digunakan untuk identifikasi sebuah thread (e.g `pthread_t id_thread[3]`)
- `<sys/wait.h>` library untuk keterkaitan terhadap library `<sys/types.h>`
- `<unistd.h>` library untuk mendapatkan lokasi direktori saat bekerja dimana (e.g. `getcwd()`)

<br>

## Soal 2.a.
## **Analisa Soal**
Soal meminta untuk membuat program perkalian matriks `4x3` dan `3x6` serta menampilkan hasilnya. Yang mana matriks ini berisi angka 1 - 20 (tanpa perlu membuat filter angka).

**Cara Pengerjaan**
---
Perkalian dilakukan antara matriks `4x3` dan `3x6` sehingga menghasilkan matriks `4x6`. Dalam perkalian matriks, kami menggunakan `thread "kali matriks"` dengan detil *code* sebagai berikut :
```cpp
void *kalimatriks(void *arg)
{
    pthread_t id = pthread_self();

    if (pthread_equal(id, tid[0])) // baris 1 matriks c
        for (int i = 0; i < 6; i++)
            C[0][i] = A[0][0] * B[0][i] + A[0][1] * B[1][i] + A[0][2] * B[1][i];
    else if (pthread_equal(id, tid[1])) // baris 2 matriks c
        for (int i = 0; i < 6; i++)
            C[1][i] = A[1][0] * B[0][i] + A[1][1] * B[1][i] + A[1][2] * B[1][i];
    else if (pthread_equal(id, tid[2])) // baris 3 matriks c
        for (int i = 0; i < 6; i++)
            C[2][i] = A[2][0] * B[0][i] + A[2][1] * B[1][i] + A[2][2] * B[1][i];
    else if (pthread_equal(id, tid[3])) // baris 4 matriks c
        for (int i = 0; i < 6; i++)
            C[3][i] = A[3][0] * B[0][i] + A[3][1] * B[1][i] + A[3][2] * B[1][i];
}
```
Dikarenakan hasil dari perkalian matriks ini digunakan pada [Soal 2.b.](#soal-2b), maka setelah perkalian sukses tereksekusi hasil nya akan tersimpan pada *shared memory* dengan sebagai berikut :
```c
key_t key = 1234;
    int shmid = shmget(key, sizeof(int) * row * column, IPC_CREAT | 0666);
    matrix = (int *)shmat(shmid, NULL, 0);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            matrix[i * column + j] = C[i][j];
            printf("%d\t", matrix[i * column + j]);
        }
        printf("\n");
    }

    sleep(10);

    shmdt(matrix);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
```
_catatan : Dalam pengembalian nilai dari pemanggilan sistem `shmget(key,sizeof(int) * row * column, IPC_CREAT | 0666)` dibantu oleh fungsi `shmid`. Selanjutnya dalam mendaftarkan segmen terhadap data space dari proses merupakan arti dari kode ` matrix[i * column + j] = C[i][j];`. Lalu pelepasan segmen *shared memory* dari proses oleh fungsi `shmdt(matrix)`. Dan dalam mengetahui informasi yang berkaitan dengan suatu shared memory digunakan fungsi `shmctl(shmid, IPC_RMID, NULL)`._
<br>


## Soal 2.b.
## **Analisa Soal**
Pada poin ini diminta untuk menampilkan dan menggunakan *shared memory* dari *output* program [Soal 2.a.](#soal-2a). Maka matriks tersebut dilakukan kalkulasi terhadap matriks inputan baru dari pengguna . Secara detail perhitungannya adalah setiap sel yang berasal dari Matriks A menjadi angka untuk faktorial, kemudian sel dari Matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil). Hal yang harus diperhatikan : 
- `If a >= b  -> a!/(a-b)!`
- `If b > a -> a!`
- `If 0 -> 0`

**Cara Pengerjaan**
---
Kita perlu mengambil hasil dari perhitungan perkalian matriks dari [Soal 2.a.](#soal-2a) melalui shared variable terlebih dahulu. Dalam menampilkannya matriks tersebut akan tersimpan dalam `matriks[i][j]` agar dapat digunakan untuk mengkalkulasikan penjumlahan dari n - 1 :
```c
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
```
_catatan : Seperti biasa kita definisikan terlebih dahulu dari dimensi matriks dan variable-variable pendukung guna jalannya dari hasil matriks shared memory yang diperoleh dari [Soal 2.a.](#soal-2a)._

<br>

Sedangkan dalam melakukan penjumlahan dari n hingga 1 atau faktorialnya, akan digunakan `thread "factorial"` yang didalamnya terdapat rumus dari `deret geometri`. Variable `n` disini akan menyimpan nilai dari `matriks[i][j]` yang ingin kita jumlahkan dari n hingga 1.
```c
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
```
_catatan : rumus deret geometri menyesuaikan dengan instruksi dari soal_

<br>


## Soal 2.c.
## **Analisa Soal**
Pada soal ini kita diminta untuk membuat program yang berfungsi untuk melakukan validasi terhadap 5 proses teratas dalam mengetahui *resource* mana saja yang memakan pada komputernya dengan command `"ps aux | sort -nrk 3,3 | head -5"` yang mana hal ini dilakukan dengan `IPC Pipes`.

**Cara Pengerjaan**
---
Disini kami mendeklarasikan dan menggunakan 2 `pipe()` dengan fork. Pembuatan `pipe` dibantu oleh fungsi `pipe()`. Apabila fork berhasil dilakukan dan *child process* berhasil dibuat `pid1 == 0`, maka pipe pertama `pipe1[1]` akan terduplikasi menjadi file deskriptor dari stdout, lalu pipe pertama tersebut `pipe1` akan ditutup dan program akan melakukan eksekusi command `ps aux`
```c
if (pid1 == 0)
    {
        pid2 = fork();
        if (pid2 == 0)
        {
            close(pipe1[0]);
            close(pipe2[1]);
            close(pipe2[0]);
            printf("1");
            dup2(pipe1[1], STDOUT_FILENO);

            close(pipe1[1]);
            char *argv[] = {"ps", "aux", NULL};
            execv("/usr/bin/ps", argv);
        }
```

<br>

Selanjutnya *child process*nya akan tereksekusi terlebih dahulu dengan  `pipe` yang kedua `pipe2`, menduplikasi `pipe` pertama `pipe1` menjadi file deskriptor `stdin`, menduplikasi pipe kedua `pipe2[1]` menjadi file deskriptor dari `stdout`, dan menutup `pipe` pertama `pipe1`. *Child process* disini akan mengeksekusi command `sort -nrk 3,3`.
```c
 else
        {
            printf("2");
            close(pipe1[1]);
            close(pipe2[0]);
            dup2(pipe1[0], STDIN_FILENO);
            dup2(pipe2[1], STDOUT_FILENO);
            close(pipe2[1]);
            close(pipe1[0]);

            char *argv[] = {"sort", "-nrk", "3,3", NULL};
            execv("/usr/bin/sort", argv);
        }
    }
```

<br>

Lalu *Parent Process* `pid1` kembali melakukan eksekusi dengan `pip` yang kedua `pipe2` menduplikasi `pipe` dan terduplikasi menjadi file deskriptor dari stdout, lalu `pipe` tersebut akan ditutup dan program akan melakukan eksekusi command `head -5`.
```c
else
    {
        printf("3");
        close(pipe1[1]);
        close(pipe1[0]);
        dup2(pipe2[0], STDIN_FILENO);
        // dup2(pipe2[1], STDOUT_FILENO);

        close(pipe2[0]);
        close(pipe2[1]);
        // close(pipe2[1]);
        char *argv[] = {"head", "-5", NULL};
        execv("/usr/bin/head", argv);
    }
```


**Kendala**
---
Pada awalnya dari kelompok kami ...
<br>

**Screenshoot Hasil Run Program Soal2.a [soal2](./soal2/soal2a.c)**
![hasil](./img/soal2/....png)
---

**Screenshoot Hasil Run Program Soal2.b [soal2](./soal2/soal2b.c)**
![hasil](./img/soal2/....png)
---

**Screenshoot Hasil Run Program Soal2.c [soal2](./soal2/soal2c.c)**
![hasil](./img/soal2/....png)
---


<br>
<br>

## Soal 3
---

Source Code tersedia pada : [soal3.c](./soal3/soal3.c)

## **Analisa Soal**
Secara umum, kami menangkap bahwa program yang harus dibuat merupakan sebuah thread yang mana program tersebut dapat memindahkan file-file atau bahkan folder-folder. Khususnya program ini memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang hasilnya terdapat di working direktory ketika programn kategori tersebut dijalankan. Detil dari program yang diminta sebagai berikut :
1. Program dapat menerima opsi `-f` yaitu dapat memindah sebuah file dari direktori lain dengan lengkap dengan bisa lebih dari satu file kemudian dijadikan kelompok folder sesuai ekstensinya dan di masukan ke dalam folder tersebut dari file-file yang ekstensinya sesuai. Adapun nilai balikan atau *return* yang diminta berupa pesan jika berhasil maka `File 1 : Berhasil Dikategorikan` dan apabila tidak berhasil maka `File 2 : Sad, gagal :(`. <br>`Contoh run : $./soal3 -f /home/mikehotel/Downloads/anime.jpg /home/mikehotel/Downloads/dump.txt`
2. Program juga dapat menerima opsi `-d` untuk melakukan pengkategorian pada suatu directory yang dituju. Namun pada opsi ini, *user* dibatasi satu input direktori saja, tidak seperti file yang bebas menginput file sebanyak mungkin. Adapun nilai balikan atau *return* yang diminta berupa pesan jika berhasil maka `"Direktori sukses disimpan!"` dan apabila tidak berhasil maka `"Yah, gagal disimpan :("` <br>`Contoh run : $./soal3 -d /home/mikehotel/Downloads`
3. Selain menerima opsi-opsi `-f` dan `-d`, program juga menerima opsi `*`. Pada opsi ini akan mengkategorikan seluruh file yang ada di *working directory* ketika program dijalankan. <br>`Contoh run : $./soal3 \*`
4. Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, maka file tersimpan pada folder `"Unkwnown"`. Apabila file hidder, maka masuk folder `"Hidden"`.
5. Setiap satu file yang dikategorikan dijalankan oleh satu thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.
6. Kategori folder tidak dibuat secara manual, harus melalui Program C
7. Program ini tidak case sensitive. Contoh : `JPG` dan `jpg`  adalah sama.
8. Jika ekstensi lebih dari satu (contoh `".tar.gz"`) maka akan masuk ke dalam folder dengan titik terdepan.
9. Dilarang menggunakan `fork`, `exec`, dan `system()`.
10. Pada bagian point nomor 2 dan 3 berlaku rekursif.
<br>

**Cara pengerjaan**
---

Dalam menyelesaikan program yang diminta oleh [soal3](#soal-3), pertama-tama yang diperlukan adalah melakukan *import* library yang digunakan :
```c
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
```
- `<sys/types.h>` library digunakan untuk identifikasi sebuah thread (e.g `pthread_t id_thread[3]`)
- `<sys/stat.h>` library untuk melakukan pemanggilan fungsi dalam pembuatan sebuah direktori baru (e.g. `mkdir()`)
- `<ctype.h>` library untuk melakukan konversi menjadi *lower case*  (e.g. `_tolower_()`)
- `<dirent.h>` library untuk membuka, membaca, dan menutup dari sebuah direktori (e.g. `opendir(), closedir(), readdir()`)
- `<errno.h>` library untuk memberikan tambahan error pada sistem yang sesuai dengan IEEE Std 1003.1-2001  (e.g. `ECHILD`)
- `<math.h>` library untuk mendefinisikan ragam fungsi dari matematika dalam satu makro (e.g. `HUGE_VAL`)
- `<pthread.h>` library untuk mendapatkan ID dari pemanggilan sebuah thread(e.g. `pthread_self()`)
- `<semaphore.h>` library untuk melakukan operasi *semaphore* (e.g. `sem_t()`)
- `<stdio.h>` library untuk fungsi input-output (e.g. `printf(), sprintf()`)
- `<stdlib.h>` library untuk fungsi umum (e.g. `exit(), atoi()`)
- `<string.h>` library untuk melakukan manipulasi *arrays of character* (e.g. `*memset()`)
- `<syslog.h>` library untuk mengirimkan pesan ke *system logger* (e.g. `openlog()`)
- `<time.h>` library untuk melakukan manipulasi date dan time (e.g. `time(), strftime()`)
- `<unistd.h>` library untuk mendapatkan lokasi direktori saat bekerja dimana (e.g. `getcwd()`)

<br>

Selanjutnya, pada awal `main function` program ini membutuhkan beberapa argumen yang bergantung pada mode apa dijalankannya. Jika dijalankan pada mode `-f` maka argumen yang dibutuhkan dalam penginputan `path file` dapat lebih dari satu. Kemudian jika dijalankan pada mode `-d` maka argumen yang dibutukan dalam penginputan `path file` hanya satu direktori. Lalu apabila dijalankan pada mode `*` tidak membutuhkan argumen lain selain itu.
<br>


## Soal 3.a.
## **Analisa Soal**
Pada soal 3.a kami diminta untuk membuat program dapat menerima inputan mode `-f` yang mana pengguna dapat menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. Dengan output jika berhasil `File 1 : Berhasil Dikategorikan` sedangkan jika gagal maka `File 2 : Sad, gagal :(`.

**Cara Pengerjaan**
---
Dalam menjawab soal 3.a. kami membuat inisialisasi variable, tipe data, dan melakukan memset pada variabel yang diperlukan dalam proses ini. Pertama-tama yang dilakukan adalah mengecek apakah argument yang diinputkan oleh pengguna merupakan `-f`. Selanjutnya dilakukan perulangan sebanyak dari argumen inputan file yang akan dikategorikan *user*. Lalu akan dilakukan pengkategorian dengan `thread` pada setiap argumen inputan file dan diteruskan ke dalam fungsi `eksekusi`.
```cpp
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

```
_catatan : Dalam melakukan pengkategorian argumen file inputan user dilakukan dengan thread secara paralel yang mana juga dibantu oleh fungsi `pthread_create` dalam inisialisasi eksekusi proses argumen file yang pertama kemudian dilakukan semacam proses tunggu agar thread yang lebih dulu selesai terlebih dahulu dengan bantuan fungsi `pthread_join`. Setelah dapat terjalankan thread sebanyak argumen inputan file user maka dilakukan *sleep* program selama 10 detik_

<br>

Selanjutnya pada fungsi eksekusi setelah diberi argumen inputan file user yang pertama hingga ke - N akan melakukan pemanggilan id thread dan melakukan pemotongan dari path yang telah diberikan pada argumen user tadi. Pemotongan ini bertujuan untuk mendapatkan nama file dan ekstensi file tersebut. Setelah mendapatkannya ekstensi file nantinya akan dijadikan sebagai nama folder baru terhadap *working directory* dengan pemindahan nama file yang telah didapatkan tadi.
```cpp
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
    char ekstensiFile[100];

    if (strncmp(arr3, ".", 1) == 0)
    {
        strcpy(ekstensiFile, "hidden");
        n = 2;
    }
    else
    {
        potongtitik = strtok(arr2[m - 1], ".");
        while (potongtitik != NULL)
        {
            arr[n] = potongtitik;
            n++;
            potongtitik = strtok(NULL, ".");
        }
        if (n > 2)
        {
            sprintf(ekstensiFile, "%s.%s", arr[n - 2], arr[n - 1]);
        }
        else
        {
            strcpy(ekstensiFile, arr[n - 1]);
        }

        for (int i = 0; ekstensiFile[i]; i++)
        {
            ekstensiFile[i] = tolower(ekstensiFile[i]);
        }
    }
```
_catatan : Dalam melakukan pemotongan dari path file yang telah diinputkan oleh user, maka dibantu oleh fungsi `strtok()` dengan pemisahan batas 2 hal yaitu pertama berdasarkan slash `"/"` dan yang kedua berdasarkan titik `"."` jika berada pada posisi akhir dari path suatu argumen path inputan file. Selanjutnya pemisahan berdasarkan titik akan disimpan kedalam sebuah variable array `ekstensiFile` guna nantinya dalam pembuatan direktori yang akan dikelompokan berdasarkan file dengan ekstensi yang sesuai dan dilakukan perubahan menjadi semuanya `lower case`, jika sebuah file dalam namanya terdapat lebih dari satu titik maka yang dijadikan folder adalah 2 kata sebelum dan sesudah titik terakhir dengan pengecekan berapakali strtok dipanggil. Untuk nama file disimpan dalam variable array `arr[n]` guna memasukan ke dalam folder yang sesuai juga nantinya._

<br>

Kemudian setelah didapatkan nama-nama folder yang telah dipotong-potong, maka hal selanjutnya adalah melakukan eksekusi untuk pembuatan dari folder berdasarkan nama-nama ekstensi file inputan pengguna. Pertama-tama yang dilakukan adalah melakukan pengecekan dari nama folder tersebut apakah ada atau tidak, dengan bantuan dari variable available yang secara default di set dengan nilai `0` maka hal ini berguna untuk pembuatan folder baru jika belum ada terhadap suatu ekstensi. Selanjutnya jika terdapat folder telah dibaut maka available akan diisi dengan nilai `1` dan tidak dibuatkan sebuah folder baru. Apabila suatu nama file tidak jelas atau tidak diketahui nama dari ekstensi filenya, maka akan secara default ditempatkan pada folder `Unknown`.
```cpp
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

```
_catatan : Pembuatan folder dan pemindahan nya ini dibantu oleh library `dirent`. Selain itu program nantinya akan membaca dari nama ekstensi file tersebut apakah tersedia atau tidak foldernya dalam *working directory* saat ini yang mana hal ini dibantu oleh fungsi `readdir()`. Selanjutnya jika ditemukan tersedia yang mana dalam hal ini juga dibantu komparasi oleh fungsi `strcmp()` maka status berubah menjadi 1. Apabila status masih 0 yang berarti belum ada folder yang tersedia dengan nama ekstensi tersebut, maka akan dibuatkan direktori baru dengan bantuan fungsi `mkdir()`. Begitupun halnya pada folder `Unknown`_

<br>

Lalu setelah folder dengan masing-masing ekstensi dari nama file dibuat, maka hal selanjutnya ialah memasukan file-file tersebut sesuai dengan ekstensinya.
```cpp
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
```
_catatan : Pemindahan file-file terhadap folder yang sesuai nama ekstensinya dibantu oleh fungsi `strcat()` dan melakukan pemindahan suatu file dari direktori lain ke *working directory* saat ini dengan bantuan fungsi `rename()` jika rename mengembalikan 0 value maka rename sukses dan emngganti status menjadi 1_

<br>

## Soal 3.b.
## **Analisa Soal**
Pada soal ini kita akan memindahkan semua file yang terdapat dalam 1 directory. Prosesnya tidak terlalu banyak perbedaan dari soal 3.a dimana kita hanya perlu menghandle list file dalam directory sebelum menjalankan fungsi eksekusi.
**Cara Pengerjaan**
---
Guna menyelesaikan permasalahan b, kami mengikuti arahan dari Tips dengan menggunakan fungsi readdir dalam listing file yang terdapat dalam directory tersebut. Dimana kami akan mendefinisikan terlebih dahulu dengan typedata DIR, dilanjutkan dengan menggunakan fungsi `opendir()` untuk membantu proses listing file didalam directory tersebut. Program akan mengecek apakah  Setiap file yang terdapat dalam direktori tersebut diproses dalam fungsi yang sama dengan poin sebelumnya. Pada poin ini program diminta harus rekursif dimana program tersebut bisa membaca file yang terdapat dalam sebuah directory. Disini kami akan melakukan pengecekan jika d_type nya bernilai 4 maka dia adalah directory yang selanjutnya akan coba dibaca list file didalamnya untuk dijalankan pada fungsi eksekusi. Pada fungsi eksekusi thread terdapat variabel global status yang ditujukan untuk mengetahui apakah proses rename atau pemindahan file berhasil atau tidak, jika status tetap 0 maka gagal untuk dipindahkan sedangkan 1 berhasil dipindahkan.

```cpp
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

            pthread_create(&(id_thread[i]), NULL, eksekusi, tempata); 
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
```
<br>

## Soal 3.c.
## **Analisa Soal**
Pada soal ini program akan dituntut untuk memindahkan semua file yang terdapat dalam listing directorynya. Soal ini tidak jauh berbeda dengan poin 3.b dimana pada soal ini kita akan bekerja dengan current working directory dimana program dijalankan.

**Cara Pengerjaan**
---
Dalam menjawab soal 1.c. kami menyelesaikannya dengan bantuan dari fungsi `getcwd()` dimana fungsi ini berguna untuk mengembalikan string path directory dimana sekarang program sedang dijalankan. Seperti biasa program akan melakukan listing file yang terdapat dalam curent work directory, dengan bantuan `readdir()`, jika d_type bernilai 4 yang menandakan dia adalah directory maka akan dilakukan rekursi untuk mendapat file file yang terdapat dalam sub-directorynya. Setiap file akan dimasukan dalam variabel tempata dan akan dikirimkan ke fungsi ekseksusi dengan bantuan thread untuk proses foldering.

```cpp
if (getcwd(gantiDir, sizeof(gantiDir)) != NULL)
    {
        printf("Current working dir: %s\n", gantiDir);
    }

....
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
```
<br>

## Soal 3.d.

## **Analisa Soal**
Pada soal ini kita diminta untuk menghandle beberapa tipe file yang tidak lumrah seperti file yang tidak memiliki ekstensi, file yang tersembunyi. Tujuan pada poin ini adalah terdapat folder Unknown untuk file yang tidak memiliki ekstensi dan folder hidden untuk file yang tersembunyi.

**Cara Pengerjaan**
---
Untuk folder unknown terjadi jika sebuah file tidak terdapat string `.` didalamnya, maka pada proses slicing titik hanya terjadi satu kali dan mengakibatkan nilai n menjadi 1. Variabel `arr2[m-1]` menyimpan nama file secara utuh tanpa pathnya, dari nama file ini dilakukan slicing dengan titik, jumlah slicing bisa dihitung dari banyaknya nilai pada variabel n dimana variabel n dinisiasi dengan 0 dan setidaknya proses pemotongan titik ini terjadi satu kali. Jika n itu tetap bernilai 1 maka program akan membuat directory Unknown dan akan mengeset target yang akan dipindahkan ke path folder Unknown tadi.
```cpp
void *eksekusi(void *arg)
....
    int n=0;
    potongtitik = strtok(arr2[m - 1], ".");
    while (potongtitik != NULL)
    {
        arr[n] = potongtitik;
        n++;
        potongtitik = strtok(NULL, ".");
    }
....
    if (n > 1)
    {
        ....
        proses pembuatan directory sesuai ekstensi
        ....
    }
    else
    {
        sprintf(tempat2, "%s/Unknown", gantiDir);
        mkdir(tempat2, 0777);
    }

    char source[1024], target[1024];
    sprintf(source, "%s", arg);

    if (n == 1)
    {
        sprintf(target, "%s/Unknown", gantiDir);
    }
....
```
_Code file tanpa ekstensi_

Dalam file hidden, kami mengetahui bahwa file hidden char pertamanya pastilah `.` . Jadi pada variabel `arr3` yang akan menyimpan nama file secara utuh tanpa path-nya akan dilakukan pengecekan apakah char pertamanya `.` atau bukan, jika iya akan diset variabel `ekstensiFile` yang akan menjadi menjadi folder tujuan pemindahan file sebagai hidden, dan mengeset nilai n sebagai 2 karena pada unknown file handling sebelumnya jika n bernilai 1 maka file akan diarahkan ke folder Unknown.

```cpp
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
    char ekstensiFile[100];

    if (strncmp(arr3, ".", 1) == 0)
    {
        strcpy(ekstensiFile, "hidden");
        n = 2;
    }
```
_Code handling hidden file_
<br>

## Soal 3.e.

## **Analisa Soal**
Untuk menyelesaikan soal pada poin 3.e dimana program dituntut untuk menjalankan setiap file yang dihandle dalam fungsi thread supaya file bisa diproses secara bersamaan dan tidak memakan banyak waktu. 

**Cara Pengerjaan**
---
Berikut adalah contoh pemanggilan fungsi thread pada fungsi tag `-f`. Dimana fungsi eksekusi akan dipanggil dengan `pthread_create((&(id_thread[i]), NULL, eksekusi, argv[j])` dimana variabel id_thread[] akan digunakan sebagai id dari thread yang berjalan, `ekseksui` adalah nama fungsi yang akan dipanggil dan `argv[j]` digunakan sebagai argument yang dikirimkan dalam fungsi eksekusi yaitu argument ketika menjalankan fitur tag `-f` yaitu path file yang akan dipindahkan.

```cpp
int i = 0, j = 0;
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
```
_Code thread pada tag `-f`_

Penggunaan thread pada fungsi tag `*` dimana akan sangat mirip dalam pemanggilan dalam tag `-f` namun yang dijadikan sebagai argument ke fungsi eksekusi adalah path ke tiap file yang terdapat dalam directory program dijalankan.

```cpp
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
```
_Code thread pada tag `*`_
<br>


**Cara Pengerjaan**
---
Dalam menjawab soal 3 kami ...
<br>


**Kendala**
---
Pada awalnya dari kelompok kami ...
<br>

**Screenshoot Hasil Run Program Soal3 [soal3](./soal3/soal3.c)**
![hasil](./img/soal3/....png)
---