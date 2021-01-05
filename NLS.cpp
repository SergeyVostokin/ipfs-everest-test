// NLS.exe
// Вход:  стандартный входной поток поток, последовательность ЛК 7 порядка с упорядоченными по возрастанию элементами главной диагонали из LS.exe 
// Выход: стантартный выходной поток, последовательность ЛК 7 порядка с упорядоченными по возрастанию элементами первой строки  

#include <string>
#include <iostream>
#include <chrono>

using namespace std;

unsigned permutation[7];
unsigned square[7][7];

int main()
{
	int counter = 0;
	unsigned digit;
	string line;

	auto start = chrono::system_clock::now();
	
	getline(cin, line);
	
	for (;;) {
	
		for (int i = 0; i < 7; i++) for (int j = 0; j < 7; j++) {
			if (cin >> digit) square[i][j] = digit;
			else {
				cerr << "Error!!!";
				return EXIT_FAILURE;
			}
		}

		bool goon = true;
		for (int i = 0; i < 7; i++) {
			if (square[i][i] != i) { goon = false; break; }
		}

		if (!goon) {
			auto end =chrono::system_clock::now();
			chrono::duration<double> duration = (end - start);
			cerr << "Completed in " << duration.count() << " seconds" << endl;
			return EXIT_SUCCESS;
		}

		for (int j = 0; j < 7; j++)	permutation[square[0][j]] = j;

		for (int i = 0; i < 7; i++) for (int j = 0; j < 7; j++) {
			square[i][j] = permutation[square[i][j]];
		}

		for (int i = 0; i < 7; i++){
			for (int j = 0; j < 7; j++) {
				cout << square[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		cerr << ++counter << endl;
	}

    return EXIT_SUCCESS;
}

