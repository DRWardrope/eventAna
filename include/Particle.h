#ifndef PARTICLE_H
#define PARTICLE_H
#include "Math/Vector4D.h"
#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiE4D.h"
#include "TString.h"
#include <map>
#include <vector>

//#include "ParticleFwd.h"
//class Particle : public ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >
class Particle : public ROOT::Math::PtEtaPhiEVector
{
	public:
		Particle() : ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.), m_id(-99) {};
		Particle(int index) : ROOT::Math::PtEtaPhiEVector(-99., -99., -99., -99.), m_id(index) {};
		Particle(const float& pt, const float& eta, const float& phi, const float& E, int id=-99) 
			: ROOT::Math::PtEtaPhiEVector(pt, eta, phi, E), m_id(id) {};
		Particle(ROOT::Math::PtEtaPhiEVector p4, int id=-99) : ROOT::Math::PtEtaPhiEVector(p4), m_id(id) {};
		bool operator==(const Particle& other) const { return this->index() == other.index(); }
		
	//	Particle operator+(const Particle& rhs) const;
		float	deltaPhi(const Particle&) const;
		//float	deltaR() const {return -1.;} ;
		float	deltaR(const Particle&) const;
		const 	TString& label() const { return m_label; };
		int		index() const { return m_id; };
		bool	isMatched(const std::vector<Particle>&, float dRMax=0.4) const;
		std::vector<Particle> match(const std::vector<Particle>&, float dRMax=0.4);
		ROOT::Math::PtEtaPhiEVector p4() const { return ROOT::Math::PtEtaPhiEVector(this->Pt(), this->Eta(), this->Phi(), this->E()); };
		int		pdgId() const { return m_pdgId;}
		void	setIndex(const int& id) { m_id = id; };
		void	setLabel(const TString& label) { m_label = label;}
		void	setP4(const ROOT::Math::PtEtaPhiEVector& p4) { this->SetCoordinates(p4.Pt(), p4.Eta(), p4.Phi(), p4.E()); }
		void	setStatus(const int& status) { m_status = status;}
		void	setPdgId(const int& pdgId) { m_pdgId = pdgId; }
		int		status() const { return m_status;}
	
	protected:
	int m_id, m_pdgId, m_status;
	TString m_label;
};
#endif
