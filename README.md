# OpenMPI-Dijkstra

Dijkstra algorithm being run in parallel. Dijkstra algorithm used from [GeeksforGeeks Dijkstra algorithm](https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/).

## How to run
1. Change mpi_hostfile according to needs that saves all host address that will be used.
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
Notes:
If you use other IP Addresses, change the content in Makefile to fit current IP Addresses

2. Change the user and address in Makefile
3. Run Makefile (if Makefile doesn't work, try changing the space in front of line to 1 tab)
```sh
$ make
```

4. Run Program

For first time:
```sh
$ chmod +x ./run.sh
```
run the program:
```sh
$ ./run.sh
```

input np (number of processor) and N (number of nodes)

Do this steps if you don't want to use Makefile (as continuation from step 1):

2. Compile dijkstra.c
```sh
$ mpicc src/dijkstra.c -o <executable-name>
```
3. Copy executable to all server

Example: 
```sh
$ scp <executable-name> <user>@<ip-address>:~
```

4. Run the program with command
```sh
$ mpirun -np <process-number> --hostfile mpi_hostfile <executable_name> <N-nodes>
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

Algoritma lebih baik: <br>
Karena setiap proses akan membentuk dan mengisi matrix masing-masing, akan memakan waktu lebih lama dibandingkan melakukan MPI_Bcast matrix master ke proses lainnya. Selain itu, karena pengisian matrix menggunakan random number, jika diterapkan pada bahasa pemrograman lain maka matrix setiap proses akan berbeda. Pada C, matrix dapat sama walaupun prosesnya berbeda karena seed yang sama akan membentuk angka random yang sama.

##### Jumlah Thread
Jumlah thread yang digunakan adalah 6 karena sejumlah banyak VM server ITB sehingga akan memaksimalkan paralelisasi program dan meminimalkan waktu yang dibutuhkan.

##### Pengukuran Kinerja
| Percobaan | Jumlah N | Dijkstra Serial | Dijkstra Parallel |
|----------:|---------:|----------------:|------------------:|
| 1 | 100      | 37253 microseconds    | 33769 microseconds      |
| 2 | 100      | 37221 microseconds    | 38403 microseconds      |
| 3 | 100      | 36836 microseconds    | 39562 microseconds      |
| 1 | 500      | 1054301 microseconds    | 287382 microseconds      |
| 2 | 500      | 1110788 microseconds    | 293217 microseconds      |
| 3 | 500      | 1134733 microseconds    | 300272 microseconds      |
| 1 | 1000     | 8448298 microseconds    | 2527607 microseconds      |
| 2 | 1000     | 8581740 microseconds    | 2083669 microseconds      |
| 3 | 1000     | 8499586 microseconds    | 2055728 microseconds      |
| 1 | 3000     | 247181511 microsecond     | 82786284 microseconds      |

Untuk jumlah N 3000, kami hanya melakukan satu percobaan karena saat dilakukan percobaan berikutnya terjadi error akibat tidak cukup memori.


##### Analisis Perbandingan Kinerja
Berdasarkan perbandingan algoritma dijkstra serial dan paralel untuk N tertentu, didapat rata-rata:
- Untuk N 100, dijkstra paralel lebih lambat sebesar 0.381%
- Untuk N 500, dijkstra paralel lebih cepat sebesar 73.305%
- Untuk N 1000, dijkstra paralel lebih cepat sebesar 73.885%
- Untuk N 3000, dijkstra paralel lebih cepat sebesar 66.508%

Sehingga secara rata-rata, dijkstra paralel lebih cepat dari sekuensial sebesar 53.329%. 

Untuk N 100, terjadi penurunan sebesar 0.381% yang mungkin dikarenakan besar tabel yang diproses masih sebanding dengan algoritma sekuensial sehingga waktunya akan mirip. Untuk N yang lebih besar, terjadi peningkatan waktu yang drastis karena jumlah tabel yang diproses oleh masing-masing VM jauh lebih kecil daripada sekuensial sehingga waktunya menjadi jauh lebih singkat.

## Pembagian Tugas
| NIM      |                      Tugas                      |
|----------|:-----------------------------------------------:|
| 13517059 | Dijkstra, Paralelisasi, Write File, Dokumentasi |
| 13517125 |     Dijkstra, Paralesisasi, Pembangunan graf    |