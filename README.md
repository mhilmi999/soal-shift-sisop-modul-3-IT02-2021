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

## Soal 1

Source Code tersedia pada : [client.c](./soal1/Client/client.c)
Source Code tersedia pada : [server.c](./soal1/Server/server.c)

## **Analisa Soal**

Secara umum, kami menangkap bahwa program yang harus dibuat merupakan sebuah .....

Hal-hal yang perlu diperhatikan diantaranya :


<br>

**Cara pengerjaan**
---


<br>

## Soal 1.a.
## **Analisa Soal**

**Cara Pengerjaan**
---
Dalam menjawab soal 1.a. kami membuat ...
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

Source Code tersedia pada : [soal2a.c](./soal2/soal2a.c)
Source Code tersedia pada : [soal2a.c](./soal2/soal2b.c)
Source Code tersedia pada : [soal2a.c](./soal2/soal2c.c)

## **Analisa Soal**

Dari kami merasa soal ini lebih utamanya ......


<br>

**Cara pengerjaan**
---

<br>

## Soal 2.a.
## **Analisa Soal**
Soal meminta untuk 




**Cara Pengerjaan**
---
Dengan menggunakan ...
<br>


## Soal 2.b.
## **Analisa Soal**
Pada poin ini diminta untuk ...

**Cara Pengerjaan**
---
Kita perlu mengambil ....

<br>


## Soal 2.c.
## **Analisa Soal**
Pada soal ini kita diminta untuk
**Cara Pengerjaan**
---

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