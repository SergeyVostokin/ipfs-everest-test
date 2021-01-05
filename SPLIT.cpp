// SPLIT.exe
// вход:  стантартный входной поток, последовательность ЛК 7-ого порядка с упорядоченными по возрастанию элементами первой строки;
//        параметр командной строки, на сколько файлов M (от 1 до 99) вида nls00.txt, nls01.txt, nls02.txt, ... разбивать входной поток
// выход: M файлов вида nls00.txt, nls01.txt, nls02.txt, ... в рабочем каталоге программы SPLIT.exe

#include <string>
#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;

int M = 0;
unsigned square[7][7];
const int NUM_NLS7 = 171200;

int main(int argc, char*argv[])
{
	if ((argc == 2) && (M = atoi(argv[1])) && (1 <= M && M <= 99)) {
		unsigned digit;

		auto start = chrono::system_clock::now();

		for (int m = 0; m < M; m++) {

			char name_buf[10];
			sprintf_s(name_buf, "nls%.2d.txt", m);
			ofstream myout(name_buf);

			cerr << "writing " << name_buf << endl;

			int recs_in_m = NUM_NLS7 / M + ((m < NUM_NLS7 % M) ? 1 : 0);

			for(int rec = 0; rec < recs_in_m; rec++)
			{
				for (int i = 0; i < 7; i++) for (int j = 0; j < 7; j++) {
					if (cin >> digit) square[i][j] = digit;
					else {
						cerr << "Error!!!";
						return EXIT_FAILURE;
					}
				}

				for (int i = 0; i < 7; i++) {
					for (int j = 0; j < 7; j++) {
						myout << square[i][j] << " ";
					}
					myout << endl;
				}
				myout << endl;
			}
		}

		auto end = chrono::system_clock::now();
		chrono::duration<double> duration = (end - start);
		cerr << "Completed in " << duration.count() << " seconds" << endl;
		return EXIT_SUCCESS;
	}
	cerr << "Bad parameter: required <a number in range 1..99>";
    return EXIT_FAILURE;
}

