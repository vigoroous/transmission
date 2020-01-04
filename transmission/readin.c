#include "readin.h"

int readStream(char* path1, char* path2) {
	//char buf[4194304]; /* массив дл€ считываемых байт, но не символов! */
	//size_t object = ;
	char* buf = (char*)malloc(sizeof(size_t) * 4000000);          // динамическое выделение пам€ти под символьную строку
	//if (sizeof(buf) != object) exit(1);                       // если выделение пам€ти не выполнилось, завершить программу

	FILE* in, * out;
	size_t n_obj, out_obj;
	//in = fopen(path1, "rb");
	if ((in = fopen(path1, "rb")) == NULL) {
		printf("Cannot open file.\n");
		return 1;
	}
	if ((out = fopen(path2, "wb")) == NULL) {
		printf("Cannot open file.\n");
		return 1;
	}

	if (in != NULL)
	{
		while ((n_obj = fread(buf, 1, 4000000, in)) != 0) /* читаем до конца файла */
		{
			/*
			здесь производитс€ обработка считанной информации
			n_obj Ц количество действительно прочитанных
			единиц информации
			*/
			out_obj = fwrite(buf, sizeof(char), n_obj, out);

			//printf("%i%c", n_obj, '\n');
		}
		fclose(out);
		fclose(in); /* закроем файл */
		free(buf);
	}
	return 0;
}