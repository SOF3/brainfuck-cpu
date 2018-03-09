#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
	char byte, write;
	while(!cin.eof()){
		cin.read(&byte, 1);
		switch(byte){
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
				break;
			case ']':
				write = 7;
				break;
			case '\n':
			case ' ':
			case '\t':
				break;
			default:
				cerr << "bfc: Encountered unknown byte " << (int) byte << endl;
		}
		cout.write(&write, 1);
	}

	return 0;
}
