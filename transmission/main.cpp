extern "C" {
#include "readin.h"
}
#include "transmission.h"


int main(int argc, char** argv)
{
	printf("%i%c",argc, '\n');

	
	for (int i = 0; i < argc; ++i) {

		printf("%s%c", argv[i], '\n');
	}
	if ((argc == 3) && (argv[1] != NULL) && (argv[2] != NULL)) {
		printf("%i%s", readStream(argv[1], argv[2]), " - readStream result.\n");
	}
	else {
		printf("NullPointerException\n");
	}
	return 0;
}