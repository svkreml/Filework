//#include <fstream>
//#include <iostream>
//#include <string>
//#include <windows.h>


//using namespace std;

namespace FileWork
{
	
		
	//Создание нового файла(не срабатывает, если файл существует)(возвращает  0 или 1) как-то странно выходит, переделать...
	bool Create(string name)
	{
		ifstream ofs(name);
		if (ofs) {
			cout << "Ошибка создания" << endl;
			ofs.close();
			return 0;
		}
		else {
			ofstream ofs(name);
			cout << "Файл " << name << " cоздан" << endl;
			ofs.close();
			return 1;
		}
		
	}
	//Перемещение части кода, для изменения, добавления в центр или удаления записей, вида(файл,позиция, сдвиг),(если файл больше ОЗУ, наверное, так делать не стоит.)
	void Move(string name, int recordposition, int recordmove)
	{
		int recordsize;
		fstream ofs (name, fstream::in | fstream::out | fstream::binary);
		ofs.seekg(recordposition, ios::beg); // указатель ввода на recordposition от начала файла
		ofs.seekp(0, ios::end); // указатель вывода на конец файла
		recordsize = ofs.tellp();                    // размер записи равен конеч. позиции
		recordsize -= recordposition - 1;            // минус (recordposition - 1). ПОЧЕМУ -1???
		char *record = new char[recordsize];         // выделяется массив размера recordsize
		ofs.seekg(recordposition, ios::beg);
		ofs.get(record, recordsize);                          // берем кусок, записываем в рекорд
		ofs.seekp(recordposition + recordmove, ios::beg);
		ofs.write(record, recordsize-1);
		if (recordmove < 0) {
			HANDLE hFile = CreateFileA(name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE) {
				SetFilePointer(hFile, ofs.tellp(), NULL, FILE_BEGIN);
				SetEndOfFile(hFile);
				CloseHandle(hFile); //уменьшить размер файла
			}
		}
		cout << "Перемещение" << endl;
		ofs.close();
		delete []record;
	}
	//Чтение заголовка файла
	void IndexRead(string name)
	{
		char magic[14];
		ifstream ofs(name, fstream::out | fstream::binary);
		//ofs.seekg(0, ios::beg); //- наверное не нужна
		ofs.read(magic, 14);
		//cout << "Чтение заголовка:" << endl;
		cout << "Magic number: " << ((int)((unsigned char)(magic[0]) << 24) + (int)((unsigned char)(magic[1]) << 16) + (int)((unsigned char)(magic[2]) << 8) + (int)((unsigned char)(magic[3]))) << endl;
		cout << "Количество записей: " << ((int)((unsigned char)(magic[4]) << 24) + (int)((unsigned char)(magic[5]) << 16) + (int)((unsigned char)(magic[6]) << 8) + (int)((unsigned char)(magic[7]))) << endl;
		cout << "Размер имени: " << ((int)((unsigned char)(magic[8]) << 8) + (int)((unsigned char)(magic[9]))) << endl;
		cout << "Размер записи: " << ((int)((unsigned char)(magic[10]) << 8) + (int)((unsigned char)(magic[11]))) << endl;
		cout << "Тип записи: " << (int)(magic[12]) << endl;
		cout << "---: " << (int)(magic[13]) << endl;
	}
	//Заполнение заголовка(запуск в заполненом файле приведёт к его порче);      magic begins here...
	void IndexWrite(string name, unsigned int mn, unsigned int count, unsigned short sizename,
		unsigned short sizep, unsigned char typep, unsigned char reserve)
	{
		char magic[14] = { mn >> (24), mn >> (16), mn >> (8), mn, count >> (24),
			count >> (16), count >> (8), count,
			sizename >> (8), sizename, sizep >> (8), sizep, typep, reserve };
		ofstream ofs (name, fstream::in);
		//ofs.seekp(0, ios::beg); //- наверное не нужна
		ofs.write(magic, 14);
		cout << "Создание заголовка" << endl;
		IndexRead(name);
		ofs.close();
	}
	// Чтение из заголовка на экран;    просмотр функции опасен для моска (фунуция как таковая не требуется, и находится здесь лишь в целял устрашения)
	
	//считывает int с текущей позиции
	int ReadInt(string name, int recordposition)
	{
		char output[4];
		ifstream ofs (name, fstream::out | fstream::binary);
		ofs.seekg(recordposition, ios::beg);
		ofs.read(output, 4);
		cout << "Чтение " << recordposition << " " << ((int)((unsigned char)(output[0]) << 24) + (int)((unsigned char)(output[1]) << 16) + (int)((unsigned char)(output[2]) << 8) + (int)((unsigned char)(output[3]))) << endl;
		return ((int)((unsigned char)(output[0]) << 24) + (int)((unsigned char)(output[1]) << 16) + (int)((unsigned char)(output[2]) << 8) + (int)((unsigned char)(output[3])));
		ofs.close();
	}
	//считывает short с текущей позиции
	unsigned short ReadSInt(string name, int recordposition)
	{
		char output[2];
		ifstream ofs(name, fstream::out | fstream::binary);
		ofs.seekg(recordposition, ios::beg);
		ofs.read(output, 2);
		cout << "Чтение " << recordposition << " " << ((int)((unsigned char)(output[0]) << 8) + (int)((unsigned char)(output[1]))) << endl;
		return ((int)((unsigned char)(output[0]) << 8) + (int)((unsigned char)(output[1])));
		ofs.close();
	}
	/*string Read(string name, int recordposition)
	{
		unsigned short recordsize = ReadSInt(name, recordposition);
		string  output;
		ifstream ofs(name, fstream::out | fstream::binary);
		ofs.seekg(recordposition+2, ios::beg);
		ofs.read((char *)&output, 15);
		cout << "Чтение " << recordposition << " " << output << endl;
		//cout.write(output, recordsize);
		return output; 
	}
	*/
	string Read(string name, int recordposition)
	{
		unsigned short recordsize = ReadSInt(name, recordposition);
		char *output = new char[recordsize];
		ifstream ofs(name, fstream::out | fstream::binary);
		ofs.seekg(recordposition+2, ios::beg);
		ofs.read(output, 15);
		output[recordsize]=0;
		//cout << "Чтение " << recordposition << " " << output << endl;
		//cout.write(output, recordsize);
		return output; // почему в конце лезет мусор???
		ofs.close();
		delete []output;
	}
	void Write(string name, int number, int recordposition)
	{ 
		ofstream ofs(name, fstream::in | fstream::binary);
		char magic[4] = { number >> 24, number >> 16, number >> 8, number };
		ofs.write(magic, 2);
		cout << "Запись Int " << recordposition << " " << number<<  endl;
		ofs.close();
	}
	void WriteS(string name,short int number, int recordposition)
	{
		ofstream ofs(name, fstream::in | fstream::binary);
		char magic[2] = { number >> 8, number };
		ofs.write(magic, 2);
		cout << "Запись Short Int " << recordposition << " " << number<<  endl;
	ofs.close();
	}
	void Write(string name,char number, int recordposition)
	{
		char c[1] = { number };
		ofstream ofs(name, fstream::in | fstream::binary);
		ofs.write(c, 1);
		cout << "Запись char " << recordposition << " " << number << endl;
	ofs.close();
	}
	void Write(string name, string record, int recordposition)
	{
		ofstream ofs;
		ofs.open(name, fstream::in | fstream::binary);		
		short x;
		x = record.length();
		char magic[2] = { x >> 8 ,x};
		ofs.seekp(recordposition, ios::beg);
		ofs.write(magic,2);
		ofs <<record;
		cout << "Запись" << endl;
		ofs.close();
	}

	/* Что-то происходит здесь не так
	void Changename(string name, string newname)
	{
		unsigned short oldsize, newsize;
		fstream ofs (name, fstream::in | fstream::out | fstream::binary);
		ofs.seekg(8, ios::beg);
		newsize = newname.size();
		oldsize = ReadSInt(name, 8);
		Move(name, 14 + oldsize, newsize - oldsize);
		ofs.seekp(14, ios::beg);
		ofs << newname;
		ofs.seekp(8, ios::beg);
		ofs.write((char *)&newsize, 2); записывает байты не в том порядке возможно это и был моя вина?...
	}
	*/
	void Changename(string name, string newname)
	{
		unsigned short oldsize;
		unsigned short newsize;
		char magic[2];
		fstream ofs(name, fstream::in | fstream::out | fstream::binary);
		ofs.seekg(8, ios::beg);
		ofs.read(magic, 2);
		newsize = newname.size();
		oldsize = ((int)((unsigned char)(magic[0]) << 8) + (int)((unsigned char)(magic[1])));
		Move(name, 14 + oldsize, newsize - oldsize);
		ofs.seekp(14, ios::beg);
		ofs << newname;
		magic[0] = newsize >> 8;
		magic[1] = newsize;
		ofs.seekp(8, ios::beg);
		ofs.write(magic, 2);
		ofs.close();
	}
	unsigned short IndexSize(string name)
	{
		return 14 + ReadSInt(name, 8);
	}
}