#include "Programa.hpp"

Programa::Programa ( std::string name, bool bWrite ) {
  std::string modMif, modSbtl;
  modMif  = name + ".mif";
  modSbtl = name + ".stbl";
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

std::string
Programa::getName () {
  return this->name;
}

bool
Programa::setName ( std::string name ) {
  this->name = name;
  return true; // TODO Quando pode dar false?
}

int
Programa::getSize() {
  return this->size;
}

int
Programa::setSize(int value) {
  this->size = value;
  return value;
}

bool
Programa::setVerbose ( bool bVerbose ) {
  this->bVerbose = bVerbose;
  this->tableLocal.setVerbose(bVerbose);
  this->tableExtern.setVerbose(bVerbose);
  return this->bVerbose;
}

bool
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
    boost::split(fields, line, boost::is_any_of("\t "), boost::token_compress_on);

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

void
Programa::translatePositionLocal(int move) {
  this->tableLocal.translateSymbol(move);
}

void
Programa::translatePositionExtern(int move) {
  // this->tableExtern.translateSymbol(move);
}

void
Programa::printAllData () { // Imprime todo o conteúdo do módulo
  std::cout << "Programa:" << this->getName() << std::endl;
  std::cout << "Size = " << this->size << std::endl;
  std::cout << "--- Local ----" << std::endl;
  this->tableLocal.printSymbols();
  std::cout << "--- Extern ---" << std::endl;
  this->tableExtern.printSymbols();
  std::cout << "================" << std::endl;
}
