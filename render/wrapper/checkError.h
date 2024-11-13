#pragma once
#define GL_CALL(func) func;checkError();

void checkError();