//
// Created by jaanonim on 12.03.24.
//
#include <iostream>
#include <utility>

class CodeException : public std::exception {
public:
    virtual char *what() = 0;

    const int code = 1;
};

class MessageException : public CodeException {
public:
    explicit MessageException(std::string message) : message(std::move(message)) {}

    std::string message;
};

class MissingHomeEnv : public CodeException {
    char *what() override {
        return "Missing env home variable.";
    }

    const int code = 10;
};

class CannotSaveConfig : public CodeException {
    char *what() override {
        return "Cannot save config.";
    }

    const int code = 12;
};

class CannotReadConfig : public MessageException {
public:
    explicit CannotReadConfig(const std::string &m) : MessageException(m) {}

    char *what() override {
        return "Cannot read config.";
    }

    const int code = 11;
};

class InvalidArgs : public MessageException {
public:
    explicit InvalidArgs(const std::string &m, bool print_help = false) : MessageException(m),
                                                                          print_help(print_help) {}

    char *what() override {
        return "Invalid args.";
    }

    bool print_help = false;
    const int code = 100;
};

class GroupError : public MessageException {
public:
    explicit GroupError(const std::string &m) : MessageException(m) {}

    char *what() override {
        return "Group error.";
    }

    const int code = 101;
};