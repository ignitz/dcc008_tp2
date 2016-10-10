#include "symbol_table.hpp"


/**************************
 * Symbol
 */
int Symbol::id = 0;

Symbol::Symbol(std::string name, int value) {
  id++;
  this->name = name;
  this->value = value;
  this->type = variable; // por default
  this->num_bytes = 0;
}

Symbol::Symbol(std::string name, int value, int num_bytes) {
  id++;
  this->name = name;
  this->value = value;
  this->type = variable;
  this->num_bytes = num_bytes;
}

Symbol::Symbol(std::string name, int value, std::string num_bytes) {
  id++;
  this->name = name;
  this->value = value;
  this->type = variable;
  this->num_bytes = std::stoi (num_bytes);
}

Symbol::Symbol(std::string name, int value, TypeSymbol type) {
  id++;
  this->name = name;
  this->value = value;
  this->type = type;
}

/**************************
 * SymbolTable
 */
SymbolTable::SymbolTable() {
  // Tá tudo bem Entei
  this->location_counter = 0;
}

int
SymbolTable::get_symbol_value(int i) {
  if (i >= 1 && i <= (int) this->symbol.size())
    return this->symbol.at(i-1)->value;
  else
    return -1;
}

int
SymbolTable::get_symbol_value(std::string name) {
  int size = this->symbol.size();
  if (name.back() == ':') name.pop_back();
  for (int i = 0; i < size; i++)
    if (this->symbol.at(i)->name.compare(name) == 0) return this->symbol.at(i)->value;
  return -1;
}

bool
SymbolTable::checkSymbol(std::string name) {
  int size = this->symbol.size();
  for (int i = 0; i < size; i++)
    if (this->symbol.at(i)->name.compare(name) == 0) return true;
  return false;
}

bool
SymbolTable::insertSymbol(std::string name, int value) {
  if (this->checkSymbol(name)) // Verifica se ja tem na tabela
    return false;
  if (name.front() == '_')
    this->symbol.push_back(new Symbol(name, value, TypeSymbol::label));
  else
    this->symbol.push_back(new Symbol(name, value));
  return true;
}

bool // Especial para .data
SymbolTable::insertSymbol(std::string name, int value, std::string num_bytes) {
  if (this->checkSymbol(name))
    return false;
  if (name.front() == '_')
    this->symbol.push_back(new Symbol(name, value, TypeSymbol::label));
  else
    this->symbol.push_back(new Symbol(name, 0, num_bytes));
  return true;
}

bool
SymbolTable::redefine(int location_counter) {
  bool check = false;
  int size = this->symbol.size();
  for (int i = 0; i < size; i++) {
    if (this->symbol.at(i)->name.front() != '_') {
      this->symbol.at(i)->value = location_counter;
      location_counter += this->symbol.at(i)->num_bytes;

      check = true;
    }
  }
  return check;
}


void
SymbolTable::printSymbols() {
  int size = this->symbol.size();
  std::cout << "**************************" << std::endl;
  std::cout << "Symbol Table:" << std::endl;
  for (int i = 0; i < size; i++) {
    std::cout << this->symbol.at(i)->name;
    symbol.at(i)->name.size() < 7 ? std::cout << "\t\t" : std::cout << "\t";
    std::cout << std::hex << this->symbol.at(i)->value << std::endl;
  }
  std::cout << "**************************" << std::endl;
}
