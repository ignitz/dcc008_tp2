#ifndef _PASS_TWO_
#define _PASS_TWO_
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <boost/algorithm/string.hpp> // tokens
#include <vector>
#include "symbol_table.hpp"
#include "table_opcodes.hpp"
#include "util.hpp"

void pass_two( std::ifstream& , std::fstream& , TableOpcode& );
#endif
