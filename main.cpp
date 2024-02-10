#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#pragma execution_charaster_set( "utf-8" )
using namespace std;

class client_db {
	//в поле класса записываем коннект к БД (не смог передать извне класса и запомнить в поле класса)
	pqxx::connection conn = pqxx::connection("host=localhost"
			" port=5432"
			" dbname=sql_hw_05"
			" user=postgres"
			" password=33995062");
	


	//client_db() = delete;
	public:
	//конструктор коннектится к дб и создает столы clients и numbers
	client_db() {
		try {
			pqxx::transaction tr(conn);
			//создаем таблицу clients
			tr.exec("CREATE TABLE IF NOT EXISTS clients("
				" id SERIAL PRIMARY KEY,"
				" name VARCHAR(60) not null,"
				" surname VARCHAR(60) not null,"
				" email VARCHAR(60) UNIQUE"
				" );"
			);
			tr.exec("create table IF NOT EXISTS numbers("
				" id SERIAL PRIMARY KEY,"
				" number VARCHAR(60) UNIQUE,"
				" client_id INTEGER REFERENCES clients (id) ON DELETE CASCADE"
				" );"
			);
			tr.commit();
		}
		catch (pqxx::broken_connection& er) {
			throw runtime_error(er.what());
		}
	}

	//добавляет клиента в clients и номер(если есть) в numbers
	void addClient(string name, string surname, string email, string number="") {
		int check = -999;
		//проверяем указанного клиента на уникальность:
		// проверяем методом findClient_id
		//		а. если возвращает значение, значит такой клиент уже есть => значит меняет переменную check => значит кинем исключение, что такой клиент есть
		//		b. если не возвращает => метод findClient_id кинет исключение и check не изменится=> поймаем его через catch и создадим клиента
		try {
			check = findClient_id(name, surname, email);

		}
		catch (std::invalid_argument&) {

			pqxx::transaction tr(conn);
			//вставляем в таблицу clients клиента 
			tr.exec("INSERT INTO clients(name, surname, email) "
				"VALUES ('" + tr.esc(name) + "', '" + tr.esc(surname) + "', '" + tr.esc(email) + "')");
			tr.commit();
			int clientId = findClient_id(name, surname, email);
			//если был указан номер, то вносим в таблицу намберс, привязывая к айди добавленного клиента (он же последний)
			if (number.length() != 0) {
				pqxx::transaction tr(conn);
				tr.exec("INSERT INTO numbers(number, client_id) "
					"VALUES ('" + tr.esc(number) + "', '" + to_string(clientId) + "')");
				tr.commit();
			}
		}
		if (check != -999) {
			throw std::invalid_argument("This client already exists!");
		}
	}

	//находит клиента и возвращает id клиента по имени,фамилии, емейл
	int findClient_id(string name, string surname, string email) {
		pqxx::work tr(conn);
		int id = -99;
		try {
			id = tr.query_value<int>("SELECT id FROM clients "
				"WHERE name = '" + name + "' AND surname = '" + surname + "' AND email = '" + email + "';");
		}
		catch (...) {
			throw std::invalid_argument("This cliend was not founded by name/surname/email!");
		}
		
	
		return id;
	}

	//добавляет номер к существующему клиенту
	void addClientNumber(string client_id, string number) {
		try {
			pqxx::transaction tr(conn);
			tr.exec("INSERT INTO numbers(number, client_id) "
				"VALUES ('" + tr.esc(number) + "', '" + tr.esc(client_id) + "')");
			tr.commit();
		}
		catch (...) {
			throw std::invalid_argument("This number alreay exists!");
		}
	}

	//апдейтит данные по клиенту
	void updateClient(string client_id, string parameter, string value) {
		try {
			pqxx::work tr(conn);
			tr.exec("UPDATE clients SET " + tr.esc(parameter) + "='" + tr.esc(value) + "' WHERE id='" + tr.esc(client_id) + "'");
			tr.commit();
		}
		catch (...) {
			throw invalid_argument("Impossible to UPDATE!");
		}
	}

	//удаляет телефон клиента
	void deleteNumberFromClient(string number) {
		try {
			pqxx::work tr(conn);
			tr.exec("DELETE FROM numbers WHERE number='" + number + "'");
			tr.commit();
		}
		catch (...) {
			throw invalid_argument("Impossible to delete this number");
		}
	}

	//удаляет клиента
	void deleteClient(string id) {
		try {
			pqxx::work tr(conn);
			tr.exec("DELETE from clients WHERE id='" + id + "'");
			tr.commit();
		}
		catch (...) {
			throw invalid_argument("Impossible to delete by this id");
		}
	}
	//находит клиента по телефону 
	int findClient_id(string number) {
		pqxx::work tr(conn);
		int id = -99;
		try {
			int id = tr.query_value<int>("SELECT client_id FROM numbers WHERE number='" + tr.esc(number) + "'");
		}
		catch (...) {
			throw std::invalid_argument("This client was not founded by this number!");
		}
		return id;
	}
};

int main(){
	setlocale(LC_ALL, "rus");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	try {

		client_db db;
		/*db.addClient("one", "one", "one", "one");
		db.addClient("tow", "tow", "tow", "222");
		db.addClient("three", "three", "three");
		db.addClient("ffff", "fff", "fff", "fff");
		db.addClientNumber("2", "numb2222");
		db.updateClient("2", "name", "TWOOOOO");
		db.deleteNumberFromClient("three");
		db.deleteClient("4");
		db.addClient("fidsfve", "fdfsive", "fisdfve", "fsdf324ive");*/


		//cout << "IDDDDD: " << db.findClient_id("five") << endl;
		cout << "IDDD by name " << db.findClient_id("onesdfs", "one", "one");
	}
	catch (exception& er) {
		cout << er.what();
	}


	return 0;
}
