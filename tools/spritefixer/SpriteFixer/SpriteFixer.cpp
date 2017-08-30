// SpriteFixer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>

using namespace std;

char out[1000];
char buffer[1000];

int main(int argc, char* argv[])
{
	if (argc != 3) {
		printf("Use with \"SpriteFixer.exe infile outfile\"\n");
		return 0;
	}

	FILE* inFile;
	fopen_s(&inFile, argv[1], "rb+");
	FILE* outFile; 
	fopen_s(&outFile, argv[2], "wb+");

	fseek(inFile, 0L, SEEK_END);
	int sz = ftell(inFile);
	fseek(inFile, 0L, SEEK_SET);

	assert(sz == 256 * 3);
	
	fread(buffer, 1, sz, inFile);

	int ip = 256 * 3 - 3;
	int op = 0;
	for (int i = 0; i < 256; i++) {
		out[op] = buffer[ip];
		out[op+1] = buffer[ip+1];
		out[op+2] = buffer[ip+2];
		ip -= 3;
		op += 3;
	}

	fwrite(out, 1, sz, outFile);

	fclose(inFile);
	fclose(outFile);
}

