#include "readin.h"

int readStream(char* path1, char* path2) {
	//char buf[4194304]; /* ������ ��� ����������� ����, �� �� ��������! */
	//size_t object = ;
	char* buf = (char*)malloc(sizeof(size_t) * 4000000);          // ������������ ��������� ������ ��� ���������� ������
	//if (sizeof(buf) != object) exit(1);                       // ���� ��������� ������ �� �����������, ��������� ���������

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
		while ((n_obj = fread(buf, 1, 4000000, in)) != 0) /* ������ �� ����� ����� */
		{
			/*
			����� ������������ ��������� ��������� ����������
			n_obj � ���������� ������������� �����������
			������ ����������
			*/
			out_obj = fwrite(buf, sizeof(char), n_obj, out);

			//printf("%i%c", n_obj, '\n');
		}
		fclose(out);
		fclose(in); /* ������� ���� */
		free(buf);
	}
	return 0;
}