#include "readin.h"

int readStream() {
	//char buf[4194304]; /* ������ ��� ����������� ����, �� �� ��������! */
	//size_t object = ;
	char* buf = (char*)malloc(sizeof(size_t) * 4000000);          // ������������ ��������� ������ ��� ���������� ������
	//if (sizeof(buf) != object) exit(1);                       // ���� ��������� ������ �� �����������, ��������� ���������

	FILE* in, * out;
	size_t n_obj, out_obj;
	in = fopen("file.bin", "rb");
	if ((out = fopen("out.bin", "wb")) == NULL) {
		printf("Cannot open file.\n");
		exit(1);
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
}