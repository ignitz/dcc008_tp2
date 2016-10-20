#include "Linker.hpp"

#define HEAD_MIF "DEPTH = 256;\nWIDTH = 8;\nADDRESS_RADIX = HEX;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n"
#define END_MIF "END;\n"


Linker::Linker( std::vector<std::string> sArgs ) {
  this->bVerbose = false;

  // Checar se o arquivo já existe
  this->output = new Programa(sArgs[0], true);
  sArgs.erase(sArgs.begin());

  this->mainProg = new Programa(sArgs[0]);
  sArgs.erase(sArgs.begin());

  for(auto const& value: sArgs) {
    this->modulos.push_back(new Programa(value));
  }
}

Linker::Linker( std::vector<std::string> sArgs, bool bVerbose ) : Linker(sArgs) {
  this->bVerbose = bVerbose;
  if (this->bVerbose) {
    std::cout << "Setando verbose nos módulos" << std::endl;
    this->output->setVerbose(bVerbose);
    this->mainProg->setVerbose(bVerbose);
    for(auto const& modulo: this->modulos) {
      modulo->setVerbose(bVerbose);
    }
    std::cout << "Imprimindo a tabela original (sem deslocamento):" << std::endl;
    this->printAllData();
  }
}

Linker::~Linker() {
  if (this->bVerbose) std::cout << "Liberando mainProg." << std::endl;
  delete mainProg;
  if (this->bVerbose) std::cout << "Liberando output." << std::endl;
  delete output;
  if (this->bVerbose) std::cout << "Liberando módulos:" << std::endl;
  for(auto const& modulo: this->modulos) {
    if (this->bVerbose) std::cout << modulo->getName() << std::endl;
    delete modulo;
  }
}

void // Aqui concatena todos os MIFs em um arquivo temporário
Linker::appendFiles () {

  this->output->writeFileMif(HEAD_MIF); // Insere o cabeçalho do arquivo

  // Copia todo o conteúdo do mainProg ao início do output
  auto fields = this->mainProg->getFileMif();
  for (auto& linefields : fields) {
    if (linefields.size() >= 2) {
      this->output->writeFileMif(linefields[0] + "        :  ");
      this->output->writeFileMif(linefields[1] + ";\n");
    }
  }

  // iMove serve pra relocacar transladando os endereços
  int iMove = this->mainProg->getSize();
  if (iMove % 2 != 0) iMove++; // Alinhamento em par bytes

  std::string address;

  // Neste bloco adiciona os módulos restantes em output
  for (auto& modulo : this->modulos) {
    fields = modulo->getFileMif();
    for (size_t i = 0; i < fields.size(); i++) {
      if (fields[i].size() >= 2) {
        // Translada e converte pra hex
        address = int_to_hex(std::stoi (fields[i][0].insert(0, "0x"), nullptr, 0) + iMove);
        transform(address.begin(), address.end(), address.begin(), ::toupper);
        this->output->writeFileMif(
          address + "        :  " + fields[i][1] + ";\n");
      }
    }
    iMove += modulo->getSize();
    if (iMove % 2 != 0) iMove++; // Alinhamento par bytes
  }

  // Adiciona zeros até o endereço FF
  address = "[" + int_to_hex(iMove) + "..FF]";

  transform(address.begin(), address.end(), address.begin(), ::toupper);
  this->output->writeFileMif(address + " :  00000000;\n");
  this->output->writeFileMif(END_MIF);
}

void // Atualiza todas os endereços dos símbolos locais
Linker::updateAddress () {
  std::vector<std::string> symbol_names;
  std::vector<int> locations;
  int value;

  if (this->mainProg->getSize() % 2 != 0) // Faz um alinhamento para par bytes
    this->mainProg->setSize(this->mainProg->getSize() + 1);
  int sumSize = this->mainProg->getSize();

  // Neste bloco é relocado todos os endereços dos módulos
  for (auto const& modulo : this->modulos) {
    if (modulo->getSize() % 2 != 0)
      modulo->setSize(modulo->getSize() + 1);
    modulo->translatePositionLocal(sumSize); // Translada a posição do símbolo
    sumSize += modulo->getSize();
  }

  // Procura por todas as tabelas até achar e daí atualizar o valor correto
  this->updateAllExterns(this->mainProg);
  for (auto& modulo : this->modulos ) {
    this->updateAllExterns(modulo);
  }

  // Aqui insere os valores atualizados de todos os símbolos locais
  for (auto & modulo : this->modulos) {
    symbol_names = modulo->getLocalNames();
    for (auto & name : symbol_names) {
      value = modulo->getLocalSymbolValue(name);
      locations = modulo->getLocalLocations(name);
      for (auto & location : locations) {
        // Aproveita já atualiza todos os endereços no output MIF
        this->setBinMif(this->output, location + 1, value);
      }
    }
  }

  if (this->bVerbose) {
    std::cout <<
    std::endl <<
    "Imprimindo as tabelas após as atualizações:" << std::endl;
    this->printAllData();
  }
}

// Atualiza todos os endereos dos símbolos Externs
std::vector<Symbol*>
Linker::updateAllExterns( Programa* programa ) {
  if (this->bVerbose) {
    std::cout << "Atualizando Externs do programa " <<
        programa-> getName() << std::endl;

  }
  std::vector<Symbol*> symbols;

  for ( auto & name : programa->getExternNames() ) {
    symbols.push_back(new Symbol (name));
    symbols[symbols.size()-1]->location = programa->getExternLocations(name);

    for ( auto const& modulo : this->modulos ) { // Pega o último valor declarado
      if (modulo->checkSymbolLocal(name))
        symbols[symbols.size()-1]->value = modulo->getLocalSymbolValue( name );
    }
    for ( auto const& location : symbols[symbols.size()-1]->location )
      this->setBinMif(this->output, location + 1, symbols[symbols.size()-1]->value);

    programa->setExternValue( name, symbols[symbols.size()-1]->value );
  }

  return symbols;
}

void // Insere o dado em binario em tal lugar xD
Linker::setBinMif( Programa* programa, int iAddress, int value) {
  std::string line, insert;
  programa->setBeginMif();
  while(true) { //Se posiciona até depois de BEGIN
    if ( programa->getLineNoMove().compare("") == 0
      || programa->getLine().compare("BEGIN") == 0)
      break;
  }
  programa->getLine(); // Espero que sempre tenha essa linha vazia

  for (int i = 0; i < iAddress; i++)
    programa->getLine();

  line = programa->getLineNoMove();
  if (this->bVerbose){
    std::cout << "String do arquivo: ";
    std::cout << line << std::endl;
  }

  if ( std::stoi(line.substr(0, 2), nullptr, 16) == iAddress ) {
    insert = line.substr(0, line.find_last_of(":")) + ":  " + int_to_binary(value) + ";";
    programa->writeFileMif(insert);

    if (this->bVerbose){
      std::cout << "String modificada: " ;
      std::cout << insert << std::endl;
    }
  };
}

void // Imprime todo o conteúdo do linkador
Linker::printAllData () {
  std::cout << "Executando printAllData do Linker" << std::endl;
  std::cout << "- Arquivo de saída MIF" << std::endl;
  this->output->printAllData();
  std::cout << "- Main" << std::endl;
  this->mainProg->printAllData();
  std::cout << "- Módulos" << std::endl;
  for (auto const& modulo : this->modulos) {
    modulo->printAllData();
  }
}
