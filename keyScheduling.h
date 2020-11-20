#include <iostream>

#ifndef TRIPLEDES_KEYSCHEDULING_H
#define TRIPLEDES_KEYSCHEDULING_H
std::string getKey();
std::string leftShift(std::string key);
std::string rightShift(std::string key);
void generateSubKeys(std::string key, std::string subKey[]);
void generateSubKeysDecryption(std::string key, std::string subKey[]);

#endif //TRIPLEDES_KEYSCHEDULING_H
