#pragma once
#ifndef _TTF_UTILITY_H_
#define _TTF_UTILITY_H_
#define DEBUG
#include <fstream>
#include <assert.h>
#include <iostream>

static void InverseEudianRead(const char * source, char * target, const int per_data_size, int & offset, const int data_num = 1)
{
	assert(target != NULL && source != NULL);
	char * ptr = (char*)target;
	for (int i = 0; i < data_num; i++)
	{
		for (int j = per_data_size - 1; j >= 0; j--)
		{
			ptr[j + i * per_data_size] = source[offset + (per_data_size - 1 - j)];
		}
		offset += per_data_size;
	}
}

template<typename T>
void TRead(char * source, T * buffer, int &  offset)
{
	InverseEudianRead(source, (char*)buffer, sizeof(T), offset);
}

template<typename T>
void TReadN(char * source, T * buffer, int n, int & offset)
{
	InverseEudianRead(source, (char*)buffer, sizeof(T), offset);
}

#endif