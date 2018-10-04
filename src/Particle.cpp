#include "Particle.h"

float Particle::deltaPhi(const Particle& that) const 
{
        float dphi = this->Phi() - that.Phi();
        while(dphi >=  M_PI)
          dphi -= 2.*M_PI;
        while(dphi < -M_PI)
          dphi += 2.*M_PI;
        return dphi;
}
float Particle::deltaR(const Particle& that) const
{
        float dPhi = this->deltaPhi(that);
        float deltaR = sqrt((this->Eta()-that.Eta())*(this->Eta()-that.Eta())+dPhi*dPhi);
        return deltaR;
}
std::vector<Particle> Particle::match(const std::vector<Particle>& particles, float dRMax)
{
	std::vector<Particle> matchedParts;
	for(std::vector<Particle>::const_iterator part = particles.begin(); part != particles.end(); ++part)
	{
		if(this->deltaR(*part) < dRMax) matchedParts.push_back(*part);
	}
	return matchedParts;
}
bool Particle::isMatched(const std::vector<Particle>& particles, float dRMax) const
{
	for(std::vector<Particle>::const_iterator part = particles.begin(); part != particles.end(); ++part)
	{
		if(this->deltaR(*part) < dRMax) return true;
	}
	return false;
}
