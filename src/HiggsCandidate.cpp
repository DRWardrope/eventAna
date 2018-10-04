#include "HiggsCandidate.h"

HiggsCandidate::HiggsCandidate(const Jet& jet): m_fatjet(jet), m_jet(std::vector<Jet>()), 
												Particle(jet.pt(), jet.eta(), jet.phi(), jet.E(), jet.index()*100), 
												m_truthHiggsId(-99), m_dRbb(-99), m_muCorrId({-99, -99})
{
	m_xt = -99.;	
	//m_truthHiggsId = -99;
	if(jet.matchedTrackJets().size() > 1)//This might correspond to a two-tag candidate
	{
		m_jet = {{jet.matchedTrackJets().at(0),jet.matchedTrackJets().at(1)}}; 
		m_dR = m_jet[0].deltaR(m_jet[1]);
		m_dRTrackJets = m_dR;
		if(m_jet[0].isLooseBTagged())
		{
			m_looselyBTagged = m_jet[1].isLooseBTagged();
			if(m_jet[0].isBTagged())
			{
				m_fullyBTagged = true; //Candidate is fully b-tagged, but for 1-tag or 2-tag category?
				m_numTightTags = 1;
				if(m_jet[1].isBTagged())
				{
					m_category = 1; //2-tag candidate --> category 1
					m_numTightTags = 2;
				}else
				{
					m_category = 2; //1-tag candidate --> category 2 
					//m_numTightTags = 2; //Need to think about this
				}
			}else
			{
				m_numTightTags = 0;
				m_fullyBTagged = false;
				m_category = 1; //Call these candidates "2-tag" for background modelling purposes 
			}
		}
	}else if(jet.matchedTrackJets().size() == 1)
	{
		m_jet = {{jet.matchedTrackJets().at(0)}};
		m_dR = 0.;
		m_dRTrackJets = 0.;
		m_category = 2; //Only possible to find 1-tag --> category 2.
		m_fullyBTagged = jet.isBTagged(); 
		m_looselyBTagged = jet.isLooseBTagged(); 
		m_numTightTags = (jet.isBTagged() ? 2 : 0);
	}
	//m_truthHiggsId = -99;
	m_xt = -99.;	
}
HiggsCandidate::HiggsCandidate(const Jet& jet1, const Jet& jet2): m_truthHiggsId(-99), m_dRbb(-99), m_muCorrId({-99, -99})
{
	if(jet1.pt() > jet2.pt())
	{
		this->setIndex(jet1.index()*10 + jet2.index()), 
		m_jet = {jet1, jet2};
	}else
	{
		this->setIndex(jet2.index()*10 + jet1.index()), 
		m_jet = {jet2, jet1};
	}

	ROOT::Math::PtEtaPhiEVector dijet(jet1 + jet2);
	this->SetCoordinates(dijet.Pt(), dijet.Eta(), dijet.Phi(), dijet.E());
	m_dR = jet1.deltaR(jet2);
	m_dRTrackJets = m_dR;
	m_category = 0;
	m_numTightTags = 0;	
	if(jet1.isBTagged()) ++m_numTightTags;
	if(jet2.isBTagged()) ++m_numTightTags;
	m_fullyBTagged = jet1.isBTagged() && jet2.isBTagged();
	m_looselyBTagged = jet1.isLooseBTagged() && jet2.isLooseBTagged();
	m_xt = -99.;	
	//m_truthHiggsId = -99;
}
const Jet* HiggsCandidate::getFatJet() const
{
	if(m_category >= 1 &&  m_category <= 4) return &m_fatjet;
	else std::cout <<"HiggsCandidate::getJet: This HiggsCandidate is not associated with a fat-jet."<< std::endl;
	return nullptr;
}
float HiggsCandidate::deltaR(const Particle& that) const
{
        float dPhi = this->deltaPhi(that);
        float deltaR = sqrt((this->Eta()-that.Eta())*(this->Eta()-that.Eta())+dPhi*dPhi);
        return deltaR;
}
void HiggsCandidate::setFatJetCoordinates(const ROOT::Math::PtEtaPhiEVector& p4) 
{
	m_fatjet.SetCoordinates(p4.Pt(), p4.Eta(), p4.Phi(), p4.E());
	this->setP4(p4);
}
void HiggsCandidate::setJetCoordinates(const int& jetNum, const ROOT::Math::PtEtaPhiEVector& p4) 
{
	m_jet[jetNum].SetCoordinates(p4.Pt(), p4.Eta(), p4.Phi(), p4.E());
	if(m_category == 0)
	{
		//std::cout<<"HiggsCandidate::setJetCoordinates("<< jetNum <<"): jet 0 + jet 1 = "<< m_jet1.p4() <<" + "<< m_jet2.p4() <<" = ";
		ROOT::Math::PtEtaPhiEVector hcandP4 = m_jet[0].p4() + m_jet[1].p4();		
		//std::cout<< hcandP4 << std::endl;
		this->setP4(hcandP4);
	}
}
bool HiggsCandidate::sharesJet(const HiggsCandidate& other) const
{
	if(m_category - other.category() != 0) 
	{
		return false;
	}		
	if(m_category == 2)
	{
		if(this->getFatJet()->index() == other.getFatJet()->index()) return true;
		else return false;
	}
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(this->getJet(i)->index() == other.getJet(j)->index()) return true;
		}
	}
	return false;
}
float HiggsCandidate::mv2c10CaloSum() const
{
	float sum = 0.;
	if(m_category != 0) std::cout<<"HiggsCandidate::mv2c10CaloSum: is not appropriate for candidates of category "<< m_category <<"!"<< std::endl;
	// resolved 
	sum += 2; //Add 2, so 0 < sum < 4, rather than -2 < sum < 2
	sum += m_jet[0].mv2c10();
	sum += m_jet[1].mv2c10();
	return sum;
}
float HiggsCandidate::mv2c10Sum() const
{
	float sum = 0.;
	if(m_category == 0) 
	{
		// resolved 
		sum += 2; //Add 2, so 0 < sum < 4, rather than -2 < sum < 2
		//if (m_jet[0].matchedTrackJets().size() > 0) sum += m_jet[0].matchedTrackJets().at(0).mv2c10();
		//if (m_jet[1].matchedTrackJets().size() > 0) sum += m_jet[1].matchedTrackJets().at(0).mv2c10();
		sum += m_jet[0].mv2c10();
		sum += m_jet[1].mv2c10();
	}else if (m_category == 1) 
	{
		sum += 2; //Add 2, so 0 < sum < 4, rather than -2 < sum < 2
		// semimerged (here 'm_jet[0]' and 'm_jet[1]' are actually the trackjets)
		sum += m_jet[0].mv2c10();
		sum += m_jet[1].mv2c10();
	}else if (m_category == 2) 
	{
		// merged (here 'fatjet' is the jet, only one associatedtrackjet by construction)
		sum += 1;
		if (m_fatjet.matchedTrackJets().size() > 0) sum += m_fatjet.matchedTrackJets().at(0).mv2c10(); 
	}
	if (sum == 0.) return -99.;
	return sum; 
}
float HiggsCandidate::jetPtScalarSum() const
{
	if(m_category != 0) return m_fatjet.Pt();
	else return m_jet[0].Pt() + m_jet[1].Pt();
}
