#include "TopCandidate.h"

TopCandidate::TopCandidate(const Jet& bjet, const Jet& wJet1, const Jet& wJet2): 
								m_bJet(bjet), m_wJet({wJet1, wJet2}), m_xt(-99)
{
	this->setIndex(bjet.index()*100+ wJet1.index()*10 + wJet2.index());

	ROOT::Math::PtEtaPhiEVector top(bjet + wJet1 + wJet2);
	ROOT::Math::PtEtaPhiEVector w(wJet1 + wJet2);
	this->SetCoordinates(top.Pt(), top.Eta(), top.Phi(), top.E());
	m_mW = w.M();
	m_ptW = w.Pt();
	m_mt = top.M();
	m_xt = sqrt(std::pow((m_mW-80.4)/(0.1*m_mW), 2) + std::pow((top.M() - 172.5)/(0.1*m_mt), 2));	
	m_dRjjW = wJet1.deltaR(wJet2);
	m_dRWb = bjet.deltaR(w);
	//m_truthTopId = -99;
}
float TopCandidate::deltaR(const Particle& that) const
{
        float dPhi = this->deltaPhi(that);
        float deltaR = sqrt((this->Eta()-that.Eta())*(this->Eta()-that.Eta())+dPhi*dPhi);
        return deltaR;
}
bool TopCandidate::sharesJet(const TopCandidate& other) const
{
	if(m_bJet.index() == other.getBJetRef().index()) return true;
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			if(m_wJet[i].index() == other.getWJetRef(j).index()) return true;
		}
	}
	return false;
}
/*void TopCandidate::setFatJetCoordinates(const ROOT::Math::PtEtaPhiEVector& p4) 
{
	m_fatjet.SetCoordinates(p4.Pt(), p4.Eta(), p4.Phi(), p4.E());
	this->setP4(p4);
}
void TopCandidate::setJetCoordinates(const int& jetNum, const ROOT::Math::PtEtaPhiEVector& p4) 
{
	m_jet[jetNum].SetCoordinates(p4.Pt(), p4.Eta(), p4.Phi(), p4.E());
	if(m_category == 0)
	{
		//std::cout<<"TopCandidate::setJetCoordinates("<< jetNum <<"): jet 0 + jet 1 = "<< m_jet1.p4() <<" + "<< m_jet2.p4() <<" = ";
		ROOT::Math::PtEtaPhiEVector hcandP4 = m_jet[0].p4() + m_jet[1].p4();		
		//std::cout<< hcandP4 << std::endl;
		this->setP4(hcandP4);
	}
}
float TopCandidate::mv2c10CaloSum() const
{
	float sum = 0.;
	if(m_category != 0) std::cout<<"TopCandidate::mv2c10CaloSum: is not appropriate for candidates of category "<< m_category <<"!"<< std::endl;
	// resolved 
	sum += 2; //Add 2, so 0 < sum < 4, rather than -2 < sum < 2
	sum += m_jet[0].mv2c10();
	sum += m_jet[1].mv2c10();
	return sum;
}
float TopCandidate::mv2c10Sum() const
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
float TopCandidate::jetPtScalarSum() const
{
	if(m_category != 0) return m_fatjet.Pt();
	else return m_jet[0].Pt() + m_jet[1].Pt();
}*/
