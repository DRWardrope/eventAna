/*************************************************************************************************
*	simpleAnalysis
*	xhhAnalysis --> GBReweighting --> FINALANALYSIS --> LimitSetting 
*	simpleAnalysis makes plots and writes them to an output ROOT file. This can be used to produce
*	plots, or as a limit-setting input file
*	4th March 2016, David Wardrope, David.Wardrope@cern.ch
*************************************************************************************************/
#include "sigContaminationAnalysis.h"
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
	SimpleEvent eData_SemiTagged((TTree*) m_fIn->Get("data_SemiTagged"));	
	TTree* tData_FullyTagged = combineTTrees((TTree*) m_fIn->Get("Multijet_FullyTagged"), (TTree*) m_fIn->Get("TTBar_FullyTagged"), "GWAAR!");
	SimpleEvent eData_FullyTagged(tData_FullyTagged);	
	std::cout<<"Got data TTrees."<< std::endl;

	//Get signals
	std::vector<TTree*> v_H_SemiTagged = getSignals(2);
	std::vector<TTree*> v_H_FullyTagged = getSignals(4);
	std::string fOutName = m_fIn->GetName();
	fOutName.erase(fOutName.begin(), fOutName.begin()+std::string("Processed_EventAnaResults").size());
	//TFile* fOut = TFile::Open(("Hists"+fOutName).c_str(), "RECREATE");

	std::map<std::string, double> fracs_SemiTagged = calcSigFracs(eData_SemiTagged, v_H_SemiTagged);
	std::map<std::string, double> fracs_FullyTagged = calcSigFracs(eData_FullyTagged, v_H_FullyTagged);

	printFractions(fracs_SemiTagged, fracs_FullyTagged);
	/*writePlots("DataFullyTagged", eData_FullyTagged); 
	writePlots("Multijet", eMultijet); 
	writePlots("TTBar", eTTBar_FullyTagged);

	for(auto sig : v_rsc10)
	{
		std::cout<<"writing plots for "<< sig->GetName() << std::endl;
		SimpleEvent sigFE(sig);	
		writePlots(sigFE.name(), sigFE); 
	}*/
	//fOut->Close();
	m_fIn->Close();
	fDummy->Close();

	return 0;
}

std::map<std::string, double> calcSigFracs(SimpleEvent& data, std::vector<TTree*> sigTrees)
{
	SimpleHistSetCollection dataHists = makePlots(data.name(), data);
	double nDataSig = dataHists.sumWeights("signal");
	std::cout<<"calcSigFracs: "<< data.name() <<", sum of weights in signal region = "<< nDataSig << std::endl;
	std::map<std::string, double> fractions;
	for(auto sigTree : sigTrees)
	{
		SimpleEvent sig(sigTree);	
		SimpleHistSetCollection sigHists = makePlots(sig.name(), sig);
		double fraction = sigHists.sumWeights("signal")/nDataSig;
		std::string name = sig.name();
		name.erase(name.find_last_of('_'), name.size());
		
		fractions.insert(std::pair<std::string, double>(name, fraction));
	}
	return fractions;
}
SimpleHistSetCollection makePlots(std::string title, SimpleEvent& evt)
{
	std::cout<<"makePlots("<< title <<"): tree contains ";
	std::cout<< evt.numEntries() <<" entries"<< std::endl;
	SimpleHistSetCollection hists(title, "resolved");

	for(long i = 0; i < evt.numEntries(); ++i)
	{
		evt.getEntry(i);
		if(evt.klfit_6j > -48.) continue;
		if(evt.extraMuon1_mt < 40.)
		{
			hists.fillPlots(evt, evt.weight);
		}
	}
	std::cout<<"makePlots: ends."<< std::endl;
	return hists;
}

std::vector<TTree*> getSignals(const int& minNumTightTags)
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
	std::string tagStatus = "SemiTagged";
	if(minNumTightTags == 4) tagStatus = "FullyTagged";
	for(auto mass: masses)
	{
		std::cout<<"Accessing TTrees for m = "<< mass <<" GeV."<< std::endl;
		vSig.push_back((TTree*) m_fIn->Get((std::string("H_hh_bbbb_M")+mass+"_"+tagStatus).c_str()));
		//vSig.push_back((TTree*) m_fIn->Get((std::string("RS_G_hh_bbbb_c10_M")+mass+"_FullyTagged").c_str()));
	}
	return vSig;
}

void printFractions(const std::map<std::string, double>& fracSemi, const std::map<std::string, double>& fracFully)
{
	std::cout<<"Signal\t\tSemi-Tagged Signal Fraction\t\tFully-Tagged Signal Fraction\t\tRatio"<<std::endl;
	for(auto pair : fracSemi)
	{
		std::cout<< pair.first <<"\t"<< pair.second << "\t"<< fracFully.at(pair.first) <<"\t"<< fracFully.at(pair.first)/pair.second << std::endl;
	}

	std::ofstream latexOut("SignalFractions.tex");//, 'w');
    latexOut.precision(3);
    latexOut << std::ios::showpoint;
    latexOut << std::ios::scientific;
    latexOut <<R"(\documentclass{article})" << std::endl;
	latexOut <<R"(\usepackage{booktabs})" << std::endl;
	latexOut <<R"(\begin{document})"<< std::endl;
    latexOut <<R"(\begin{table}[htb])" << std::endl;
	latexOut <<R"(\begin{center})" << std::endl;
	latexOut <<R"(\begin{tabular}{lccc})" << std::endl;
    latexOut <<R"(\toprule)"<<std::endl;
	latexOut <<R"(Signal & Semi-Tagged $f^{semi}_{signal}$ & Fully-Tagged $f^{fully}_{signal}$ & $\frac{f^{fully}_{signal}}{f^{semi}_{signal}}$\\)"<<std::endl;
	for(auto pair : fracSemi)
	{
		std::string process = "$"+pair.first;
		process.replace(process.find_first_of('_'), 1, "\\to ");
		process.replace(process.find_first_of('_'), 1, "\\to ");
		process.replace(process.find_first_of('_'), 1, "(");
		process += ")$";
		latexOut << process <<" & "<< pair.second <<" & "<< fracFully.at(pair.first) <<" & "
					<< fracFully.at(pair.first)/pair.second <<R"(\\)"<< std::endl;
	}
    latexOut <<R"(\bottomrule
				\end{tabular})"<<std::endl;
    latexOut <<R"(\caption[Cut-Flow]{Cut flow table for $\int\mathcal{L}dt = 20 \rm fb^{-1}$})"<< std::endl;
    latexOut <<R"(\label{tab:cutflow})"<< std::endl;
	latexOut <<R"(\end{center})"<<std::endl;
	latexOut <<R"(\end{table})"<<std::endl;
    latexOut <<R"(\end{document})"<< std::endl;
    latexOut.close();

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
TTree* combineTTrees(TTree* a, TTree* b, const TString& newName)
{
    std::cout<<"combineTTrees("<< a->GetName() <<", "<< b->GetName() <<"): begins."<< std::endl;
    TList* treesToCombine = new TList();
    if(a->GetEntries() > 0)
    {
        treesToCombine->Add(a);
    }else{
        std::cout<<"combineTTrees: there are no entries in "<< a->GetName() << std::endl;
    }
    if(b->GetEntries() > 0)
    {
        treesToCombine->Add(b);
    }else{
        std::cout<<"combineTTrees: there are no entries in "<< b->GetName() << std::endl;
    }
    //std::cout<<"combineTTrees: will merge "<< tree
    TTree* newTree = TTree::MergeTrees(treesToCombine);
    std::cout<<"combineTTrees: merged TTrees."<< std::endl;
    newTree->SetName(newName);
    std::cout<<"combineTTrees: renamed merged TTree."<< std::endl;
    return newTree;
    //std::cout<<"combineTTrees: returning TTree with "<< newTree->GetEntries() <<" entries."<< std::endl;
}

