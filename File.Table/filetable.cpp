#include "filetable.h"

using namespace File;
using namespace File::Table;

FileImpl::FileImpl(const std::string &filename)
: _filename(filename), _fd(NULL), _is_table(false), _default_value(NULL)
{
	memset(&_head, 0, sizeof(_head));
}

FileImpl *
FileImpl::open(const std::string &filename)
{
	FileImpl *impl = new FileImpl(filename);
	if (!impl->open()) {
		delete impl;
		return NULL;
	}
	return impl;
}

FileImpl *
FileImpl::create(const std::string &filename, const std::string &name, Type type, int size, bool is_required)
{
	FileImpl *impl = new FileImpl(filename);
	impl->_fd = fopen(filename.c_str(), "w+b");
	if (impl->_fd == NULL) {
		impl->close();
		return NULL;
	}
	impl->_table_name = name;
	(*(unsigned long *)&impl->_head.magic) = HEAD_MN;
	impl->_head.name_len = name.size();
	impl->_head.type = type;
	impl->_head.size = size;
	impl->_head.is_required = is_required;
	impl->write_header();
	impl->close();
	if (!impl->open()) {
		delete impl;
		return NULL;
	}
	return impl;
}

FileImpl::~FileImpl()
{
	close();
}

bool
FileImpl::open()
{
	if (is_open())
		close();
	_fd = fopen(_filename.c_str(), "r+b");
	if (_fd == NULL)
		return false;
	_is_table = read_head();
	if (!_is_table)
		close();
	return _is_table;
}

bool
FileImpl::is_open() const
{
	return _fd != NULL;
}

bool
FileImpl::close()
{
	if (_fd)
		fclose(_fd);
	memset(&_head, 0, sizeof(_head));
	_fd = NULL;
	if (_default_value)
		delete[] _default_value;
	_default_value = NULL;
	_is_table = false;
	return true;
}

bool
FileImpl::write_header()
{
	fpos_t curr;
	int  writed;

	fgetpos(_fd, &curr);
	go_start();	
	writed = fwrite(&_head, sizeof(_head), 1, _fd);
	if (writed == 1) {
		if (_head.name_len > 0) {
			fwrite(_table_name.c_str(), _table_name.size(), 1, _fd);
		}
	}
	fsetpos(_fd, &curr);
	return true;
}

bool
FileImpl::read_head()
{
	fpos_t curr;
	int  readed;
	bool result = false;

	memset(&_head, 0, sizeof(_head));
	fgetpos(_fd, &curr);
	go_start();
	readed = fread(&_head, sizeof(_head), 1, _fd);
	if (readed == 1) {
		result = ((*(unsigned long *)&_head.magic) == HEAD_MN);
		if (result) {
			char *name = new char[_head.name_len + 1];
			readed = fread(name, _head.name_len, 1, _fd);
			if (readed == 1) {
				name[_head.name_len] = 0;
				_table_name = std::string(name);
			}
			delete [] name;

			if (_head.default_size > 0) {
				_default_value = new char[_head.default_size];
				readed = fread(_default_value, _head.default_size, 1, _fd);
				if (readed == _head.default_size) {
					//Do
				}
			}
		}
	} else {
		result = false;
	}
	fsetpos(_fd, &curr);
	return result;
}

bool
FileImpl::go_start()
{
	if (is_open()) {
		rewind(_fd);
		return true;
	}
	return false;
}

bool
FileImpl::go_data()
{
	if (go_start()) {
		fpos_t curr;

		fgetpos(_fd, &curr);
		curr += sizeof(_head);
		curr += _head.name_len;
		if (_head.default_size > 0)
			curr += _head.default_size;
		fsetpos(_fd, &curr);
	}
	return true;
}

bool
FileImpl::go_end()
{
	if (is_open()) {
		fseek(_fd, 0, SEEK_END);
	}
	return false;
}

bool
FileImpl::go(int id)
{
	int i = 0;
	if (id < 0 || _head.count <= (unsigned int)id)
		return false;
	go_data();
	for (i = 0; i < id; ++i) {
		fpos_t pos;
		char len[4];
		int readed;

		readed = fread(len, 1, sizeof(len), _fd);
		if (readed == sizeof(len)) {
			fgetpos(_fd, &pos);
			pos += (*(int *)len);
			fsetpos(_fd, &pos);
		} else {
			return false;
		}
	}
	return true;
}

int
FileImpl::put(const char *data, int size)
{
	char len[4];
	int writed;
	if (size > _head.size)
		return -1;
	go_end();
	(*(int *)len) = size;
	writed = fwrite(len, sizeof(len), 1, _fd);
	writed = fwrite(data, size, 1, _fd);
	_head.count++;
	write_header();
	go_data();
	return _head.count - 1;
}

bool 
FileImpl::update(int id, const char *data, int size)
{
	fpos_t curr;
	fpos_t set;
	int len = 0;
	int readed = 0;
	FILE *fd;
	char *buffer;

	if (!go(id))
		return false;
	fgetpos(_fd, &curr);
	readed = fread(&len, 1, sizeof(int), _fd);
	set = curr + sizeof(int) + len;
	buffer = new char[len];
	readed = fread(buffer, 1, len, _fd);
	fd = copyto(set);
	fsetpos(_fd, &curr);

	fwrite(&size, 1, 4, _fd);
	fwrite(data, 1, size, _fd);
	copyfrom(fd);
	fsetpos(_fd, &curr);
	return true;
}

bool
FileImpl::del(unsigned int id)
{
	fpos_t curr;
	fpos_t set;
	int len = 0;
	int readed = 0;
	FILE *fd;

	if (!go(id))
		return false;
	fgetpos(_fd, &curr);
	readed = fread(&len, 1, sizeof(int), _fd);
	set = curr + sizeof(int) + len;
	fd = copyto(set);
	fsetpos(_fd, &curr);
	copyfrom(fd);
	fsetpos(_fd, &curr);
	_head.count--;
	return write_header();
}

FILE *
FileImpl::copyto(fpos_t pos) const
{
	char buffer[256];
	int  readed;
	fpos_t curr;
	FILE *fd;

	
	fd = tmpfile();
	if (fd) {
		fgetpos(_fd, &curr);
		fsetpos(_fd, &pos);
		while ((readed = fread(buffer, 1, sizeof(buffer), _fd)) > 0) {
			fwrite(buffer, 1, readed, fd);
		}
		fsetpos(_fd, &curr);
		rewind(fd);
	}
	return fd;
}

void
FileImpl::copyfrom(FILE *fd) const
{
	char buffer[256];
	int  readed;
	fpos_t curr;

	fgetpos(_fd, &curr);
	while ((readed = fread(buffer, 1, sizeof(buffer), fd)) > 0) {
		fwrite(buffer, 1, readed, _fd);
	}
	fsetpos(_fd, &curr);
	fclose(fd);
}

void
FileImpl::iterate(Next next)
{
	char len[4];
	int readed;
	int size;
	bool doNext = true;

	if (_head.count == 0)
		return;
	go_data();
	for (unsigned int i = 0; i < _head.count && doNext; ++i) {
		readed = fread(len, sizeof(len), 1, _fd);
		size = (*(int *)len);
		char *buffer = new char[size];
		readed = fread(buffer, size, 1, _fd);
		doNext = next(i, buffer, size);
		delete buffer;
	}
}
