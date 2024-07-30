#include "include.h"
#include "git.h"

int main (int argc, char* argv[]) {
  std::vector<std::string> args;
  for (int arg = 0; arg < argc; arg++)
    args.push_back(argv[arg]);
  Session session(args);
  return 0;
}