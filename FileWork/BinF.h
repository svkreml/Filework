//Файл в посторении более не учавствует ибо я в нём запутался


#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;
string path;
fstream fs(path, ios::binary | ios::out | ios::in);
namespace BinF
{ 
	//---------------
	void Move(int recordposition, int recordmove)
	{
		int recordsize;
		fs.seekg(recordposition, ios::beg); // указатель ввода на recordposition от начала файла
		fs.seekp(0, ios::end); // указатель вывода на конец файла
		recordsize = fs.tellp();                    // размер записи равен конеч. позиции
		recordsize -= recordposition - 1;            // минус (recordposition - 1). ПОЧЕМУ -1???
		char *record = new char[recordsize];         // выделяется массив размера recordsize
		fs.seekg(recordposition, ios::beg);
		fs.get(record, recordsize);                          // берем кусок, записываем в рекорд
		fs.seekp(recordposition + recordmove, ios::beg);
		fs.write(record, recordsize - 1);
		if (recordmove < 0) {
			HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE) {
				SetFilePointer(hFile, fs.tellp(), NULL, FILE_BEGIN);
				SetEndOfFile(hFile);
				CloseHandle(hFile); //уменьшить размер файла
			}
		}
		delete[]record;
	}
	//---------------
	//---------------Write{
	void Write(int A, int recpos)
	{
		fs.seekp(recpos, ios::beg);
		fs.write((char*)&A, sizeof A);
	}
	void Write( unsigned int A, int recpos)
	{
		fs.seekp(recpos, ios::beg);
		fs.write((char*)&A, sizeof A);
	}
	void Write( short A, int recpos)
	{
		fs.seekp(recpos, ios::beg);
		fs.write((char*)&A, sizeof A);
	}
	void Write(unsigned short A, int recpos)
	{
		fs.seekp(recpos, ios::beg);
		fs.write((char*)&A, sizeof A);
	}
	void Write(char A, int recpos)
	{
		fs.seekp(recpos, ios::beg);
		fs.write((char*)&A, sizeof A);
	}
	void Write(unsigned char A, int recpos)
	{
		fs.seekp(recpos, ios::beg);
		fs.write((char*)&A, sizeof A);
	}
	void Write(string A, int recpos)
	{
		fs.seekp(recpos, ios::beg);
		Write(A.length(), recpos);
		fs.write((char*)&A, A.length());
	}
	//---------------Write}

	//---------------Read{
	int Readi(int recpos)
	{
		int A;
		fs.seekg(recpos, ios::beg);
		fs.read((char*)&A, sizeof A);
		return A;
	}
	short Reads(int recpos)
	{
		short A;
		fs.seekg(recpos, ios::beg);
		fs.read((char*)&A, sizeof A);
		return A;
	}
	char Readc(int recpos)
	{
		char A;
		fs.seekg(recpos, ios::beg);
		fs.read((char*)&A, sizeof A);
		return A;
	}
	string Readstr(int recpos)
	{
		string A;
		unsigned short recordsize = Readi(recpos);
		fs.seekg(recpos, ios::beg);
		fs.read((char*)&A, recordsize);
		return A;
	}
	//---------------Read}

	//--------------Index{
	void IndexCreate(unsigned int mn, 
		unsigned short sizep, unsigned char typep )
	{
		Write(mn,0);
		Write((unsigned int)0,4);//count
		Write((unsigned short)0,8);//sizename
		Write(sizep,10);
		Write(typep,12);
		Write((char)0,13);//reserve
	}
	void Changename(string newname)
	{
		unsigned short oldsize = Reads(8), newsize = newname.length();
		Move(14 + oldsize, newsize - oldsize);
		Write(newname,14);
	}
	//--------------Index}
	void Readall()//string
	{
		int recpos=14+Reads(8);
		for (unsigned int i = 1; i = Readi(4); i++)
		{
			cout <<i<<")   "<< Readstr(recpos)<< endl;
			recpos = fs.tellp();
		}


	}
}