#define _CRT_SECURE_NO_WARNINGS // для strlen()

#include <fstream>
#include <string>
#include <windows.h>
#include <iostream>

using namespace std;

#include "filework2.h"
#include "mycode.h"

int main()
{
	char* record = "It's kinda funny that i have managed to realize this function";
	setlocale(LC_ALL, "Russian");
	string name;
	name="D:\\File1.db";
	//lololo
	//FileWork::Create(name);
	//FileWork::Move(name,3, 6);
	//cout << (FileWork::Read(7, 10)) << endl;
	//FileWork::IndexWrite(name,0, 0, 0, 0, 0, 0);
	Write::string_wrt(record, name);
	Write::string_read(name);
	//FileWork::Changename(name,"All works well");
	//FileWork::IndexRead(name);
	//cout << FileWork::IndexSize(name) << endl;
	//FileWork::Write(name, "Note number one", FileWork::IndexSize(name));
	//cout << FileWork::Read(name, FileWork::IndexSize(name));
	system("pause");
	return EXIT_SUCCESS;
}
