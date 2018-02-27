
#include "opencv2/core.hpp"
#include "json.hpp"
#include <fstream>

#define argi(name) cmd.get<int>(name)

using namespace cv;
using namespace std;
using json = nlohmann::json;

int main(int argc, const char **argv) {

	int nFiles;
	bool transform = false;
	int transNumber = 0;
	string inputFolder = "bin/data/files/";               //video input files
	vector<String> fileNames;
	try
	{
		const char *keys =
			"{ t | | }"
			"{ h | | }";
		CommandLineParser cmd(argc, argv, keys);

		if (cmd.has("t")) {
			transform = true;
			transNumber = argi("t");
			cout << transNumber << endl;

		}
		if (argc > 1) {
			transform = true;
			string x = argv[1];
			transNumber = stoi(x);
		}

		//lets load all filenames
		glob(inputFolder.c_str(), fileNames); // loads path to filenames into vector
		nFiles = fileNames.size();
		cout << " [!] number of files to process: " << nFiles << "\n\n";

		int nv = 0;      // index to transverse video collection

		while (nv < nFiles) {

			const char *result = "";
			string filePath = fileNames.at(nv);
			size_t lastindex1 = filePath.find_last_of("\\");
			string name = filePath.substr(lastindex1);

			std::string sprobe;
			sprobe = "ffprobe -v quiet -print_format json -show_format -show_streams \"";
			sprobe += filePath;
			sprobe += "\" > \"data\\info";
			sprobe += name;
			sprobe += ".json\"";

			int probeStatus = system(sprobe.c_str());

			cout << "Probing: " << sprobe << endl;

			std::string sjson;

			sjson += "data\\info";
			sjson += name;
			sjson += ".json";

			std::ifstream i(sjson.c_str());
			json j;
			i >> j;

			int r;

			try {
				json rotation = j["streams"][0]["tags"]["rotate"];

				string tempR = rotation;
				if (tempR == "") r = 0;
				else r = std::stoi(tempR);

			}
			catch (const std::domain_error &e) {
				r = 0;
			}

			cout << " [" << nv + 1 << "]" << " - Detected orientation: " << r << " degrees" << endl;

			if (transform) {

				std::string str2;
				str2 = "ffmpeg -hide_banner -loglevel panic -y -i ";
				str2 += filePath;
				str2 += " -vf \"transpose =";
				str2 += std::to_string(transNumber);
				str2 += "\" -c:v libx264 -crf 23 -acodec copy data\\fixed\\";
				str2 += name;
				result = str2.c_str();
				system(result);
				cout << " transforming: " << str2 << endl;

			}
			else {

				if (r != 0) {

					std::string str;
					str = "ffmpeg -hide_banner -loglevel panic -y -i ";
					str += filePath;
					str += " -metadata:s:v:0 rotate=0 data\\fixed\\";
					str += name;
					result = str.c_str();
					system(result);

					cout << "Stripping: " << str << endl;
				}
			}

			nv++;
		}
	}
	catch (const exception &e)
	{
		cout << "error: " << e.what() << endl;
		return -1;
	}
	cout << "Orientation metadata stripped!" << endl;
	system("pause");
	return 0;


}
