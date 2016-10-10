#include "table_opcodes.hpp"

// Parâmetros para serem passados pelos construtores
#define EXIT      "exit",     0
#define LOADI     "loadi",    1
#define STOREI    "storei",   2
#define ADD       "add",      3
#define SUBTRACT  "subtract", 4
#define MULTIPLY  "multiply", 5
#define DIVIDE    "divide",   6
#define JUMP      "jump",     7
#define JMPZ      "jmpz",     8
#define JMPN      "jmpn",     9
#define MOVE      "move",     10
#define LOAD      "load",     11
#define STORE     "store",    12
#define LOADC     "loadc",    13
#define CLEAR     "clear",    14
#define MOVESP    "moveSp",   15
#define SLT       "slt",      16
#define CALL      "call",     17
#define LOADSP    "loadSp",   18
#define STORESP   "storeSp",  19
#define RET       "ret",      20
#define LOADRA    "loadRa",   21
#define STORERA   "storeRa",  22
#define ADDI      "addi",     23
#define SGT       "sgt",      24
#define SEQ       "seq",      25
#define JMPP      "jmpp",     26

Opcode::Opcode(std::string name, int value) {
  this->name = name;
  this->value = value;
}

bool
TableOpcode::isOpcode(std::string name) {
  int size = this->opcode.size();
  for (int i = 0; i < size; i++)
    if (name.compare(this->opcode[i]->name) == 0) return true;
  return false;
}

std::string
TableOpcode::extract_opcode(std::vector<std::string> fields) {
  if (fields[0].back() == ':') // Eh label?
    return this->isOpcode(fields[1]) ? fields[1] : ""; // Pega o proximo token
  return this->isOpcode(fields[0]) ? fields[0] : "";
}

int
TableOpcode::get_opcode_value(std::string name) {
  int size = this->opcode.size();
  for (int i = 0; i < size; i++) {
    if (name.compare(this->opcode[i]->name) == 0) return this->opcode[i]->value;
  }
  return -1;
}

std::string
TableOpcode::get_data(std::vector<std::string> fields) {
  std::string r;

  if (this->location_counter == this->get_symbol_value(fields[0])) {
    int many = std::stoi(fields[2]);
    for (int i = 0; i < many; i++) {
      r += int_to_hex(this->location_counter);
      r += "        :  ";
      if (i == many-1){
        r += string_binary(std::stoi(fields[3])) + ";\n";
      }
      else { // Extendo números negativos com 1
        if (std::stoi(fields[3]) < 0)
          r += std::string("11111111") + ";\n";
        else
          r += std::string("00000000") + ";\n";
      }
      this->location_counter++;
    }
  }
  else{
    std::cerr << "Erro no endereço do símbolo "
      << fields[0] << std::endl;
    exit(EXIT_FAILURE);
  }
  return r;
}

std::string
TableOpcode::get_line_mif(std::vector<std::string> fields) {
  std::string r; // to return
  if (fields.size() == 0) return "";

  if (fields[0].back() == ':' && fields[0].front() == '_')
    fields.erase(fields.begin()); // Elimina label

  if (fields[0].back() == ':') {
    // do .data
    if (fields.size() >= 4 && fields[1].compare(".data") == 0) {
      r += this->get_data(fields);
    }
  }
  else {

    int binary = this->get_instruction(fields);

    r += int_to_hex(this->location_counter);
    r += "        :  ";
    r += string_binary( (binary & 0xFF00) >> 8 ) + ";\n";
    this->location_counter++;
    r += int_to_hex(this->location_counter);
    r += "        :  ";
    r += string_binary( binary & 0x00FF ) + ";\n";
    this->location_counter++;

    if (this->verbose) { // DEBUG
      std::cout << string_binary( (binary & 0xFF00) >> 8 )
        << string_binary( binary & 0x00FF ) << std::endl;
    }

  }

  if (this->verbose) std::cout << r; // DEBUG
  return r;
}

int
TableOpcode::get_register(std::string name) {
  if (name.size() == 2) {
    return (int) name[1];
  }
  else {
    std::cerr << "Registrador esperado: " << name << std::endl;
    exit(EXIT_FAILURE);
  }
  return 0;
}

int
TableOpcode::get_instruction(std::vector<std::string> fields) {
  int binary, choice, temp;
  binary = choice = this->get_opcode_value(fields[0]);
  
  temp = 0;
  switch (choice) {
    case 0: // EXIT      "exit",     0
      binary = 0;
      break;
    case 1: // LOADI     "loadi",    1
    case 2: // STOREI    "storei",   2
      temp = this->get_register(fields[1]);
      binary <<= 11;
      binary |= (temp << 8) & (7 << 8);
      if (isInteger(fields[2]))
        binary |= (std::stoi(fields[2])) & 0xFF;
      else
        binary |= (this->get_symbol_value(fields[2])) & 0xFF;
      break;
    case 3: // ADD       "add",      3
    case 4: // SUBTRACT  "subtract", 4
    case 5: // MULTIPLY  "multiply", 5
    case 6: // DIVIDE    "divide",   6
    case 10: // MOVE      "move",     10
    case 11: // LOAD      "load",     11
    case 12: // STORE     "store",    12
      binary <<= 11;
      temp = this->get_register(fields[1]);
      binary |= (temp << 8) & (7 << 8);
      temp = this->get_register(fields[2]);
      binary |= (temp << 5) & (7 << 5);
      break;
    case 7: // JUMP      "jump",     7
      binary <<= 11;
      if (isInteger(fields[1]))
        binary |= (std::stoi(fields[1])) & 0xFF;
      else
        binary |= (this->get_symbol_value(fields[1])) & 0xFF;
      break;
    case 8: // JMPZ      "jmpz",     8
    case 9: // JMPN      "jmpn",     9
    case 26: // JMPP      "jmpp",     26
      binary <<= 11;
      temp = this->get_register(fields[1]);
      binary |= (temp << 8) & (7 << 8);
      if (isInteger(fields[2]))
        binary |= (std::stoi(fields[2])) & 0xFF;
      else
        binary |= (this->get_symbol_value(fields[2])) & 0xFF;
      break;
    case 13: // LOADC     "loadc",    13
      binary <<= 11;
      temp = this->get_register(fields[1]);
      binary |= (temp << 8) & (7 << 8);
      if (isInteger(fields[2]))
        binary |= (std::stoi(fields[2])) & 0xFF;
      else {
        std::cerr << "Constante esperado em " << fields[2] << std::endl;
        exit(EXIT_FAILURE);
      }
      break;
    case 14: // CLEAR     "clear",    14
      binary <<= 11;
      temp = this->get_register(fields[1]);
      binary |= (temp << 8) & (7 << 8);
      break;
    case 15: // MOVESP    "moveSp",   15
    case 21: // LOADRA    "loadRa",   21
    case 22: // STORERA   "storeRa",  22
      binary <<= 11;
      if (isInteger(fields[1]))
        binary |= (std::stoi(fields[1])) & 0xFF;
      else {
        std::cerr << "Constante esperado em " << fields[1] << std::endl;
        exit(EXIT_FAILURE);
      }
      break;
    case 16: // SLT       "slt",      16
    case 24: // SGT       "sgt",      24
    case 25: // SEQ       "seq",      25
      binary <<= 11;
      temp = this->get_register(fields[1]);
      binary |= (temp << 8) & (7 << 8);
      temp = this->get_register(fields[2]);
      binary |= (temp << 5) & (7 << 5);
      temp = this->get_register(fields[3]);
      binary |= (temp << 2) & (7 << 2);
      break;
    case 17: // CALL      "call",     17
      binary <<= 11;
      binary |= (this->get_symbol_value(fields[1])) & 0xFF;
      break;
    case 18: // LOADSP    "loadSp",   18
    case 19: // STORESP   "storeSp",  19
      binary <<= 11;
      temp = this->get_register(fields[1]);
      binary |= (temp << 8) & (7 << 8);
      if (isInteger(fields[2]))
        binary |= (std::stoi(fields[2])) & 0xFF;
      else {
        std::cerr << "Constante esperado em " << fields[1] << std::endl;
        exit(EXIT_FAILURE);
      }
      break;
    case 20: // RET       "ret",      20
      binary <<= 11;
      break;
    case 23: // ADDI      "addi",     23
      binary <<= 11;
      temp = this->get_register(fields[1]);
      binary |= (temp << 8) & (7 << 8);
      if (isInteger(fields[2]))
        binary |= (std::stoi(fields[2])) & 0xFF;
      else {
        std::cerr << "Constante esperado em " << fields[1] << std::endl;
        exit(EXIT_FAILURE);
      }
      break;
  }
  return binary;
}

TableOpcode::TableOpcode() {
  // Insere todos os Opcodes em uma tabela
  this->opcode.push_back(new Opcode( EXIT ));
  this->opcode.push_back(new Opcode( LOADI ));
  this->opcode.push_back(new Opcode( STOREI ));
  this->opcode.push_back(new Opcode( ADD ));
  this->opcode.push_back(new Opcode( SUBTRACT ));
  this->opcode.push_back(new Opcode( MULTIPLY ));
  this->opcode.push_back(new Opcode( DIVIDE ));
  this->opcode.push_back(new Opcode( JUMP ));
  this->opcode.push_back(new Opcode( JMPZ ));
  this->opcode.push_back(new Opcode( JMPN ));
  this->opcode.push_back(new Opcode( MOVE ));
  this->opcode.push_back(new Opcode( LOAD ));
  this->opcode.push_back(new Opcode( STORE ));
  this->opcode.push_back(new Opcode( LOADC ));
  this->opcode.push_back(new Opcode( CLEAR ));
  this->opcode.push_back(new Opcode( MOVESP ));
  this->opcode.push_back(new Opcode( SLT ));
  this->opcode.push_back(new Opcode( CALL ));
  this->opcode.push_back(new Opcode( LOADSP ));
  this->opcode.push_back(new Opcode( STORESP ));
  this->opcode.push_back(new Opcode( RET ));
  this->opcode.push_back(new Opcode( LOADRA ));
  this->opcode.push_back(new Opcode( STORERA ));
  this->opcode.push_back(new Opcode( ADDI ));
  this->opcode.push_back(new Opcode( SGT ));
  this->opcode.push_back(new Opcode( SEQ ));
  this->opcode.push_back(new Opcode( JMPP ));
}
