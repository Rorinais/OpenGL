#pragma once
#include<iostream>
using namespace std;
 


void error_out(string file, unsigned int linenum);

#define ErrorOut()error_out(__FILE__,__LINE__)

#define GLCheckError()(glGetError()==GL_NO_ERROR)

bool ReadFile(const char* fileName, string& outFile);
