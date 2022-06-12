#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
const long row1 = 3, col1 = 2;
const long row2 = 2, col2 = 3;
int* serialmultipication(long M1[row1][col1], long M2[row2][col2]) 
{
    const long s_N1 = row1 * col1;
    const long s_N2 = row2 * col2;
    const long s_N3 = row1 * col2;
    int Mat1[s_N1];
    int Mat2[s_N2];
    int product[s_N3];
    long count1 = 0;
    long count2 = 0;
    long count3 = 0;
  
    // changing matrix1 to a single array following the row major order
    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col1; j++)
        {

            Mat1[count1] = M1[i][j];
            count1++;

        }
    }

    // changing matrix2 to a single array following the column major order
        for (int i = 0; i < col2; i++)
        {
            for (int x = 0; x < row2; x++)
            {
               
                    Mat2[count2] = M2[x][i];
                    count2++;
             
          
            }
        } 
      
            for (int y = 0; y < row1; y++)
            {

                for (int i = 0; i < col2; i++)
                {
                    int sum = 0;
                    for (int j = 0; j < col1; j++)
                    {
                        sum += Mat1[y * col1 + j] * Mat2[i * col1 + j];

                    }
                    if (count3 < s_N3)
                    {
                        product[count3] = sum;
                    }
                    count3++;
                }
               
            }

            return product;

}
int* ParallelMultiplication(long M1[row1][col1], long M2[row2][col2])
{
    const long N1 = row1 * col1; // N for Matrix 1
    const long N2 = row2 * col2; // N for Matrix 2
    const long N3 = row1 * col2; // N for Matrix 3
    int* Mat1{}; // Array to hold matrix 1
    int* Mat2{}; // Array to hold matrix 2
    int product[N3]{};// Array to hold matrix 3
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;

    Mat1 = new int[N1];
    // changing matrix1 to a single array following the row major order
    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col1; j++)
        {

            Mat1[count1] = M1[i][j];
            count1++;

        }
    }
      
    Mat2 = new int[N2];
    // changing matrix2 to a single array following the column major order
    for (int i = 0; i < col2; i++)
    {
        for (int x = 0; x < row2; x++)
        {

            Mat2[count2] = M2[x][i];
            count2++;


        }
    }
  

    int* rbuf;
    rbuf = (int*)malloc(N2 * sizeof(int));


    MPI_Allgather(Mat2, N2, MPI_INT, rbuf, N2, MPI_INT, MPI_COMM_WORLD);

  
        if (rbuf == 0)
        {
            exit(0);
        }
        else
        {
            for (int y = 0; y < row1; y++)
            {

                for (int i = 0; i < col2; i++)
                {
                    int sum = 0;
                    for (int j = 0; j < row2; j++)
                    {
                        sum += Mat1[y * row2 + j] * rbuf[i * row2 + j];

                    }

                    product[count3] = sum;
                    count3++;
                }
            }
          return product;
        
        }
       
}
int main()
{
    int lower = 1, upper = 10;
    int rank, size;
    long matrix1[row1][col1]{}, matrix2[row2][col2]{};
    int n = col1 * row2;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        srand(time(NULL));
        for (int i = 0; i < row1; i++)
        {
            for (int j = 0; j < col1; j++)
            {
                matrix1[i][j] = (rand() % (upper - lower + 1)) + lower;
            }
        }
        for (int i = 0; i < row2; i++)
        {
            for (int j = 0; j < col2; j++)
            {
                matrix2[i][j] = (rand() % (upper - lower + 1)) + lower;
            }
        }
        
     
    }

    if (col1 == row2)
    {
        double serial_time = -MPI_Wtime();
        serialmultipication(matrix1, matrix2);
        serial_time += MPI_Wtime();
        double parallel_time = -MPI_Wtime();
        ParallelMultiplication(matrix1, matrix2);
        parallel_time += MPI_Wtime();
        if (rank == 0)
        {
            printf("time for sequention multiplication = %f", serial_time);
            printf("\ntime for parallel multiplication = %f", parallel_time);
         
        }
    }
    else
        printf("this matices can not be multipied");


    MPI_Finalize();
} 