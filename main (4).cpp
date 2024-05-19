#include <iostream>
#include <fstream>
#include <thread>
#include <semaphore.h>
#include <fcntl.h>
#include <chrono>

using namespace std;

// Флаг, указывающий, произошло ли только что чтение (для логики потока чтения)
bool just_read = false;

// Именованный семафор для межпоточной связи
sem_t* semaphore;

// Функция для записи сообщений в файл в отдельном потоке
void writeThreadF() {
  string message;
  std::ofstream file;

  while (true) {
    // Открыть файл для добавления
    file.open("text.txt", ios::app);

    if (file.is_open()) {
      // Получить ввод от пользователя
      cout << "Введите сообщение для записи: ";
      getline(cin, message);

      // Записать сообщение в файл
      file << message << '\n';
      file.close();

      // Небольшая пауза для имитации работы
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      // Дважды подать сигнал семафору, чтобы уведомить читателя о записи
      sem_post(semaphore);
      sem_post(semaphore);

      // Установить флаг, чтобы указать, что произошло чтение (по сути, запись)
      just_read = true;
    } else {
      cerr << "Ошибка при открытии файла для записи." << endl;
    }
  }
}


void readThreadF() {
  string text;
  std::ifstream file;

  // Открыть файл для чтения
  file.open("text.txt");

  while (true) {
    // Ожидать на семафоре (блокировать до получения двух сигналов)
    sem_wait(semaphore);
    sem_wait(semaphore);

    if (file.is_open()) {
      // Прочитать строку из файла
      getline(file, text);

      // Проверить, произошло ли только что чтение (с помощью флага)
      if (!just_read) {
        // Если чтение не произошло (то есть, не было записи), у читателя нет новых данных
        continue;
      }

      // Распечатать прочитанное сообщение
      cout << "Прочитанное сообщение: " << text << endl;

      // Небольшая пауза для имитации работы
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      // Сбросить флаг, чтобы указать, что чтение завершено
      just_read = false;
    } else {
      cerr << "Ошибка при открытии файла для чтения." << endl;
    }
  }
}

int main() {
  // Создать именованный семафор для синхронизации
  const char* semaphore_name = "/my_named_semaphore";
  semaphore = sem_open(semaphore_name, O_CREAT, 0644, 0);

  if (semaphore == SEM_FAILED) {
    cerr << "Ошибка при создании семафора." << endl;
    return 1;
  }

  // Создать потоки для записи и чтения
  thread write(writeThreadF);
  thread read(readThreadF);

  // Дождаться завершения обоих потоков
  write.join();
  read.join();

  // Уничтожить семафор после завершения работы
  sem_destroy(semaphore);

  return 0;
}