#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main(int argc, char* argv[]){
	if(argc < 2){
		cout << "Usage: " << argv[0] << " FILE_BASENAME" << endl;
		return 1;
	}

	string baseName = argv[1];

	ifstream is(baseName + ".bf");
	ofstream min(baseName + ".min.bf");
	ofstream os(baseName + ".bfc");
	ofstream rom(baseName + ".bfc.txt");

	if(is.fail() || min.fail() || os.fail() || rom.fail()) return 2;

	const char ROM_HEADER[] = {'v', '2', '.', '0', ' ', 'r', 'a', 'w'};
	const int ROM_HEADER_SIZE = 8;
	rom.write(ROM_HEADER, ROM_HEADER_SIZE);

	char byte, write;
	bool newline = true;
	int n = 0;
	int line = 1;
	int loops = 0;
	while(!is.eof()){
		is.read(&byte, 1);
		if(!newline && byte != '\n'){
			continue;
		}
		switch(byte){
			case '\n':
				newline = true;
				++line;
				continue;
			case ' ':
			case '\t':
				continue;
			case '>':
				write = 0;
				break;
			case '<':
				write = 1;
				break;
			case '+':
				write = 2;
				break;
			case '-':
				write = 3;
				break;
			case '.':
				write = 4;
				break;
			case ',':
				write = 5;
				break;
			case '[':
				write = 6;
				++loops;
				if(loops > 62){
					cerr << "Warning: stack overflow: the loop stack in brainfuck-cpu does not support more than 62 loops" << endl;
				}
				break;
			case ']':
				write = 7;
				--loops;
				if(loops < 0){
					cerr << "Warning: syntax error: unmatched ] on line " << line << " in " << baseName << ".bf" << endl;
				}
				break;
			default:
				newline = false;
				continue;
		}
		min.write(&byte, 1);
		os.write(&write, 1);
		char romBytes[] = {
			((n++) & 7) == 0 ? '\n' : ' ',
			(char) (write + '0')
		};
		rom.write(romBytes, 2);
	}
	if(loops > 0){
		cerr << "Warning: strict error: end of file reached with an unclosed [" << endl;
	}
	is.close();
	min.close();
	write = 8;
	os.write(&write, 1);
	os.close();
	char romFooter[] = {
		((n++) & 7) == 0 ? '\n' : ' ',
		(char) (write + '0'),
		'\n'
	};
	const int ROM_FOOTER_SIZE = 3;
	rom.write(romFooter, ROM_FOOTER_SIZE);
	rom.close();

	cout << "Success, wrote " << n << " instructions" << endl;
	return 0;
}
