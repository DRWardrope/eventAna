#include "VLQCandidateProducer.h"
#include "HiggsCandidate.h"
#include <algorithm>
#include <array>
#include <cmath>

std::vector<VLQCandidate> VLQCandidateProducer::produce(const std::vector<Jet>& jets, const std::vector<HiggsCandidate>& higgses, const float& jetPtMin)
{

	std::vector<VLQCandidate> vlqCands;
	if(higgses.size() == 0 || jets.size() == 0) return vlqCands;
	if(m_debug) std::cout << "VLQCandidateProducer::produce("<<jets.size() << " jets, " << higgses.size() <<" higgses, jet pt > "<< jetPtMin <<"):" << std::endl;

	// production
	std::cout << "Number of jets: " << jets.size() << " number of HC: " << higgses.size() << std::endl;
	for(std::vector<HiggsCandidate>::const_iterator higgsIt = higgses.begin(); higgsIt < higgses.end(); ++higgsIt)
	{
		for(std::vector<Jet>::const_iterator jetIt = jets.begin(); jetIt < jets.end(); ++jetIt)	
		{
			if(jetIt->Pt() < jetPtMin) continue;
			if(!jetIt->isBTagged()) continue;
			VLQCandidate vlq(*jetIt, *higgsIt);
			if(vlq.deltaRbH() > 1.2) vlqCands.push_back(vlq);
			break;
		}
	}
	if (m_debug) std::cout << "VLQCandidateProducer::produce returning" << vlqCands.size() << " vlqCands without jet repetition." << std::endl;
	return vlqCands;
}
