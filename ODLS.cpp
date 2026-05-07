// ODLS.exe
// вход: параметр командной строки, file1 [file2]
//       file1 имя первого файла в разбиении множества нормализованных ЛК 7-ого порядка
//       file2 имя второго файла в разбиении множества нормализованных ЛК 7-ого порядка (необязательный параметр)
// выход: стандартный выходной поток, ортогональные нормализованные пары ДЛК 7-ого порядка (с первой строкой 00 11 22 33 44 55 66)  


#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <set>

using namespace std;

#define NDLS  171200
#define DIM   7
#define DDIM  49

unsigned ndls1[NDLS][DDIM];
unsigned ndls2[NDLS][DDIM];

int M1 = 0, M2 = 0;
int counter = 0;

bool is_ortogonal_pair(unsigned i[DDIM], unsigned j[DDIM]);
void print_pair(unsigned f[DDIM], unsigned s[DDIM]);
bool load_file(char* s_file, unsigned ndls[NDLS][DDIM], int &M);

struct sq7x7 {
	unsigned sq[DDIM];
	bool operator<(const sq7x7&arg)const {
		for (int i = 0; i < DDIM; i++) {
			if (sq[i] < arg.sq[i]) return true;
			else if (sq[i] == arg.sq[i]) continue;
			else return false;
		}
		return false;
	}
};

std::set<sq7x7> orto_mates;

void add_to_orto_mates(unsigned ndls[DDIM]) {
	sq7x7 sq;
	memcpy(sq.sq, ndls, sizeof(unsigned[DDIM]));
	orto_mates.insert(sq);
}

int main(int argc, char*argv[])
{
	if (argc == 3) {

		auto start = chrono::system_clock::now();

		if (!load_file(argv[1], ndls1, M1) || !load_file(argv[2], ndls2, M2)) return EXIT_FAILURE;

		for (int m1 = 0; m1 < M1; m1++)
			for (int m2 = 0; m2 < M2; m2++)
				if (is_ortogonal_pair(ndls1[m1], ndls2[m2])) {
					add_to_orto_mates(ndls1[m1]); add_to_orto_mates(ndls2[m2]);
					cout << ++counter << endl << endl;	print_pair(ndls1[m1], ndls2[m2]);
					cout << ++counter << endl << endl;  print_pair(ndls2[m2], ndls1[m1]);
				}

		auto end = chrono::system_clock::now();
		chrono::duration<double> duration = (end - start);
		cout << "Completed in "; cerr << duration.count(); cout << " seconds" << endl;
		cout << "Number of DLS with mates is " << orto_mates.size() << endl;
		return EXIT_SUCCESS;

	}
	else if (argc == 2) {
		auto start = chrono::system_clock::now();

		if (!load_file(argv[1], ndls1, M1)) return EXIT_FAILURE;

		for (int m1 = 0; m1 < M1; m1++)
			for (int m2 = m1 + 1; m2 < M1; m2++)
				if (is_ortogonal_pair(ndls1[m1], ndls1[m2])) {
					add_to_orto_mates(ndls1[m1]); add_to_orto_mates(ndls1[m2]);
					cout << ++counter << endl << endl;	print_pair(ndls1[m1], ndls1[m2]);
					cout << ++counter << endl << endl;  print_pair(ndls1[m2], ndls1[m1]);
				}

		auto end = chrono::system_clock::now();
		chrono::duration<double> duration = (end - start);
		cout << "Completed in "; cerr << duration.count(); cout << " seconds" << endl;
		cout << "Number of DLS with mates is " << orto_mates.size() << endl;
		return EXIT_SUCCESS;
	}

	cout << "Bad parameter: required <file1> [<file2>]";
	return EXIT_FAILURE;
}

bool is_ortogonal_pair(unsigned f[DDIM], unsigned s[DDIM])
{
	static unsigned my_set[DDIM];
	int cur_elem = 0;

	for (int i = 0; i < DDIM; i++) {
		unsigned inserted = f[i] * DIM + s[i];

		for (int j = 0; j < cur_elem; j++)
			if (my_set[j] == inserted) return false;

		my_set[cur_elem++] = inserted;
	}

	return true;
}

void print_pair(unsigned f[DDIM], unsigned s[DDIM])
{
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			cout << f[i*DIM + j] << s[i * DIM + j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

bool load_file(char* s_file, unsigned ndls[NDLS][DDIM], int &M)
{
	ifstream file(s_file);
	cout << "loading file " << s_file << endl;

	if (file) {
		int k;
		for (k = 0; !file.eof(); k++)
		{
			unsigned digit;
			for (int i = 0; i < DIM; i++) for (int j = 0; j < DIM; j++) {
				if (file >> digit)  ndls[k][i*DIM + j] = digit;
				else {
					if (i == 0 && j == 0) {
						M = k;
						cout << "file " << s_file << " loaded" << endl;
						return true;
					}
					else {
						cout << "Error!!! Bad file";
						return false;
					}
				}
			}
		}
	}
	else {
		cout << "Error!!! Cannot open file";
		return false;
	}
	return true;
}
