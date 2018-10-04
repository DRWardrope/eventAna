#include "MuonProducer.h"
std::vector<Muon> MuonProducer::produce(const float& ptMin, bool reqPassMedium)
{
	if(m_debug) std::cout<<"MuonProducer::produce("<< ptMin <<", "<< reqPassMedium <<"):" << std::endl;
	fillArrays();
	std::vector<Muon> muons;
	for(int m = 0; m < m_muon_pt->size(); ++m)
	{
		float pt = 1e-3*m_muon_pt->at(m);
		if(pt < ptMin) continue;
		ROOT::Math::PtEtaPhiMVector muonP4(pt, m_muon_eta->at(m), m_muon_phi->at(m), 0.105658);
		Muon muon(pt, m_muon_eta->at(m), m_muon_phi->at(m), muonP4.E(), m, 
					1e-3*m_muon_eLoss->at(m), m_muon_quality->at(m), m_muon_isolated->at(m));
		muons.push_back(muon);
	}
    std::sort(muons.begin(), muons.end(), [](const Muon a, const Muon b){ return b.Pt() < a.Pt();});
 
	if(m_debug) std::cout<<"MuonProducer::produce: returning "<< muons.size() <<" muons."<< std::endl;
	return muons;
}

void MuonProducer::setupBranches()
{
	nullPtrs();
	m_evt.addBranch("muon_pt");
	m_evt.addBranch("muon_eta");
	m_evt.addBranch("muon_phi");
	m_evt.addBranch("muon_EnergyLoss");
	m_evt.addBranch("muon_quality");
	m_evt.addBranch("muon_isLooseTrackOnlyIso");
}
void MuonProducer::fillArrays()
{
	if(m_debug) std::cout<<"MuonProducer::fillArrays: beginning"<< std::endl;
	cleanUp();
	m_evt.getEntry("muon_pt", &m_muon_pt);
	m_evt.getEntry("muon_eta", &m_muon_eta);
	m_evt.getEntry("muon_phi", &m_muon_phi);
	m_evt.getEntry("muon_EnergyLoss", &m_muon_eLoss);
	m_evt.getEntry("muon_quality", &m_muon_quality);
	m_evt.getEntry("muon_isLooseTrackOnlyIso", &m_muon_isolated);
}
void MuonProducer::cleanUp()
{
	nullPtrs();
	delete m_muon_pt;
	delete m_muon_eta;
	delete m_muon_phi;
	delete m_muon_eLoss;
	delete m_muon_quality;
	delete m_muon_isolated;
	nullPtrs();
}
void MuonProducer::nullPtrs()
{
	m_muon_pt = nullptr;
	m_muon_eta = nullptr;
	m_muon_phi = nullptr;
	m_muon_eLoss = nullptr;
	m_muon_quality = nullptr;
	m_muon_isolated = nullptr;
}
