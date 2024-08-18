# dugit - (Dupe Git) - Version 0.0.1
Multi-remote synchronisation tool for git.

## Notice
This tool is currently still a **work-in-progress**. While the `sync` and `commit` commands have worked for the multiple test runs I have made, I have yet to make unit tests for these two commands (will need to create unit tests to work on a dummy test repository). There are many scenarios I have tested, especially regarding merge conflict handling (my biggest worry). Removed --autostash flag as I don't like how it works, instead manually stashing (which can be recorded as having happened within the program). Implemented SIGINT handling, however again, will need extensive unit testing (interrupting at every point of the program, under every scenario). The `help` usage is incomplete and will need more work, it's also missing usage examples (might make an `examples` command to show every possible use case). So far, have only used with **SSH**, I have no idea if this works with other types of credentials, but it should. All memory management testing is done via **Valgrind**. Dugit is set up so that you can only use one instance of dugit on a repository at a time.

### Usage
...Needs work

### Installation
- Clone this repository via
```
git clone --branch=main git@github.com:sds108/test.git
```
- System-wide install can be done via (just make sure you are inside the dugit repository when you do this)
```
cd ~/dugit && sudo /usr/bin/cmake --build ./build --config Debug --target install -j 18 --
```
- To uninstall, there currently is **no** uninstall script, so you must go and delete the `dugit` directory found at `/usr/local/bin`.
```
sudo rm -rf /usr/local/bin/dugit
```