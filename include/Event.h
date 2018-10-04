#ifndef EVENT_H
#define EVENT_H

#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include "TFile.h"
#include "TH1.h"
#include "TriggerBits.h"
#include "TTree.h"
extern bool m_debug;

class Event 
{
	public:
		Event() : m_fileIn(NULL), m_xsecWeight(-99.), m_trigger(), m_eventID(0) {};
		Event(std::vector<std::pair<std::string, std::pair<double, double> > >::const_iterator fileIt, 
				const std::string& treeName, const std::vector<std::string>& trigList) 
			: m_fileInName(fileIt->first), m_xsecWeight(fileIt->second.first), 
				m_xsec(fileIt->second.second), m_eventID(0) 
		{ 
			if(m_debug) std::cout <<"Event: Analyse events from File("
									<< fileIt->first <<", "<< fileIt->second.first <<")"<< std::endl;
			m_fileIn = TFile::Open(m_fileInName.c_str(), "READ");
			m_tree = (TTree*) m_fileIn->Get(treeName.c_str());
			determineWhetherMC();
			m_tree->SetBranchStatus("*", 0);
			if(m_debug) std::cout <<"Event: Set branch status to 0" << std::endl;
			setTriggers(trigList);
			setupBranches(); 
			load(0);
		};
		~Event()
		{
			m_fileIn->Close();
			delete m_fileIn;
		};
		void addBranch(const char* name)
		{
			unsigned int found = 0;
			m_tree->SetBranchStatus(name, true, &found);
			if(m_debug) std::cout<<"Event::addBranch("<< name <<"): found = "<< found << std::endl;
			if(found)
			{
				TBranch* branch = m_tree->GetBranch(name);
				m_data.insert(std::pair<std::string, TBranch*>(name, branch));
			}else{
				 std::cout <<"ERROR: Event::addBranch: "<< name <<" was not found in TTree "<< m_tree->GetName() 
								<<" of "<< m_fileIn->GetName()<< std::endl;
			}
		}
		void addMCWeight();
		void addPileUpWeight();
		void addJvtEffSF();
		void addWeight(const std::string&, float);
		float avgNumIntsPerBX() const {return m_avgNumIntsPerBX;}
		void determineWhetherMC();
		void fillArrays();
		void getEntry(const char* name, void* handle)
		{
			std::map<std::string, TBranch*>::const_iterator branchIt; 
			branchIt = m_data.find(name);
			if(branchIt != m_data.end())
			{
				if(branchIt->second != NULL)
				{
					branchIt->second->SetAddress(handle);
					branchIt->second->GetEntry(m_eventID);
				}else{
					std::cout<<"Event::getEntry: Error! TBranch "<< name <<" was not found in the TTree!"<< std::endl;
				}
			}else{
				std::cout<<"Event::getEntry: Error! TBranch "<< name <<" was not found in this Event object."<< std::endl;
				std::cout<<"TBranches stored are:"<< std::endl;
				for(auto pair: m_data)
				{
					std::cout<<	pair.first <<" : "<< pair.second << std::endl;
				}
			}	
		}
		long id() const {return m_eventID; }
		int number() const {return m_eventNumber; }
		bool isMC() {return m_isMC;}
		void load(size_t eventID)
		{
			if(eventID < this->nEvents())
			{
				m_eventID = eventID;
				fillArrays();
				m_weights.clear();
				m_weights.insert(std::pair<std::string, float>("Cross-section", m_xsecWeight));//SPEED: Needn't be inserted every time.
				m_weightProd = m_xsecWeight;
				if(m_isMC) 
				{
					addMCWeight();
					addPileUpWeight();
					addJvtEffSF();
				}
			}else{
				std::cout<<"ERROR! goToEvent("<< eventID <<") failed because total events = "<< this->nEvents() << std::endl;
			}
		}
		float met() const{return m_met;}
		float metphi() const{return m_met_phi;}
		size_t nEvents() const {return m_tree->GetEntries(); }
		void setNJets(const int nJets) { m_nJets = nJets; }
		void setTriggers(const std::vector<std::string>& trigList)
		{
			if(m_isMC)
			{
				std::vector<std::string> trigListCopy;
				std::regex bjetTrigMatch = std::regex("_b");
				std::smatch matches;
				
				for(auto trig: trigList)
				{
					if(std::regex_search(trig, matches, bjetTrigMatch))
					{
						trigListCopy.push_back(trig+"_emul");	
					}else{
						trigListCopy.push_back(trig);	
					}
				}
				std::cout<<"After modification, the trigger list is:"<< std::endl;
				for(auto trig: trigListCopy)
				{
					std::cout<< trig << std::endl;
				}
			
				m_trigger = TriggerBits(trigListCopy);
			}else{
				m_trigger = TriggerBits(trigList);
			}
		}
		void setTriggerBits(const std::bitset<32>& bitSet) { m_trigger.setBits(bitSet); }
		void setTriggerBits(const unsigned long& bitSet) { m_trigger.setBits(bitSet); }
		void setTriggerBits(const TriggerBits bitSet) { m_trigger = bitSet; }
		void setupBranches();
		TTree* tree() const 
		{ 
			return m_tree;
		}
		bool triggered(const std::string& path) const { return m_trigger.triggered(path); }
		bool triggered() const { return m_trigger.triggered(); }
		
		TriggerBits triggerBits() const { return m_trigger; }//this is the list of triggers we select events with.
		float weight() const { return m_weightProd; } //return product of all weights
		float weight(const std::string&) const; //return specified weight only 
		const std::map<std::string, float>& weights() const { return m_weights; }
		float xsec() const {return m_xsec; }
		int eventNumber() const {return m_eventNumber; }
		int nJets() const { return m_nJets; }
		int runNumber() const {return m_runNumber; }

	private:
		TFile* m_fileIn;
		TriggerBits m_trigger; //this is the list of triggers we select events with.
		TTree* m_tree;
		std::string m_fileInName;
		float m_xsec, m_xsecWeight;
		float m_pileupWeight, m_jvtEffSF;
		float m_mcWeight;//used to reweight JZW samples event-by-event
		float m_mhhTruth;//used to reweight JZW samples event-by-event
		float m_weightProd; //Product of all weights in m_weights
		std::map<std::string, float> m_weights;
		long m_eventID;
		int m_eventNumber;
		int m_runNumber;
		int m_nJets;
		bool m_isMC;
		std::vector<std::string>* m_passedTriggers; //This holds TBranch contents.
		float m_met, m_met_phi;
		float m_avgNumIntsPerBX;
		//Container for arrays pointed to by the TTree.
		std::map<std::string, TBranch*> m_data; 
};
#endif 
