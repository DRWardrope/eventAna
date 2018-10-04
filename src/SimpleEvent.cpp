#include "SimpleEvent.h"

SimpleEvent::SimpleEvent(TTree* treeIn)
{
	//std::cout<<"SimpleEvent("<< treeIn <<"): begins."<< std::endl;
	tree = treeIn;
	m_name = treeIn->GetName();
	resetVariables();
	treeIn->SetBranchStatus("*", 0);
	setupBranch("weight", &weight);	
	setupBranch("Xhh", &xhh);	
	setupBranch("Rhh", &rhh);	
	setupBranch("hcand1_m", &h1_m);	
	setupBranch("hcand1_pt", &h1_pt);	
	setupBranch("hcand2_m", &h2_m);	
	setupBranch("hcand2_pt", &h2_pt);	
	setupBranch("extraJet1_pt", &extraJet1_pt);
	setupBranch("extraJet1_HH_dPhi", &extraJet1_HH_dPhi);
	setupBranch("extraMuon1_pt", &extraMuon1_pt);	
	setupBranch("extraMuon1_mt", &extraMuon1_mt);	
	setupBranch("hcand1_hcand2_m", &hh_m);	
	setupBranch("klfit_6j", &klfit_6j);	
	setupBranch("klfit_7j", &klfit_7j);	
	setupBranch("hcand1_jet1_flav", &h1_j1_flav);
	setupBranch("hcand1_jet2_flav", &h1_j2_flav);
	setupBranch("hcand2_jet1_flav", &h2_j1_flav);
	setupBranch("hcand2_jet2_flav", &h2_j2_flav);
	setupBranch("hcand1_jet1_pt", &h1_j1_pt);
	setupBranch("hcand1_jet2_pt", &h1_j2_pt);
	setupBranch("hcand2_jet1_pt", &h2_j1_pt);
	setupBranch("hcand2_jet2_pt", &h2_j2_pt);
	setupBranch("jet_n", &jet_n);
	setupBranch("HLT_j80_bmv2c2060_split_xe60_L12J50_XE40", &hlt_b80_xe60);
	setupBranch("HLT_j380", &hlt_j380);
	setupBranch("HLT_j225_bmv2c2060_split", &hlt_b225);
	setupBranch("HLT_j100_2j55_bmv2c2060_split", &hlt_j100_2b55);
	setupBranch("HLT_j75_bmv2c2070_split_3j75_L14J15.0ETA25", &hlt_b75_3j75);
	setupBranch("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25", &hlt_2b35_2j35);
	
	if(m_name == "Multijet_FullyTagged")
	{
		setupBranch("weight1", &weight1);	
		setupBranch("weight2", &weight2);	
	}
	setupBranch("MEt", &met);
	setupBranch("hcand1_numTightTags", &h1_numTightTags);
	setupBranch("hcand2_numTightTags", &h2_numTightTags);
	
	/*setupBranch("TriggerBits", &triggerBits);
	setupBranch("hcand1_hcand2_dR", &hh_dR);	
	setupBranch("hcand1_hcand2_dEta", &hh_dEta);	
	setupBranch("hcand1_nTrackJets",&h1_nTJ);
	setupBranch("hcand2_nTrackJets",&h2_nTJ);
	setupBranch("hcand1_dRjj", &h1_dRjj);	
	setupBranch("hcand1_fullyBTagged", &h1_fullyBTagged);	
	setupBranch("hcand1_category", &h1_category);	
	setupBranch("hcand2_dRjj", &h2_dRjj);	
	setupBranch("hcand2_fullyBTagged", &h2_fullyBTagged);	
	setupBranch("hcand2_category", &h2_category);	
	setupBranch("hcand1_Xt", &h1_xt);	
	setupBranch("hcand2_Xt", &h2_xt);	
	setupBranch("cosThetaStar", &cosThetaStar);	
	setupBranch("cosTheta1", &cosTheta1);	
	setupBranch("cosTheta2", &cosTheta2);	
	setupBranch("Phi", &phi);	
	setupBranch("Phi1", &phi1);	
	setupBranch("MEt", &met);	
	setupBranch("MEt_phi", &met_phi);	*/
	//std::cout<<"SimpleEvent("<< treeIn->GetName() <<"): ends."<< std::endl;
}
void SimpleEvent::setupBranch(const TString& name, void* address)
{ 
	tree->SetBranchStatus(name, 1);
	tree->SetBranchAddress(name, address);
}
void SimpleEvent::resetVariables()
{
	h1_m = -99; 
	h2_m = -99;
	xhh = -99; rhh = -99; 
	extraMuon1_pt = -99.; extraMuon1_mt = -99.;
	extraJet1_pt = -99.; extraJet1_HH_dPhi = -99.;
	weight = -99.; weight1 = -99.; weight2 = -99.;
	hh_m = -99; 
	klfit_6j = 99; klfit_7j = 99;
	h1_j1_flav = -99; h1_j2_flav = -99; h2_j1_flav = -99; h2_j2_flav = -99;
	h1_j1_pt = -99; h1_j2_pt = -99; h2_j1_pt = -99; h2_j2_pt = -99;
	jet_n = -99;
	hlt_b80_xe60 = 0; hlt_j380 = 0; hlt_b225 = 0; hlt_j100_2b55 = 0; hlt_b75_3j75 = 0; hlt_2b35_2j35 = 0;
	h1_pt = -99; h2_pt = -99;
	/*; h1_dRjj = -99;
	h1_category = -99; h1_fullyBTagged = false;
	h2_dRjj = -99;
	h1_nTJ = -99;
	h2_nTJ = -99;
	triggerBits = -99;
	h2_category = -99; 
	h2_fullyBTagged = false;
	hh_dR = -99; hh_dEta = -99;
	h1_xt = -99; h2_xt = -99;
	cosThetaStar = -99; cosTheta1 = -99; cosTheta2 = -99; phi = -99; phi1 = -99;
	met = -99; met_phi = -99;*/
}
long SimpleEvent::getEntry(const unsigned int i)
{ 
	resetVariables();
	return tree->GetEntry(i); 	
}

bool SimpleEvent::inValRegion1() const
{
	return sqrt(std::pow((h1_m-90)/(0.1*h1_m), 2) + std::pow((h2_m-86)/(0.1*h2_m), 2)) < 1.6;
}
bool SimpleEvent::inValRegion2() const
{
	return sqrt(std::pow((h1_m-160)/(0.1*h1_m), 2) + std::pow((h2_m-153)/(0.1*h2_m), 2)) < 1.6;
}
