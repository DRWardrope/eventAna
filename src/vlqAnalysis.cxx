#include <algorithm>
#include <fstream>
#include <regex>
#include <sys/stat.h>
#include "vlqAnalysis.h"
#include "TROOT.h"

bool m_doSystematics = true;
std::string m_calib = "";
bool m_debug = false;
bool m_test = false;
std::string m_inputFile = "";

unsigned long m_totEvents = 0;

int main(int argc, char** argv)
{ 
	time_t start, end;
	time(&start);
	gROOT->ProcessLine("#include <vector>");//to enable vector branches in ttrees
	TH1::SetDefaultSumw2();
	if(processCommandLine(argc, argv)) return 1;

	float lumi = 43.65; //Integrated luminosity in fb^{-1} for 2017 according to Marco
	setupFiles(lumi); //fills m_fileList, vector of <TFile, <normalisation, cross-section> >
	setupSystList();  //fills m_systList
	setupTriggerList();//fills m_trigList

	//Open the output file for the first time, to blank it. Then shut it again. 
	//Then copy event count histograms from the input files to this blank file.
	//analyseEvents will later reopen this file.
	m_fOut = TFile::Open(m_outputFileName.c_str(), "RECREATE");
	m_fOut->Close();
	for(std::vector<std::pair<std::string, std::pair<double, double> > >::const_iterator file = m_fileList.begin(); 
			file != m_fileList.end(); ++file)
	{

                // hack to get meta data hist...
                TString infilename  = file->first;
                TFile * infile      = TFile::Open(infilename, "READ");
                TH1D * metadata_in  = (TH1D*)infile->Get("MetaData_EventCount");
                TFile * outfile     = TFile::Open(m_outputFileName.c_str(), "UPDATE");
                outfile->cd();
                TH1D * metadata_out = (TH1D*)metadata_in->Clone();
                TString metaname    = TString("MetaData_EventCount_") + TString(getNiceName(infilename.Data()).c_str());
                metadata_out->SetNameTitle(metaname, metaname);
                metadata_out->Write();
                outfile->Close();

		analyseEvents(file);
	}

	std::cout<<"========================================================"<< std::endl;
	time(&end);
	double elapsed = difftime(end, start);
	int mins = elapsed / 60;
	int secs = (int)elapsed % 60;
	std::cout<<"Analysing "<< m_totEvents <<" events took "<< mins << " mins, " << 
		  secs <<" s: "<< m_totEvents/elapsed <<" events/s" << std::endl;
}
void analyseEvents(std::vector<std::pair<std::string, std::pair<double, double> > >::const_iterator fileIt)
{
	//analyseEvents runs on all TTrees corresponding to systematic variations in m_systList
	m_fOut = TFile::Open(m_outputFileName.c_str(), "UPDATE");//Add extra information to already-existing file
	std::vector<std::string> systList = m_systList;
	if(isData(fileIt->first)) systList = {"Nominal"};//Data is nominal by definition	
	for(auto syst: systList)
	{
		//Create cutflow histogram to understand acceptance*efficiency of each selection step
		int nBins =11;
		TH1F* hCutFlow = new TH1F((getNiceName(fileIt->first)+"_"+syst+"_CutFlow").c_str(), ";;", nBins, 0.5, nBins+0.5);
		hCutFlow->GetXaxis()->SetBinLabel(1, "tree entries");
		hCutFlow->GetXaxis()->SetBinLabel(2, "#sum(i): All");
		hCutFlow->GetXaxis()->SetBinLabel(3, "#sum(w_{i}): All");
		hCutFlow->GetXaxis()->SetBinLabel(4, "#sum(i): Triggered");
		hCutFlow->GetXaxis()->SetBinLabel(5, "#sum(w_{i}): Triggered");
		hCutFlow->GetXaxis()->SetBinLabel(6, "#sum(i): 1 large-R jet p_{T} > 300 GeV");
		hCutFlow->GetXaxis()->SetBinLabel(7, "#sum(w_{i}): 1 jet p_{T} > 300 GeV");
		hCutFlow->GetXaxis()->SetBinLabel(7, "#sum(i): 3 small-R jets p_{T} > 30 GeV");
		hCutFlow->GetXaxis()->SetBinLabel(8, "#sum(w_{i}): 3 small-R jets p_{T} > 30 GeV");
		hCutFlow->GetXaxis()->SetBinLabel(9, "#sum(i): 1 b-tagged small-R jet");
		hCutFlow->GetXaxis()->SetBinLabel(10, "#sum(w_{i}): 1 b-tagged small-R jet");
		hCutFlow->GetXaxis()->SetBinLabel(8, "#sum(i): 1 Higgs candidate");
		hCutFlow->GetXaxis()->SetBinLabel(9, "#sum(w_{i}): 1 Higgs candidate");

		TreeManager treesOut(getCategory(fileIt->first)+"_"+syst);
		Event event(fileIt, syst, m_trigList);
		std::string fileName = fileIt->first;
		std::cout<<"File = "<< fileName <<", name = "<< getCategory(fileIt->first)<<", variation = "<< syst <<", normalisation =" << fileIt->second.first << std::endl;
		unsigned long maxEvents = event.nEvents();
		hCutFlow->SetBinContent(1, maxEvents);
		if((m_debug || m_test) && maxEvents > 1000) 
		{
			if(m_test && maxEvents >= 10000) maxEvents = 10000;
			else maxEvents = 1000;
		}
		ElectronProducer electronProducer(event);
		ParticleProducer partProducer(event);
		MuonProducer muonProducer(event);
		JetProducer ak4JetProducer(event, m_calib, false, 70);
		FatJetProducer caloJet10Producer(event);
		TrackJetProducer trackJet02Producer(event, 70);
		HiggsCandidateProducer higgsProducer(event);
		VLQCandidateProducer vlqProducer(event);
		m_totEvents += maxEvents;
		int reportFrac = maxEvents/10;
		for(unsigned long i = 0; i < maxEvents; ++i)
		{
			if(i%reportFrac == 0) 
			{
				std::cout<<std::setprecision(2)<<"analyseEvent("<< fileIt->first <<"): "
							<< i/(0.01*maxEvents) <<"\% of events processed"<< std::endl; 
				std::cout<<std::setprecision(5)<<std::endl;
			}
			event.load(i);
			hCutFlow->Fill(2);
			hCutFlow->Fill(3, event.weight());
			if(!event.triggered()) continue; 
			hCutFlow->Fill(4);
			hCutFlow->Fill(5, event.weight());
			std::vector<Jet> trackR02 = trackJet02Producer.produce(10., false);
			std::vector<Jet> ak4Jets = ak4JetProducer.produce(40., 2.5, false, trackR02);//min pt, max eta, require b-tag
			event.setNJets(ak4Jets.size());
			int nBTags = 0;
			float btagWeight = 1;
			float allBTagWeight = 1;
			for(auto jet : ak4Jets)
			{
				if(jet.isBTagged()) 
				{
					++nBTags;
					btagWeight *= jet.btagSF();
				}
				allBTagWeight *= jet.btagSF();
			}
			event.addWeight("BTagSF", allBTagWeight);
			float btrigSF = 1.;	
			if(nBTags > 3)
			{
  				if(m_applyBTrigSF)
				{
				   	int dsid = getDSID(fileName);
				   	if(m_sampleHandler.isSignal(dsid)) 
					{
						   TrigSF trigSF = m_btrig->Method2( ak4Jets.at(0).Pt(), ak4Jets.at(1).Pt(), ak4Jets.at(2).Pt(), ak4Jets.at(3).Pt() );
						   btrigSF = trigSF.tOR;
				   	}
			   	}

			    hCutFlow->Fill(6);
				hCutFlow->Fill(7, event.weight());
				eventsOut << event.runNumber() <<", "<< event.number() <<", "<< ak4Jets.size() <<", "<< nBTags <<", ";
				int nBJet = 0;
				for(auto jet : ak4Jets)
				{
					if(jet.isBTagged()) 
					{
						++nBJet;
						eventsOut << jet.Pt() <<", "<< jet.mv2c10() ;
						if(nBJet < 4) eventsOut <<", ";
						else
						{
							eventsOut << std::endl;
							break;
						}
					}
				}
			}
			event.addWeight("BTrigSF", btrigSF);
			std::vector<Jet> caloR10 = caloJet10Producer.produce(150., 2.0, trackR02); //Leave leeway for muon correction..
			
			std::vector<Electron> electrons = electronProducer.produce(4.); 
			std::vector<Muon> muons = muonProducer.produce(4.); 
			
			std::vector<HiggsCandidate> higgs = higgsProducer.produceResolved(ak4Jets, muons, 0., 14000.);
			if(m_debug) std::cout<<"analyseEvents: Produced "<< higgs.size() <<" resolved candidates."<<std::endl;
			if(higgs.size() < 2) continue;
			//Check if these candidates are fully b-tagged before counting them as candidates for cut-flow 

			std::array<HiggsCandidate, 2> combinedHiggs;
			combinedHiggs = {{higgs[0], higgs[1]}};

			if(combinedHiggs[0].fullyBTagged() && combinedHiggs[1].fullyBTagged())
			{
				hCutFlow->Fill(8);
				hCutFlow->Fill(9, event.weight());
			}
			if(event.isMC())
			{
				if(isSignal(fileName))
				{
					std::vector<Particle> truthHiggs = partProducer.produce(25);		
					if(m_debug) std::cout<<"Have found "<< truthHiggs.size() <<" truth Higgs particles."<< std::endl;
					//Find the partons associated with the Higgs candidates' jets.
					
					std::vector<int> indices;
					for(auto& higgs: combinedHiggs)
					{
						for(int j = 0; j < 2; ++j)	
						{
							for(int idx: higgs.getJet(j)->partonIdx())
							{
								indices.emplace_back(idx);
							}
						}
					}
					if(m_debug) std::cout<<"analyseEvents: found "<< indices.size() <<" particle indices to match to Higgs bosons" << std::endl;
					std::vector<Particle> jetPartons = partProducer.produceByIndex(indices);		
					//Match these partons to Higgs bosons, if possible.	
					
					std::vector<const Particle*> hDaughters = matchBToH(truthHiggs, jetPartons);
				
					for(auto& higgs: combinedHiggs)
					{
						setTruthHiggsId(higgs, hDaughters);
					}
				}
			}
			treesOut.fillTree(event, combinedHiggs, ak4Jets, electrons, muons);
		}
		treesOut.write(m_fOut);
		hCutFlow->Write();
	}
	m_fOut->Close();
}

std::vector<HiggsCandidate> chooseBestPair(const std::vector<HiggsCandidate>& hcands,
                                                std::function<float(const HiggsCandidate&, const HiggsCandidate&)>& bestPairChooser)
{
    if(hcands.size() <= 1) return hcands; //No choice to make, simply return the vector.
    float bestScore = 9999.;
    HiggsCandidate bestH1, bestH2; //Will have index = -99, unless set equal to real candidate.
    for(std::vector<HiggsCandidate>::const_iterator hcand1 = hcands.begin(); hcand1 != hcands.end(); ++hcand1)
    {
        for(std::vector<HiggsCandidate>::const_iterator hcand2 = hcand1 + 1; hcand2 != hcands.end(); ++hcand2)
        {
            if(hcand1->sharesJet(*hcand2)) continue; //Relevant for resolved analysis.
            float score = bestPairChooser(*hcand1, *hcand2);
            if(score < bestScore)
            {
                bestScore = score;
                bestH1 = *hcand1;
                bestH2 = *hcand2;
            }
        }
    }
    std::vector<HiggsCandidate> bestHCands;
    if(bestH1.index() != -99)  //Can only ever find pairs of candidates...
    {
        bestHCands.push_back(bestH1);
        bestHCands.push_back(bestH2);
    }
    return bestHCands;
}
std::vector<const Particle*> matchBToH(const std::vector<Particle>& higgses, const std::vector<Particle>& partons)
{
	//Function to try and match b-quarks to Higgs bosons. 
	//matchedBQuarks will contain 4 elements, with [0] and [1] corresponding to the daughters of 
	//the first Higgs, with [2] and [3] corresponding to the second Higgs' daughters.
	if(m_debug) std::cout<<"matchBToH: matching "<< partons.size() <<" b-quarks, with "<< higgses.size() <<" Higgs bosons"<< std::endl;
	std::vector<const Particle*> matchedBQuarks;
	for(unsigned int i = 0; i < higgses.size(); ++i)
	{
		const Particle* bestB1 = nullptr;		
		const Particle* bestB2 = nullptr;		
		float smallestDR = 0.1;
		for(auto& b1: partons)
		{
			if(abs(b1.pdgId()) != 5)
			{
	//			std::cout<<"matchBToH: found jet with PDG ID = "<< b1.pdgId()<< std::endl;
				continue; //Only interested in b-quarks!
			}
			for(auto& b2: partons)
			{
				if(abs(b2.pdgId()) != 5)
				{
	//				std::cout<<"matchBToH: found jet with PDG ID = "<< b2.pdgId()<< std::endl;
					continue;
				}
				if(b1.Pt() < b2.Pt()) continue; //Demand leading b-quark is first to get unique permutation
				if(b1.pdgId()*b2.pdgId() > 0) continue; //We want to match b#bar{b} pairs
				if(fabs((b1+b2).M() - 125.) > 15) continue;
				float dR = higgses[i].deltaR(b1+b2);
	//			std::cout<<"matchBToH: (b1+b2).M() = "<< (b1+b2).M() <<", dR = "<< dR << std::endl;
				if(dR < smallestDR)
				{
					smallestDR = dR;
					bestB1 = &b1;
					bestB2 = &b2;
				}
			}
		}
		if(m_debug) std::cout<<"matchBToH: best match dR = "<< smallestDR << std::endl;
		matchedBQuarks.push_back(bestB1);
		matchedBQuarks.push_back(bestB2);
	}
	return matchedBQuarks;	
}

void setupSystList()
{
	m_systList.emplace_back("Nominal");
	//If file is not signal, m_systList = "Nominal"
	int dsid = getDSID(m_inputFile);
	if(m_doSystematics && m_sampleHandler.isSignal(dsid))
	{
		m_systList.emplace_back("JET_GroupedNP_1__1down");
		m_systList.emplace_back("JET_GroupedNP_1__1up");
		m_systList.emplace_back("JET_GroupedNP_2__1down");
		m_systList.emplace_back("JET_GroupedNP_2__1up");
		m_systList.emplace_back("JET_GroupedNP_3__1down");
		m_systList.emplace_back("JET_GroupedNP_3__1up");
		m_systList.emplace_back("JET_EtaIntercalibration_NonClosure__1down");
		m_systList.emplace_back("JET_EtaIntercalibration_NonClosure__1up");
		m_systList.emplace_back("JET_JER_SINGLE_NP__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_B_0__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_B_0__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_B_1__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_B_1__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_B_2__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_B_2__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_C_0__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_C_0__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_C_1__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_C_1__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_C_2__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_C_2__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_0__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_0__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_1__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_1__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_2__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_2__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_3__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_3__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_4__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_Eigen_Light_4__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_extrapolation__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_extrapolation__1up");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_extrapolation_from_charm__1down");
		m_systList.emplace_back("akt4EMTopo_FT_EFF_extrapolation_from_charm__1up");
		m_systList.emplace_back("JET_JvtEfficiency__1down");
		m_systList.emplace_back("JET_JvtEfficiency__1up");
		m_systList.emplace_back("MET_SoftTrk_ResoPara");
		m_systList.emplace_back("MET_SoftTrk_ResoPerp");
		m_systList.emplace_back("MET_SoftTrk_ScaleDown");
		m_systList.emplace_back("MET_SoftTrk_ScaleUp");
		
	}
}
void setupFiles(float lumiFb)
{
	std::cout<<"setupFiles: Luminosity is "<< lumiFb <<" fb-1" << std::endl;

	//std::string prefix = "mc15_13TeV.";
	if(m_inputFile.size() != 0)
	{
		addFile(m_inputFile, lumiFb);
	}
}
void setupTriggerList()
{
	//Use combined triggers for resolved + merged
	m_trigList = std::vector<std::string>();
	//1-jet trigger
	m_trigList.push_back("HLT_j460");
}
bool setupBTriggerSF()
{
	m_btrig = new BTrigger;
	const std::string mcFile    = "data/2016BJetTriggerSignalEff.root";
	const std::string dataFile  = "data/BJetTriggerEfficiencies-00-02-01.root";
	const std::string mcGraph   = "off70_match_hlt60_jetPt_rebin_ratio";
	const std::string dataGraph = "g_SF_offJets70_match_hlt_match_hlt60_jetPt";
	if ( ! m_btrig->LoadMCEff (mcFile  , mcGraph  ) ) return false;
	if ( ! m_btrig->LoadDataSF(dataFile, dataGraph) ) return false;  
	return true;
}

void addFile(std::string fileLocation, double lumi)
{
	struct stat buffer;   
  	if(stat(fileLocation.c_str(), &buffer) == 0)
	{
		std::cout<<"addFile("<< fileLocation <<", "<< lumi <<"): "<< std::endl;
		TFile* f = TFile::Open(fileLocation.c_str(), "READ");
		std::cout<< f->GetName() <<": "<< std::endl;
		double normalisation = 1.;
		double xSec = -99.;
		if(getCategory(fileLocation).find("data") == std::string::npos) //i.e. not data events
		{
			//get DSID from fileLocation
			int dsid = getDSID(fileLocation);
			xSec = m_sampleHandler[dsid].xsec;
            double sumWeights = m_sampleHandler[dsid].totalWeight;
            normalisation = (lumi*xSec)/sumWeights;
		}
		//Use weighted numbers in order to get correct ttbar and HH normalisation.
		//These samples have weights different from one!
		m_fileList.push_back(std::pair<std::string, std::pair<double, double> >(fileLocation, 
					std::pair<double, double>(normalisation, xSec)));
	}else{
		std::cout<<"addFile("<< fileLocation <<"): file does not exist!"<< std::endl;
	}
}
int getDSID(const std::string& fileName)
{
//mc15_13TeV.301505.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c10_M2500.merge.DAOD_EXOT8.e3820_s2608_s2183_r7772_r7676_p2669.root
	std::regex dsidMatch = std::regex("[0-9]{6}");
	std::smatch matches;
	if(std::regex_search(fileName, matches, dsidMatch))
	{
		return std::stoi(matches[0]);
	}else{
		std::cout<<"getDSID: no DSID in filename: "<< fileName << std::endl;
		return -99;
	}
}
std::string getNiceName(const std::string& origName)
{
	//Search using regular expressions
	std::regex rsgMatch = std::regex("RS_G_hh_bbbb_c[1-2]0_M[0-9]{3,4}");
	std::regex runMatch = std::regex("data1[5-6]_13TeV.period.");
	std::regex susyMatch = std::regex("GGMHinoh_[0-9]{3,4}");
	std::regex dsidMatch = std::regex("[0-9]{6}");
	std::smatch matches;
	
	std::string name = origName;
	if(std::regex_search(origName, matches, dsidMatch))
	{
		int dsid = std::stoi(matches[0]);
		if(m_sampleHandler.contains(dsid))
		{
			name = m_sampleHandler[dsid].name;
		}else{
			name = matches[0]; //No nicer name, so simply use DSID. 
		}
	}else if(std::regex_search(origName, matches, runMatch))
	{
			name = matches[0];
	}
	return name;
}
std::string getCategory(TFile* file)
{
	std::string name = getNiceName(file->GetName());
	return getCategory(name);
}
std::string getCategory(const std::string& name_notnice)
{
	std::string name = getNiceName(name_notnice);
	return name;
}
bool isData(const std::string& name)
{   
    std::regex dataMatch = std::regex("data1[5-6]");
    std::smatch matches;
        
    if(std::regex_search(name, matches, dataMatch))
    {   
        return true;
    }else
    {
        return false;
    }
}

bool isSignal(const std::string& name)
{
	std::regex dsidMatch = std::regex("[0-9]{6}");
	std::smatch matches;
	
	if(std::regex_search(name, matches, dsidMatch))
	{
		return m_sampleHandler.isSignal(std::stoi(matches[0]));
	}else return false;
}
int processCommandLine(int argc, char** argv)
{
	for(int i = 1; i < argc; ++i)
	{
		if(std::string(argv[i]) == std::string("--noSyst")) m_doSystematics = false;
		else if(std::string(argv[i]) == std::string("--debug")) m_debug = true;
		else if(std::string(argv[i]) == std::string("--test")) m_test = true;
		else if(std::string(argv[i]) == std::string("--calib")) m_calib = argv[++i];
		else if(std::string(argv[i]) == std::string("--trackJetBTagging")) m_trackJetBTagging = true;
		else if(std::string(argv[i]) == std::string("--inputFile"))
		{
			m_inputFile = argv[++i];
		}
		else
		{
			std::cout<<"Unrecognized option "<< argv[i] << std::endl;
			return 1;
		}
	}
	#ifdef DEBUG_BUILD
		m_test = true;
	#endif
	m_outputFileName = "EventAnaResults";
	if(m_calib.size() != 0) m_outputFileName += "_"+m_calib;
	m_outputFileName += ".root";
	return 0;
}

void setTruthHiggsId(HiggsCandidate& hcand, const std::vector<const Particle*>& hDaughters)
{
	if(m_debug) std::cout<<"setTruthHiggsId: begins. hcand category = "<< hcand.category() 
					<<". There are "<< hDaughters.size() <<" hDaughters." << std::endl;
	std::array<std::vector<int>, 2> matches;
	for(int j = 0; j < 2; ++j)
	{
		for(unsigned int b = 0; b < hDaughters.size(); ++b)
		{
			if(!hDaughters[b]) continue;
			if(hcand.getJet(j)->deltaR(*(hDaughters[b])) < 0.4) matches[j].push_back(b);
		}	
		if(matches[j].size() == 0) //Jet has no matched Higgs daughters, so set HiggsParentId to -98.
		{
			hcand.setTruthHiggsId(-98);
			return;
		}
	}	
	for(auto a: matches[0])
	{
		for(auto b: matches[1])
		{
			if(a/2 == b/2 && a != b)//Demand daughters come from same Higgs and are unique
			{
				hcand.setPartonP4(hDaughters[a]->p4()+hDaughters[b]->p4());
				hcand.setDRbb(hDaughters[a]->deltaR(*(hDaughters[b])));
				hcand.setTruthHiggsId(a/2);
				return;
			}
		}
	}
	//If we reach here, both jets contain Higgs daughters, but they are not from the same Higgs
	hcand.setTruthHiggsId(-97);
}
