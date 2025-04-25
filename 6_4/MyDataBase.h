#pragma once
#include <iostream>
#include <pqxx/pqxx>

class MyDataBase {
private:
	pqxx::connection* db_conn;
public:
	MyDataBase();

	// Метод, создающий структуру таблиц.
	void createDB();

	// Метод для удаления таблиц
	void dropDB();

	// Метод, позволяющий добавить нового клиента, возвращает id клиента
	int  addClient(std::string first_name, std::string second_name, std::string email, std::string phone);

	// Метод, позволяющий добавить телефон для существующего клиента.
	void addClientPhone(int client_id, std::string phone);

	// Метод, позволяющий изменить данные о клиенте.
	void changeClientData(int client_id, std::string first_name, std::string second_name, std::string email);

	// Метод, позволяющий удалить телефон у существующего клиента.
	void deleteClientPhone(int client_id, std::string phone);

	// Метод, позволяющий удалить существующего клиента.
	void deleteClient(int client_id);

	// Метод, позволяющий найти клиента по его данным по имени, фамилии, email или телефону. Возвращает id клиента
	int selectClient(std::string first_name, std::string second_name, std::string email, std::string phone);
};