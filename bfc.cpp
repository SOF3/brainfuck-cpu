#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]){
	if(argc < 3){
		cout << "Usage: " << argv[0] << " <source bf file> <target bfc file>" << endl;
		return 1;
	}

	ifstream is(argv[1]);
	if(is.fail()) return 2;
	string targetFile = argv[2];
	ofstream os(targetFile);
	if(os.fail()) return 2;
	ofstream rom(targetFile + ".txt");
	if(rom.fail()) return 2;

	const char ROM_HEADER[] = {'v', '2', '.', '0', ' ', 'r', 'a', 'w', '\n'};
	rom.write(ROM_HEADER, 9);

	char byte;
	bool newline = true;
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
		os.write(&write, 1);
		char romBytes[] = {(char) (write + '0'), ' '};
		rom.write(romBytes, 2);
	}
	is.close();
	os.close();
	rom.put('\n');
	rom.close();
	return 0;
}
