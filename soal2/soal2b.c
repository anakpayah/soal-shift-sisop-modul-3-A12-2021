#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>

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

void print_matriz(int **matrix, int Rows, int Cols){
    printf("\n");
        for(int i=0; i<Rows; i++){
            for(int j=0; j<Cols; j++)
                printf("%d\t",matrix[i][j]);
            printf("\n");
        }
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
    

    print_matriz(matrix, Rows, Cols);
}