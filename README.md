# soal-shift-sisop-modul-3-A12-2021
 Repo github soal shift modul 3 sisop semester genap tahun 2021

<hr>

## Soal nomor 1


<hr>

## Soal nomor 2
### Penjelasan Soal
Untuk soal 2a, kita perlu mengkalikan dua matriks 4x3 dan 3x6 yang diinput dan print hasilnya. Untuk soal 2b, kita perlu mengambil matriks hasil soal 2a (matriks a) menggunakan shared memory dan melakukan perhitungan dengan matriks input baru. Setiap cel dari matriks a akan diperhitungkan dengan cel pada matriks b menggunakan thread dengan ketentuan sebagai berikut:  
If a >= b  -> a!/(a-b)!  
If b > a -> a!  
If 0 -> 0  
Untuk soal 2c, kita perlu menggunakan pipe untuk mengeksekusi command “ps aux | sort -nrk 3,3 | head -5”.  
### Soal 2a
```c
    key_t key = 1234;
    size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(int));
    shmId = shmget(key, sizeMatrix, IPC_CREAT|0666);    
    matrix = shmat(shmId, NULL, 0); 
    create_index((void*)matrix, Rows, Cols, sizeof(int));

    int x[4][3], y[3][6], z[4][6];
    for(int i=0; i<4; i++){
        for(int j=0; j<3; j++) scanf("%d",&x[i][j]);
    }
    for(int i=0; i<3; i++){
        for(int j=0; j<6; j++) scanf("%d",&y[i][j]);
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++) z[i][j] = x[i][0] * y[0][j] + x[i][1] * y[1][j] + x[i][2] * y[2][j];
    }
    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++) matrix[i][j] = z[i][j];
    }
    print_matrix(matrix, Rows, Cols);
``` 
Untuk melakukan write ke shared memory, kita perlu key dimana disini kita beri key = 1234, shmid yang didapatkan menggunakan shmget, dan kita perlu mengattach data yang ingin diwrite ke shared memory menggunakan shmat. Kita tidak bisa mengattach data array 2 dimensi menggunakan shmat sehingga kita perlu mengattach array 1 dimensi dan array tersebut diubah menjadi array 2 dimensi setelah diattach ke shared memory. Setelah itu kita scan matriks input dan mengalikan matriks tersebut dan menyimpan matriks hasil ke variabel "matrix[][]" yang sudah diattach ke shared memory.  
### Soal 2b
```c
    key_t key = 1234;
    size_t sizeMatrix = sizeof_dm(Rows,Cols,sizeof(int));
    shmId = shmget(key, sizeMatrix, IPC_CREAT|0600);    
    matrix = shmat(shmId, NULL, 0); 
    create_index((void*)matrix, Rows, Cols, sizeof(int));
    
    int b[4][6];
    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++) scanf("%d",&b[i][j]);
    }

    pthread_t *threads;
    threads = (pthread_t*)malloc(24*sizeof(pthread_t));
      
    int count = 0;
    int* data = NULL;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 6; j++){
            data = (int *)malloc((2)*sizeof(int));
            data[0] = matrix[i][j];
            data[1] = b[i][j];
               
            pthread_create(&threads[count++], NULL, fact, (void*)(data));
        }
    }
    for (int i = 0; i < 24; i++) 
    {
        void *k;
        pthread_join(threads[i], &k);
        unsigned long long *p = (unsigned long long *)k;
        printf("%llu ",*p);
        if ((i + 1) % 6 == 0)
            printf("\n");
    }
```
Proses menggunakan shared memory sama seperti soal 2a. Kita scan input matriks (matriks b) dan membuat thread. Kita membuat array data dimana data[0] menyimpan cel matriks a dan data[1] menyimpan cel matriks b, kemudian kita memggunakan pthread_create untuk memanggil fungsi faktorial. Setelah itu kita menggabungkan thread dengan pthread_join dan menampilkan hasilnya. Fungsi faktorial (fact()) adalah sebagai berikut:
```c
void *fact(void* arg){
    int *data = (int *)arg;
    unsigned long long k = 1;
    if(data[0] == 0 || data[1] == 0) k = 0;
    else if(data[0] >= data[1]){
        for(int i=data[0]; i>(data[0]-data[1]); i--) k *= i;
    }
    else if(data[1] > data[0]){
        for(int i=data[0]; i>1; i--) k *= i;
    }
      
    unsigned long long *p = (unsigned long long*)malloc(sizeof(unsigned long long));
         *p = k;

    pthread_exit(p);
}
```
### Kendala
Sebelumnya kita tidak bisa melakukan faktorial untuk angka besar karena menggunakan data type int. Setelah revisi kita mengubah beberapa variabel untuk menggunakan data type unsigned long long supaya bisa melakukan faktorial sampai angka 20.
### Screenshot Output
![image](https://user-images.githubusercontent.com/7587945/119265387-b65d5280-bc10-11eb-8768-2378a82d66c2.png)

### Soal 2c
```c
  if (pipe(fd1) == -1) {
    perror("Pipe Failed");
    exit(1);
  }

  if ((pid = fork()) == -1) {
    perror("Fork Failed");
    exit(1);
  } 
  else if (pid == 0) {
    dup2(fd1[1], 1);
    close(fd1[0]);
    close(fd1[1]);
    execlp("ps", "ps", "aux", NULL);
    _exit(1);
  }

  if (pipe(fd2) == -1) {
    perror("Pipe Failed");
    exit(1);
  }

  if ((pid = fork()) == -1) {
    perror("Fork Failed");
    exit(1);
  } 
  else if (pid == 0) {
    dup2(fd1[0], 0);
    dup2(fd2[1], 1);
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);
    execlp("sort", "sort", "-nrk", "3,3", NULL);
    _exit(1);
  }

  close(fd1[0]);
  close(fd1[1]);

  if ((pid = fork()) == -1) {
    perror("Fork Failed");
    exit(1);
  } 
  else if (pid == 0) {
    dup2(fd2[0], 0);
    close(fd2[0]);
    close(fd2[1]);
    execlp("head", "head", "-5", NULL);
    _exit(1);
  }
```
Di sini terdapat dua pipe, yaitu fd1[2] dan fd2[2]. Alur program ini adalah buat pipe fd1 -> fork -> write ke fd1 -> exec "ps aux" -> buat pipe fd2 -> fork -> read dari fd1 -> write ke fd2 -> exec "sort -nlk" -> fork -> read dari fd2 -> exec "head -5".
### Screenshot Output
![image](https://user-images.githubusercontent.com/7587945/119265407-cecd6d00-bc10-11eb-87cf-1a59e1248059.png)



<hr>

## Soal nomor 3    
### Penjelasan Soal  
Soal berupa dimana Alex ingin kita membuat suatu program C untuk mengkatagorikan folder folder dan file-file yang berantkan, progaram ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya ke dalam folder yang sesuai di working directory ketika program dijalankan.  
program ini diminta untuk 3 opsi yang bisa dilakukan :  
1. -f untuk mengkategorikan file yang diminta memlalui argumen dari program yang dijalankan.
2. -d untuk mengkategorikan dile yang ada di suatu direktory tertentu ke folder program yang dijalankan.  
3. \* untuk mengkategorikan file yang ada di direktori saat ini.  
untuk segala proses yang dilakukan, saya menggunkan thread untuk melakukan proses secara bersamaan dan lebih cepat didalam program saya.  
### opsi 1 argumen -f.  
untuk melakukan argume -f ini, hanya perlu membuka thread untuk tiap file yang ingin dikategorigan dan melakukan thread join agar bisa berjalan bersamaan. dalam soal bagian ini kita hanya membuat thread untuk argumen yang hanya diinputkan. untuk selain yang diinputkan tidak akan terjadi apa-apa.
```c
	for(int i=2; i<argc; i++) {
		err = pthread_create(&tid[i], NULL, move, (void *)argv[i]);
		if(err != 0) printf("File %d : Sad, gagal :(\n", i-1);
        else printf("File %d : Berhasil Dikategorikan\n", i-1);
	}
	for(int j=2; j<argc; j++) pthread_join(tid[j], NULL);
 ```  
dalam potongan kode diatas, program akan menge-printf `sad, gagal :(` jika thread yang dibuat tidak bisa dan sebaliknya lalu me-ngeprint `Berhasil Dikategorikan`.  

### opsi 2 & 3 argumen -d, argumen \*
kedua opsi ini sebenranya mirip bahkan sama, yang membedakan hanya arguen -d diikuti pilhan pilihan tertentu folder mana saja yang akan di sesuaikan, di lain argumen untuk argumen \* untuk memproses segala folder di working directory seluruhnya.
```c
DIR *dir;
	struct dirent *tmp;
	int i=0;
	if(strcmp(argv[1], "-d") == 0) {
    	dir = opendir(argv[2]);
		strcpy(folder, argv[2]);
	}
	else if((argv[1][0]=='*') && (strlen(argv[1])==1)) {
		dir = opendir(cwd);
		strcpy(folder, cwd);
	}```  
 itu adalah potongan kode untuk memproses masing masing argumen.  nanti akan dilanjut kan dengan hal yang sama yait code sebagai berikut.  
 ```c
 while((dir!=NULL) && (tmp=readdir(dir))) {
		if(tmp->d_type==DT_DIR && strcmp(tmp->d_name, ".")!=0 && strcmp(tmp->d_name, "..")!=0){
			strcpy(deepfolder,folder);
			strcat(deepfolder,"/");
			strcat(deepfolder,tmp->d_name);
			strcat(deepfolder,"/");
			dalam();
		}
        if(strcmp(tmp->d_name, ".")==0 || strcmp(tmp->d_name, "..")==0 || strcmp(tmp->d_name, "soal3.c")==0 || strcmp(tmp->d_name, "soal3")==0 || tmp->d_type==DT_DIR) 
		continue;
        err = pthread_create(&tid[i], NULL, move, (void*) tmp->d_name);
        if(err != 0) printf("Yah, gagal disimpan :(\n");
        else printf("Direktori sukses disimpan!\n");
        i++;
    }
    for(int j=0; j<i; j++) pthread_join(tid[j], NULL);
    closedir(dir);
    return;
```  
dalam isi di perulangan `while` untuk memproses penamaan string lalu dibawahnya ada syarat `if` untuk memproses penamaan ekstensi yang sudah ada sebelumnya. kemudian masuk ke thread dan masuk ke move. namun dalam perulangan `while` ada suatu fungsi bernama `dalam()` fungsi tersbut untuk mengatur bilamana ada suatu folder didalamfolder tersebut ada lagi dan seterusnya. untuk fungsi tersebut sebagai berikut.  
```c
void dalam(){
	DIR *dir;
	struct dirent *tmp;
	int i=0;
	memset(deepfolder2, '\0', sizeof(deepfolder2));
	strcpy(deepfolder2,deepfolder);
	printf("%s\n", deepfolder2);
	dir = opendir(deepfolder2);
	
	while((dir!=NULL) && (tmp=readdir(dir))) {
		if(tmp->d_type==DT_DIR && strcmp(tmp->d_name, ".")!=0 && strcmp(tmp->d_name, "..")!=0){
			strcat(deepfolder2,tmp->d_name);
			strcat(deepfolder2,"/");
			dalam();
		}
        if(strcmp(tmp->d_name, ".")==0 || strcmp(tmp->d_name, "..")==0 || strcmp(tmp->d_name, "soal3.c")==0 || strcmp(tmp->d_name, "soal3")==0 || tmp->d_type==DT_DIR) 
		continue;
        err = pthread_create(&tid[i], NULL, move, (void*) tmp->d_name);
        if(err != 0) printf("Yah, gagal disimpan :(\n");
        else printf("Direktori sukses disimpan!\n");
        i++;
    }
    
    for(int j=0; j<i; j++) pthread_join(tid[j], NULL);
    closedir(dir);
    return;
}
```  
### folder "Unknown" dan folder "Hidden"
untuk proses yang satu ini untuk memmbuat suatu folder dimana ekstensinya tidak ada maka masuk ke `unknown` atau folder `hidden` bilamana ekstensinya disembunyikan. proses pemindahan ini dan juga pada seluruh proses dari argumen yang lainnya di letakan pada fungsi kode `move()`. fungsi kode tersebut sebagi berikut.  
```c
void *move(void *arg) {
	char *filepath = (char *)arg;
	char *extension = NULL;
	extension = strchr(filepath, '.');
	char ext[1000];
	memset(ext, '\0', sizeof(ext));
	
	if((extension-filepath-strlen(cwd)+1)==2 || (extension-filepath+1)==1) strcpy(ext, "Hidden");
	else if(extension) {
		extension++;
        for(int i=0; i<strlen(extension); i++) {
               	ext[i] = tolower(extension[i]);
    	}
	}
	else strcpy(ext, "Unknown");
	
    char *filename = NULL;
    filename = strrchr(filepath, '/');
    if(filename) filename++;
	else filename = filepath;

	char folderpath[1000];
	strcpy(folderpath, cwd);
	strcat(folderpath, "/");
	strcat(folderpath, ext);
	mkdir(folderpath, S_IRWXU);
	
	if(strlen(deepfolder2)>1){
		char fullname[1000];

		strcpy(fullname, deepfolder2);
		strcat(fullname, "/");
		strcat(fullname, filename);
		strcat(folderpath, "/");
		strcat(folderpath, filename);
	rename(fullname, folderpath);
	}
	else if(strlen(folder) > 1) {
		char fullname[1000];

		strcpy(fullname, folder);
		strcat(fullname, "/");
		strcat(fullname, filename);
		strcat(folderpath, "/");
		strcat(folderpath, filename);
	rename(fullname, folderpath);
	}
	else {					
	    strcat(folderpath, "/");
        strcat(folderpath, filename);
		rename(filepath, folderpath);
	}
	return NULL;
}
```  
dalam fungsi ini untuk mengubah penamaanfile dan ekstensi yang susai dan masuk pada folder sesaui dengan ekstensinya masing masing. Pemindahan ektensi tersbut menggunakan fungsi `strchr` dengan pemisah "." dan "/". jika foldertidak ada ekstensisanya maka akan dipindahkan pada folder `unknown` dan ke folder `hidden` juga bilamana ekstensi di sembunyikan.  

### Kendala pengerjaan
pengerjaan soal ini dilakukan pada masa timeline revisi sehingga tidak terinput pada submit praktikum yang sebenernya dikarenakan kurangnya waktu dari pihak yang mengerjakan (Afifan S Yahya).  
  
### contoh output untuk argumen \*
![image](https://user-images.githubusercontent.com/75328763/119264534-d0496600-bc0d-11eb-9818-92503345ae46.png)

