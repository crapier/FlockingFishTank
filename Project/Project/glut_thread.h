#pragma once

#include "globals.h"


struct glut_args {
	int argc;
	char** argv;
};

DWORD WINAPI glut_main(LPVOID lpParam);