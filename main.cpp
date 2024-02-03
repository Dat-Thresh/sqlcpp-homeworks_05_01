#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#pragma execution_charaster_set( "utf-8" )
using namespace std;

class client_db {
	//� ���� ������ ���������� ������� � �� (�� ���� �������� ����� ������ � ��������� � ���� ������)
	pqxx::connection conn = pqxx::connection("host=localhost"
			" port=5432"
			" dbname=sql_hw_05"
			" user=postgres"
		" password=33995062");


	//client_db() = delete;
	public:
	//����������� ����������� � �� � ������� ����� clients � numbers
	client_db() {
		pqxx::transaction tr(conn);
		//������� ������� clients
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

	//��������� ������� � clients � �����(���� ����) � numbers
	void addClient(string name, string surname, string email, string number="") {
		pqxx::transaction tr(conn);
		//��������� � ������� clients ������� 
		tr.exec("INSERT INTO clients(name, surname, email) "
			"VALUES ('" + tr.esc(name) + "', '" + tr.esc(surname) + "', '" + tr.esc(email) + "')");
		tr.commit();
		int clientId = findClient_id(name,surname,email);
		//���� ��� ������ �����, �� ������ � ������� �������, ���������� � ���� ������������ ������� (�� �� ���������)
		if (number.length() != 0) {
			pqxx::transaction tr(conn);
			tr.exec("INSERT INTO numbers(number, client_id) "
				"VALUES ('" + tr.esc(number) + "', '" + to_string(clientId) + "')");
			tr.commit();
		}
	}

	//������� ������� � ���������� id ������� �� �����,�������, �����
	int findClient_id(string name, string surname, string email) {
		pqxx::work tr(conn);
		int id = tr.query_value<int>("SELECT id FROM clients "
			"WHERE name = '" + name + "' AND surname = '" + surname + "' AND email = '" + email + "';");
		return id;
	}

	//��������� ����� � ������������� �������
	void addClientNumber(string client_id, string number) {
		pqxx::transaction tr(conn);
		tr.exec("INSERT INTO numbers(number, client_id) "
			"VALUES ('" + tr.esc(number) + "', '" + tr.esc(client_id) + "')");
		tr.commit();
	}

	//�������� ������ �� �������
	void updateClient(string client_id, string parameter, string value) {
		pqxx::work tr(conn);
		tr.exec("UPDATE clients SET " + tr.esc(parameter) + "='" + tr.esc(value) + "' WHERE id='" + tr.esc(client_id) + "'");
		tr.commit();
	}

	//������� ������� �������
	void deleteNumberFromClient(string number) {
		pqxx::work tr(conn);
		tr.exec("DELETE FROM numbers WHERE number='" + number + "'");
		tr.commit();
	}

	//������� �������
	void deleteClient(string id) {
		pqxx::work tr(conn);
		tr.exec("DELETE from clients WHERE id='" + id + "'");
		tr.commit();
	}
	//������� ������� �� �������� 
	int findClient_id(string number) {
		pqxx::work tr(conn);
		int id = tr.query_value<int>("SELECT client_id FROM numbers WHERE number='" + tr.esc(number) + "'");
		return id;
	}
};

int main(){
	//setlocale(LC_ALL, "rus");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	try {

		client_db db;
		/*db.addClient("one", "one", "one", "111");
		db.addClient("tow", "tow", "tow", "222");
		db.addClient("three", "three", "three");
		db.addClient("ffff", "fff", "fff", "fff");
		db.addClientNumber("2", "numb2222");
		db.updateClient("2", "name", "TWOOOOO");
		db.deleteNumberFromClient("three");
		db.deleteClient("4");*/

		cout << "IDDDDD: " << db.findClient_id("numb2222") << endl;
		cout << "IDDD by name " << db.findClient_id("one", "one", "one");
	}
	catch (exception& er) {
		cout << er.what();
	}


	return 0;
}
