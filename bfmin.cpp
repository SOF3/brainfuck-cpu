#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
	char byte;
	bool newline = true;
	int line = 1;
	int loops = 0;
	while(!cin.eof()){
		cin.read(&byte, 1);
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
			case '<':
			case '+':
			case '-':
			case '.':
			case ',':
				break;
			case '[':
				++loops;
				if(loops > 62){
					cerr << "bfmin: Warning: stack overflow: the loop stack in brainfuck-cpu does not support more than 62 loops" << endl;
				}
				break;
			case ']':
				--loops;
				if(loops < 0){
					cerr << "bfmin: Warning: syntax error: unmatched ] on line " << line << endl;
				}
				break;
			default:
				newline = false;
				continue;
		}
		cout.write(&byte, 1);
	}

	if(loops > 0){
		cerr << "bfmin: Warning: strict error: end of file reached with an unclosed [" << endl;
	}

	return 0;
}
