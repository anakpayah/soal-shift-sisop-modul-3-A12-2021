#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>
#include <pthread.h>
#include <stdlib.h>

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement){
    size_t size = rows * (sizeof(void *) + (cols * sizeElement));
    return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement){
    int i;  
    size_t sizeRow = cols * sizeElement;
    m[0] = m+rows;
    for(i=1; i<rows; i++){      
        m[i] = (m[i-1]+sizeRow);
    }
}

void print_matrix(int **matrix, int Rows, int Cols){
    printf("\n");
        for(int i=0; i<Rows; i++){
            for(int j=0; j<Cols; j++)
                printf("%d\t",matrix[i][j]);
            printf("\n");
        }
}

void *fact(void* arg){
    int *data = (int *)arg;
    int k = 1;
    if(data[0] == 0 || data[1] == 0) k = 0;
    else if(data[0] >= data[1]){
        for(int i=data[0]; i>(data[0]-data[1]); i--) k *= i;
    }
    else if(data[1] > data[0]){
        for(int i=data[0]; i>1; i--) k *= i;
    }
      
    int *p = (int*)malloc(sizeof(int));
         *p = k;

    pthread_exit(p);
}

int main(int argc, char **argv){
    int **matrix;
    int Rows = 4, Cols = 6, shmId;

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
        int *p = (int *)k;
        printf("%d ",*p);
        if ((i + 1) % 6 == 0)
            printf("\n");
    }
    
}