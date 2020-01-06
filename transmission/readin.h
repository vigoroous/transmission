#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copy_file(char* path1, char* path2);

int write_to_file(const char* path, char* buf, size_t n_obj); //прототипирование функции

int read_from_file(const char* path, char* buf, size_t n_obj);