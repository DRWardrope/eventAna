// STL includes
#include <iostream>

// local includes
#include "BTrigger.h"

// ROOT includes
#include "TFile.h"
#include "TH1F.h"



BTrigger::BTrigger() :
  mcEff_pt(nullptr),
  dataSF_pt(nullptr),
  nToys(10000)
{

}

BTrigger::~BTrigger()
{

  delete mcEff_pt;
  delete dataSF_pt;
  
}


bool BTrigger::LoadMCEff(const std::string & filepath, const std::string & graphname)
{

  TFile * file = new TFile(filepath.c_str(), "read");

  if ( ! file->IsOpen() ) {
    std::cout << "Couldn't open MC file :" << filepath << std::endl;
    return false;
  }

  if ( ! file->GetListOfKeys()->Contains( graphname.c_str() ) ) {
    std::cout << "Couldn't get MC graph :" << graphname << std::endl;
    return false;    
  }

  TGraphAsymmErrors * g = static_cast<TGraphAsymmErrors *>( file->Get( graphname.c_str() ) );
  mcEff_pt = new Graph(g);
  delete file;
  
  return true;
  
}


bool BTrigger::LoadDataSF(const std::string & filepath, const std::string & graphname)
{

  TFile * file = new TFile(filepath.c_str(), "read");

  if ( ! file->IsOpen() ) {
    std::cout << "Couldn't open data file :" << filepath << std::endl;
    return false;
  }

  if ( ! file->GetListOfKeys()->Contains( graphname.c_str() ) ) {
    std::cout << "Couldn't get data graph :" << graphname << std::endl;
    return false;    
  }

  TGraphAsymmErrors * g = static_cast<TGraphAsymmErrors *>( file->Get( graphname.c_str() ) );
  dataSF_pt = new Graph(g);
  delete file;
  
  return true;

}


TrigDec BTrigger::Method1(float j1_pt, float j2_pt, float j3_pt, float j4_pt, float sigma = 0.)
{
  
  float j1_mcEff = mcEff_pt->Eval( j1_pt, 0. );
  float j2_mcEff = mcEff_pt->Eval( j2_pt, 0. );
  float j3_mcEff = mcEff_pt->Eval( j3_pt, 0. );
  float j4_mcEff = mcEff_pt->Eval( j4_pt, 0. );

  float j1_dataSF = dataSF_pt->Eval( j1_pt, sigma );
  float j2_dataSF = dataSF_pt->Eval( j2_pt, sigma );
  float j3_dataSF = dataSF_pt->Eval( j3_pt, sigma );
  float j4_dataSF = dataSF_pt->Eval( j4_pt, sigma );

  float rdn = ran.Uniform(1.);

  int j1 = (j1_mcEff*j1_dataSF > rdn) ? 1 : 0;
  int j2 = (j2_mcEff*j2_dataSF > rdn) ? 1 : 0;
  int j3 = (j3_mcEff*j3_dataSF > rdn) ? 1 : 0;
  int j4 = (j4_mcEff*j4_dataSF > rdn) ? 1 : 0;

  // HLT_2j35_bmv2c1060_split_2j35_L14J15_0ETA25
  bool t1 = (j1_pt>=35. && j2_pt>=35. && j3_pt>=35. && j4_pt>=35.) && (j1 + j2 + j3 + j4 >= 2);

  // HLT_j100_2j55_bmv2c1060_split
  bool t2 = (j1_pt>=100. || j2_pt>=100. || j3_pt>=100. || j4_pt>=100.) && ((j1_pt>=55.)*j1 + (j2_pt>=55.)*j2 + (j3_pt>=55.)*j3 + (j4_pt>=55.)*j4 >= 2);

  // HLT_j225_bmv2c1060_split
  bool t3 = (j1_pt>=225)*j1 || (j2_pt>=225)*j2 || (j3_pt>=225)*j3 || (j4_pt>=225)*j4;

  // std::cout << "=============================================================" << std::endl;
  // std::cout << "mc eff  : " << j1_mcEff << " " << j2_mcEff << " " << j3_mcEff << " " << j4_mcEff << std::endl;
  // std::cout << "data SF : " << j1_dataSF << " " << j2_dataSF << " " << j3_dataSF << " " << j4_dataSF << std::endl;
  // std::cout << rdn << " : " << j1 << " " << j2 << " " << j3 << " " << j4 << std::endl;
  
  return TrigDec(t1, t2, t3);
  
}



TrigSF BTrigger::Method2(float j1_pt, float j2_pt, float j3_pt, float j4_pt)
{

  int nt1  = 0;
  int nt2  = 0;
  int nt3  = 0;
  int ntOR = 0;
  
  for (int i = 0; i < nToys; ++i) {
    float sigma = ran.Gaus(0.,1.);
    TrigDec trigDec = Method1(j1_pt, j2_pt, j3_pt, j4_pt, sigma);
    if (trigDec.t1) ++nt1;
    if (trigDec.t2) ++nt2;
    if (trigDec.t3) ++nt3;
    if (trigDec.t1 || trigDec.t2 || trigDec.t3) ++ntOR;
  } 
  
  float t1  = float(nt1) /float(nToys);
  float t2  = float(nt2) /float(nToys);
  float t3  = float(nt3) /float(nToys);
  float tOR = float(ntOR)/float(nToys);

  return TrigSF(t1, t2, t3, tOR);
  
}
  
