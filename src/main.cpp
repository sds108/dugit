#include "include.h"
#include "git.h"
#include "session.h"

Session* session;

// SIGINT Handler
void sig_handler (int signum) {
  std::cout << "\nSignal Interrupt Sequence...\n";
  std::cout << session << std::endl;

  // Terminate program sequence
  if (session != NULL) {
    std::cout << "Attempting to clean up session...\n";
    session->clean_up();
    delete(session);
    session = NULL;
  } exit(signum);
}

int main (int argc, char* argv[]) {
  std::vector<std::string> args;
  for (int arg = 0; arg < argc; arg++)
    args.push_back(argv[arg]);
  if (args.empty()) return 1;
  args.erase(args.begin());

  // Signal handler
  signal(SIGINT, sig_handler);
  session = new Session;
  if (session == NULL) return 1;

  // Perform Startup Sequence
  if (session->session_startup_sequence())
    // Execute args
    session->args_parser(args);
  
  // Delete session
  if (session != NULL) {
    delete(session);
    session = NULL;
  } return 0;
}