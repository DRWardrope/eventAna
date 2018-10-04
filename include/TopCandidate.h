#ifndef TOPCANDIDATE_H
#define TOPCANDIDATE_H
/*************************************************************************************************
*
* TopCandidate: class to represent Top candidates.
* David Wardrope, 26th April 2017
*
*************************************************************************************************/
#include <array>
#include <memory>
#include "Jet.h"
#include "TRandom3.h"

class TopCandidate : public Particle
{
	public:
		TopCandidate(): Particle(-99), m_bJet(Jet()), m_wJet(std::vector<Jet>())  {};
							//m_dRTrackJets(-99.), m_dRbb(-99), m_xt(-99.), m_fullyBTagged(false), m_category(-99),
							//m_truthTopId(-99), m_muCorrId({-99, -99})  {};
		TopCandidate(const Jet& bjet1, const Jet& wJet1, const Jet& wJet2);
		//TopCandidate(const Jet& fatjet, const Jet& subjet1, const Jet& subjet2); //Track-jets are now stored in Jet object.
//		TopCandidate& 			operator=(const TopCandidate&);
//		int 			category() const {return m_category;}
		float			deltaR(const Particle&) const;
		float			deltaRjjW() const { return m_dRjjW; }
		float			deltaRWb() const { return m_dRWb; }
/*		float 			deltaR() const { return m_dR; };
		float 			deltaRbb() const { return m_dRbb; };
		float 			deltaRTrackJets() const { return m_dRTrackJets; };
		//float getDeltaR(const TString&) const ;
		bool			fullyBTagged() const { return m_fullyBTagged; };*/
		const Jet*		getBJet() { return &m_bJet; }
		const Jet*		getWJet(int jetNum) { return &m_wJet[jetNum]; }
		const Jet&		getBJetRef() const { return m_bJet; }
		const Jet&		getWJetRef(int jetNum) const { return m_wJet[jetNum]; }
		float			mW() const { return m_mW; }
		float			ptW() const { return m_ptW; }
		//int 			getJetIndex(int jetNum) const { return this->getJet(jetNum)->index(); }
		/*float			mv2c10Sum() const;
		float			mv2c10CaloSum() const;
		int				numTightTags() const {return m_numTightTags;}
		float			jetPtScalarSum() const;
		void    		setBJetCoordinates(const ROOT::Math::PtEtaPhiEVector& p4);
		void    		setWJetCoordinates(const int& jetNum, const ROOT::Math::PtEtaPhiEVector& p4);
		void    		setJetPartonP4(const int& jetNum, const ROOT::Math::PtEtaPhiEVector& p4){ m_jet[jetNum].setPartonP4(p4);}
		void			setXt(const float& xt) { m_xt = xt; }*/
		bool			sharesJet(const TopCandidate&) const;
		float			xt() const { return m_xt; }

		//float getValue(Double_t (ROOT::Math::PtEtaPhiEVector::*floatFunc)() const, const TString& variation) const;
	protected:
		std::vector<Jet> m_wJet;
		Jet m_bJet;
		float m_xt, m_mW, m_mt, m_dRWb, m_dRjjW, m_ptW;
		//std::map<TString, float> m_varTopCandidate_dR;
};
#endif
