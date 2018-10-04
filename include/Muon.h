#ifndef MUON_H
#define MUON_H
#include "Particle.h"

class Muon : public Particle
{
	public:
		Muon() : Particle(), m_eLoss(-99.){};
		Muon(int index) : Particle(index), m_eLoss(-99.) {};
//		Muon& operator=(const Muon&);
		Muon(float pt, float eta, float phi, float E, int id=-99)
			: Particle(pt, eta, phi, E, id), m_eLoss(-99.){};
		Muon(float pt, float eta, float phi, float E, int id, float eLoss, int quality, bool isolated)
			: Particle(pt, eta, phi, E, id), m_eLoss(eLoss), m_quality(quality), m_isolated(isolated){};
	        //Muon operator+(const Muon& rhs) const;
		bool operator==(const Muon& other) { return this->index() == other.index(); }
		float eLoss() const { return m_eLoss; }
		bool isolated() const { return m_isolated; }
		int quality() const { return m_quality; }
		void setELoss(const float& eLoss) { m_eLoss = eLoss; }
	protected:
		float m_eLoss;
		int m_quality;
		bool m_isolated;
};
//typedef std::vector<Muon> MuonCollection
#endif 
