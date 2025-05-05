#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Windows.h>
#include <iostream>
#include "MyDatabase.h"

int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	try {
		std::string connectionString =
			"host=localhost"
			" port=5432"
			" dbname=postgres"
			" user=postgres"
			" password=admin";
		
		auto postgres = std::make_unique<Wt::Dbo::backend::Postgres>(connectionString);
		Wt::Dbo::Session session;
		session.setConnection(std::move(postgres));
		session.mapClass<Publisher>("publisher");
		session.mapClass<Book>("book");
		session.mapClass<Shop>("shop");
		session.mapClass<Stock>("stock");
		session.mapClass<Sale>("sale");

		// Создание таблиц
		session.createTables();
		{
			// Заполнение таблиц
			Wt::Dbo::Transaction transaction{ session };

			// Издательство
			std::unique_ptr<Publisher> publisher{ new Publisher() };
			publisher->name = "Питер";
			Wt::Dbo::ptr<Publisher> publisherPtr = session.add(std::move(publisher));

			// Книга
			std::unique_ptr<Book> book{ new Book() };
			book->title = "Белый клык";
			book->publisher = publisherPtr;
			Wt::Dbo::ptr<Book> bookPtr = session.add(std::move(book));

			// Магазин
			std::unique_ptr<Shop> shop{ new Shop() };
			shop->name = "Книжный мир";
			Wt::Dbo::ptr<Shop> shopPtr = session.add(std::move(shop));

			// Склад
			std::unique_ptr<Stock> stock{ new Stock() };
			stock->book = bookPtr;
			stock->shop = shopPtr;
			stock->count = 10;
			Wt::Dbo::ptr<Stock> stockPtr = session.add(std::move(stock));

			// Продажи
			std::unique_ptr<Sale> sale{ new Sale() };
			sale->price = 599.50;
			sale->date_sale = "";
			sale->stock = stockPtr;
			sale->count = 1;
			Wt::Dbo::ptr<Sale> salePtr = session.add(std::move(sale));

			transaction.commit();
		}

		std::string pub_name = "";

		std::cout << "Введите имя издателя: ";
		std::cin >> pub_name;
		{
			Wt::Dbo::Transaction transaction{ session };

			// Получение издателя
			Wt::Dbo::ptr<Publisher> pub_db = session.find<Publisher>().where("name = ?").bind(pub_name);
			std::cout << pub_db->name  << std::endl;

			// Получение книг издателя
			Wt::Dbo::collection <Wt::Dbo::ptr<Book>> book_db = session.find<Book>().where("publisher_id = ?").bind(pub_db.id());
			for (const Wt::Dbo::ptr<Book>& b : book_db) {
				std::cout << "Book: " << b->title << std::endl;
			}

			// Получение списка магазинов, где продаются книги заданного издателя
			using Result = std::string;
			Wt::Dbo::collection<Result> result = session.query<Result>("SELECT DISTINCT(sh.name) FROM publisher pb").join("book bk ON pb.id = bk.publisher_id").join("stock st ON bk.id = st.book_id").join("shop sh ON st.shop_id = sh.id").where("pb.name = ?").bind(pub_name);

			std::cout << "Магазины: " << std::endl;
			for (const std::string& res : result) {
				std::cout << res << std::endl;
			}
		}
	}
	catch (const Wt::Dbo::Exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}