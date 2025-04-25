#include <pqxx/pqxx>
#include <iostream>
#include <Windows.h>
#include "MyDataBase.h"

int main()
{
	SetConsoleOutputCP(CP_UTF8);

	MyDataBase testDB;	// Создание объекта класса
	int id;	// id клиента

	testDB.createDB();	// Создание таблиц в БД

	id = testDB.addClient("Николай", "Сидоров", "sidorov@mail.ru", "+7(923)673-22-33"); // Добавление клиента в БД
	std::cout << "id добавленого 1го добавленного клиента: " << id << std::endl;
	testDB.addClientPhone(id, "+7(953)345-34-34");	// Добавить телефон для существующего клиента
	testDB.changeClientData(id, "Петр", "Мамонов", "mamonov@mail.ru"); // Изменить данные о клиенте.
	testDB.deleteClientPhone(id, "+7(923)673-22-33"); //Удалить телефон у существующего клиента 
	testDB.deleteClient(id); // Удалить существующего клиента

	id = testDB.addClient("Петр", "Иванов", "ivanov@mail.ru", "+7(999)871-35-67"); // Добавление клиента в БД
	std::cout << "id добавленого 2го добавленного  клиента: " << id << std::endl;
	id = testDB.selectClient("", "", "", "+7(999)871-35-67"); // найти клиента по его данным — имени, фамилии, email
	std::cout << "id клиента найденного по номеру: " << id << std::endl;
	id = testDB.selectClient("Петр", "Иванов", "ivanov@mail.ru", ""); // найти клиента по телефону
	std::cout << "id клиента найденного имени, фамилии и номеру: " << id << std::endl;
	
	testDB.dropDB(); // Удаление таблиц БД

	return 0;
}