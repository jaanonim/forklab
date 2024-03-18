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

class HTTPError : public MessageException {
public:
    explicit HTTPError(const std::string &m, const int response_code) : MessageException(m),
                                                                        response_code(response_code) {}

    char *what() override {
        return "HTTP Error.";
    }

    const int response_code;
    const int code = 150;
};

class Unauthorized : public MessageException {
public:
    explicit Unauthorized() : MessageException(
            "Cannot get access to gitlab API.\nMake sure that you have configure correct gitlab access token using command:\n\tforklab auth TOKEN") {}

    char *what() override {
        return "Unauthorized.";
    }

    const int code = 151;
};

class EmptyResponse : public MessageException {
public:
    explicit EmptyResponse() : MessageException(
            "Response has empty body.") {}

    char *what() override {
        return "Empty response.";
    }

    const int code = 152;
};


class DirectoryError : public MessageException {
public:
    explicit DirectoryError(const std::string &m) : MessageException(m) {}

    char *what() override {
        return "Directory error.";
    }

    const int code = 102;
};

class GenericError : public MessageException {
public:
    explicit GenericError(const std::string &m) : MessageException(m) {}

    char *what() override {
        return "Error.";
    }

    const int code = 200;
};