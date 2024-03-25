# forklab

CLI tool for automated forking and cloning repos on gitlab.

### Installation

You can download latest executable from [github releases](https://github.com/jaanonim/forklab/releases) or build from
sources.

### Build

Requires: `cmake` and `OpenSSL`

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

Your executable is `build/forklab`

### Usage

Run `forklab h` for help.

```
   __           _    _       _     
  / _|         | |  | |     | |    
 | |_ ___  _ __| | _| | __ _| |__  
 |  _/ _ \| '__| |/ / |/ _` | '_ \ 
 | || (_) | |  |   <| | (_| | |_) |
 |_| \___/|_|  |_|\_\_|\__,_|_.__/ 
                                   
Tool for automated forking and cloning repos on gitlab.

Usage: forklab [COMMAND]

Run without arguments to fork and clone projects in interactive mode.
Commands:
                auth            Sets auth token for gitlab.
        c,      create          Create new group interactively.
        d,      delete          Delete group of given index.
        h,      help            Shows this help.
        l,      list            Lists groups.

Detailed usage:
   forklab
        Runs in interactive mode enables to fork and clone projects for configured groups.

   forklab auth TOKEN
        Pass gitlab personal auth token as TOKEN argument.

   forklab create
        Run's create in interactive mode.

   forklab create NAME ID_IN ID_OUT PATH [COMMAND]
        Crate group with given name that will get projects from gitlab group of ID_IN and fork it
        to group of ID_OUT and then clone it to PATH (need to be absolute path to existing folder).
        Optionally can COMMAND can be passed that will be run in director of newly forked project.

   forklab delete
        Run's delete in interactive mode.

   forklab delete NAME
        Deletes group with given NAME.

   forklab help
        Prints this beautiful help.

   forklab list
        Lists all created groups.
```

To get your personal auth token
follow [this link](https://gitlab.com/-/user_settings/personal_access_tokens?name=Forklab&scopes=api) and
press `Add new token`. (then run `forklab auth YOUR_TOKEN`)

To find group id follow [this tutorial](https://docs.gitlab.com/ee/user/group/#get-the-group-id).

### Config

You can change location of config file by setting env variable `FORKLAB_CONFIG_PATH`. By default, it's `~/.config/`

**Warning config file include your gitlab auth token so don't share it!**

### Used libraries

- [cpp-inquirer](https://github.com/aelliixx/cpp-inquirer)
- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- [spinners](https://github.com/jkuri/spinners)
- [{fmt}](https://github.com/fmtlib/fmt)
- [json](https://github.com/nlohmann/json)

### Contributing

Feel free to open issues and pull requests.
If you want to port it to windows, I will be happy to point potential things that need to be changed and accept PR.
