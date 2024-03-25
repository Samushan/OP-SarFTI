#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex> 

using namespace std;

struct Contact
{
    string fullName;
    string phoneNumber;
};

class PhoneBook {
private:
    vector<Contact> contacts;
    string filename;

    //Сохранение в файл
    void saveToFile() {
        ofstream file(filename);
        if (file.is_open()) 
        {
            for (const auto& contact : contacts) 
            {
                file << contact.fullName << "," << contact.phoneNumber << endl;
            }
            file.close();
        }
        else 
        {
            cout << "Файл для записи не открыт." << endl;
        }
    }

    // Загрузка данных для книги из файла
    void loadFromFile() 
    {
        ifstream file(filename);
        if (file.is_open()) 
        {
            string line;
            while (getline(file, line)) 
            {
                size_t pos = line.find(',');
                if (pos != string::npos) 
                {
                    string name = line.substr(0, pos);
                    string number = line.substr(pos + 1);
                    contacts.push_back({ name, number });
                }
            }
            file.close();
        }
        else 
        {
            cout << "Файл не найден" << endl;
        }
    }

    // Проверка номера телефона по российским стандартам
    bool isValidRussianPhoneNumber(const string& number) 
    {
        regex pattern("(\\+7|8)\\d{10}"); // Формат: +7XXXXXXXXXX или 8XXXXXXXXXX
        return regex_match(number, pattern);
    }

public:
    PhoneBook(const string& filename) : filename(filename) 
    {
        loadFromFile();
    }

    ~PhoneBook() 
    {
        saveToFile();
    }

    // Проверка имени
    bool isValidName(const string& name) 
    {
        regex pattern("[a-zA-Zа-яА-ЯёЁ' -]+"); // Разрешены буквы, пробелы и апостроф
        return regex_match(name, pattern);
    }

    // Добавление контакта в книгу
    void addContact(const string& fullName, const string& phoneNumber) 
    {
        //проверка на наличие
        auto it = find_if(contacts.begin(), contacts.end(), [&](const Contact& contact) 
            {
            return contact.fullName == fullName;
            });
        if (it != contacts.end()) 
        {
            cout << "Контакт присутствует в книге." << endl;
            return;
        }

        // Проверки имени и телефона
        if (!isValidName(fullName)) 
        {
            cout << "Неверное имя. Используйте только буквы, пробелы и апостроф." << endl;
            return;
        }

        if (!isValidRussianPhoneNumber(phoneNumber)) 
        {
            cout << "Неверный номер телефона. Введите российский номер телефона в формате +7XXXXXXXXXX или 8XXXXXXXXXX." << endl;
            return;
        }

        // Добавление нового контакта
        contacts.push_back({ fullName, phoneNumber });
        cout << "Контакт успешно добавлен." << endl;
    }

    // Удаление контакта 
    void removeContact(const string& fullName) {
        // поиск контакта
        auto it = find_if(contacts.begin(), contacts.end(), [&](const Contact& contact) 
            {
            return contact.fullName == fullName;
            });
        if (it != contacts.end()) 
        {
            contacts.erase(it);
            cout << "Контакт удалён." << endl;
        }
        else 
        {
            cout << "Контакт не найден." << endl;
        }
    }

    // Поиск контакта по полному имени
    void searchContact(const string& fullName) 
    {
        auto it = find_if(contacts.begin(), contacts.end(), [&](const Contact& contact) 
            {
            return contact.fullName == fullName;
            });
        if (it != contacts.end()) 
        {
            cout << "Контакт найден: " << it->fullName << ", " << it->phoneNumber << endl;
        }
        else 
        {
            cout << "Контакт не найден." << endl;
        }
    }
};

int main() 
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Rus");

    PhoneBook phoneBook("kniga.txt");

    // Создание консольного меню
    while (true) 
    {
        cout << "Меню:\n";
        cout << "1. Добавить контакт\n";
        cout << "2. Удалить контакт\n";
        cout << "3. Поиск контакта\n";
        cout << "4. Выйти\n";
        cout << "Введите ваш выбор: ";

        int choice;
        cin >> choice;

        switch (choice) 
        {
        case 1: 
        {
            string fullName, phoneNumber;
            cout << "Введите полное имя: ";
            cin.ignore(); //оичстка буфера
            getline(cin, fullName);
            cout << "Введите номер телефона: ";
            cin >> phoneNumber;
            cin.ignore(); //очистка буфера
            phoneBook.addContact(fullName, phoneNumber);
            break;
        }
        case 2: 
        {
            string fullName;
            cout << "Введите полное имя для удаления: ";
            cin.ignore(); // очистка буфера
            getline(cin, fullName);
            phoneBook.removeContact(fullName);
            break;
        }
        case 3: 
        {
            string fullName;
            cout << "Введите полное имя для поиска: ";
            cin.ignore(); // очистка буфера
            getline(cin, fullName);
            phoneBook.searchContact(fullName);
            break;
        }
        case 4: 
        {
            cout << "Завершение программы." << endl;
            return 0;
        }
        default:
            cout << "Неверный выбор. Выберите другой вариант." << endl;
        }
    }

    return 0;
}