/*************************************************************************************************
*	flavourAnalysis
*	xhhAnalysis --> GBReweighting --> TTBARANALYSIS 
*	flavourAnalysis makes plots and writes them to an output ROOT file. This can be used to produce
*	plots for investigations...
*	20th March 2016, David Wardrope, David.Wardrope@cern.ch
*************************************************************************************************/
#include "flavourAnalysis.h"
#include "SimpleHistSet.h"
bool m_debug = false;

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
	SimpleEvent eTTBarAllHad_FullyTagged((TTree*) m_fIn->Get("TTBarAllHad_FullyTagged"));	
	SimpleEvent eTTBarNonAllHad_FullyTagged((TTree*) m_fIn->Get("TTBarNonAllHad_FullyTagged"));	
	SimpleEvent eTTBarAllHad_SemiTagged((TTree*) m_fIn->Get("TTBarAllHad_SemiTagged"));	
	SimpleEvent eTTBarNonAllHad_SemiTagged((TTree*) m_fIn->Get("TTBarNonAllHad_SemiTagged"));	

	std::vector<TTree*> v_dijet_SemiTagged = getDijets(2);
	std::vector<TTree*> v_dijet_FullyTagged = getDijets(4);
	std::string fOutName = m_fIn->GetName();
	fOutName.erase(fOutName.begin(), fOutName.begin()+std::string("Processed_EventAnaResults").size());
	TFile* fOut = TFile::Open(("TTBar_Hists"+fOutName).c_str(), "RECREATE");

	writePlots("TTBarNonAllHad_FullyTagged", eTTBarNonAllHad_FullyTagged);
	writePlots("TTBarAllHad_FullyTagged", eTTBarAllHad_FullyTagged);
	writePlots("TTBarNonAllHad_SemiTagged", eTTBarNonAllHad_SemiTagged);
	writePlots("TTBarAllHad_SemiTagged", eTTBarAllHad_SemiTagged);

	std::cout<<"Dijet Trees: semi-tagged = "<< v_dijet_SemiTagged.size() <<" trees; fully-tagged = "<< v_dijet_FullyTagged.size() <<" trees."<< std::endl;
	for(auto dijet : v_dijet_SemiTagged)
	{
		std::cout<<"writing plots for "<< dijet->GetName() << std::endl;
		SimpleEvent dijetFE(dijet);	
		writePlots(dijetFE.name(), dijetFE); 
	}
	for(auto dijet : v_dijet_FullyTagged)
	{
		std::cout<<"writing plots for "<< dijet->GetName() << std::endl;
		SimpleEvent dijetFE(dijet);	
		writePlots(dijetFE.name(), dijetFE); 
	}
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
	SimpleHistSetCollection histsTTCS(title, "resolved_tt");

	for(long i = 0; i < evt.numEntries(); ++i)
	{
		evt.getEntry(i);
		if(evt.klfit_6j > -48.) continue;
		if(evt.extraMuon1_mt < 40.)
		{
			hists.fillPlots(evt, evt.weight);
		}else{
			histsTTCS.fillPlots(evt, evt.weight);
		}
		//if(title == "Multijet") std::cout<<"Multijet event "<< i <<" weight = "<< evt.weight <<", xhh = "<< evt.xhh << std::endl;
	}
	hists.writePlot("hh_flavourSum");	
	hists.writePlot("h1_h2_flavourSum");	
	histsTTCS.writePlot("hh_flavourSum");	
	histsTTCS.writePlot("h1_h2_flavourSum");	
	//hists.writePlots();	
	//histsTTCS.writePlots();	
	std::cout<<"writePlots: ends."<< std::endl;
}

std::vector<TTree*> getDijets(const int& numTightTags)
{
	std::cout<<"getDijets():"<< std::endl;
	std::vector<TTree*> vTrees;
	std::string tagStatus = "SemiTagged";
	if(numTightTags == 4) tagStatus = "FullyTagged";
	for(int i = 1; i < 9;  ++i)
	{
//JZ1W_SemiTagged
		std::cout <<"getDijets:"<< (std::string("JZ")+std::to_string(i)+"W_"+tagStatus) << std::endl; 
		vTrees.push_back((TTree*) m_fIn->Get((std::string("JZ")+std::to_string(i)+"W_"+tagStatus).c_str()));
	}
	return vTrees;
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
