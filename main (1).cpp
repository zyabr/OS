#include <stdio.h>
#include <thread>

// обычное перемножение матриц
void mulMatrix(int matrixA[4][4], int matrixB[4][4], int result[4][4]) {
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }
    
}

// высчитываем по элементно итоговую матрицу при перемножение через поток
int mulMatixElement(int index[], int matrixA[4][4], int matrixB[4][4], int& res ) {
    
    int i = index[0];
    int j = index[1];
    for (int k = 0; k < 4; k++) {
        res += matrixA[i][k] * matrixB[k][j];
    }
    return res;
    
}

int main() {
    
int matrixA[4][4] = { {23, 46, 69, 9}, 
                     {92, 115, 138, 23}, 
                     {138, 161, 184 ,543},
                     {207 ,230 ,253 ,43} };

int matrixB[4][4] = {{277 ,570 ,897 ,65}, 
                    {1195 ,1075 ,194 ,8},
                    {15 ,23 ,22,34},
                    {2675,20,39,228}};
int result[4][4];
    
int resultWithoutThreads[4][4];
    
int index00[] = {0, 0};
int index01[] = {0, 1};
int index02[] = {0, 2};
int index03[] = {0, 3};

int index10[] = {1, 0};
int index11[] = {1, 1};
int index12[] = {1 ,2 };
int index13[]={1,3 };

int index20[] = {2, 0};
int index21[] = {2, 1};
int index22[] = {2 ,2 };
int index23[]=  {2,3 };

int index30[] = {3, 0};
int index31[] = {3, 1};
int index32[] = {3 ,2 };
int index33[]=  {3,3 };

int result00 = 0;
int result01 = 0;
int result02 = 0;
int result03 = 0;

int result10 = 0;
int result11 = 0;
int result12 = 0;
int result13 = 0;

int result20 = 0;
int result21 = 0;
int result22 = 0;
int result23 = 0;

int result30 = 0;
int result31 = 0;
int result32 = 0;
int result33 = 0;
    
    
    clock_t start1 = clock();
    std::thread th1(mulMatixElement, index00, matrixA, matrixB, std::ref(result00));
    th1.join();
    result[0][0] = result00;
    std::thread th2(mulMatixElement, index01, matrixA, matrixB, std::ref(result01));
    th2.join();
    result[0][1] = result01;
    std::thread th3(mulMatixElement, index02, matrixA, matrixB, std::ref(result02));
    th3.join();
    result[0][2] = result02;
    std::thread th4(mulMatixElement, index03, matrixA, matrixB, std::ref(result03));
    th4.join();
    result[0][3] = result03;
    
    std::thread th5(mulMatixElement, index10, matrixA, matrixB, std::ref(result10));
    th5.join();
    result[1][0] = result10;
    std::thread th6(mulMatixElement, index11, matrixA, matrixB, std::ref(result11));
    th6.join();
    result[1][1] = result11;
    std::thread th7(mulMatixElement, index12, matrixA, matrixB, std::ref(result12));
    th7.join();
    result[1][2] = result12;
    std::thread th8(mulMatixElement, index13, matrixA, matrixB, std::ref(result13));
    th8.join();
    result[1][3] = result13;
    
    std::thread th9(mulMatixElement, index20, matrixA, matrixB, std::ref(result20));
    th9.join();
    result[2][0] = result20;
    std::thread th10(mulMatixElement, index21, matrixA, matrixB, std::ref(result21));
    th10.join();
    result[2][1] = result21;
    std::thread th11(mulMatixElement, index22, matrixA, matrixB, std::ref(result22));
    th11.join();
    result[2][2] = result22;
    std::thread th12(mulMatixElement, index23, matrixA, matrixB, std::ref(result23));
    th12.join();
    result[2][3] = result23;
    
    std::thread th13(mulMatixElement, index30, matrixA, matrixB, std::ref(result30));
    th13.join();
    result[3][0] = result30;
    std::thread th14(mulMatixElement, index31, matrixA, matrixB, std::ref(result31));
    th14.join();
    result[3][1] = result31;
    std::thread th15(mulMatixElement, index32, matrixA, matrixB, std::ref(result32));
    th15.join();
    result[3][2] = result32;
    std::thread th16(mulMatixElement, index33, matrixA, matrixB, std::ref(result33));
    th16.join();
    result[3][3] = result33;
    
    
    clock_t end1 = clock();
    
    double seconds1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
    printf("Время выполнения с потоками %f \n \n", seconds1);

    clock_t start2 = clock();
    
    mulMatrix(matrixA, matrixB, resultWithoutThreads);
    
    clock_t end2 = clock();
    
    double seconds2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
    
    printf("Время выполнения без потоков %f \n \n", seconds2);
    
    // Полученные значения матрицы
    printf("Полученные значения матрицы\n \n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }

    return 0;
}