#include "util.hpp"

bool
clearLine(std::string& line) {
  using namespace std;
  if (line.find(";") != string::npos) // Retira comentários
    line.erase(line.find(";"), line.size() - line.find(";"));

  while (line.find("\t") != string::npos) // Retira tabs
    line.replace(line.find("\t"), 1, " ");
  while (line.find("  ") != string::npos) // Retira espaços duplos
    line.replace(line.find("  "), 2, " ");
  if (line.find_first_of(" ") == 0)
    line.replace(line.find_first_of(" "), 1, "");
  if (line.find_last_of(" ") == 0)
      line.replace(line.find_last_of(" "), 1, "");
  return line.size() == 0 ? true : false; // Retorna true se a linha ficou vazia
}


// Imprime 8 bits em binário
std::string
string_binary(int x) {
  std::string r;

  for (int i = 0; i < 8; i++) {
    r += std::to_string((x & 0x80) >> 7);
    x <<= 1;
  }
  return r;
}

std::string
int_to_hex( int i ) {
  std::stringstream stream;
  stream << std::setfill ('0') << std::setw(2)
         << std::hex << i;
  return stream.str();
}

bool
isInteger(const std::string & s) {
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

void
print(std::vector <std::string> & v)
{
  using namespace std;
  for (size_t n = 0; n < v.size(); n++)
    cout << "\"" << v[ n ] << "\"\n";
  cout << endl;
}
