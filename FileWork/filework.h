//Файл в посторении более не учавствует ибо я в нём запутался


#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace FileWork

{
	string name1;
	//char lastcchar[4];
	void Writeinttochar(unsigned short int number, unsigned int recordposition)
	{
		ofstream ofs;
		ofs.open(name1, fstream::in | fstream::binary);
		char magic[2] = { number >> (8), number };
		ofs.seekp(recordposition, ios::beg);
		ofs.write(magic, 2);
	}
	/*
	int chartoint(char magic[4])
		
	{
		int;
		return ((int)((unsigned char)(magic[0]) << 24) + (int)((unsigned char)(magic[1]) << 16) + (int)((unsigned char)(magic[2]) << 8) + (int)((unsigned char)(magic[3])));
	}
	*/

	//Создание файла, если уже существует выводится ошибка
	void Create(string name)
	{
		ifstream ofs(name);
		if (ofs) {
			cout << "Ошибка создания" << endl;
			ofs.close();

		}
		else {
			ofstream ofs(name);
			cout << "Файл " << name << " cоздан" << endl;
		}
	}
	//Объявление файла текущей работы, в дальнейше система выбора файла может быть переделана.
	void Open(string name)
	{
		ifstream ofs(name, ios::in|ios::_Nocreate);
		if (!ofs){ cout << "Ошибка открытия" << endl; }
		else {
			name1 = name;
			cout << "Файл " << name << " Выбран" << endl;
		}
	}
	//Запись в файл вида (текст записи,позиция для записи) -
	void Write(string record, unsigned int recordposition)
	{			
		ofstream ofs;
		ofs.open(name1, fstream::in | fstream::binary);
		ofs.seekp(recordposition, ios::beg);
		ofs << record;
		cout << "Запись" << endl;
	}
	//Запись в файл строк с длинной сообщения (текст записи,позиция для записи)
	void WriteS(string record, int recordposition)
	{
		ofstream ofs;
		ofs.open(name1, fstream::in | fstream::binary);
		ofs.seekp(recordposition, ios::beg);
		Writeinttochar();
		ofs << record;
		cout << "Запись" << endl;
	}
	/*void Write(int record, int recordposition)
	{
		ofstream ofs;
		ofs.open(name1, fstream::in | fstream::binary);
		ofs.seekp(recordposition, ios::beg);
		
		ofs << inttochar(record);
		cout << "Запись" << endl;
	}
	void Write(char record, int recordposition, char count)
	{
		int i;
		ofstream ofs;
		ofs.open(name1, fstream::in | fstream::binary);
		ofs.seekp(recordposition, ios::beg);
		for (i = 0; i <= count; i++)
		{
			char record1[1];
		ofs.write(record1, 1);
	}
		cout << "Запись" << endl;
	}
	*/
	//Чтение из файла вида (позиция для чтения, длина считываемого)
	static string Read(int recordposition, int recordsize)
	{
		char *output = new char[recordsize];
		ifstream ofs;
		ofs.open(name1, fstream::out | fstream::binary);
		ofs.seekg(recordposition, ios::beg);
		ofs.getline(output, recordsize);
		cout << "Чтение" << endl;
		return output;
	}
	
	//Заполнение заголовка(запуск в заполненом файле приведёт к его порче);      magic begins here...
	void IndexWrite(unsigned int mn, unsigned int count, unsigned short sizename,
		unsigned short sizep, unsigned char typep, unsigned char reserve)
	{
		char magic[14] = { mn >> (24), mn >> (16), mn >> (8), mn, count >> (24),
			count >> (16),count >> (8), count ,
			sizename >> (8), sizename, sizep >> (8), sizep, typep, reserve };
		ofstream ofs;
		ofs.open(name1, fstream::in );
		//ofs.seekp(0, ios::beg); //- наверное не нужна
		ofs.write(magic, 14);
		cout << "Создание заголовка" << endl;
	}
	// Чтение из заголовка на экран;    просмотр функции опасен для моска
	void IndexRead()
	{
		char magic[14];
		ifstream ofs;
		ofs.open(name1, fstream::out | fstream::binary);
		//ofs.seekg(0, ios::beg); //- наверное не нужна
		ofs.read(magic, 14);
		cout << "Чтение заголовка:" << endl;
		cout << "Magic number: " <<  ((int)((unsigned char)(magic[0]) << 24) + (int)((unsigned char)(magic[1]) << 16) + (int)((unsigned char)(magic[2]) << 8) + (int)((unsigned char)(magic[3]))) << endl;
		cout << "Количество записей: " <<  ((int)((unsigned char)(magic[4]) << 24) + (int)((unsigned char)(magic[5]) << 16) + (int)((unsigned char)(magic[6]) << 8) + (int)((unsigned char)(magic[7]))) << endl;
		cout << "Размер имени: " << ((int)((unsigned char)(magic[8]) << 8) + (int)((unsigned char)(magic[9]))) << endl;
		cout << "Размер записи: " << ((int)((unsigned char)(magic[10]) << 8) + (int)((unsigned char)(magic[11]))) << endl;
		cout << "Тип записи: " << (int)(magic[12]) << endl;
		cout << "---: " << (int)(magic[13]) << endl;
	}
	//Здесь должно было быть нечто, скорее всего в виде отдельной функции не нужное
	void Reload(int a)
	{
		switch (a){
		case 1:{}
			break;
		case 2:{}
			break;
		case 0:{}
			break;
		}
		cout << "Обновление заголовка" << endl;
	}
	//Изменение имени таблицы
	void Changename(string newname)
	{
		unsigned short oldsize;
		unsigned short newsize;
		char magic[2];
		fstream ofs (name1, fstream::in | fstream::out | fstream::binary);
		ofs.seekg(8, ios::beg);
		ofs.read(magic, 2);
		newsize = newname.size();
		oldsize = ((int)((unsigned char)(magic[0]) << 8) + (int)((unsigned char)(magic[1])));
		Move(14 + oldsize, newsize - oldsize);
		ofs.seekp(14, ios::beg);
		ofs << newname;
		magic[0] = newsize >> 8;
		magic[1] = newsize;
		ofs.seekp(8, ios::beg);
		ofs.write(magic, 2);

	}
	void Move(int recordposition, int recordmove)
	{
		int recordsize;
		fstream ofs;
		ofs.open(name1, fstream::in | fstream::out | fstream::binary);
		ofs.seekg(recordposition, ios::beg);
		ofs.seekp(0, ios::end);
		recordsize = ofs.tellp();
		recordsize -= recordposition - 1;
		char *record = new char[recordsize];
		ofs.seekg(recordposition, ios::beg);
		ofs.get(record, recordsize);
		ofs.seekp(recordposition + recordmove, ios::beg);
		ofs << record;
		if (recordmove < 0) {
			ofs.seekp(-recordmove, ios::end);
			ofs.end;
		}
		cout << "Перемещение" << endl;
	}
}
