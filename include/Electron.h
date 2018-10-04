#ifndef ELECTRON_H
#define ELECTRON_H
#include "Particle.h"

class Electron : public Particle
{
	public:
		Electron() : Particle(), m_likelihood(-99.), m_isolated(false){};
		Electron(int index) : Particle(index), m_likelihood(-99.), m_isolated(false) {};
		Electron(float pt, float eta, float phi, float E, int id=-99)
			: Particle(pt, eta, phi, E, id), m_likelihood(-99.), m_isolated(false) {};
		Electron(float pt, float eta, float phi, float E, int id, int likelihood, bool isolated)
			: Particle(pt, eta, phi, E, id), m_likelihood(likelihood), m_isolated(isolated){};
		bool operator==(const Electron& other) { return this->index() == other.index(); }
		bool isolated() const { return m_isolated; }
		int likelihood() const { return m_likelihood; }
	protected:
		int m_likelihood;
		bool m_isolated;
};
#endif 
