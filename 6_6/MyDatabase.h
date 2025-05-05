#pragma once
#include <Wt/Dbo/WtSqlTraits.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <iostream>

class Book;
class Publisher;
class Shop;
class Stock;
class Sale;

class Publisher {
public:
	std::string name = "";
	Wt::Dbo::collection< Wt::Dbo::ptr<Book> > book;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, book, Wt::Dbo::ManyToOne, "publisher");
	}
};

class Book {
public:
	std::string title = "";
	Wt::Dbo::ptr<Publisher> publisher;
	Wt::Dbo::collection< Wt::Dbo::ptr<Stock> > stock;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::belongsTo(a, publisher, "publisher");
		Wt::Dbo::hasMany(a, stock, Wt::Dbo::ManyToOne, "book");
	}
};

class Shop {
public:
	std::string name = "";
	Wt::Dbo::collection< Wt::Dbo::ptr<Stock> > stock;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, stock, Wt::Dbo::ManyToOne, "shop");
	}
};

class Stock {
public:
	int count = 0;
	Wt::Dbo::collection< Wt::Dbo::ptr<Sale> > sale;
	Wt::Dbo::ptr<Shop> shop;
	Wt::Dbo::ptr<Book> book;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::hasMany(a, sale, Wt::Dbo::ManyToOne, "stock");
		Wt::Dbo::belongsTo(a, shop, "shop");
		Wt::Dbo::belongsTo(a, book, "book");
	}
};

class Sale {
public:
	float price = 0;
	std::string date_sale;
	int count = 0;
	Wt::Dbo::ptr<Stock> stock;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, date_sale, "date_sale");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::belongsTo(a, stock, "stock");
	}
};
