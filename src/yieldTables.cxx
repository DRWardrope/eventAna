/*************************************************************************************************
*	yieldTables
*	yieldTables makes tables of event yields and writes them to an output tex file. 
*	27th March 2017, David Wardrope, David.Wardrope@cern.ch
*************************************************************************************************/
#include "yieldTables.h"
bool m_debug = false;

int main(int argc, char** argv)
{
	if(processCommandLine(argc, argv) != 0) return -99;
	if(!m_fIn)
	{
		std::cout<<"ERROR! No input file specified."<< std::endl;
		return 1;
	}

	std::cout<<"Get data and background Histograms."<< std::endl;
	//Create SimpleEvent objects for data, backgrounds and signals.
	std::vector<std::string> regions = {{"controlReg1", "controlReg2", "valReg1", "valReg2", "signal"}};

	std::map<std::string, double> dataCounts, ttbarCounts, multijetCounts, smhhCounts, totalBackCounts;
	for(auto region: regions)
	{  //Use MEt histograms until I put something properly bounded in
		std::string disc = "MEt";
		getYields(dataCounts, "DataFullyTagged", region, disc);
		getYields(multijetCounts, "Multijet", region, disc);
		getYields(ttbarCounts, "TTBar", region, disc);
		//getYields(smhhCounts, "SM_HH_FullyTagged", region, disc);
		totalBackCounts.insert(std::pair<std::string, double>(region, multijetCounts[region]+ttbarCounts[region]));

	}
	printYields(dataCounts, multijetCounts, ttbarCounts, smhhCounts, totalBackCounts);
	return 0;
}

void getYields(std::map<std::string, double>& table, const std::string& name, const std::string& region, const std::string& disc)
{
	std::cout<<"getYields("<< name <<", "<< region <<", "<< disc <<"):"<< std::endl;
	TH1F* h = (TH1F*) m_fIn->Get((name+"_resolved_"+region+"_"+disc).c_str());
	int nBins = h->GetNbinsX()+1;
	table.insert(std::pair<std::string, double>(region, h->Integral(0, nBins)));
}

void printYields(const std::map<std::string, double>& data, const std::map<std::string, double>& multijet,
						const std::map<std::string, double>& ttbar, const std::map<std::string, double>& smHH,
						const std::map<std::string, double>& total)
{
	std::cout<<"printYields:"<< std::endl;
	std::ofstream latexOut("YieldsInRegions.tex");//, 'w');
    latexOut.precision(3);
    latexOut << std::ios::showpoint;
    latexOut << std::ios::scientific;
    latexOut <<R"(\documentclass{article})" << std::endl;
	latexOut <<R"(\usepackage{booktabs})" << std::endl;
	latexOut <<R"(\begin{document})"<< std::endl;
    latexOut <<R"(\begin{table}[htb])" << std::endl;
	latexOut <<R"(\begin{center})" << std::endl;
	latexOut <<R"(\begin{tabular}{l)";
	for(int i = 0; i < data.size(); ++i) latexOut <<"c";
	latexOut <<R"(})" << std::endl;
    latexOut <<R"(\toprule)"<<std::endl;
	latexOut <<"Sample & ";
	for(std::map<std::string, double>::const_iterator it = data.begin(); it != data.end(); ++it)
	{
		latexOut<< it->first;
		if(it == (++data.rbegin()).base()) latexOut <<R"(\\)" << std::endl;
		else latexOut <<" & ";
	}
	printRow(latexOut, "Data", data);
	printRow(latexOut, "Multijet", multijet);
	printRow(latexOut, "TTBar", ttbar);
    latexOut <<R"(\midrule)"<< std::endl;
	printRow(latexOut, "Total Background", total);
    latexOut <<R"(\midrule)"<< std::endl;
	printRow(latexOut, "SM HH", smHH);
    latexOut <<R"(\bottomrule)"<< std::endl;
	latexOut <<R"(\end{tabular})"<<std::endl;
    latexOut <<R"(\caption[Cut-Flow]{Yields})"<< std::endl;
    latexOut <<R"(\label{tab:cutflow})"<< std::endl;
	latexOut <<R"(\end{center})"<<std::endl;
	latexOut <<R"(\end{table})"<<std::endl;
    latexOut <<R"(\end{document})"<< std::endl;
    latexOut.close();

}
void printRow(std::ofstream& latexOut, const std::string& name, const std::map<std::string, double>& counts)
{
	std::cout<<"printRow("<< name <<")"<< std::endl;
	latexOut << name << " & ";
	for(std::map<std::string, double>::const_iterator it = counts.begin(); it != counts.end(); ++it)
	{
		if(name == "Data" && it->first == "signal") latexOut <<"--";
		else latexOut << it->second;  
		if(it == (++counts.rbegin()).base()) latexOut <<R"(\\)" << std::endl;
		else latexOut <<" & ";
	}
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
	return 0;
}
