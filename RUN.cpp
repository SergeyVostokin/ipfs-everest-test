#define _CRT_SECURE_NO_DEPRECATE

#include <templet.hpp>
#include <everest.hpp>
#include <cstdlib>

using namespace std;

const int M = 3;

int main()
{
	//--- for getting access-token and Everest-application-ID use the following code ---
	/*templet::everest_engine teng0("login","pass");
	teng0.save_access_token();

	if (teng0) {
		teng0.print_app_description("odls1");
		teng0.print_app_description("odls2");
		string token;
		teng0.get_access_token(token);
		std::cout << "\n" << token << "\n";
	  return EXIT_SUCCESS;
	}*/

	templet::everest_engine teng("fsih61ho6nm85f51xo1xyx8tj4ws9e8wvddr4pe8eyp72xomf1mgwut04apf1nr8");

	if (!teng) {
		std::cout << "...task engine not connected..." << std::endl;
		return EXIT_FAILURE;
	}

	templet::everest_task odls1[M];
	templet::everest_task odls2[M][M];
	
	json in;
	
	//auto start = chrono::high_resolution_clock::now();

	for (int i = 0; i < M; i++) {
		odls1[i].app_id("5ff9d4041100001000ae2c37");
		odls1[i].engine(teng);
		
		in["name"] = string("odls1-")+to_string(i);
		in["inputs"]["m"] = i;

		odls1[i].submit(in);
	}

	for (int i = 0; i < M; i++) for (int j = i + 1; j < M; j++) {
		odls2[i][j].app_id("5ff9d4801100003100ae2c3a");
		odls2[i][j].engine(teng);

		in["name"] = string("odls2-") + to_string(i) + string("-") + to_string(j);
		in["inputs"]["m1"] = i;
		in["inputs"]["m2"] = j;

		odls2[i][j].submit(in);
	}

	auto start = chrono::high_resolution_clock::now();
	teng.run();
	auto stop  = chrono::high_resolution_clock::now();

	templet::everest_error cntxt;
	if (teng.error(/*&cntxt*/)) {//	<--exeption is here(to be fixed)
		std::cout << "...task engine error..." << std::endl;
		//templet::everest_engine::print_error(&cntxt); <-- exeption is here (to be fixed)
		return EXIT_FAILURE;
	}

	chrono::duration<double> diff = stop - start;
	cout << "Computation time is " << diff.count() << " seconds";

	return EXIT_SUCCESS;
}