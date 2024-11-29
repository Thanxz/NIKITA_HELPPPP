//system("chcp 1251 > nul");
//setlocale(LC_ALL, "Russian");
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Information {
    string carrier;   // Носитель
    int volume;       // Объем
    string title;     // Название
    string author;    // Автор

    // Для бинарного ввода-вывода, добавляем явный метод для записи и чтения
    void writeToFile(ofstream& file) {
        size_t carrier_len = carrier.size();
        size_t title_len = title.size();
        size_t author_len = author.size();

        // Запись данных в файл
        file.write(reinterpret_cast<char*>(&carrier_len), sizeof(carrier_len));
        file.write(carrier.c_str(), carrier_len);

        file.write(reinterpret_cast<char*>(&title_len), sizeof(title_len));
        file.write(title.c_str(), title_len);

        file.write(reinterpret_cast<char*>(&author_len), sizeof(author_len));
        file.write(author.c_str(), author_len);
    }

    void readFromFile(ifstream& file) {
        size_t carrier_len, title_len, author_len;
        file.read(reinterpret_cast<char*>(&carrier_len), sizeof(carrier_len));
        carrier.resize(carrier_len);
        file.read(&carrier[0], carrier_len);

        file.read(reinterpret_cast<char*>(&volume), sizeof(volume));

        file.read(reinterpret_cast<char*>(&title_len), sizeof(title_len));
        title.resize(title_len);
        file.read(&title[0], title_len);

        file.read(reinterpret_cast<char*>(&author_len), sizeof(author_len));
        author.resize(author_len);
        file.read(&author[0], author_len);
    }
};

// Функция для вывода информации
void printInformation(const vector<Information>& data) {
    for (const auto& info : data) {
        cout << "Носитель: " << info.carrier << "\n";
        cout << "Объем: " << info.volume << "\n";
        cout << "Название: " << info.title << "\n";
        cout << "Автор: " << info.author << "\n\n";
    }
}

// Функция для создания файла и записи в него данных
void createAndWriteFileText(const string& filename, const vector<Information>& data) {
    ofstream file(filename);
    if (!file) {
        cerr << "Не удалось открыть файл для записи.\n";
        return;
    }

    for (const auto& info : data) {
        file << info.carrier << "\n"
            << info.volume << "\n"
            << info.title << "\n"
            << info.author << "\n";
    }

    file.close();
}

// Функция для вывода данных из текстового файла
void displayFileText(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Не удалось открыть файл для чтения.\n";
        return;
    }

    string carrier, title, author;
    int volume;
    while (getline(file, carrier)) {
        file >> volume;
        file.ignore();
        getline(file, title);
        getline(file, author);
        file.ignore();

        cout << "Носитель: " << carrier << "\n"
            << "Объем: " << volume << "\n"
            << "Название: " << title << "\n"
            << "Автор: " << author << "\n\n";
    }

    file.close();
}

// Функция для удаления первого элемента с заданным объемом
void deleteElementFromFileText(const string& filename, int volumeToDelete) {
    ifstream file(filename);
    ofstream tempFile("temp.txt");

    if (!file || !tempFile) {
        cerr << "Не удалось открыть файлы.\n";
        return;
    }

    string carrier, title, author;
    int volume;
    bool deleted = false;

    while (getline(file, carrier)) {
        file >> volume;
        file.ignore();
        getline(file, title);
        getline(file, author);
        file.ignore();

        if (!deleted && volume == volumeToDelete) {
            deleted = true;
            continue;  // Пропускаем удаляемый элемент
        }

        tempFile << carrier << "\n" << volume << "\n" << title << "\n" << author << "\n";
    }

    file.close();
    tempFile.close();

    remove(filename.c_str());
    rename("temp.txt", filename.c_str());
}

// Функция для добавления K элементов в конец файла
void addElementsToFileText(const string& filename, const vector<Information>& newData) {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "Не удалось открыть файл для добавления.\n";
        return;
    }

    for (const auto& info : newData) {
        file << info.carrier << "\n"
            << info.volume << "\n"
            << info.title << "\n"
            << info.author << "\n";
    }

    file.close();
}

// Создание и запись в бинарный файл
void createAndWriteFileBinary(const string& filename, const vector<Information>& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Не удалось открыть бинарный файл для записи.\n";
        return;
    }

    for (const auto& info : data) {
        info.writeToFile(file);
    }

    file.close();
}

// Чтение данных из бинарного файла
void displayFileBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Не удалось открыть бинарный файл для чтения.\n";
        return;
    }

    Information info;
    // Чтение данных из файла
    while (file.peek() != EOF) {
        info.readFromFile(file);  // Здесь объект info не является const
        cout << "Носитель: " << info.carrier << "\n"
            << "Объем: " << info.volume << "\n"
            << "Название: " << info.title << "\n"
            << "Автор: " << info.author << "\n\n";
    }

    file.close();
}

void writeToFileBinary(const string& filename) {
    ofstream file(filename, ios::binary | ios::app);  // Открытие в режиме добавления
    if (!file) {
        cerr << "Не удалось открыть бинарный файл для записи.\n";
        return;
    }

    Information newInfo;
    // Заполнение newInfo данными
    newInfo.carrier = "Диск";
    newInfo.volume = 100;
    newInfo.title = "Информация о диске";
    newInfo.author = "Автор";

    // Запись данных в файл
    newInfo.writeToFile(file);  // Здесь также info не является const

    file.close();
}

// Удаление первого элемента в бинарном файле
void deleteElementFromFileBinary(const string& filename, int volumeToDelete) {
    ifstream file(filename, ios::binary);
    ofstream tempFile("temp.bin", ios::binary);

    if (!file || !tempFile) {
        cerr << "Не удалось открыть файлы.\n";
        return;
    }

    Information info;
    bool deleted = false;

    while (file.peek() != EOF) {
        info.readFromFile(file);

        if (!deleted && info.volume == volumeToDelete) {
            deleted = true;
            continue;  // Пропускаем удаляемый элемент
        }

        info.writeToFile(tempFile);
    }

    file.close();
    tempFile.close();

    remove(filename.c_str());
    rename("temp.bin", filename.c_str());
}

// Добавление элементов в бинарный файл
void addElementsToFileBinary(const string& filename, const vector<Information>& newData) {
    ofstream file(filename, ios::binary | ios::app);  // Открытие файла для добавления данных
    if (!file) {
        cerr << "Не удалось открыть бинарный файл для добавления.\n";
        return;
    }

    // Проход по всем элементам в newData
    for (const auto& info : newData) {
        info.writeToFile(file);  // Вызов writeToFile для каждого элемента
    }

    file.close();  // Закрытие файла
}

int main() {
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");
    vector<Information> data = {
        {"CD", 700, "Music Album", "Artist 1"},
        {"DVD", 4700, "Movie", "Director 2"},
        {"Flash Drive", 32, "Data Storage", "Company 3"}
    };

    string filenameText = "info.txt";
    string filenameBinary = "info.bin";

    // 1. Создать файл и записать данные
    createAndWriteFileText(filenameText, data);
    createAndWriteFileBinary(filenameBinary, data);

    // 2. Вывести содержимое файла
    cout << "Вывод из текстового файла:\n";
    displayFileText(filenameText);
    cout << "Вывод из бинарного файла:\n";
    displayFileBinary(filenameBinary);

    // 3. Удаление элемента с заданным объемом
    deleteElementFromFileText(filenameText, 700);
    deleteElementFromFileBinary(filenameBinary, 700);

    // 4. Добавление данных
    vector<Information> newData = { {"BluRay", 2500, "SciFi Movie", "Director 4"} };
    addElementsToFileText(filenameText, newData);
    addElementsToFileBinary(filenameBinary, newData);

    // 5. Вывести измененный файл
    cout << "Измененный текстовый файл:\n";
    displayFileText(filenameText);
    cout << "Измененный бинарный файл:\n";
    displayFileBinary(filenameBinary);

    return 0;
}

