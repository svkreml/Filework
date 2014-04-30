namespace Write {
	void string_wrt(char* rec, string name) {
		fstream ofs(name);
		ofs.seekg(14, ios::beg);
		ofs.write(rec, strlen(rec));
		cout << "Writing" << endl;
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