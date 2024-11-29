//system("chcp 1251 > nul");
//setlocale(LC_ALL, "Russian");
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Information {
    string carrier;   // ��������
    int volume;       // �����
    string title;     // ��������
    string author;    // �����

    // ��� ��������� �����-������, ��������� ����� ����� ��� ������ � ������
    void writeToFile(ofstream& file) {
        size_t carrier_len = carrier.size();
        size_t title_len = title.size();
        size_t author_len = author.size();

        // ������ ������ � ����
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

// ������� ��� ������ ����������
void printInformation(const vector<Information>& data) {
    for (const auto& info : data) {
        cout << "��������: " << info.carrier << "\n";
        cout << "�����: " << info.volume << "\n";
        cout << "��������: " << info.title << "\n";
        cout << "�����: " << info.author << "\n\n";
    }
}

// ������� ��� �������� ����� � ������ � ���� ������
void createAndWriteFileText(const string& filename, const vector<Information>& data) {
    ofstream file(filename);
    if (!file) {
        cerr << "�� ������� ������� ���� ��� ������.\n";
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

// ������� ��� ������ ������ �� ���������� �����
void displayFileText(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "�� ������� ������� ���� ��� ������.\n";
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

        cout << "��������: " << carrier << "\n"
            << "�����: " << volume << "\n"
            << "��������: " << title << "\n"
            << "�����: " << author << "\n\n";
    }

    file.close();
}

// ������� ��� �������� ������� �������� � �������� �������
void deleteElementFromFileText(const string& filename, int volumeToDelete) {
    ifstream file(filename);
    ofstream tempFile("temp.txt");

    if (!file || !tempFile) {
        cerr << "�� ������� ������� �����.\n";
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
            continue;  // ���������� ��������� �������
        }

        tempFile << carrier << "\n" << volume << "\n" << title << "\n" << author << "\n";
    }

    file.close();
    tempFile.close();

    remove(filename.c_str());
    rename("temp.txt", filename.c_str());
}

// ������� ��� ���������� K ��������� � ����� �����
void addElementsToFileText(const string& filename, const vector<Information>& newData) {
    ofstream file(filename, ios::app);
    if (!file) {
        cerr << "�� ������� ������� ���� ��� ����������.\n";
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

// �������� � ������ � �������� ����
void createAndWriteFileBinary(const string& filename, const vector<Information>& data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "�� ������� ������� �������� ���� ��� ������.\n";
        return;
    }

    for (const auto& info : data) {
        info.writeToFile(file);
    }

    file.close();
}

// ������ ������ �� ��������� �����
void displayFileBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "�� ������� ������� �������� ���� ��� ������.\n";
        return;
    }

    Information info;
    // ������ ������ �� �����
    while (file.peek() != EOF) {
        info.readFromFile(file);  // ����� ������ info �� �������� const
        cout << "��������: " << info.carrier << "\n"
            << "�����: " << info.volume << "\n"
            << "��������: " << info.title << "\n"
            << "�����: " << info.author << "\n\n";
    }

    file.close();
}

void writeToFileBinary(const string& filename) {
    ofstream file(filename, ios::binary | ios::app);  // �������� � ������ ����������
    if (!file) {
        cerr << "�� ������� ������� �������� ���� ��� ������.\n";
        return;
    }

    Information newInfo;
    // ���������� newInfo �������
    newInfo.carrier = "����";
    newInfo.volume = 100;
    newInfo.title = "���������� � �����";
    newInfo.author = "�����";

    // ������ ������ � ����
    newInfo.writeToFile(file);  // ����� ����� info �� �������� const

    file.close();
}

// �������� ������� �������� � �������� �����
void deleteElementFromFileBinary(const string& filename, int volumeToDelete) {
    ifstream file(filename, ios::binary);
    ofstream tempFile("temp.bin", ios::binary);

    if (!file || !tempFile) {
        cerr << "�� ������� ������� �����.\n";
        return;
    }

    Information info;
    bool deleted = false;

    while (file.peek() != EOF) {
        info.readFromFile(file);

        if (!deleted && info.volume == volumeToDelete) {
            deleted = true;
            continue;  // ���������� ��������� �������
        }

        info.writeToFile(tempFile);
    }

    file.close();
    tempFile.close();

    remove(filename.c_str());
    rename("temp.bin", filename.c_str());
}

// ���������� ��������� � �������� ����
void addElementsToFileBinary(const string& filename, const vector<Information>& newData) {
    ofstream file(filename, ios::binary | ios::app);  // �������� ����� ��� ���������� ������
    if (!file) {
        cerr << "�� ������� ������� �������� ���� ��� ����������.\n";
        return;
    }

    // ������ �� ���� ��������� � newData
    for (const auto& info : newData) {
        info.writeToFile(file);  // ����� writeToFile ��� ������� ��������
    }

    file.close();  // �������� �����
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

    // 1. ������� ���� � �������� ������
    createAndWriteFileText(filenameText, data);
    createAndWriteFileBinary(filenameBinary, data);

    // 2. ������� ���������� �����
    cout << "����� �� ���������� �����:\n";
    displayFileText(filenameText);
    cout << "����� �� ��������� �����:\n";
    displayFileBinary(filenameBinary);

    // 3. �������� �������� � �������� �������
    deleteElementFromFileText(filenameText, 700);
    deleteElementFromFileBinary(filenameBinary, 700);

    // 4. ���������� ������
    vector<Information> newData = { {"BluRay", 2500, "SciFi Movie", "Director 4"} };
    addElementsToFileText(filenameText, newData);
    addElementsToFileBinary(filenameBinary, newData);

    // 5. ������� ���������� ����
    cout << "���������� ��������� ����:\n";
    displayFileText(filenameText);
    cout << "���������� �������� ����:\n";
    displayFileBinary(filenameBinary);

    return 0;
}

