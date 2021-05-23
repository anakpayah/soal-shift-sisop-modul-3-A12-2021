# soal-shift-sisop-modul-3-A12-2021
 Repo github soal shift modul 3 sisop semester genap tahun 2021

<hr>

## Soal nomor 1


<hr>

## Soal nomor 2


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

