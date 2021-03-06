/*
Написати програму «Автоматизована інформаційна система залізничного вокзалу». 
Система містить: відомості про  відправлення поїздів. Для кожного поїзду вказують: номер, час відправлення, станція призначення. Забезпечити:
o	Уведення даних в систему(або завантаження з файлу);
o	Доповнення системи новими даними;
o	Вивід інформації про всі поїзди на вибір користувача(за зростанням номерів, за напрямом чи по часу відправлення);
o	Вивід інформації про певний  поїзд(find_if);
o	Вилучення поїзда з системи
Використати вектор(дек), ітератори, алгоритми, лямбда.

*/

#include "stdafx.h"
#include <iostream> 
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>


using namespace std;


class Directory
{
private:

	struct Train
	{
		char num[8];
		char station[40];
		char time[6];

		void print()
		{
			cout << num << "\t" << station << "\t" << time << "\n";
		}
	};

	vector<Train> table;
public:

	// уведення даних в систему
	void add()
	{
		Train t;
		cout << "\tAdd\nNumber of train: ";
		cin.getline(t.num, 8);
		cout << "Station of destination: ";
		cin.getline(t.station, 40);
		cout << "Departure time:\t";
		cin.getline(t.time, 6);

		table.push_back(t); //додаємо у вектор
		save(t); // пишемо у файл
		printPerson(t); //виводимо у консоль

	}

	// вилучення поїзда з системи
	void del()
	{
		// виводимо список
		int i = 0;
		for (Train elem : table)
		{
			cout << "(" << ++i << ") ";
			elem.print();
		}

		// запитуємо який по рахунку елемент видалити
		cout << "Enter which train delete: ";
		cin >> i;

		// видаляємо і перезаписуємо файл без видаленого елемента
		table.erase(table.begin() + i - 1);
		ofstream file("train.bin", ios::binary);
		for (Train elem : table)
		{
			file.write((char*)&elem, sizeof(elem));
		}
	
		file.close();
		printAll();
	}

	// запис у файл
	void save(const Train &trn)
	{
		ofstream file("train.bin", ios::binary | ios::app);
		file.write((char*)&trn, sizeof(trn));
		file.close();
	}

	// завантаження з файлу
	void load()
	{
		ifstream file("train.bin", ios::binary);
		if (!file)
			return;
		Train elem;
		vector<Train> tmp;
		while (file.read((char*)&elem, sizeof(elem)))
		{
			tmp.push_back(elem);
		}

		file.close();
		table = tmp;
		printAll();

	}

	// друк конкретного запису
	void printPerson(Train &t)
	{
		t.print();
	}

	// вивід усіх записів
	void printAll()
	{
		cout << "\t= All schedule =\n";
		for (Train elem : table)
			elem.print();
		cout << "\t= End of schedule =\n\n";
	}


	// вивід інформації про певний  поїзд(find_if);
	void find(const char * station)
	{
		cout << "\tFind by station:\t" << station << endl;
		vector<Train>::iterator it = find_if(table.begin(), table.end(), [station](Train &t) { return (strcmp(station, t.station) == 0); });
		if (it != table.end())
		{
			table[it - table.begin()].print();
		}
		else
			cout << "Train " << station << " not found\n";
		cout << "\t- - - \n";
	}

	// сортуємо за номером потяга
	void sortByNum()
	{
			cout << "\n\tSort by number\n";
			sort(table.begin(), table.end(), [](Train &t1, Train &t2) {return strcmp(t1.num, t2.num) < 0; });
			printAll();
	}

	// сортуємо за станцією прибуття
	void sortByStation()
	{
			cout << "\n\tSort by station\n";
			sort(table.begin(), table.end(), [](Train &t1, Train &t2) {return  strcmp(t1.station, t2.station) < 0; });
			printAll();
	}

	// сортуємо за часом прибуття
	void sortByTime()
	{	
			cout << "\n\tSort by time\n";
			sort(table.begin(), table.end(), [](Train &t1, Train &t2) {return  strcmp(t1.time, t2.time) < 0; });
			printAll();	
	}

	//Вивід інформації про всі поїзди на вибір користувача(за зростанням номерів, за напрямом чи по часу відправлення);
	void sortBy()
	{
		int select;
		cout << "Select type of sort:\n1. Number\n2. Station\n3. Time\n";
		cin >> select;
		switch (select)
		{
			case 1:
				sortByNum();
				break;
			case 2:
				sortByStation();
				break;
			case 3:
				sortByTime();
				break;
			default:
				sortByStation();
				break;
		}
		
	}

};

int main()
{
	Directory d;

	d.load();
	//d.add();
	//d.del();

	// пошук
	d.find("Kovel");
	d.find("Warsaw");

	// сортування
	d.sortByStation();
	d.sortBy();

	cout << endl;
	system("pause");
	return 0;
}
