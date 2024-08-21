# dugit - (Dupe Git) - Version 0.0.2
Multi-remote synchronisation tool for git.

## Notice
This tool is currently still a **work-in-progress**. While the `sync` and `commit` commands have worked for the multiple test runs I have made, I have yet to make unit tests for these two commands (will need to create unit tests to work on a dummy test repository). There are many scenarios I have tested, especially regarding merge conflict handling (my biggest worry). Removed --autostash flag as I don't like how it works, instead manually stashing (which can be recorded as having happened within the program). Implemented SIGINT handling, however again, will need extensive unit testing (interrupting at every point of the program, under every scenario). So far, have only used with **SSH**, I have no idea if this works with other types of credentials, but it should. All memory management testing is done via **Valgrind**. Dugit is set up so that you can only use one instance of dugit on a repository at a time.

---
### Installation
- Clone this repository via
```
git clone --branch=main git@github.com:sds108/test.git
```
- System-wide install can be done via (just make sure you are inside the dugit repository when you do this)
```
cd dugit && sudo /usr/bin/cmake --build ./build --config Debug --target install -j 18 --
```
- To uninstall, there currently is **no** uninstall script, so you must go and delete the `dugit` directory found at `/usr/local/bin`.
```
sudo rm -rf /usr/local/bin/dugit
```
---
### Usage
```
usage: dugit <command> [<args>]
```
---
**These are common Dugit arguments, please use carefully:**
```
--abort-merge   When merging branches, any Dugit command will automatically
                quit if a merge conflict is detected, and leave the merge
                conflict for the user to fix. If you want Dugit to
                automatically abort the merge, use this flag.

--stage-all     If you wish to stage (add) all changes made to the repository
                including untracked changes (not yet tracked by git, if you do
                'git status' git will tell you if you have any untracked changes),
                you can use this flag to stage untracked changes. Otherwise, by
                default, Dugit stages only tracked changes.

--commit        When using the "sync" command, by default, Dugit performs an
                automatic stash so that any uncommited changes (both staged and
                unstaged) are stashed for the duration of the syncing, and will
                try to be reapplied (popped) back once the syncing is done.
                If there is work that the user would like to instead first commit
                before syncing, the user may use this flag to do so.

--auto-message  When committing any work or merge, by default the user will be
                prompted to enter a custom commit message. This flag can be enabled
                to skip this step and let Dugit make an automatic commit message.

--no-warning    When a Dugit command is executed, Dugit will prompt the user
                with verification questions, giving the user a chance to stop
                further execution of the command if desired. To skip these
                warnings, use this flag.

--fast-forward  When merging, by default Dugit does not allow fast forwarding,
                but if desired, the user may this flag to allow fast forwarding.
```
---
**These are common Dugit commands used in various situations:**

<span style="color:red;">***Please read how to use flags before using commands***</span>
```
help            Get information on how to use Dugit (this command).

examples        Get examples of usage.

version         Use this command to get the currently installed version of dugit.

sync    <args>  This command's main purpose sync each of the connected remotes with
                each other, this entails fetching changes from each remote,
                merging on the local repository, committing all of the merges,
                and then pushing back to each remote.

commit  <args>  This command's main purpose is to stage and commit all local
                changes on the local repository. This command does not push or sync
                with the remote repositories. If you want to automatically push
                the commit as well, look into using the "sync" command with the
                "--commit" flag. (See examples at the end)
```
### Example Usage
- To find out the installed version of dugit, one can use the following command.
  > dugit version
---
- If one is stuck and does not know how to use dugit, the same information as displayed in this README will be printed to one's command-line via this command.
  > dugit help

#### Committing
- Commit only tracked changes. The user will be asked whether they want to ingnore the untracked changes.
  > dugit commit
---
- Commit only tracked changes, and avoid the untracked changes warning.
  > dugit commit --no-warning
---
- Commit only tracked changes, avoid the untracked changes warning, and generate an automatic commit message. This is the quickest way to commit your work.
  > dugit commit --no-warning --auto-message
---
- Commit everything, including untracked changes.
  > dugit commit --stage-all 
---
- Commit everything, including untracked changes, with an automatic message. This is the fastest way to commit all of your changes.
  > dugit commit --stage-all --auto-message

- **Notice**, using `--no-warning` when `--stage-all` is enabled with the `commit` command does nothing, as the only warning one may get while committing is to check whether one indeed wants to not stage the untrackted changes within the repository.

#### Syncing
- The simplest way to sync one's local and remote repositories (without committing one's un-committed local changes) is to use the `sync` command in its simplest form. This process stashes any local un-committed changes, fetches changes from each remote repository connected, attempts to merge these remote changes into the local repository, and commits all of the merges. After this, the local merge of all remote repositories is pushed back to each remote repository, and at the end, any un-committed changes that were stashed earlier are popped back.
  > dugit sync
- **Notice**, any flags used for committing will apply to committing of the merges at the end. If using `--auto-message`, an automatic merge commit message will be generated. If not using `--no-warning` you will be asked about any untracked changes and whether you would like to stage them. If you use the `--stage-all` flag, **untracked changes will be automatically added to your merge commit**, this will have to be fixed in later patches.
---
- If the user would like to fast forward merge, this can be enabled simply with the following.
  > dugit sync --fast-forward
---
- If the user would like to abort if any of the merges fail, this can be done using the following command. Here, the merge will be aborted, and dugit will attempt to pop back any stashed changes.
  > dugit sync --abort-merge
---
- If the user would like to commit local un-committed changes (staged and unstage), the latter can be achieved via the following command.
  > dugit sync --commit
- **Notice**, any flags used for committing will apply to both committing of the local changes, and committing of the merges at the end. If using `--auto-message`, an automatic merge commit message will be generated when committing merges. If not using `--no-warning` you will once again be asked about any untracked changes and whether you would like to stage them.
---
- One can use all of the flags mentioned for committing in the latter section with the `sync` command in the same way. Having read the previous sections, one should understand what happens in each combination of each flag.
  ```
  dugit sync --commit --stage-all
  dugit sync --commit --stage-all --abort-merge
  dugit sync --commit --stage-all --abort-merge --auto-message
  dugit sync --commit --stage-all --abort-merge --auto-message --no-warning
  dugit sync --commit --stage-all --abort-merge --auto-message --no-warning --fast-forward
  ```