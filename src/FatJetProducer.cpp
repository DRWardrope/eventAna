#include "FatJetProducer.h"
std::vector<Jet> FatJetProducer::produce(const float& minPt, const float& etaMax, const std::vector<Jet>& trackJets)
{
	fillArrays();
	std::vector<Jet> jets;
	for(int j = 0; j < m_jet_pt->size(); ++j)
	{
		//Currently, pt and mass are stored in Mev for R=1.0 jets
		if(0.001*m_jet_pt->at(j) > 1500.) continue;//Not calibrated by JETM group
		if(0.001*m_jet_m->at(j) < 50.) continue;//Not calibrated either
		if(0.001*m_jet_pt->at(j) < minPt) continue;
		if(fabs(m_jet_eta->at(j)) > etaMax) continue;
		ROOT::Math::PtEtaPhiMVector jetP4(0.001*m_jet_pt->at(j), m_jet_eta->at(j), m_jet_phi->at(j), 0.001*m_jet_m->at(j));
		Jet jet(jetP4.Pt(), m_jet_eta->at(j), m_jet_phi->at(j), jetP4.E(), j); //No mv2c20 for fat-jet.
		jet.setRParam(1.0);
		jet.setTrackJetIdx(m_jet_trackJetIdx->at(j));
		std::vector<Jet> matchedTrackJets;
		for(int tjIdx: jet.trackJetIdx())
		{
			for(auto trackJet : trackJets)
			{
				if(trackJet.index() == tjIdx) matchedTrackJets.push_back(trackJet);
			}
		}	
		std::sort(matchedTrackJets.begin(), matchedTrackJets.end(), [](const Jet a, const Jet b) {return b.mv2c10() < a.mv2c10();} ); 
		jet.setMatchedTrackJets(matchedTrackJets);
		jets.push_back(jet);
	}
	std::sort(jets.begin(), jets.end(), [](const Jet a, const Jet b) 
	{
        return b.Pt() < a.Pt();   
    }); 
	return jets;
}


void FatJetProducer::setupBranches()
{
	nullPtrs();
	std::string branchStem = "calojet_akt10_";
	m_evt.addBranch((branchStem+"pt").c_str());
	m_evt.addBranch((branchStem+"eta").c_str());
	m_evt.addBranch((branchStem+"phi").c_str());
	m_evt.addBranch((branchStem+"m").c_str());
	m_evt.addBranch((branchStem+"idxTrackJets").c_str());
}
void FatJetProducer::fillArrays()
{
	if(m_debug) std::cout<<"FatJetProducer::fillArrays: beginning"<< std::endl;
	cleanUp();
	nullPtrs();
	std::string branchStem = "calojet_akt10_";
	m_evt.getEntry((branchStem+"pt").c_str(), &m_jet_pt);
	m_evt.getEntry((branchStem+"eta").c_str(), &m_jet_eta);
	m_evt.getEntry((branchStem+"phi").c_str(), &m_jet_phi);
	m_evt.getEntry((branchStem+"m").c_str(), &m_jet_m);
	m_evt.getEntry((branchStem+"idxTrackJets").c_str(), &m_jet_trackJetIdx);
}
void FatJetProducer::cleanUp()
{
	delete m_jet_pt;
	delete m_jet_eta;
	delete m_jet_phi;
	delete m_jet_m;
	delete m_jet_trackJetIdx;
}
void FatJetProducer::nullPtrs()
{
	m_jet_pt = nullptr;
	m_jet_eta = nullptr;
	m_jet_phi = nullptr;
	m_jet_m = nullptr;
	m_jet_trackJetIdx = nullptr;
}
