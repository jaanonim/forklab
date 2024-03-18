# forklab

CLI tool for automated forking and cloning repos on gitlab.

### Installation

You can download latest executable from github or build from sources.

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

### Config

You can change location of config file by setting env variable `FORKLAB_CONFIG_PATH`. By default, it's `~/.config/`

**Warning config file include your gitlab auth token so don't share it!**

### Used libraries:

- [cpp-inquirer](https://github.com/aelliixx/cpp-inquirer)
- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- [spinners](https://github.com/jkuri/spinners)
- [{fmt}](https://github.com/fmtlib/fmt)
- [json](https://github.com/nlohmann/json)
