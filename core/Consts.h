#ifndef _CONSTS_H_
#define _CONSTS_H_


static std::string dummy = "";
static std::vector<std::string> _dummy = {};
static std::vector<bool> dummy_bool = {};

enum ATTRIBUTE {
	NUMERIC = 0,
	NOMINAL,
	STRING,
	DATE,
	RELATIONAL
};

enum ATTRIBUTE_ORDERING {
	ORDERING_SYMBOLIC = 0,
	ORDERING_ORDERED,
	ORDERING_MODULO
};

static const char* TRIMCHARS = " \t\n\r\f\v.";

#endif	// _CONSTS_H_
