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

void print_matrix(int **matrix, int Rows, int Cols){
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
}