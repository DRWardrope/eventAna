#include "JetProducer.h"
#include <algorithm>
std::vector<Jet> JetProducer::produce(const float& ptMin, const float& etaMax, bool rejectNonBTagged, 
										const std::vector<Jet>& trackJets)
{
	if(m_debug) std::cout<<"JetProducer::produce("<< ptMin <<", "<< rejectNonBTagged <<"):" << std::endl;
	fillArrays();
	std::vector<Jet> jets;
  	if(m_debug) std::cout<<"JetProducer::produce: number of jets = "<< m_jet_pt->size() <<std::endl;
	for(int j = 0; j < m_jet_pt->size(); ++j)
	{
		if(fabs(m_jet_eta->at(j)) > etaMax) continue;
		float pt = 1e-3*m_jet_pt->at(j);
		if(pt < ptMin) continue;
		//Apply JVT cut
		if(pt < 60. && fabs(m_jet_eta->at(j)) < 2.4 && m_jet_jvt->at(j) < 0.59) continue;

		Jet jet(pt, m_jet_eta->at(j), m_jet_phi->at(j), 0.001*m_jet_E->at(j), j, m_jet_mv2c10->at(j));
		jet.setIsMuonCorrected(fabs(m_jet_pt_noMuCorr->at(j)/m_jet_pt->at(j) - 1) > 1e-4);
		jet.setTrackJetIdx(m_jet_trackJetIdx->at(j));
		std::vector<Jet> matchedTrackJets;
		for(int tjIdx: jet.trackJetIdx())
		{
			std::vector<Jet>::const_iterator thisTrackJet = trackJets.end();
			for(std::vector<Jet>::const_iterator trackJet = trackJets.begin(); trackJet != trackJets.end(); ++trackJet)
			{
				if(trackJet->index() == tjIdx)
				{
					thisTrackJet = trackJet;
				}
			}
			if(thisTrackJet == trackJets.end()) continue;
			matchedTrackJets.push_back(*thisTrackJet);
		}
		// sort track jets and add them to calo jet object
		std::sort(matchedTrackJets.begin(), matchedTrackJets.end(), [](const Jet a, const Jet b) {return b.mv2c10() < a.mv2c10();} ); 
		jet.setMatchedTrackJets(matchedTrackJets);

		if(!m_trackJetBTagging)
		{
			if(m_jet_mv2c10->at(j) > Jet::findMinBTag(85)) 
			{
				jet.setIsLooseBTagged(true);
				if(m_jet_mv2c10->at(j) > m_minBWeight) 
				{
					jet.setIsBTagged(true);
				}else 
				{
					jet.setIsBTagged(false);
				}
			}else 
			{
				jet.setIsLooseBTagged(false);
				jet.setIsBTagged(false);
			}
		}else
		{
			if(jet.matchedTrackJets().size() > 0)
			{
				if(jet.matchedTrackJets().at(0).isLooseBTagged()) 
				{
					jet.setIsLooseBTagged(true);
					if(jet.matchedTrackJets().at(0).isBTagged())
					{
						jet.setIsBTagged(true);
					}else
					{
						jet.setIsBTagged(false);
					}
				}else
				{
					jet.setIsLooseBTagged(false);
				}
			}else
			{
				jet.setIsLooseBTagged(false);
				jet.setIsBTagged(false);
			}
		}
		if(rejectNonBTagged && !jet.isBTagged()) continue;

		jet.setRParam(0.4);
		jet.setPassBadLoose(m_jet_passBadLoose->at(j));
		jet.setPtRecoFactor(m_jet_factorPtReco->at(j));
		jet.setTruthJetP4(ROOT::Math::PtEtaPhiEVector(-9999., -9999., -9999., -9999.));
		if(m_debug) std::cout<<"m_evt.isMC() = "<< m_evt.isMC() << std::endl;
		if(m_evt.isMC())
		{
			jet.setBTagSF(m_jet_mv2c10_SF->at(j));
			jet.setPartIdx(m_jet_partIdx->at(j));
			jet.setPdgId(m_jet_pdgId->at(j));
			if(m_jet_truthJetIdx->at(j).size() > 0)
			{
				int tjIdx = m_jet_truthJetIdx->at(j)[0];//Take first jet, assuming this is the best match.
				ROOT::Math::PtEtaPhiEVector truthJetP4(1e-3*m_truthJet_pt->at(tjIdx), m_truthJet_eta->at(tjIdx), 
														m_truthJet_phi->at(tjIdx), 1e-3*m_truthJet_E->at(tjIdx));
				jet.setTruthJetP4(truthJetP4);
			}
		}else
		{
			jet.setBTagSF(1.);
			jet.setPdgId(-99);
			jet.setPartIdx(std::vector<int>());
		}
		if(m_debug) std::cout<<"JetProducer::produce: pushing back jet."<< std::endl;
		jets.push_back(jet);
	}
	std::sort(jets.begin(), jets.end(), [](const Jet a, const Jet b) 
	{
        return b.Pt() < a.Pt();   
    }); 
	if(m_debug) std::cout<<"JetProducer::produce: returning "<< jets.size() <<" jets."<< std::endl;
	return jets;
}

void JetProducer::setupBranches()
{
	nullPtrs();
	std::string ptBranchStem = m_branchStem;
	if(m_calib.size() != 0) ptBranchStem += m_calib+"_";
	m_evt.addBranch((m_branchStem+"pt").c_str());
	m_evt.addBranch((m_branchStem+"pt_muCorr").c_str());
	m_evt.addBranch((m_branchStem+"eta_muCorr").c_str());
	m_evt.addBranch((m_branchStem+"phi_muCorr").c_str());
	m_evt.addBranch((m_branchStem+"E_muCorr").c_str());
	m_evt.addBranch((m_branchStem+"Jvt").c_str());
	m_evt.addBranch((m_branchStem+"clean_passLooseBad").c_str());
	m_evt.addBranch((m_branchStem+"mv2c10").c_str());
	m_evt.addBranch((m_branchStem+"factorPtReco").c_str());
	if(m_evt.isMC())
	{
		m_evt.addBranch((m_branchStem+"mv2c10_SFFix70").c_str());
		m_evt.addBranch((m_branchStem+"pdgid").c_str());
		m_evt.addBranch((m_branchStem+"idxTruthParts").c_str());
		m_evt.addBranch((m_branchStem+"idxTruthJets").c_str());
		std::string truthJetStem = "truthbjet_akt4_";
		m_evt.addBranch((truthJetStem+"pt").c_str());
		m_evt.addBranch((truthJetStem+"eta").c_str());
		m_evt.addBranch((truthJetStem+"phi").c_str());
		m_evt.addBranch((truthJetStem+"E").c_str());
	}
	m_evt.addBranch((m_branchStem+"idxTrackJets").c_str());
}
void JetProducer::fillArrays()
{
	if(m_debug) std::cout<<"JetProducer::fillArrays: beginning"<< std::endl;
	cleanUp();

	m_evt.getEntry((m_branchStem+"pt").c_str(), &m_jet_pt_noMuCorr);
	m_evt.getEntry((m_branchStem+"pt_muCorr").c_str(), &m_jet_pt);
	m_evt.getEntry((m_branchStem+"eta_muCorr").c_str(), &m_jet_eta);
	m_evt.getEntry((m_branchStem+"phi_muCorr").c_str(), &m_jet_phi);
	m_evt.getEntry((m_branchStem+"E_muCorr").c_str(), &m_jet_E);
	m_evt.getEntry((m_branchStem+"mv2c10").c_str(), &m_jet_mv2c10);
	m_evt.getEntry((m_branchStem+"factorPtReco").c_str(), &m_jet_factorPtReco);
	if(m_evt.isMC())
	{
		m_evt.getEntry((m_branchStem+"mv2c10_SFFix70").c_str(), &m_jet_mv2c10_SF);
		m_evt.getEntry((m_branchStem+"pdgid").c_str(), &m_jet_pdgId);
		m_evt.getEntry((m_branchStem+"idxTruthParts").c_str(), &m_jet_partIdx);
		m_evt.getEntry((m_branchStem+"idxTruthJets").c_str(), &m_jet_truthJetIdx);
		std::string truthJetStem = "truthbjet_akt4_";
		m_evt.getEntry((truthJetStem+"pt").c_str(), &m_truthJet_pt);
		m_evt.getEntry((truthJetStem+"eta").c_str(), &m_truthJet_eta);
		m_evt.getEntry((truthJetStem+"phi").c_str(), &m_truthJet_phi);
		m_evt.getEntry((truthJetStem+"E").c_str(), &m_truthJet_E);
	}
	m_evt.getEntry((m_branchStem+"Jvt").c_str(), &m_jet_jvt);
	m_evt.getEntry((m_branchStem+"clean_passLooseBad").c_str(), &m_jet_passBadLoose);
	m_evt.getEntry((m_branchStem+"idxTrackJets").c_str(), &m_jet_trackJetIdx);
}
void JetProducer::cleanUp()
{
	nullPtrs();
	delete m_jet_pt;
	delete m_jet_pt_noMuCorr;
	delete m_jet_eta;
	delete m_jet_phi;
	delete m_jet_E;
	delete m_jet_mv2c10;
	delete m_jet_factorPtReco;
	delete m_jet_mv2c10_SF;
	delete m_jet_jvt;
	delete m_jet_passBadLoose;
	delete m_jet_pdgId;
	delete m_jet_trackJetIdx;
	delete m_jet_truthJetIdx;
	delete m_jet_partIdx;
	delete m_truthJet_pt;
	delete m_truthJet_eta;
	delete m_truthJet_phi;
	delete m_truthJet_E;
	nullPtrs();
}
void JetProducer::nullPtrs()
{
	m_jet_pt = nullptr;
	m_jet_pt_noMuCorr = nullptr;
	m_jet_eta = nullptr;
	m_jet_phi = nullptr;
	m_jet_E = nullptr;
	m_jet_mv2c10 = nullptr;
	m_jet_mv2c10_SF = nullptr;
	m_jet_factorPtReco = nullptr;
	m_jet_jvt = nullptr;
	m_jet_passBadLoose = nullptr;
	m_jet_pdgId = nullptr;
	m_jet_trackJetIdx = nullptr;
	m_jet_truthJetIdx = nullptr;
	m_jet_partIdx = nullptr;
	m_truthJet_pt = nullptr;
	m_truthJet_eta = nullptr;
	m_truthJet_phi = nullptr;
	m_truthJet_E = nullptr;
}
