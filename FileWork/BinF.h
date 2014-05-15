#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;
string path;
fstream fs(path, ios::binary | ios::out | ios::in);
fstream fs0(path, ios::binary | ios::out | ios::in);
fstream fs1(path, ios::binary | ios::out | ios::in);
namespace BinF
{ 
	//---------------
	void Move(int recordposition, int recordmove)
	{
		int recordsize;
		fs.seekg(recordposition, ios::beg); // ��������� ����� �� recordposition �� ������ �����
		fs.seekp(0, ios::end); // ��������� ������ �� ����� �����
		recordsize = fs.tellp();                    // ������ ������ ����� �����. �������
		recordsize -= recordposition - 1;            // ����� (recordposition - 1). ������ -1???
		char *record = new char[recordsize];         // ���������� ������ ������� recordsize
		fs.seekg(recordposition, ios::beg);
		fs.get(record, recordsize);                          // ����� �����, ���������� � ������
		fs.seekp(recordposition + recordmove, ios::beg);
		fs.write(record, recordsize - 1);
		if (recordmove < 0) {
			HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE) {
				SetFilePointer(hFile, fs.tellp(), NULL, FILE_BEGIN);
				SetEndOfFile(hFile);
				CloseHandle(hFile); //��������� ������ �����
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
		char factor = Readc(12);
		unsigned int count = Readi(4);
		int recpos = 14 + Reads(8);
		if (factor == 3)
		{
			
			for (unsigned int i = 1; i = count ; i++)
			{
				cout << i << ")   " << Readstr(recpos) << endl;
				recpos = fs.tellp();
			}
		}
		else if (factor == 0)
			for (unsigned int i = 1; i = count; i++)
			{
				cout << i << ")   " << Reads(recpos) << endl;
				recpos = fs.tellp();
			}
		else if (factor == 1)
			for (unsigned int i = 1; i = count; i++)
			{
			cout << i << ")   " << Readi(recpos) << endl;
			recpos = fs.tellp();
			}
		// e.t.c. ...
	}

	//-------------------Add{
	void Add(string str)
	{
		unsigned int count = Readi(4);
		Write(count+1,4);
		fs.seekp(0, ios::end);
		Write(str.length(), fs.tellp());
		Write(str,fs.tellp());
	}
	void Add(int str)
	{
		unsigned int count = Readi(4);
		Write(count + 1, 4);
		fs.seekp(0, ios::end);
		Write(str, fs.tellp());
	}
	void Add(short str)
	{
		unsigned int count = Readi(4);
		Write(count + 1, 4);
		fs.seekp(0, ios::end);
		Write(str, fs.tellp());
	}
	void Add(char str)
	{
		unsigned int count = Readi(4);
		Write(count + 1, 4);
		fs.seekp(0, ios::end);
		Write(str, fs.tellp());
	}
	//-------------------Add}
	void Sort(string column, string key)
	{
		unsigned int count = Readi(4);
		int x=0; 
		for (unsigned int i = 1; i = count; i++)
		{
			if (Readstr(14 + Reads(8)+ x)== key) 
			{
				Write(i,4*i);// ������ ������ ������ � �� �� ���� ������ �����..., ����� ������ � ������...
			}
		}
	}
}
