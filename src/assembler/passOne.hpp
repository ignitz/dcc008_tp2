#ifndef _PASS_ONE_
#define _PASS_ONE_
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <boost/algorithm/string.hpp> // tokens
#include <vector>
#include "symbol_table.hpp"
#include "table_opcodes.hpp"
#include "util.hpp"

void pass_one( std::ifstream& , TableOpcode& );
#endif
