#ifndef _FILETABLE_H_
#define _FILETABLE_H_
#include <string>
#include <stdlib.h>
#include <stdio.h>

#define HEAD_MN (unsigned long)0x11223344

#if defined(__cplusplus)
extern "C" {
#endif

#pragma warning(disable: 4996)
#pragma pack(push, 1)
struct __table_head {
	unsigned char  magic[4];
	unsigned short name_len;
	unsigned int   count;
	unsigned char  is_required;
	unsigned char  type;
	unsigned short size;
	unsigned short default_size;
};
typedef struct __table_head table_head;
#pragma pack(pop)

#if defined(__cplusplus)
}
#endif

namespace File {
namespace Table {

	class FileImpl {
		FileImpl(const FileImpl &other);
		FileImpl &operator=(const FileImpl &other);

		FileImpl(const std::string &filename);
		
		bool  read_head();
		bool  write_header();
		bool  go(int id);
		FILE *copyto(fpos_t pos) const;
		void  copyfrom(FILE *fd) const;

		const std::string _filename;
		table_head        _head;
		FILE *            _fd;
		bool              _is_table;

		std::string       _table_name;
		char             *_default_value;
	public:
		enum Type
		{
			String,
			Integer,
			Double,
			Boolean
		};
		typedef bool(*Next)(int id, const char *data, int size);
		~FileImpl();

		bool open();
		bool close();
		bool is_open() const;
		bool go_start();
		bool go_data();
		bool go_end();

		int  put(const char *data, int size);
		bool update(int id, const char *data, int size);
		bool del(unsigned int id);
		void iterate(Next next);		

		inline int record_count() const
		{
			return _head.count;
		}

		inline bool is_table() const 
		{
			return _is_table;
		}
		inline const std::string &table_name() const
		{
			return _table_name;
		}
		inline char *default_value() const
		{
			return _default_value;
		}
		inline int default_size() const
		{
			return _head.default_size;
		}
		static FileImpl *open(const std::string &filename);
		static FileImpl *create(const std::string &filename, const std::string &name, Type type = String, int size = 1, bool is_required = false);
	};
}
}


#endif

