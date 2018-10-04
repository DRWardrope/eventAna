/*************************************************************************************************
*	simpleAnalysis
*	xhhAnalysis --> GBReweighting --> FINALANALYSIS --> LimitSetting 
*	simpleAnalysis makes plots and writes them to an output ROOT file. This can be used to produce
*	plots, or as a limit-setting input file
*	4th March 2016, David Wardrope, David.Wardrope@cern.ch
*************************************************************************************************/
#include "simpleAnalysis.h"
#include "SimpleHistSet.h"
bool m_debug = false;
bool m_doMDC = false;

int main(int argc, char** argv)
{
	if(processCommandLine(argc, argv) != 0) return -99;
	if(!m_fIn)
	{
		std::cout<<"ERROR! No input file specified."<< std::endl;
	}
	SetAtlasStyle();
	gStyle->SetPalette(109, 0);
	//gStyle->SetPalette(kCool);
	TH1::SetDefaultSumw2();
	TH1::AddDirectory(kFALSE);

	TFile* fDummy = TFile::Open("Dummy.root", "RECREATE");
	
	std::cout<<"Get data and background TTrees."<< std::endl;
	//Create SimpleEvent objects for data, backgrounds and signals.
	SimpleEvent eData_FullyTagged((TTree*) m_fIn->Get("data_FullyTagged"));	
	std::cout<<"Got data TTree."<< std::endl;
	SimpleEvent eMultijet((TTree*) m_fIn->Get("Multijet_FullyTagged"));	
	std::cout<<"Got multijet TTree."<< std::endl;
	SimpleEvent eTTBar_FullyTagged((TTree*) m_fIn->Get("TTBar_FullyTagged"));	
	std::cout<<"Got ttbar TTree."<< std::endl;

	//Get signals
	std::vector<TTree*> v_rsc10 = getSignals();
	std::string fOutName = m_fIn->GetName();
	fOutName.erase(fOutName.begin(), fOutName.begin()+std::string("GB_Processed_EventAnaResults").size());
	TFile* fOut = TFile::Open(("Hists"+fOutName).c_str(), "RECREATE");

	writePlots("DataFullyTagged", eData_FullyTagged); 
	writePlots("Multijet", eMultijet); 
	writePlots("TTBar", eTTBar_FullyTagged);

	for(auto sig : v_rsc10)
	{
		std::cout<<"writing plots for "<< sig->GetName() << std::endl;
		SimpleEvent sigFE(sig);	
		writePlots(sigFE.name(), sigFE); 
	}
	SimpleEvent eSMHH_FullyTagged((TTree*) m_fIn->Get("SM_HH_FullyTagged"));	
	writePlots("SM_HH_FullyTagged", eSMHH_FullyTagged);
	fOut->Close();
	m_fIn->Close();
	fDummy->Close();

	return 0;
}

void writePlots(std::string title, SimpleEvent& evt)
{
	std::cout<<"writePlots("<< title <<"): tree contains ";
	std::cout<< evt.numEntries() <<" entries"<< std::endl;
	SimpleHistSetCollection hists(title, "resolved");

	TF1* erfLowPt = new TF1("erfLowPt", "[0]*0.886*TMath::Erf([1]*(x-[2])/[2])+[3]", 0, 2000);
	erfLowPt->SetParameters(1.60125e+2, 1.88232, 5.71365e+2, 1.57723e+2);
	TF1* erfHighPt = new TF1("erfHighPt", "[0]*0.886*TMath::Erf([1]*(x-[2])/[2])+[3]", 0, 2000);
	erfHighPt->SetParameters(2.25636e+2, 1.58639, 5.91541e+2, 2.34654e+2);
	
	for(long i = 0; i < evt.numEntries(); ++i)
	{
		evt.getEntry(i);
		if(evt.klfit_6j > -48.) continue;
		if(evt.h1_numTightTags < 2 && evt.h2_numTightTags < 2) continue;
		//if(evt.hlt_2b35_2j35 == 0) continue;
		if(m_doMDC)
		{
			if(evt.h1_pt < erfHighPt->Eval(evt.hh_m)) continue;
			if(evt.h2_pt < erfLowPt->Eval(evt.hh_m)) continue;
		}
		if(evt.extraMuon1_mt < 30.)
		{
			if(title == "Multijet") hists.fillPlots(evt, evt.weight, evt.weight1, evt.weight2);
			else hists.fillPlots(evt, evt.weight);
		}
	}
	hists.writePlots();	
	std::cout<<"writePlots: ends."<< std::endl;
}

std::vector<TTree*> getSignals()
{
	std::cout<<"getSignals():"<< std::endl;
	std::vector<TTree*> vSig;
	std::vector<std::string> masses;
	for(int m = 300; m <= 1500; m += 100)
	{
		//if(m == 300 || m == 700 || m == 900 || m == 1000) continue;
		masses.push_back(std::to_string(m));
	}
	/*masses.push_back("1800");
	masses.push_back("2000");
	masses.push_back("2250");
	masses.push_back("2500");
	masses.push_back("2750");
	masses.push_back("3000");*/
	for(auto mass: masses)
	{
		std::cout<<"Accessing 2HDM TTrees for m = "<< mass <<" GeV."<< std::endl;
		vSig.push_back((TTree*) m_fIn->Get((std::string("H_hh_bbbb_M")+mass+"_FullyTagged").c_str()));
		//vSig.push_back((TTree*) m_fIn->Get((std::string("H_hh_bbbb_M")+mass+"_SemiTagged").c_str()));
		//vSig.push_back((TTree*) m_fIn->Get((std::string("RS_G_hh_bbbb_c10_M")+mass+"_FullyTagged").c_str()));
	}
	masses.clear();
	masses = {{"130", "150", "200", "300", "400", "600", "800"}};
	for(auto mass: masses)
	{
		std::cout<<"Accessing GGM_Hino TTrees for m = "<< mass <<" GeV."<< std::endl;
		vSig.push_back((TTree*) m_fIn->Get((std::string("GGM_Hino_M")+mass+"_FullyTagged").c_str()));
	}

	return vSig;
}

int processCommandLine(int argc, char** argv)
{
	m_fIn = nullptr;
	for(int i = 1; i < argc; ++i)
	{
	   if(std::string(argv[i]).rfind(".root") != std::string::npos)
	   {
			   m_fIn = TFile::Open(argv[i], "READ");
	   }else if(std::string(argv[i]) == std::string("--debug")) 
	   {
			   m_debug = true;
	   }else if(std::string(argv[i]) == std::string("--doMDC")) 
	   {
			   m_doMDC = true;
	   }else{
			   std::cout<< argv[i] << " is an unrecognized option! "<< std::endl;
			   return -99;
	   }
		   //else if(std::string(argv[i]) == std::string("--sigModel")) sigModel = argv[++i];
	}
	m_plotDir += "/";
	return 0;
}
void zeroNegativeBins(TH1* h)
{
	for(int i = 0; i < h->GetNbinsX()+1; ++i)
	{
		if(h->GetBinContent(i) < 0.)
		{
			h->SetBinContent(i, 0.);
			h->SetBinError(i, 0.);
		}
	}
}
