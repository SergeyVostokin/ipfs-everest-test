#define _CRT_SECURE_NO_DEPRECATE

#include <templet.hpp>
#include <everest.hpp>
#include <cstdlib>

using namespace std;

const int M = 9;

int main()
{
	//--- for getting access-token and Everest-application-ID use the following code ---

	/*templet::everest_engine teng0("log","pass");
	teng0.save_access_token();

	if (teng0) {
		teng0.print_app_description("odls1");
		teng0.print_app_description("odls2");
		string token;
		teng0.get_access_token(token);
		std::cout << "\n" << token << "\n";
	  return EXIT_SUCCESS;
	}*/

	templet::everest_engine teng("token");

	if (!teng) {
		std::cout << "...task engine not connected..." << std::endl;
		return EXIT_FAILURE;
	}

	string files[M];
	templet::everest_task odls1[M];
	templet::everest_task odls2[M][M];
	
	json in;
	
	auto start = chrono::high_resolution_clock::now();

	for (int i = 0; i < M; i++) {
		odls1[i].app_id("662aae561200001400e604bb");
		odls1[i].engine(teng);
		
		char name_buf[10];
		sprintf(name_buf, "nls%.2d.txt", i);
		string file(name_buf);
		bool res = teng.upload(file, files[i]);

		in["name"] = string("odls1-")+to_string(i);
		in["inputs"]["file1"] = files[i];

		odls1[i].submit(in);
		odls1[i].attach(files[i], files[i]);
	}

	for (int i = 0; i < M; i++) for (int j = i + 1; j < M; j++) {
		odls2[i][j].app_id("662ab03a1200000f00e604bd");
		odls2[i][j].engine(teng);

		in["name"] = string("odls2-") + to_string(i) + string("-") + to_string(j);
		in["inputs"]["file1"] = files[i];
		in["inputs"]["file2"] = files[j];

		odls2[i][j].submit(in);
	}
	
	teng.run();
	auto stop  = chrono::high_resolution_clock::now();

	templet::everest_error cntxt;
	if (teng.error(&cntxt)) {
		std::cout << "...task engine error..." << std::endl;
		templet::everest_engine::print_error(&cntxt);
		return EXIT_FAILURE;
	}

    double Ts = 0.0;

    for (int i = 0; i < M; i++) {
        json out = odls1[i].result();
        string str_time = out["time"].get<string>();
        Ts += stod(str_time);
    }

    for (int i = 0; i < M; i++) for (int j = i + 1; j < M; j++) {
        json out = odls2[i][j].result();
        string str_time = out["time"].get<string>();
        Ts += stod(str_time);
    }
    
	chrono::duration<double> diff = stop - start;
    double Tp = diff.count();

    cout << "Number of files is " << M << endl;
    cout << "Number of tasks is " << M + M*(M-1)/2 << endl;
	cout << "Parallel   time is " << Tp << " seconds" << endl;
    cout << "Sequential time is " << Ts <<" seconds" << endl;
    cout << "Speedup is " << Ts/Tp << endl;
    

	return EXIT_SUCCESS;
}