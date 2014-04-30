//#include <fstream>
//#include <iostream>
//#include <string>
//#include <windows.h>


//using namespace std;

namespace FileWork
{
	
		
	//�������� ������ �����(�� �����������, ���� ���� ����������)(����������  0 ��� 1) ���-�� ������� �������, ����������...
	bool Create(string name)
	{
		ifstream ofs(name);
		if (ofs) {
			cout << "������ ��������" << endl;
			ofs.close();
			return 0;
		}
		else {
			ofstream ofs(name);
			cout << "���� " << name << " c�����" << endl;
			ofs.close();
			return 1;
		}
		
	}
	//����������� ����� ����, ��� ���������, ���������� � ����� ��� �������� �������, ����(����,�������, �����),(���� ���� ������ ���, ��������, ��� ������ �� �����.)
	void Move(string name, int recordposition, int recordmove)
	{
		int recordsize;
		fstream ofs (name, fstream::in | fstream::out | fstream::binary);
		ofs.seekg(recordposition, ios::beg); // ��������� ����� �� recordposition �� ������ �����
		ofs.seekp(0, ios::end); // ��������� ������ �� ����� �����
		recordsize = ofs.tellp();                    // ������ ������ ����� �����. �������
		recordsize -= recordposition - 1;            // ����� (recordposition - 1). ������ -1???
		char *record = new char[recordsize];         // ���������� ������ ������� recordsize
		ofs.seekg(recordposition, ios::beg);
		ofs.get(record, recordsize);                          // ����� �����, ���������� � ������
		ofs.seekp(recordposition + recordmove, ios::beg);
		ofs.write(record, recordsize-1);
		if (recordmove < 0) {
			HANDLE hFile = CreateFileA(name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE) {
				SetFilePointer(hFile, ofs.tellp(), NULL, FILE_BEGIN);
				SetEndOfFile(hFile);
				CloseHandle(hFile); //��������� ������ �����
			}
		}
		cout << "�����������" << endl;
		ofs.close();
		delete []record;
	}
	//������ ��������� �����
	void IndexRead(string name)
	{
		char magic[14];
		ifstream ofs(name, fstream::out | fstream::binary);
		//ofs.seekg(0, ios::beg); //- �������� �� �����
		ofs.read(magic, 14);
		//cout << "������ ���������:" << endl;
		cout << "Magic number: " << ((int)((unsigned char)(magic[0]) << 24) + (int)((unsigned char)(magic[1]) << 16) + (int)((unsigned char)(magic[2]) << 8) + (int)((unsigned char)(magic[3]))) << endl;
		cout << "���������� �������: " << ((int)((unsigned char)(magic[4]) << 24) + (int)((unsigned char)(magic[5]) << 16) + (int)((unsigned char)(magic[6]) << 8) + (int)((unsigned char)(magic[7]))) << endl;
		cout << "������ �����: " << ((int)((unsigned char)(magic[8]) << 8) + (int)((unsigned char)(magic[9]))) << endl;
		cout << "������ ������: " << ((int)((unsigned char)(magic[10]) << 8) + (int)((unsigned char)(magic[11]))) << endl;
		cout << "��� ������: " << (int)(magic[12]) << endl;
		cout << "---: " << (int)(magic[13]) << endl;
	}
	//���������� ���������(������ � ���������� ����� ������� � ��� �����);      magic begins here...
	void IndexWrite(string name, unsigned int mn, unsigned int count, unsigned short sizename,
		unsigned short sizep, unsigned char typep, unsigned char reserve)
	{
		char magic[14] = { mn >> (24), mn >> (16), mn >> (8), mn, count >> (24),
			count >> (16), count >> (8), count,
			sizename >> (8), sizename, sizep >> (8), sizep, typep, reserve };
		ofstream ofs (name, fstream::in);
		//ofs.seekp(0, ios::beg); //- �������� �� �����
		ofs.write(magic, 14);
		cout << "�������� ���������" << endl;
		IndexRead(name);
		ofs.close();
	}
	// ������ �� ��������� �� �����;    �������� ������� ������ ��� ����� (������� ��� ������� �� ���������, � ��������� ����� ���� � ����� ����������)
	
	//��������� int � ������� �������
	int ReadInt(string name, int recordposition)
	{
		char output[4];
		ifstream ofs (name, fstream::out | fstream::binary);
		ofs.seekg(recordposition, ios::beg);
		ofs.read(output, 4);
		cout << "������ " << recordposition << " " << ((int)((unsigned char)(output[0]) << 24) + (int)((unsigned char)(output[1]) << 16) + (int)((unsigned char)(output[2]) << 8) + (int)((unsigned char)(output[3]))) << endl;
		return ((int)((unsigned char)(output[0]) << 24) + (int)((unsigned char)(output[1]) << 16) + (int)((unsigned char)(output[2]) << 8) + (int)((unsigned char)(output[3])));
		ofs.close();
	}
	//��������� short � ������� �������
	unsigned short ReadSInt(string name, int recordposition)
	{
		char output[2];
		ifstream ofs(name, fstream::out | fstream::binary);
		ofs.seekg(recordposition, ios::beg);
		ofs.read(output, 2);
		cout << "������ " << recordposition << " " << ((int)((unsigned char)(output[0]) << 8) + (int)((unsigned char)(output[1]))) << endl;
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
		cout << "������ " << recordposition << " " << output << endl;
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
		//cout << "������ " << recordposition << " " << output << endl;
		//cout.write(output, recordsize);
		return output; // ������ � ����� ����� �����???
		ofs.close();
		delete []output;
	}
	void Write(string name, int number, int recordposition)
	{ 
		ofstream ofs(name, fstream::in | fstream::binary);
		char magic[4] = { number >> 24, number >> 16, number >> 8, number };
		ofs.write(magic, 2);
		cout << "������ Int " << recordposition << " " << number<<  endl;
		ofs.close();
	}
	void WriteS(string name,short int number, int recordposition)
	{
		ofstream ofs(name, fstream::in | fstream::binary);
		char magic[2] = { number >> 8, number };
		ofs.write(magic, 2);
		cout << "������ Short Int " << recordposition << " " << number<<  endl;
	ofs.close();
	}
	void Write(string name,char number, int recordposition)
	{
		char c[1] = { number };
		ofstream ofs(name, fstream::in | fstream::binary);
		ofs.write(c, 1);
		cout << "������ char " << recordposition << " " << number << endl;
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
		cout << "������" << endl;
		ofs.close();
	}

	/* ���-�� ���������� ����� �� ���
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
		ofs.write((char *)&newsize, 2); ���������� ����� �� � ��� ������� �������� ��� � ��� ��� ����?...
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