#ifndef YIELDTABLES_H
#define YIELDTABLES_H
#include "TFile.h"
#include "TH1F.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>

void getYields(std::map<std::string, double>& table, const std::string& name, const std::string& region, const std::string& disc);
int main(int, char**);
void printRow(std::ofstream&, const std::string&, const std::map<std::string, double>&);
void printYields(const std::map<std::string, double>&, const std::map<std::string, double>&,
						const std::map<std::string, double>&, const std::map<std::string, double>&,
						const std::map<std::string, double>&);

int processCommandLine(int argc, char** argv);

TFile* m_fIn;
#endif
