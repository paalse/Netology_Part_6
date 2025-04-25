#pragma once
#include <iostream>
#include <pqxx/pqxx>

class MyDataBase {
private:
	pqxx::connection* db_conn;
public:
	MyDataBase() {
		try
		{
			db_conn = new pqxx::connection (
				"host=localhost "
				"port=5432 "
				"dbname=postgres "
				"user=postgres "
				"password=admin ");
		}
		catch (pqxx::sql_error e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	// Метод, создающий структуру таблиц.
	void createDB() {
		pqxx::work tx(*db_conn);
		
		// Создание таблицы с клиентами
		tx.exec("CREATE TABLE IF NOT EXISTS client ("
			"id serial PRIMARY KEY,"
			"first_name varchar(50) NOT NULL,"
			"second_name varchar(50) NOT NULL,"
			"email varchar(50) NOT NULL)");

		// Создание таблицы с телефонами
		tx.exec("CREATE TABLE IF NOT EXISTS phone("
			"phone varchar(50) PRIMARY KEY,"
			"client_id integer NOT NULL REFERENCES client(id) ON DELETE CASCADE)");

		tx.commit();
	}

	// Метод для удаления таблиц
	void dropDB() {
		pqxx::work tx(*db_conn);
		
		tx.exec("DROP TABLE phone");
		tx.exec("DROP TABLE client");

		tx.commit();
	}

	// Метод, позволяющий добавить нового клиента, возвращает id клиента
	int  addClient(std::string first_name, std::string second_name, std::string email, std::string phone) {
		pqxx::work tx(*db_conn);

		// Получение следующего значения сиквенса
		int id = tx.query_value<int>("SELECT nextval('client_id_seq')");
		// Выполнение вставки клиента
		tx.exec("INSERT INTO client(id, first_name, second_name, email) VALUES(" + std::to_string(id) + ", '" + first_name + "', '" + second_name + "', '" + email + "')");
		//Вставка телефона клиента
		tx.exec("INSERT INTO phone(phone, client_id) values('" + phone + "', " + std::to_string(id) + ")");
		tx.commit();

		return id;
	}

	// Метод, позволяющий добавить телефон для существующего клиента.
	void addClientPhone(int client_id, std::string phone) {
		pqxx::work tx(*db_conn);
		
		tx.exec("INSERT INTO phone(phone, client_id) values('" + phone + "', " + std::to_string(client_id) + ")");
		tx.commit();
	}

	// Метод, позволяющий изменить данные о клиенте.
	void changeClientData(int client_id, std::string first_name, std::string second_name, std::string email) {
		pqxx::work tx(*db_conn);
		
		tx.exec("UPDATE client SET first_name='" + first_name + "', second_name='" + second_name + "', email='" + email + "' WHERE id = '" + std::to_string(client_id) + "'");
		tx.commit();
	}

	// Метод, позволяющий удалить телефон у существующего клиента.
	void deleteClientPhone(int client_id, std::string phone) {
		pqxx::work tx(*db_conn);
		
		tx.exec("DELETE FROM phone p WHERE p.client_id=" + std::to_string(client_id) + " AND p.phone='" + phone + "'");
		tx.commit();
	}

	// Метод, позволяющий удалить существующего клиента.
	void deleteClient(int client_id) {
		pqxx::work tx(*db_conn);
		
		tx.exec("DELETE FROM client c WHERE c.id=" + std::to_string(client_id));
		tx.commit();
	}

	// Метод, позволяющий найти клиента по его данным по имени, фамилии, email или телефону. Возвращает id клиента
	int selectClient(std::string first_name, std::string second_name, std::string email, std::string phone) {
		pqxx::work tx(*db_conn);
		int id = 0;

		if (phone == "") {
			id = tx.query_value<int>("SELECT c.id FROM client c WHERE c.first_name='" + first_name + "' AND c.second_name='" + second_name + "' AND c.email='" + email + "'");
		}
		else {
			id = tx.query_value<int>("SELECT c.id FROM client c	JOIN phone p ON p.client_id = c.id WHERE p.phone = '" + phone + "'");
		}
		return id;
	}
};