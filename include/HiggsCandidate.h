#ifndef HIGGSCANDIDATE_H
#define HIGGSCANDIDATE_H
/*************************************************************************************************
*
* HiggsCandidate: class to represent Higgs candidates.
* Holds copies of the constituent jets, to guarantee that they will last.
* David Wardrope, 21st March 2016
*
*************************************************************************************************/
#include <array>
#include <memory>
#include "Jet.h"
#include "TRandom3.h"

class HiggsCandidate : public Particle
{
	public:
		HiggsCandidate(): Particle(-99), m_jet(std::vector<Jet>()), m_fatjet(Jet()), m_dR(-99.), 
							m_dRTrackJets(-99.), m_dRbb(-99), m_xt(-99.), m_fullyBTagged(false), m_category(-99),
							m_truthHiggsId(-99), m_muCorrId({-99, -99})  {};
		HiggsCandidate(const Jet& jet); 
		HiggsCandidate(const Jet& jet1, const Jet& jet2);
		//HiggsCandidate(const Jet& fatjet, const Jet& subjet1, const Jet& subjet2); //Track-jets are now stored in Jet object.
//		HiggsCandidate& 			operator=(const HiggsCandidate&);
		int 			category() const {return m_category;}
		float			deltaR(const Particle&) const;
		float 			deltaR() const { return m_dR; };
		float 			deltaRbb() const { return m_dRbb; };
		float 			deltaRTrackJets() const { return m_dRTrackJets; };
		//float getDeltaR(const TString&) const ;
		bool			fullyBTagged() const { return m_fullyBTagged; };
		const Jet*		getFatJet() const;
		const Jet*		getJet(int jetNum) const { return &m_jet[jetNum]; }
		const Jet&		getJetRef(int jetNum) const { return m_jet[jetNum]; }
		int 			getJetIndex(int jetNum) const { return this->getJet(jetNum)->index(); }
		bool			looselyBTagged() const { return m_looselyBTagged; };
		std::array<int, 2>	muonCorrectionId() const {return m_muCorrId;}
		float			mv2c10Sum() const;
		float			mv2c10CaloSum() const;
		int				numTightTags() const {return m_numTightTags;}
		ROOT::Math::PtEtaPhiEVector partonP4() const { return m_partonP4; }
		ROOT::Math::PtEtaPhiEVector ptRecoCorrP4() const { return m_ptRecoCorrP4; }
		float			jetPtScalarSum() const;
		void			setDRTrackJets(float dRTrackJets) { m_dRTrackJets = dRTrackJets; }
		void			setDRbb(float dRbb) { m_dRbb = dRbb; }
		void    		setFatJetCoordinates(const ROOT::Math::PtEtaPhiEVector& p4);
		void    		setJetCoordinates(const int& jetNum, const ROOT::Math::PtEtaPhiEVector& p4);
		void    		setJetPartonP4(const int& jetNum, const ROOT::Math::PtEtaPhiEVector& p4){ m_jet[jetNum].setPartonP4(p4);}
		void    		setPartonP4(const ROOT::Math::PtEtaPhiEVector& p4){ m_partonP4 = p4;}
		void    		setPtRecoCorrP4(const ROOT::Math::PtEtaPhiEVector& p4){ m_ptRecoCorrP4 = p4;}
//		void    		setJetHiggsParentId(const int& jetNum, const int& higgsParentId) { m_jet[jetNum].setHiggsParentId(higgsParentId); }
		void    		setMuonCorrectionId(int jet, int muonId) { m_muCorrId[jet] = muonId;};
		void			setTruthHiggsId(const int& truthHiggsId) { m_truthHiggsId = truthHiggsId; }
		void			setXt(const float& xt) { m_xt = xt; }
		bool			sharesJet(const HiggsCandidate&) const;
		int				truthHiggsId() const { return m_truthHiggsId; }
		float			xt() const { return m_xt; }

		//float getValue(Double_t (ROOT::Math::PtEtaPhiEVector::*floatFunc)() const, const TString& variation) const;
	protected:
		std::vector<Jet> m_jet;
		Jet m_fatjet;
		float m_dR, m_dRTrackJets, m_dRbb, m_xt;
		bool m_fullyBTagged, m_looselyBTagged;
		int m_category, m_truthHiggsId, m_numTightTags;
		std::array<int, 2> m_muCorrId; 
		ROOT::Math::PtEtaPhiEVector m_partonP4, m_ptRecoCorrP4;
		//std::map<TString, float> m_varHiggsCandidate_dR;
};
#endif
