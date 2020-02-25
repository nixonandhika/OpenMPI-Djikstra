# OpenMPI-Dijkstra

Dijkstra algorithm being run in parallel. Dijkstra algorithm used from [GeeksforGeeks Dijkstra algorithm](https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/).

## How to run
1. Make mpi_hostfile that saves all host address that will be used.
Example inside mpi_hostfile:
```sh
#localhost
<ip-address 1>
<ip-address 2>
<ip-address 3>
<ip-address 4>
<ip-address 5>
<ip-address 6>
```
2. Copy dijkstra.c to your machine
3. Run Makefile
```sh
$ make
```

Do this part if you don't want to use Makefile (as continuation from step 2):
3. Create folder "output" (output file will be saved to output folder)
4. Compile dijkstra.c
```sh
$ mpicc dijkstra.c -o <executable-name>
```
4. Copy executable to all server
Example: 
```sh
$ scp <executable-name> <user>@<ip-address>:~
```

5. Run the program with command
```sh
$ mpirun -np <process-number> --hostfile mpi_hostfile <executable_name>
```

## Laporan
##### Deskripsi Solusi
Agar dijkstra dapat berjalan paralel, adjacency matrix (graf) dari node dipecah sebanyak proses/thread (besar size) yang masing-masing kemudian melakukan dijkstra dengan pecahan matrix-nya. Setelah dijkstra selesai, masing-masing proses/thread kemudian melakukan MPI_Send ke master. Saat master telah selesai melakukan dijkstra pecahan matrix-nya, master akan melalukan MPI_receive yang akan membentuk matrix hasil. Lama proses dijkstra kemudian akan ditampilkan ke layar dan hasil matrix di write ke file result.txt
##### Analisis Solusi
Pada solusi yang diimplementasikan, terdapat beberapa kelebihan dan kekurangannya.
Kelebihan:
1. Cukup sederhana sehingga mudah di-maintain (hanya menggunakan MPI_Send dan MPI_Recv)

Kekurangan:
1. Memakan banyak memori karena setiap proses akan membangkitkan matrix masing-masing
2. Matrix akan berbeda jika di bahasa pemrograman lain

Algoritma lebih baik:
Karena setiap proses akan membentuk dan mengisi matrix masing-masing, akan memakan waktu lebih lama dibandingkan melakukan MPI_Bcast matrix master ke proses lainnya. Selain itu, karena pengisian matrix menggunakan random number, jika diterapkan pada bahasa pemrograman lain maka matrix setiap proses akan berbeda. Pada C, matrix dapat sama walaupun prosesnya berbeda karena seed yang sama akan membentuk angka random yang sama.

##### Jumlah Thread
Jumlah thread yang digunakan adalah 6 karena sejumlah banyak VM server ITB.
##### Pengukuran Kinerja
| Jumlah N | Dijkstra Serial | Dijkstra Parallel |
|----------|-----------------|:-----------------:|
| 100      | 35667 microseconds    | 30914 microseconds      |
| 500      | 1012716 microseconds    | 261089 microseconds      |
| 1000     | 8029138 microseconds    | 1753701 microseconds      |
| 3000     | 247181511 microsecond     | 82786284 microseconds      |
##### Analisis Perbandingan Kinerja
Berdasarkan perbandingan algoritma dijkstra serial dan paralel untuk N tertentu, didapat:
- Untuk N 100, dijkstra paralel lebih cepat sebesar 13.326%
- Untuk N 500, dijkstra paralel lebih cepat sebesar 74.219%
- Untuk N 1000, dijkstra paralel lebih cepat sebesar 78.158%
- Untuk N 3000, dijkstra paralel lebih cepat sebesar 66.508%

Sehingga secara rata-rata, dijkstra paralel lebih cepat dari sekuensial sebesar 58.053%

## Pembagian Tugas
| NIM      |                      Tugas                      |
|----------|:-----------------------------------------------:|
| 13517059 | Dijkstra, Paralelisasi, Write File, Dokumentasi |
| 13517125 |     Dijkstra, Paralesisasi, Pembangunan graf    |