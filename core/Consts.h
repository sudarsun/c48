#ifndef _CONSTS_H_
#define _CONSTS_H_

#include <vector>
#include <string>
#include "Typedefs.h"

static string dummy = "";
static string_array _dummy = {};
static bool_array dummy_bool = {};

enum ATTRIBUTE {
	NUMERIC = 0,
	NOMINAL,
	STRING,
	DATE,
	RELATIONAL
};

static const char* TRIMCHARS = " \t\n\r\f\v.";

#endif	// _CONSTS_H_
