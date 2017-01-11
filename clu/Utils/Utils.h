#pragma once

#include <string>
#include <vector>

#define BEGIN_NAMESPACE(X) namespace X {
#define END_NAMESPACE(X) }

#define USING_NAMESPACE(X) using namespace X;

std::string StringValue(double i);

const char SpaceDelimiters[] = " \t\n";
void ExtractTokens(std::string input, std::vector<std::string>& ret_value, const char* delim);
std::string GetEnvironmentOrDefault(std::string envName, const char* defaultValue);
int GetEnvironmentOrDefaultInteger(std::string envName, int defaultValue);
std::string ReplaceAllValues(std::string sourceLine, std::string search, std::string replace);
