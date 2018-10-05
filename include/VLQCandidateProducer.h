#ifndef VLQCANDIDATEPRODUCER_H
#define VLQCANDIDATEPRODUCER_H

#include "Event.h"
#include "VLQCandidate.h"
#include <stdlib.h>

extern bool m_debug;

class VLQCandidateProducer
{

 public:
	 VLQCandidateProducer(Event& evt): m_evt(evt) {};

	 std::vector<VLQCandidate> produce(
										const std::vector<Jet>&, 
										const std::vector<HiggsCandidate>&, 
										const float& jetPtMin = -99.
									);
 private:
	Event& m_evt;
};
#endif
