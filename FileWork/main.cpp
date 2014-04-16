#include "filework2.h"


void main()
{
	setlocale(LC_ALL, "Russian");
	string name;
	name="File.db";
	//new
	//FileWork::Create(name);
	//FileWork::Move(name,3, 6);
	//cout << (FileWork::Read(7, 10)) << endl;
	//FileWork::IndexWrite(name,0, 0, 0, 0, 0, 0);
	FileWork::Changename(name,"All works well");
	//FileWork::IndexRead(name);
	//cout << FileWork::IndexSize(name) << endl;
	//FileWork::Write(name, "Note number one", FileWork::IndexSize(name));
	//cout << FileWork::Read(name, FileWork::IndexSize(name));
	system("pause");
}
