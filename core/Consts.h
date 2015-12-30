#ifndef _CONSTS_H_
#define _CONSTS_H_

#include <vector>
#include <string>
#include "Typedefs.h"

/** Empty string text */
static string dummy = "";

/** dummy string array */
static string_array _dummy = {};

/** dummy bool array */
static bool_array dummy_bool = {};

/** ATTRIBUTE TYPE */
enum ATTRIBUTE {
    NUMERIC = 0,
    NOMINAL,
    STRING,
    DATE,
    RELATIONAL
};

/** Trim characters */
static const char* TRIMCHARS = " \t\n\r\f\v.";

#endif    // _CONSTS_H_
