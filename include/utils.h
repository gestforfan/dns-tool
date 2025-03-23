// utils.h
#ifndef UTILS_H
#define UTILS_H

#include "common.h"

std::vector<std::string> split(const std::string& s, char delimiter);
bool file_exists(const string& name);
string exec(const char* cmd);

#endif
