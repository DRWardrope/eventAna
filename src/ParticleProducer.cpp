#include "ParticleProducer.h"
std::vector<Particle> ParticleProducer::produce(int selPdgId)
{
	fillArrays();
	std::vector<Particle> particles;
	int absSelPdgId = abs(selPdgId);
	if(m_debug) std::cout<<"ParticleProducer::producer("<< selPdgId <<"): total number of particles = "<< m_part_pt->size() << std::endl;
	for(int j = 0; j < m_part_pt->size(); ++j)
	{
		if(abs(m_part_pdgId->at(j)) != absSelPdgId) continue;
		//if(m_part_pt->at(j) > 1.3e7) continue;//spurious very high pT e.g. >10 TeV parts cause crashes
		if(m_debug) std::cout << "ParticleProducer::produce part " << j <<", pdgID = "<< m_part_pdgId->at(j) 
						<< ": pT = "<< 0.001*m_part_pt->at(j) << " GeV" << std::endl;

		ROOT::Math::PtEtaPhiMVector partP4(0.001*m_part_pt->at(j), m_part_eta->at(j), m_part_phi->at(j), 0.001*m_part_m->at(j));
		Particle part(partP4.Pt(), m_part_eta->at(j), m_part_phi->at(j), partP4.E(), j);
		part.setPdgId(m_part_pdgId->at(j));

		if(m_debug) std::cout<<"ParticleProducer::produce: pushing back part."<< std::endl;
		particles.emplace_back(part);
	}

	if(m_debug) std::cout<<"ParticleProducer::produce: returning "<< particles.size() <<" parts."<< std::endl;
	return particles;
}
std::vector<Particle> ParticleProducer::produceByIndex(std::vector<int> indices)
{
	fillArrays();
	std::vector<Particle> particles;
	if(m_debug) std::cout<<"ParticleProducer::produceByIndex: total number of particles = "<< m_part_pt->size() << std::endl;
	for(int idx: indices)
	{
//		std::cout<<"ParticleProducer::produceByIndex: creating particle for idx = "<< idx << std::endl;
		ROOT::Math::PtEtaPhiMVector partP4(0.001*m_part_pt->at(idx), m_part_eta->at(idx), m_part_phi->at(idx), 0.001*m_part_m->at(idx));
		Particle part(partP4.Pt(), m_part_eta->at(idx), m_part_phi->at(idx), partP4.E(), idx);
		part.setPdgId(m_part_pdgId->at(idx));
		if(m_debug) std::cout<<"ParticleProducer::produce: pushing back part."<< std::endl;
		particles.emplace_back(part);
	}	

	if(m_debug) std::cout<<"ParticleProducer::produceByIndex: returning "<< particles.size() <<" parts."<< std::endl;
	return particles;
}

void ParticleProducer::setupBranches()
{
	nullPtrs();
	//m_evt.addBranch("nparts"); Calculate using vector sizes.
	m_evt.addBranch("truthpart_pt");
	m_evt.addBranch("truthpart_eta");
	m_evt.addBranch("truthpart_phi");
	m_evt.addBranch("truthpart_m");
	m_evt.addBranch("truthpart_pdgid");
}
void ParticleProducer::fillArrays()
{
	if(m_debug) std::cout<<"ParticleProducer::fillArrays: beginning"<< std::endl;
	cleanUp();
	//m_evt.getEntry("nparts", &m_jet_n);
	m_evt.getEntry("truthpart_pt", &m_part_pt);
	m_evt.getEntry("truthpart_eta", &m_part_eta);
	m_evt.getEntry("truthpart_phi", &m_part_phi);
	m_evt.getEntry("truthpart_m", &m_part_m);
	m_evt.getEntry("truthpart_pdgid", &m_part_pdgId);
}
void ParticleProducer::nullPtrs()
{
	m_part_pt = nullptr;
	m_part_eta = nullptr;
	m_part_phi = nullptr;
	m_part_m = nullptr;
	m_part_pdgId = nullptr;
}
void ParticleProducer::cleanUp()
{
	nullPtrs();
	delete m_part_pt;
	delete m_part_eta;
	delete m_part_phi;
	delete m_part_m;
	delete m_part_pdgId;
	nullPtrs();
}
