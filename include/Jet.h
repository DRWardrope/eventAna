#ifndef JET_H
#define JET_H

#include "Particle.h"

class Jet : public Particle
{
	public:
		Jet() : Particle(), m_muCorrected(false), m_btagged(false), m_looseBTagged(false), m_rParam(-99.),
				m_mv2c10(-99.), m_btag_sf(-99.), m_higgsParentId(-99), m_partIdx({{}}), m_trackJetIdx({{}}), 
				m_partonP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.)),
				m_truthP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.)) {};
		Jet(int index) : Particle(index), m_muCorrected(false), m_btagged(false), m_looseBTagged(false), m_rParam(-99.),
				m_mv2c10(-99.), m_btag_sf(-99.), m_higgsParentId(-99), m_partIdx({{}}), m_trackJetIdx({{}}), 
				m_partonP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.)),
				m_truthP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.))  {};
		Jet(float pt, float eta, float phi, float E, int id=-99, float mv2c10=-99.)
			: Particle(pt, eta, phi, E, id), m_muCorrected(false), m_btagged(false), m_looseBTagged(false), m_rParam(-99.),
				m_mv2c10(mv2c10), m_btag_sf(-99.), m_higgsParentId(-99), m_partIdx({{}}), m_trackJetIdx({{}}), 
				m_partonP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.)),
				m_truthP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.)) {};
		Jet(Particle part, float mv2c10=-99.) 
			: Particle(part), m_btagged(false), m_looseBTagged(false), m_rParam(-99), m_mv2c10(mv2c10), m_higgsParentId(-99),
				 m_partIdx({{}}), m_trackJetIdx({{}}), 
				m_partonP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.)),
				m_truthP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.)) {};
		Jet(ROOT::Math::PtEtaPhiEVector p4, int id=-99, float mv2c10=-99.) 
			: Particle(p4, id), m_btagged(false), m_looseBTagged(false), m_rParam(-99), m_mv2c10(mv2c10), m_higgsParentId(-99),
				 m_partIdx({{}}), m_trackJetIdx({{}}), 
				m_partonP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.)),
				m_truthP4(ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.)) {};
		float btagSF() const { return m_btag_sf; }
		static float findMinBTag(const int& btagWP)
		{
			// R=0.4 TopoEM-jets:
			//MV2c10 thresholds:
			if(btagWP == 30) return 0.9977155;
			if(btagWP == 50) return 0.9769329;
			if(btagWP == 60) return 0.934906;
			else if(btagWP == 70) return 0.8244273;
			else if(btagWP == 77) return 0.645925;
			else if(btagWP == 85) return 0.1758475;
			else return -99.;
		}
		bool isBTagged() const { return m_btagged; }
		bool isBTagged(const int& btagWP) const { return m_mv2c10 > findMinBTag(btagWP); }
		bool isLooseBTagged() const { return m_looseBTagged; }
		bool isMuonCorrected() const { return m_muCorrected; }
		bool isMatched(const std::vector<Jet>&, float dRMax=0.4) const;
		const std::vector<Jet> & matchedTrackJets() const { return m_matchedTrackJets; }
		float mv2c10() const { return m_mv2c10; }
		bool passBadLoose() const { return m_passBadLoose; }
		std::vector<int> partonIdx() const { return m_partIdx; }
		ROOT::Math::PtEtaPhiEVector partonP4() const { return m_partonP4; }
		float ptRecoFactor() const { return m_ptReco_factor; }
		float rParam() const { return m_rParam; }
		void setBTagSF(const float& btag_sf) { m_btag_sf = btag_sf; }
		void setIsBTagged(bool isBTagged) { m_btagged = isBTagged; }
		void setIsLooseBTagged(bool isLooseBTagged) { m_looseBTagged = isLooseBTagged; }
		void setIsMuonCorrected(bool isMuCorrected) { m_muCorrected = isMuCorrected; }
		void setMatchedTrackJets(const std::vector<Jet>& matchedTrackJets) { m_matchedTrackJets = matchedTrackJets; }
		void setMV2c10(const float& mv2c10) { m_mv2c10 = mv2c10; }
		void setPartonP4(const ROOT::Math::PtEtaPhiEVector& partonP4) { m_partonP4 = partonP4; }
		void setPartIdx(std::vector<int> partIdx) { m_partIdx = partIdx; }
		void setPassBadLoose(bool passBadLoose) { m_passBadLoose = (passBadLoose > 0 ? true : false); }
		void setPtRecoFactor(const float& ptRecoFactor) { m_ptReco_factor = ptRecoFactor; }
		void setRParam(const float& rParam) { m_rParam = rParam; }
		void setTrackJetIdx(std::vector<int> tjIdx) { m_trackJetIdx = tjIdx; }
		void setTruthJetP4(const ROOT::Math::PtEtaPhiEVector& truthP4) { m_truthP4 = truthP4; }
		const std::vector<int>& trackJetIdx() const { return m_trackJetIdx; }
		ROOT::Math::PtEtaPhiEVector truthJetP4() const { return m_truthP4; }
	protected:
		bool m_muCorrected, m_btagged, m_looseBTagged;
		bool m_passBadLoose;
		float m_rParam;
		float m_mv2c10, m_btag_sf;
		float m_ptReco_factor;
		int m_higgsParentId;
		std::vector<int> m_partIdx, m_trackJetIdx;
		std::vector<Jet> m_matchedTrackJets;
		ROOT::Math::PtEtaPhiEVector m_partonP4, m_truthP4;
};
#endif 
