#include "process.hpp"
#include <iostream>
#include <vector>
#include <atomic>

using namespace std;
using namespace TinyProcessLib;

int main() {
  atomic<bool> stdout_error(false);
  atomic<bool> exit_status_error(false);
  vector<thread> threads;
  for(size_t ct=0;ct<4;ct++) {
    threads.emplace_back([&stdout_error, &exit_status_error, ct]() {
      for(size_t c=0;c<2500;c++) {
#ifdef _WIN32
        const Process::string_type cmd  = L"echo Hello World " + to_wstring(c) + L" " + to_wstring(ct);
        const Process::string_type path;
#else
        const Process::string_type cmd  = "echo Hello World " + to_string(c) + " " + to_string(ct);
        const Process::string_type path = "";
#endif
        Process process(cmd, path, [&stdout_error, ct, c](const char *bytes, size_t n) {
        const auto actual = string(bytes, n);
#ifdef _WIN32
        const auto line_end = "\r\n";
#else
        const auto line_end = "\n";
#endif
        const auto expected = "Hello World " + to_string(c) + " " + to_string(ct) + line_end;
        if(actual != expected)
          stdout_error=true;
        }, [](const char*, size_t) {
        }, true);
        const auto exit_status = process.get_exit_status();
        if(exit_status != EXIT_SUCCESS)
          exit_status_error=true;
        if(exit_status_error)
          return;
        if(stdout_error)
          return;
      }
    });
  }
  
  for(auto &thread: threads)
    thread.join();
  
  if(stdout_error) {
    cerr << "Wrong output to stdout." << endl;
    return 1;
  }
  if(exit_status_error) {
    cerr << "Process returned failure." << endl;
    return 1;
  }
  
  return 0;
}