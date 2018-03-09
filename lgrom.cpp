#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
	const char ROM_HEADER[] = {'v', '2', '.', '0', ' ', 'r', 'a', 'w'};
	const int ROM_HEADER_SIZE = sizeof(ROM_HEADER);

	cout.write(ROM_HEADER, ROM_HEADER_SIZE);

	char byte;
	unsigned int n = 0;
	while(!cin.eof()){
		cin.read(&byte, 1);
		char romBytes[] = {
				((n++) & 7) == 0 ? '\n' : ' ',
				(char) (byte + '0')
		};
		cout.write(romBytes, 2);
	}

	char romFooter[] = {
			((n++) & 7) == 0 ? '\n' : ' ',
			'8',
			'\n'
	};
	cout.write(romFooter, 3);

	return 0;
}
