#include "ElectronProducer.h"
std::vector<Electron> ElectronProducer::produce(const float& ptMin, bool reqPassMedium)
{
	//Create electrons from input file variables
	if(m_debug) std::cout<<"ElectronProducer::produce("<< ptMin <<", "<< reqPassMedium <<"):" << std::endl;
	fillArrays();
	std::vector<Electron> electrons;
	for(int m = 0; m < m_electron_pt->size(); ++m)
	{
		float pt = 1e-3*m_electron_pt->at(m); //conversion from MeV --> GeV
		if(pt < ptMin) continue;
		Electron electron(pt, m_electron_eta->at(m), m_electron_phi->at(m), 0.001*m_electron_E->at(m), m, 
							m_electron_likelihood->at(m), m_electron_isolated->at(m));
		electrons.push_back(electron);
	}
    std::sort(electrons.begin(), electrons.end(), [](const Electron a, const Electron b){ return b.Pt() < a.Pt();});
 
	if(m_debug) std::cout<<"ElectronProducer::produce: returning "<< electrons.size() <<" electrons."<< std::endl;
	return electrons;
}

void ElectronProducer::setupBranches()
{
	nullPtrs();
	m_evt.addBranch("electron_pt");
	m_evt.addBranch("electron_eta");
	m_evt.addBranch("electron_phi");
	m_evt.addBranch("electron_E");
	m_evt.addBranch("electron_LH");
	m_evt.addBranch("electron_isLooseTrackOnlyIso");
}
void ElectronProducer::fillArrays()
{
	if(m_debug) std::cout<<"ElectronProducer::fillArrays: beginning"<< std::endl;
	cleanUp();
	m_evt.getEntry("electron_pt", &m_electron_pt);
	m_evt.getEntry("electron_eta", &m_electron_eta);
	m_evt.getEntry("electron_phi", &m_electron_phi);
	m_evt.getEntry("electron_E", &m_electron_E);
	m_evt.getEntry("electron_LH", &m_electron_likelihood);
	m_evt.getEntry("electron_isLooseTrackOnlyIso", &m_electron_isolated);
}
void ElectronProducer::cleanUp()
{
	nullPtrs();
	delete m_electron_pt;
	delete m_electron_eta;
	delete m_electron_phi;
	delete m_electron_E;
	delete m_electron_likelihood;
	delete m_electron_isolated;
	nullPtrs();
}
void ElectronProducer::nullPtrs()
{
	m_electron_pt = nullptr;
	m_electron_eta = nullptr;
	m_electron_phi = nullptr;
	m_electron_E = nullptr;
	m_electron_likelihood = nullptr;
	m_electron_isolated = nullptr;
}
