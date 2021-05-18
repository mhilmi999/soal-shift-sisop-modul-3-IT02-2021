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
```c
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

Untuk selanjutnya diperlukan fitur login dan register. Disini menggunakan session pada variabel otentikasi, dimana jika sudah login maka akan diganti nilai otentikasinya menjadi 1. Jika client memilih 1 maka akan diarahkan dalam fitur login dimana client akan mendapat tampilan untuk memasukan id dan password kemudian server akan me-concat inputan dari client supaya sesuai dengan database akun.txt

```cpp
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

```
<br>

## Soal 1.b.
## **Analisa Soal**

**Cara Pengerjaan**
---
Guna menyelesaikan permasalahan b, kami mengikuti arahan dari Tips dengan menggunakan ....

<br>

## Soal 1.c.
## **Analisa Soal**


**Cara Pengerjaan**
---
Dalam menjawab soal 1.c. kami menyelesaikannya dengan bantuan dari ......
<br>

## Soal 1.d.

## **Analisa Soal**


**Cara Pengerjaan**
---


<br>

## Soal 1.e.

## **Analisa Soal**


**Cara Pengerjaan**
---

<br>

## Soal 1.f.

## **Analisa Soal**


**Cara Pengerjaan**
---

<br>


## Soal 1.g.

## **Analisa Soal**


**Cara Pengerjaan**
---

<br>

## Soal 1.h.

## **Analisa Soal**


**Cara Pengerjaan**
---

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
```c
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
- `<errno.h>` ibrary untuk memberikan tambahan error pada sistem yang sesuai dengan IEEE Std 1003.1-2001  (e.g. `ECHILD`)
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
```c
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
```c
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
```
_catatan : Dalam melakukan pemotongan dari path file yang telah diinputkan oleh user, maka dibantu oleh fungsi `strtok()` dengan pemisahan batas 2 hal yaitu pertama berdasarkan slash `"/"` dan yang kedua berdasarkan titik `"."` jika berada pada posisi akhir dari path suatu argumen path inputan file. Selanjutnya pemisahan berdasarkan titik akan disimpan kedalam sebuah variable array `ekstensiFile` guna nantinya dalam pembuatan direktori yang akan dikelompokan berdasarkan file dengan ekstensi yang sesuai dan dilakukan perubahan menjadi semuanya `lower case`. Untuk nama file disimpan dalam variable array `arr[n]` guna memasukan ke dalam folder yang sesuai juga nantinya._

<br>

Kemudian setelah didapatkan nama-nama folder yang telah dipotong-potong, maka hal selanjutnya adalah melakukan eksekusi untuk pembuatan dari folder berdasarkan nama-nama ekstensi file inputan pengguna. Pertama-tama yang dilakukan adalah melakukan pengecekan dari nama folder tersebut apakah ada atau tidak, dengan bantuan dari variable available yang secara default di set dengan nilai `0` maka hal ini berguna untuk pembuatan folder baru jika belum ada terhadap suatu ekstensi. Selanjutnya jika terdapat folder telah dibaut maka available akan diisi dengan nilai `1` dan tidak dibuatkan sebuah folder baru. Apabila suatu nama file tidak jelas atau tidak diketahui nama dari ekstensi filenya, maka akan secara default ditempatkan pada folder `Unknown`.
```c
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
```c
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
```
_catatan : Pemindahan file-file terhadap folder yang sesuai nama ekstensinya dibantu oleh fungsi `strcat()` dan melakukan pemindahan suatu file dari direktori lain ke *working directory* saat ini dengan bantuan fungsi `rename()` _

<br>

## Soal 3.b.
## **Analisa Soal**

**Cara Pengerjaan**
---
Guna menyelesaikan permasalahan b, kami mengikuti arahan dari Tips dengan menggunakan ....

<br>

## Soal 3.c.
## **Analisa Soal**


**Cara Pengerjaan**
---
Dalam menjawab soal 1.c. kami menyelesaikannya dengan bantuan dari ......
<br>

## Soal 3.d.

## **Analisa Soal**


**Cara Pengerjaan**
---


<br>

## Soal 3.e.

## **Analisa Soal**


**Cara Pengerjaan**
---

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