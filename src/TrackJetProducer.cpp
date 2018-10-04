#include "TrackJetProducer.h"
std::vector<Jet> TrackJetProducer::produce(const float& ptMin, bool rejectNonBTagged)
{
	fillArrays();
	std::vector<Jet> jets;
	if(m_debug) std::cout<<"TrackJetProducer::produce: m_jet_pt.size() = "<< m_jet_pt->size() << std::endl;
	for(int j = 0; j < m_jet_pt->size(); ++j)
	{
		//std::cout<<"m_jet_pt->at("<< j <<").size() = "<< m_jet_pt->at(j).size() << std::endl;
		float pt = 1e-3*m_jet_pt->at(j);	
		if(pt > 13.e3 || pt < ptMin) continue;
		//Mass is stored in ntuple, so need to convert to E. Also E stored in MeV, so must multiply by 1e-3 to get GeV.
		ROOT::Math::PtEtaPhiMVector jetP4(pt, m_jet_eta->at(j), m_jet_phi->at(j), 0.001*m_jet_m->at(j));
		Jet jet(jetP4.Pt(), m_jet_eta->at(j), m_jet_phi->at(j), jetP4.E(), j, m_jet_mv2c10->at(j));
		if(m_jet_mv2c10->at(j) > m_minBWeight)
		{
			jet.setIsBTagged(true);
		}else{
			jet.setIsBTagged(false);
		} 
		if(m_jet_mv2c10->at(j) > findMinBTag(85))
		{
			jet.setIsLooseBTagged(true);
		}else{
			jet.setIsLooseBTagged(false);
		} 

		if(rejectNonBTagged && !jet.isBTagged()) continue;
		jet.setRParam(0.2);
		jet.setMV2c10(m_jet_mv2c10->at(j));
		if(m_evt.isMC())
		{
			jet.setBTagSF(1.);//FIXME!
			//jet.setBTagSF(m_jet_mv2c10_SF->at(j));
			jet.setPdgId(m_jet_pdgId->at(j));
			jet.setPartIdx(m_jet_partIdx->at(j));
		}else{
			jet.setBTagSF(1.);
			jet.setPdgId(-99);
			jet.setPartIdx(std::vector<int>());
		}
		jet.setTrackJetIdx(std::vector<int>(1, j));
		if(m_debug) std::cout<<"TrackJetProducer::produce: pushing back jet "<< j <<", pt = "<< jet.pt() <<" GeV."<< std::endl;
		jets.push_back(jet);
	}
	if(m_debug) std::cout<<"TrackJetProducer::produce: returning "<< jets.size() <<" jets."<< std::endl;
	return jets;
}


void TrackJetProducer::setupBranches()
{
	nullPtrs();
	std::string branchStem = "trackjet_akt2_";
	m_evt.addBranch((branchStem+"pt").c_str());
	m_evt.addBranch((branchStem+"eta").c_str());
	m_evt.addBranch((branchStem+"phi").c_str());
	m_evt.addBranch((branchStem+"m").c_str());
	m_evt.addBranch((branchStem+"mv2c10").c_str());

	if(m_evt.isMC())
	{
		//m_evt.addBranch((branchStem+"mv2c10_SFFix77").c_str());
		m_evt.addBranch((branchStem+"pdgid").c_str());
		m_evt.addBranch((branchStem+"idxTruthParts").c_str());
	}
}
void TrackJetProducer::fillArrays()
{
	if(m_debug) std::cout<<"TrackJetProducer::fillArrays: beginning"<< std::endl;
	cleanUp();
	nullPtrs();
	std::string branchStem = "trackjet_akt2_";
	m_evt.getEntry((branchStem+"pt").c_str(), &m_jet_pt);
	m_evt.getEntry((branchStem+"eta").c_str(), &m_jet_eta);
	m_evt.getEntry((branchStem+"phi").c_str(), &m_jet_phi);
	m_evt.getEntry((branchStem+"m").c_str(), &m_jet_m);
	m_evt.getEntry((branchStem+"mv2c10").c_str(), &m_jet_mv2c10);
	if(m_evt.isMC())
	{
		//m_evt.getEntry((branchStem+"mv2c10_SFFix77").c_str(), &m_jet_mv2c10_SF);
		m_evt.getEntry((branchStem+"pdgid").c_str(), &m_jet_pdgId);
		m_evt.getEntry((branchStem+"idxTruthParts").c_str(), &m_jet_partIdx);
	}
}
void TrackJetProducer::nullPtrs()
{
	m_jet_pt = nullptr;
	m_jet_eta = nullptr;
	m_jet_phi = nullptr;
	m_jet_m = nullptr;
	m_jet_mv2c10 = nullptr;
	m_jet_mv2c10_SF = nullptr;
	m_jet_pdgId = nullptr;
	m_jet_partIdx = nullptr;
}
void TrackJetProducer::cleanUp()
{
	delete m_jet_pt;
	delete m_jet_eta;
	delete m_jet_phi;
	delete m_jet_m;
	delete m_jet_mv2c10;
	delete m_jet_mv2c10_SF;
	delete m_jet_pdgId;
	delete m_jet_partIdx;
}
