#include "SymbolTable.hpp"

Symbol::Symbol(std::string name) {
  this->valueInit = false;
  this->name = name;
}

SymbolTable::SymbolTable() {
  // Tá tudo bem Entei
}

int
SymbolTable::getValue( std::string name ) {
  for ( auto const& symbol : this->symbol ) {
    if (symbol->name.compare(name) == 0) return symbol->value;
  }
  return 0;
}

bool
SymbolTable::setValue( std::string name , int value ) {
  for ( auto const& symbol : this->symbol ) {
    if (symbol->name.compare(name) == 0) {
      symbol->value = value;
      return true;
    }
  }
  return false;
}

void
SymbolTable::translateSymbol( int move ) {
  for (auto const& symbol : this->symbol) {
    symbol->value += move;
    for (size_t i = 0; i < symbol->location.size(); i++)
      symbol->location[i] += move;
  }
}

bool
SymbolTable::insertValue( int value ) {
  if ( this->symbol.size() <= 0 ) return false;
  if ( this->symbol[this->symbol.size() - 1]->valueInit ) {
    this->symbol[this->symbol.size() - 1]->location.push_back(value);
  }

  else {
    this->symbol[this->symbol.size() - 1]->value = value;
    this->symbol[this->symbol.size() - 1]->valueInit = true;
  }
  return true;
}

bool
SymbolTable::checkSymbol(std::string name) {
  for (auto const& symbol : this->symbol)
  if (symbol->name.compare(name) == 0) return true;
  return false;
}

bool
SymbolTable::insertSymbol( std::string name ) {
  if ( this->checkSymbol( name ) ) // Verifica se ja tem na tabela
  return false;
  this->symbol.push_back( new Symbol( name ) );
  return true;
}

bool
SymbolTable::setVerbose(bool bVerbose) {
  this->bVerbose = bVerbose;
  for (auto const& symbol : this->symbol)
  symbol->bVerbose = bVerbose;
  return bVerbose;
}

std::vector<int>
SymbolTable::getLocations( std::string name ) {
  std::vector<int> r;
  for( auto const& symbol : this->symbol )
    if (symbol->name.compare(name) == 0)
      return symbol->location;
  return r;
}

std::vector<std::string>
SymbolTable::getNames() {
  std::vector<std::string> r;
  for( auto const& symbol : this->symbol ) {
    r.push_back(symbol->name);
  }
  return r;
}

void // imprime a tabela de símbolos utilizando make v
SymbolTable::printSymbols() {
  for ( auto const& symbol : this->symbol ) {
    std::cout << symbol->name << ":" << std::endl;
    std::cout << std::hex << "0x" << symbol->value << "\t";
    for ( auto const& value : symbol->location )
      std::cout << std::hex << "0x" << value << "\t";
    std::cout << std::endl;
  }
}
