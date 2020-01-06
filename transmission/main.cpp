extern "C" {
#include "readin.h"
}
#include "transmission.h"
#include <string>
#include <iostream>

using std::string;

int main(int argc, char** argv)
{
	Server Simon;
	Client ÑAJIO;
	int flag = 0;
	printf("%i%c",argc, '\n');
	string path;
	printf("Enter path: ");
	std::cin >> path;

	for (int i = 0; i < argc; ++i) {

		printf("%s%c", argv[i], '\n');
	}
	if ((argc == 3) && (argv[1] != NULL) && (argv[2] != NULL)) {
		printf("%i%s", copy_file(argv[1], argv[2]), " - readStream result.\n");
	}
	else if ((argc == 4) && (!strcmp(argv[1], "open")) && (argv[2] != NULL) && (argv[3] != NULL)) {
		
		
		Simon.setup(argv[2], argv[3]);
		Simon.recievemode(path.c_str());
		Simon.shutdownServer();

	}
	else if ((argc == 4) && (!strcmp(argv[1], "connect")) && (argv[2] != NULL) && (argv[3] != NULL)) {
		
		ÑAJIO.setup(argv[2], argv[3]);
		ÑAJIO.sendfile(path.c_str());
		ÑAJIO.shutdownClient();
	}
	else{
		printf("NullPointerException\n");
	}	

	return 0;
}