#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <clocale>
#include <filetable.h>

#pragma comment(lib, "File.Table.lib")

using File::Table::FileImpl;



static void
__test_0(FileImpl *impl)
{
	char *items[] = {
		"Поле1",
		"Поле2",
		"Поле3",
		"Поле4",
		"Поле5",
		"Поле6",
		"Поле7",
		"Поле8"
	};
	for (int i = 0; i < sizeof(items)/sizeof(items[0]); i++) {
		int id = impl->put(items[i], strlen(items[i]));
		assert(id == i);
	}
}

bool 
__Output(int id, const char *data, int size)
{
	std::string output(data, size);
	fprintf(stdout, "%2d.%s\n", id, output.c_str());
	return true;
}

static void
__test_1(FileImpl *impl)
{
	impl->iterate(__Output);
}

static void
__test_2(FileImpl *impl)
{
	int p = impl->record_count();
	if (p > 2) {
		int id = p / 2;
		fprintf(stdout, "Удалить %3d запись\n", id);
		assert(impl->del(id));
		assert(p == impl->record_count() + 1);
	}
}

static void
__test_3(FileImpl *impl)
{
	int p = impl->record_count();
	if (p > 2) {
		int id = p / 2;
		fprintf(stdout, "Обновить %3d запись\n", id);
		assert(impl->update(id, "Новый", sizeof("Новый")));
		assert(p == impl->record_count());
	}
}

int
main(int argc, char **argv)
{
	setlocale(LC_ALL, "Russian");
	FileImpl *impl = FileImpl::create("field.001", "field1", FileImpl::String, 20, true);
	if (impl) {
		__test_0(impl);
		__test_1(impl);
		__test_2(impl);
		__test_1(impl);
		__test_3(impl);
		__test_1(impl);
		delete impl;
	}
	system("pause");
	return EXIT_SUCCESS;
}
