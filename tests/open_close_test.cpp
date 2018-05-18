#include "process.hpp"
#include <iostream>

using namespace std;
using namespace TinyProcessLib;

int main() {
  bool stdout_error=false;
  for(size_t c=0;c<10000;c++) {
#ifdef _WIN32
    const Process::string_type cmd = L"echo Hello World "+to_wstring(c);
#else
    const Process::string_type cmd = "echo Hello World "+to_string(c);
#endif
    const Process::string_type arg;
    Process process(cmd, arg, [&stdout_error, c](const char *bytes, size_t n) {
#ifdef _WIN32
    const auto line_end = "\r\n";
#else
    const auto line_end = "\n";
#endif
      if(string(bytes, n)!="Hello World "+to_string(c)+line_end)
        stdout_error=true;
    }, [](const char *, size_t) {
    }, true);
    const auto exit_status=process.get_exit_status();
    if(exit_status!=0) {
      cerr << "Process returned failure." << endl;
      return 1;
    }
    if(stdout_error) {
      cerr << "Wrong output to stdout." << endl;
      return 1;
    }
  }
 
  return 0;
}