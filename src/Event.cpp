#include "Event.h"
#include <algorithm>
#include <iostream>

void Event::addWeight(const std::string& title, float weight)
{
  std::map<std::string, float>::const_iterator weightIt = m_weights.find(title);

  if(weightIt != m_weights.end())
  {
    std::cout<<"Event::addWeight: Error! Weight already defined for \""<< title <<"\"!"<< std::endl;
  }else{
    m_weights.insert(std::pair<std::string, float>(title, weight));
    if(m_debug) std::cout<<"Event::addWeight: Inserting std::pair<std::string, float>("<< title <<", "<< weight <<")"<< std::endl;
    m_weightProd *= weight;
  }
}
void Event::addPileUpWeight()
{
    const std::string title = "PileUp";
	float weight = m_pileupWeight;
	addWeight(title,weight);
}
void Event::addMCWeight()
{
    const std::string title = "MC";
	float weight = m_mcWeight;
	addWeight(title,weight);
}
void Event::addJvtEffSF()
{
    const std::string title = "JvtEffSF";
	float weight = m_jvtEffSF;
	addWeight(title,weight);
}
float Event::weight(const std::string& title) const
{
	std::map<std::string, float>::const_iterator weightIt = m_weights.find(title);
	if(weightIt != m_weights.end())
	{
		return weightIt->second;
	}else{
		std::cout<<"Event::weight: Error! No weight for \""<< title <<"\"!"<< std::endl;
		return -99.;
	}
}
void Event::fillArrays()
{
	for(unsigned int id = 0; id < m_trigger.names().size(); ++id)
	{
		bool decision;
		if(m_debug) std::cout<<"Event::fillArrays: trigger decision for "<< m_trigger.names()[id] <<": ";
		this->getEntry(m_trigger.names()[id].c_str(), &decision);
		if(m_debug) std::cout<< decision << std::endl;
		m_trigger.set(id, decision);	
	}
	if(m_isMC)
	{
		this->getEntry("mcEventWeight", &m_mcWeight);
		this->getEntry("MhhTruth", &m_mhhTruth);
		this->getEntry("pileupWeight", &m_pileupWeight);
		this->getEntry("JvtEffSF", &m_jvtEffSF);
	}
	this->getEntry("runNumber", &m_runNumber);
	this->getEntry("eventNumber", &m_eventNumber);
	this->getEntry("averageInteractionsPerCrossing", &m_avgNumIntsPerBX);
	this->getEntry("MET", &m_met);
	this->getEntry("MET_phi", &m_met_phi);
	m_met *= 1e-3; //Convert MeV->GeV
}
void Event::setupBranches()
{
	if(m_debug) std::cout<<"Event::setupBranches():"<< std::endl;
	for(auto name : m_trigger.names())
	{
		this->addBranch(name.c_str());
	}
	if(m_isMC)
	{
		this->addBranch("mcEventWeight");
		this->addBranch("MhhTruth");
		this->addBranch("pileupWeight");
		this->addBranch("JvtEffSF");
	}
	this->addBranch("runNumber");
	this->addBranch("eventNumber");
	this->addBranch("averageInteractionsPerCrossing");
	this->addBranch("MET");
	this->addBranch("MET_phi");
}
void Event::determineWhetherMC()
{
    std::regex dataMatch = std::regex("data1[5-6]");
    std::smatch matches;
    
    if(std::regex_search(m_fileInName, matches, dataMatch))
    {
		m_isMC = false;
    }else
	{
		m_isMC = true;
	}
}
