//
// Tool to correct bjet trigger efficiencies to data
//

// STL includes
#include <iostream>
#include <string>
#include <vector>

// ROOT includes
#include "TRandom3.h"
#include "TGraphAsymmErrors.h"



//
// class to extrapolate between points and fluctuate within uncertainties
//
class Graph {

public:

  Graph(TGraphAsymmErrors * _g) : g(_g)
  {
    nP = g->GetN();
    x .resize(nP);
    y .resize(nP);
    ex.resize(nP);
    ey.resize(nP);
    for (int i = 0; i < nP; ++i) {
      g->GetPoint(i, x[i], y[i]);
      ex[i] = g->GetErrorX(i);
      ey[i] = g->GetErrorY(i);
    }
  }
  
  ~Graph()
  {
    delete g;    
  }
  
  float Eval(float xval, float sigma)
  {
    float yval  = 0.;
    float eyval = 0.;
    if (xval <= x[0]) {
      yval  = y[0];
      eyval = ey[0];
    }
    else if (xval >= x[nP-1]) {
      yval  = y[nP-1];
      eyval = ey[nP-1];
    }
    else {
      for (int i = 0; i < nP-1; ++i) {
	if (xval > x[i] && xval <= x[i+1]) {
	  // a = (y2-y1)/(x2-x1)
	  // y-y1 = a(x-x1) ==> y = a(x-x1)+y1
	  float a  = (y[i+1] - y[i]) / (x[i+1] - x[i]);
	  float ea = (ey[i+1] - ey[i]) / (x[i+1] - x[i]);
	  yval  = a*(xval-x[i]) + y[i];
	  eyval = ea*(xval-x[i]) + ey[i];
	  break;
	}
      }
    }
    float yval_smeared = yval + sigma*eyval;
    if (yval_smeared < 0.) yval_smeared = 0.;
    //std::cout << "xval = " << xval << "  yval = " << yval << "  yval_smeared = " << yval_smeared << std::endl;     
    return yval_smeared;
  }
  
  
private:

  TGraphAsymmErrors * g;
  std::vector<double> x;
  std::vector<double> ex;
  std::vector<double> y;
  std::vector<double> ey;
  int nP;

};



//
// structure to hold new trigger decisions
//
struct TrigDec {
public:
  TrigDec(bool _t1, bool _t2, bool _t3) :
    t1(_t1),
    t2(_t2),
    t3(_t3)
  {

  }
  TrigDec(const TrigDec & other) :
    t1(other.t1),
    t2(other.t2),
    t3(other.t3)
  {

  }
  const bool t1;
  const bool t2;
  const bool t3;
};


//
// structure to hold new trigger decisions
//
struct TrigSF {
public:
  TrigSF(float _t1, float _t2, float _t3, float _tOR) :
    t1(_t1),
    t2(_t2),
    t3(_t3),
    tOR(_tOR)
  {
    
  }
  const float t1;
  const float t2;
  const float t3;
  const float tOR;    
};


//
// class to recalculate triggers and derive SFs
//
class BTrigger {

public:

  BTrigger();
  ~BTrigger();

  bool LoadMCEff (const std::string & filepath, const std::string & graphname);
  bool LoadDataSF(const std::string & filepath, const std::string & graphname);
  
  TrigDec Method1(float j1_pt, float j2_pt, float j3_pt, float j4_pt, float sigma);
  TrigSF  Method2(float j1_pt, float j2_pt, float j3_pt, float j4_pt);
  
private:

  Graph    * mcEff_pt;
  Graph    * dataSF_pt;
  int        nToys;
  TRandom3   ran;
  
};


