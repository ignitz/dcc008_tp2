#include "Linker.hpp"

#define HEAD_MIF "DEPTH = 256;\nWIDTH = 8;\nADDRESS_RADIX = HEX;\nDATA_RADIX = BIN;\nCONTENT\nBEGIN\n\n"
#define END_MIF "END;\n"


Linker::Linker( std::vector<std::string> sArgs ) {
  this->bVerbose = false;
  this->iNumArgs = sArgs.size();

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

  this->output->writeFileMif(HEAD_MIF);

  auto fields = this->mainProg->getFileMif();
  for (auto& linefields : fields) {
    if (linefields.size() >= 2) {
      this->output->writeFileMif(linefields[0] + "        :  ");
      this->output->writeFileMif(linefields[1] + ";\n");
    }
  }

  int iMove = this->mainProg->getSize();
  if (iMove % 2 != 0) iMove++; // Alinhamento par bytes
  std::string address;

  for (auto& modulo : this->modulos) {
    fields = modulo->getFileMif();
    for (size_t i = 0; i < fields.size(); i++) {
      if (fields[i].size() >= 2) {
        // Translada e converte pra hex
        address = int_to_hex(std::stoi (fields[i][0].insert(0, "0x"), nullptr, 0) + iMove);
        // Deixa o endereço com caixa alta
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
  // Deixa o endereço com caixa alta
  transform(address.begin(), address.end(), address.begin(), ::toupper);
  this->output->writeFileMif(address + " :  00000000;\n");
  this->output->writeFileMif(END_MIF);

}

void
Linker::updateAddress () {
  std::string line1, line2;
  std::vector<std::string> fields1, fields2;

  if (this->mainProg->getSize() % 2 != 0) // Faz um alinhamento para par bytes
    this->mainProg->setSize(this->mainProg->getSize() + 1);
  int sumSize = this->mainProg->getSize();

  for (auto const& modulo : this->modulos) {
    if (modulo->getSize() % 2 != 0) // Faz um alinhamento para par bytes
      modulo->setSize(modulo->getSize() + 1);
    modulo->translatePositionLocal(sumSize);
    sumSize += modulo->getSize();
  }

  // TODO Atualizar os extern todos
  // Então, a ideia é procurar por todas as tabelas até achar e daí atualizar o valor correto


  this->output->setBeginMif();
  this->setBinMif(this->output, 10, 255);

  if (this->bVerbose) {
    std::cout <<
    std::endl <<
    "Imprimindo as tabelas após as atualizações:" << std::endl;
    this->printAllData();
  }
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

  if ( std::stoi(line.substr(0, 2), nullptr, 16) == iAddress ) {
    insert = line.substr(0, line.find_last_of(":")) + ":  " + int_to_binary(value);
    programa->writeFileMif(insert);
  };
}

void
Linker::printAllData () { // Imprime todo o conteúdo do linkador
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
