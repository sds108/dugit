#include "include.h"
#include "git.h"
#include "session.h"

int main (int argc, char* argv[]) {
  std::vector<std::string> args;
  for (int arg = 0; arg < argc; arg++)
    args.push_back(argv[arg]);
  if (args.empty()) return 1;
  args.erase(args.begin());
  Session session(args);
  return 0;
}