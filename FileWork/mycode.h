//#include <iostream>
//using namespace std;
namespace Write {
	void string_wrt(/*char* rec*/ string name) {
		fstream ofs(name);
		char dec = 'y';
		while (dec != 'n') {
			static int position = ofs.tellg();
			ofs.seekg(14, ios::beg);
			char* rec;
			cout << "¬ведите данные: "; cin >> rec;
			ofs.write(rec, strlen(rec));
			position += strlen(rec);
			cout << "Writing" << endl;
		}
		
		ofs.close();
	}

	void string_read(string name) {
		fstream ofs(name);
		//ofs.seekg(14, ios::beg);
		ofs.seekp(0, ios::end); 
		int recordsize = ofs.tellp();
		recordsize -= 13;
		char *record = new char[recordsize];
		ofs.seekg(14, ios::beg);
		ofs.get(record, recordsize);
		cout << record << endl;
		delete[]record;
	}
}