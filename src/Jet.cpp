#include "Jet.h"
bool Jet::isMatched(const std::vector<Jet>& jets, float dRMax) const
{
	for(std::vector<Jet>::const_iterator jet = jets.begin(); jet != jets.end(); ++jet)
	{
		if(this->deltaR(*jet) < dRMax) return true;
	}
	return false;
}

