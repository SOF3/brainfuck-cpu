#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "Usage: " << argv[0] << " <file base name>" << endl;
		return 1;
	}

	string baseName = argv[1];

	ifstream is(baseName + ".bf");
	ofstream min(baseName + ".min.bf");
	ofstream os(baseName + ".bfc");
	ofstream rom(baseName + ".bfc.txt");

	if(is.fail() || min.fail() || os.fail() || rom.fail()) return 2;

	const char ROM_HEADER[] = {'v', '2', '.', '0', ' ', 'r', 'a', 'w', '\n'};
	rom.write(ROM_HEADER, 9);

	char byte;
	bool newline = true;
	int n = 0;
	while(!is.eof()){
		is.read(&byte, 1);
		char write;
		switch(byte){
			case '\n':
				newline = true;
				continue;
			case ' ':
			case '\t':
				continue;
			case '>': write = 0; break;
			case '<': write = 1; break;
			case '+': write = 2; break;
			case '-': write = 3; break;
			case '.': write = 4; break;
			case ',': write = 5; break;
			case '[': write = 6; break;
			case ']': write = 7; break;
			default:
				newline = false;
				continue;
		}
		++n;
		min.write(&byte, 1);
		os.write(&write, 1);
		char romBytes[] = {(char) (write + '0'), ' '};
		rom.write(romBytes, 2);
	}
	is.close();
	os.close();
	rom.put('\n');
	rom.close();

	cout << "Success, wrote " << n << " instructions" << endl;
	return 0;
}
