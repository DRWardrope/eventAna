#include "TMVA/MethodCuts.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Factory.h"
#include "TCut.h"
#include "TFile.h"
#include "TTree.h"
#include <string>
int main(void);
void findCutValues(const int& mass, std::vector<double>& minCutValues, std::vector<double>& maxCutValues);
std::vector<int> setupMasses();
