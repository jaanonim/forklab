//
// Created by jaanonim on 12.03.24.
//
#include <iostream>

class NoValue : public std::exception {
    char *what() {
        return "Trying to read empty value.";
    }
};

class CannotSave : public std::exception {
    char *what() {
        return "Cannot save config.";
    }
};