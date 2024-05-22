#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <random>

// Стандартное перемножение матриц
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

// Поэлементное перемножение матриц с использованием потоков
int mulMatrixElementThread(std::pair<int, int> indices, int matrixA[4][4], int matrixB[4][4], int matrixC[4][4]) {
  int row = indices.first;
  int col = indices.second;

  int result = 0;
  for (int k = 0; k < 4; ++k) {
    result += matrixA[row][k] * matrixB[k][col];
  }

  matrixC[row][col] = result;
  return result;
}

int main() {
  // Инициализация генератора случайных чисел
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 100);

  // Генерация случайных матриц A и B
  int matrixA[4][4], matrixB[4][4];
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      matrixA[i][j] = dis(gen);
      matrixB[i][j] = dis(gen);
    }
  }

  // Инициализация результирующих матриц
  int resultWithoutThreads[4][4] = {};
  int resultWithThreads[4][4] = {};

  // Измерение времени выполнения для стандартного перемножения матриц
  clock_t start1 = clock();
  mulMatrix(matrixA, matrixB, resultWithoutThreads);
  clock_t end1 = clock();
  double secondsWithoutThreads = (double)(end1 - start1) / CLOCKS_PER_SEC;

  // Печать матрицы A
  printf("Матрица A:\n");
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      printf("%d ", matrixA[i][j]);
    }
    printf("\n");
  }

  // Печать матрицы B
  printf("\nМатрица B:\n");
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      printf("%d ", matrixB[i][j]);
    }
    printf("\n");
  }

  // Печать результата без потоков
  printf("\nРезультат без потоков:\n");
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      printf("%d ", resultWithoutThreads[i][j]);
    }
    printf("\n");
  }

  // Измерение времени выполнения для поэлементного перемножения матриц с использованием потоков
  std::vector<std::future<int>> results;
  clock_t start2 = clock();

  // Подготовка индексов для каждого элемента
  std::vector<std::pair<int, int>> indices;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      std::pair<int, int> index43;
      index43.first = i;
      index43.second = j;
      indices.push_back(index43);
    }
    }

    // Асинхронные вычисления
    for (auto& index : indices) {
        results.push_back(std::async(mulMatrixElementThread, index, matrixA, matrixB, resultWithThreads));
    }

    // Результаты обработки одновременно
    for (auto& future : results) {
        future.get(); 
    }

    clock_t end2 = clock();
    double secondsWithThreads = (double)(end2 - start2) / CLOCKS_PER_SEC;

    printf("Время выполнения без потоков: %f секунд\n", secondsWithoutThreads);
    printf("Время выполнения с потоками: %f секунд\n", secondsWithThreads);
    printf("\nРезультат без потоков:\n");
    
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%d ", resultWithoutThreads[i][j]);
        }
        printf("\n");
    }
    printf("\nРезультат с потоками:\n");
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%d ", resultWithThreads[i][j]);
        }
        printf("\n");
    }


    return 0;
}
