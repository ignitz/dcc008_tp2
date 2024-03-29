#include "Programa.hpp"

Programa::Programa ( std::string name, bool bWrite ) {
  std::string modMif, modSbtl;

  // Verifica se já tem a extensão mif
  if (name.substr(name.find_last_of("."), name.size()).compare(".mif") == 0) {
    modMif  = name;
    modSbtl = name.substr(0, name.find_last_of(".")) + ".stbl";
  }
  else {
    modMif  = name + ".mif";
    modSbtl = name + ".stbl";
  }

  this->setName(name);
  this->size = 0;

  if (bWrite) { // Abre os arquivos pra leitura-escrita
    this->fileMif.open( modMif , std::ios::out | std::ios::in | std::ios::trunc );
    this->fileStbl.open( modSbtl , std::ios::out | std::ios::in | std::ios::trunc );
  }
  else { // Por padrão abre os arquivos pra leitura
    this->fileMif.open( modMif , std::ios::in );
    this->fileStbl.open( modSbtl , std::ios::in );
  }

  if (!this->fileMif.is_open() || !this->fileStbl.is_open()) {
    std::cerr << "Erro ao abrir " << this->getName() << std::endl;
    exit(EXIT_FAILURE);
  }

  // Lê a tabela de símbolos
  this->readTable();
}

Programa::Programa ( std::string name ) : Programa ( name, false ) {

}

Programa::~Programa () {
  this->fileMif.close();
  this->fileStbl.close();
}

std::string // Retorna nome do programa
Programa::getName () {
  return this->name;
}

bool // Seta nome do programa
Programa::setName ( std::string name ) {
  this->name = name;
  return true; // TODO Quando pode dar false?
}

int // Retorna tamanho do programa
Programa::getSize() {
  return this->size;
}

int // Seta tamanho do programa
Programa::setSize(int value) {
  this->size = value;
  return value;
}

bool // Seta modo verbose
Programa::setVerbose ( bool bVerbose ) {
  this->bVerbose = bVerbose;
  this->tableLocal.setVerbose(bVerbose);
  this->tableExtern.setVerbose(bVerbose);
  return this->bVerbose;
}

// Retorna matriz de tokens do arquivo mif
std::vector<std::vector<std::string>>
Programa::getFileMif() {
  std::string line;
  std::vector<std::string> fields;
  std::vector<std::vector<std::string>> r;
  int state = 0;

  while(std::getline(this->fileMif, line)) {
    if (line.compare("BEGIN") == 0)
      state++;
    if (line.compare("END") == 0)
      break;

    if ( (state == 1) && (line.size() >= 8) ) {
      if (line[0] == '[') break;
      fields = tokenizer(line, "\t :;");
      for (size_t i = 0; i < fields.size(); i++)
        if (fields[i].size() == 0) fields.erase(fields.begin() + i);
      r.push_back(fields);
    }
  }

  if (state == 1) {
    return r;
  }
  else {
    std::cerr << "Erro na leitura do arquivo MIF " <<
        this->getName() << std::endl;
    exit(EXIT_FAILURE);
  }
}

void // Escreve uma string no arquivo
Programa::writeFileMif( std::string buffer ) {
  this->fileMif << buffer;
}

void // Se posiciona no início do arquivo
Programa::setBeginMif() {
  this->fileMif.seekg(0, this->fileMif.beg);
}

std::string // Lê uma linha e volta para a posição original
Programa::getLineNoMove() {
  std::string line;
  std::streampos iPosition = this->fileMif.tellg();
  std::getline (this->fileMif, line);
  this->fileMif.seekg (iPosition);
  return line;
}

std::string // Lê a linha abaixo e volta para a posição original
Programa::getSecondLineNoMove() {
  std::string line;
  std::streampos iPosition = this->fileMif.tellg();
  std::getline (this->fileMif, line);
  std::getline (this->fileMif, line);
  this->fileMif.seekg (iPosition);
  return line;
}

std::string // Lê uma linha no arquivo
Programa::getLine() {
  std::string line;
  std::getline (this->fileMif, line);
  return line;
}

bool // Lê o arquivo ".sbtl" contendo a tabela de símbolos
Programa::readTable () {

  if (!this->fileStbl.is_open()) { // Verifica se o arquivo está aberto.
    std::cerr << "Erro em abrir o arquivo " << this->getName() << std::endl;
    return false;
  }

  std::string line;
  std::vector<std::string> fields;
  std::vector<std::string> aux;
  int state = 0;

  this->fileStbl.seekg(0, this->fileStbl.beg);
  while (getline(this->fileStbl, line)) {
    // Separa a linha em tokens
    fields = tokenizer(line, "\t ");

    switch (state) {
      case 0: // Size
        if (fields.size() == 2) {
          if (fields[0].compare("Size:") == 0) {
            this->size = std::stoi (fields[1], nullptr, 0);
            state++;
          }
          else { // Se encontrar um erro entra aqui
            std::cerr << "Error em ler Size: " << std::endl;
            for (auto const& field : fields)
              std::cerr << field << std::endl;
            return false;
          }
        }
        else { // Size em formato inválido
          std::cerr << "Size não contém dois operandos: " << std::endl;
          for (auto const& field : fields)
            std::cerr << field << std::endl;
          return false;
        }
      break;

      case 1: // Símbolos locais
      case 2: // Extern
        if ( fields.size() >= 2 ) { // Tem que ter o valor mais pelo menos uma localização
          state == 1 ? this->tableLocal.insertSymbol ( fields[0] ) : this->tableExtern.insertSymbol ( fields[0] );
          aux = fields;
          aux.erase(aux.begin());
          for (auto const& field : aux) {
            if ( field.substr(0, 2).compare("0x") == 0 ) {
              state == 1 ? this->tableLocal.insertValue( std::stoi (field, nullptr, 0) ) : this->tableExtern.insertValue( std::stoi (field, nullptr, 0) );
            }
            else { // Se não reconhecer como hexadecimal
              std::cerr << "Formato do " << field << " é hexadecimal inválido" << std::endl;
              return false;
            }
          }
        }
        else if ( fields.empty() ) { // Linha vazia
          // Do nothing
          continue;
        }
        else if ( fields[0].compare("EXTERN:") == 0 ) {
          state++;
        }
        else { // Formato inválido
          std::cerr << "Símbolo em formato inválido: " << std::endl;
          for (auto const& field : fields)
            std::cerr << field << std::endl;
          return false;
        }
      break;

      default: // Caso ocorra algo inexperado
        std::cerr << "Duendes alteraram a variável state para " <<
            state << "." << std::endl;
        return false;
      break;
    } // end Switch
  } // end While

  return state == 2 ? true : false;
}

bool // Verifica se o símbolo já existe na tabela local
Programa::checkSymbolLocal( std::string name ) {
  return this->tableLocal.checkSymbol( name );
}

bool // Verifica se o símbolo já existe na tabela extern
Programa::checkSymbolExtern( std::string name ) {
  return this->tableExtern.checkSymbol( name );
}

int // Retorna valor a ser substituído do símbolo
Programa::getLocalSymbolValue( std::string name ) {
  return this->tableLocal.getValue(name);
}

// Retorna vetor de nomes de símbolos locais
std::vector<std::string>
Programa::getLocalNames() {
  return this->tableLocal.getNames();
}

// Retorna todas as ocorrências do símbolo
std::vector<int>
Programa::getLocalLocations( std::string name ) {
  return this->tableLocal.getLocations( name );
}

int // Retorna valor a ser substituído do símbolo
Programa::getExternSymbolValue( std::string name ) {
  return this->tableExtern.getValue(name);
}

// Retorna vetor de nomes de símbolos extern
std::vector<std::string>
Programa::getExternNames() {
  return this->tableExtern.getNames();
}

// Retorna todas as ocorrências do símbolo
std::vector<int>
Programa::getExternLocations( std::string name ) {
  return this->tableExtern.getLocations( name );
}

bool // Seta valor do símbolo
Programa::setExternValue( std::string name , int value ) {
  return this->tableExtern.setValue( name, value );
}

void // Translada os símbolos
Programa::translatePositionLocal(int move) {
  this->tableLocal.translateSymbol(move);
}

void // Imprime todo o conteúdo do módulo
Programa::printAllData () {
  std::cout << "Programa:" << this->getName() << std::endl;
  std::cout << "Size = " << std::dec << this->size <<
      " 0x" << std::hex << this->size << std::endl;
  std::cout << "--- Local ----" << std::endl;
  this->tableLocal.printSymbols();
  std::cout << "--- Extern ---" << std::endl;
  this->tableExtern.printSymbols();
  std::cout << "================" << std::endl;
}
