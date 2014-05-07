#define _CRT_SECURE_NO_WARNINGS // для strlen()

#include <fstream>
#include <string>
#include <windows.h>
#include <iostream>

using namespace std;
void CreateDB()
{
	int flag=48;
	char c[4] = { 0,0,0,0 };
	string name,column,type;
	//ifstream ofs(namedb+".0");
	{
		getline(cin, name, ' ');
		fstream ofs(name + '.' + (char)flag, fstream::out | ios::binary);
		cout << "Файл " << name + '.' + (char)flag << " cоздан" << endl;
		ofs.write(c,4);
		int i;
		for (i = 0; i < 2; i++) //как описать конец строки????????????????????
		{
			flag = flag + 1;
			getline(cin, column, ' ');
			getline(cin, type, ' ');
			fstream ofs1(name + '.' + (char)flag, fstream::out);
			cout << "Файл " << name + '.' + (char)flag << " cоздан" << endl;
			char magic[4] = { column.length() >> 24, column.length() >> 16, column.length() >> 8, column.length() };
			ofs.write(magic, 4);
			ofs<<column;
			ofs<<type;
			ofs1.close();
		}
		ofs.close();
	}
}
void main()
{
	setlocale(LC_ALL, "Russian");
	CreateDB();
	//database param1 1 param2 1 param2 1	
	
	system("pause");
}
