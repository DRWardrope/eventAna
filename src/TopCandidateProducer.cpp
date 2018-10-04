#include "TopCandidateProducer.h"
#include <algorithm>
#include <array>
#include <cmath>

//This first function produces resolved candidates
std::vector<TopCandidate> TopCandidateProducer::produce(const std::vector<Jet>& jets) const
{
	std::vector<TopCandidate> tops;
	if(m_debug) std::cout<<"TopCandidateProducer::produce("<< jets.size() <<" jets"<<"):" << std::endl;
	//Jets are pt-ordered by JetProducer.
	//std::cout<< jets.size() <<" jets to play with."<< std::endl;
	std::vector<Jet> mostBTagged = jets;
	std::sort(mostBTagged.begin(), mostBTagged.end(), [](const Jet a, const Jet b){ return b.mv2c10() < a.mv2c10(); });
	//std::cout<< "sorted the jets by mv2c10."<< std::endl;
	//unsigned int stop = mostBTagged.size() < 4 ? mostBTagged.size() : 4;
	//std::cout<<"Now consider the first "<< stop <<" jets"<< std::endl;
	for(std::vector<Jet>::const_iterator bJetIt = mostBTagged.begin(); bJetIt < mostBTagged.end()-2; ++bJetIt)
	{
		const Jet& bJet = *bJetIt;
		if(!bJet.isBTagged()) break;//Insist that at least one jet is b-tagged
		for(std::vector<Jet>::const_iterator wJet1It = bJetIt+1; wJet1It < mostBTagged.end()-1; ++wJet1It)
		{
			const Jet& wJet1 = *wJet1It;
			for(std::vector<Jet>::const_iterator wJet2It = wJet1It+1; wJet2It < mostBTagged.end(); ++wJet2It)
			{
				const Jet& wJet2 = *wJet2It;
				TopCandidate top = TopCandidate(bJet, wJet1, wJet2);
				/*if(top.deltaRjjW() < 100./top.ptW()) continue;
				if(top.deltaRjjW() > 220./top.ptW()) continue;
				if(top.deltaRWb() < 2.75-0.015*top.Pt()+2e-5*top.Pt()*top.Pt()) continue;
				if(top.deltaRWb() > 4.4-0.015*top.Pt()+2e-5*top.Pt()*top.Pt()) continue;*/
				tops.emplace_back(top);
			}
		}
	}
	//Sort Top candidates by xt, using a lambda expression
	std::sort(tops.begin(), tops.end(), [](const TopCandidate a, const TopCandidate b) 
	{
        //return b.Pt() < a.Pt();   
		return b.xt() > a.xt(); //Want smallest first!
    }); 
	if(m_debug) std::cout<<"TopCandidateProducer::produceResolved: returning "<< tops.size() <<" top candidates."<< std::endl;
	return tops;
}
std::vector<TopCandidate> TopCandidateProducer::produce(const std::array<HiggsCandidate, 2>& hcands, const std::vector<Jet>& jets) const
{
	std::vector<TopCandidate> tops;
	if(m_debug) std::cout<<"TopCandidateProducer::produce("<< jets.size() <<" jets"<<"):" << std::endl;
	//Jets are pt-ordered by JetProducer.
	//std::cout<< jets.size() <<" jets to play with."<< std::endl;
	std::vector<Jet> hcandJets;
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2;  ++j)
		{
			Jet hcJet = hcands[i].getJetRef(j);
			hcandJets.push_back(hcJet);
		}
	}
	for(std::vector<Jet>::const_iterator bJetIt = hcandJets.begin(); bJetIt < hcandJets.end(); ++bJetIt)
	{
		const Jet& bJet = *bJetIt; //Higgs candidates should be treated as b-tagged by definition.
		//DRW 170913: Test rejecting top candidates if both W-jets are part of Higgs candidate: 
		//i.e. both (pseudo-) b-tagged
		for(std::vector<Jet>::const_iterator wJet1It = jets.begin(); wJet1It < jets.end()-1; ++wJet1It)
		{
			const Jet& wJet1 = *wJet1It;
			if(wJet1 == bJet) continue;
			//DRW 170913:
			bool wJet1FromHiggs = false;
			for(auto hjet: hcandJets)
			{
				if(hjet == wJet1) wJet1FromHiggs = true;
			}
			for(std::vector<Jet>::const_iterator wJet2It = wJet1It+1; wJet2It < jets.end(); ++wJet2It)
			{
				const Jet& wJet2 = *wJet2It;
				if(wJet2 == bJet) continue;
				bool wJet2FromHiggs = false;
				for(auto hjet: hcandJets)
				{
					if(hjet == wJet2) wJet2FromHiggs = true;
				}
				if(wJet1FromHiggs && wJet2FromHiggs) 
				{	
					//std::cout<<"TopCandidateProducer::produce: both W jets are part of a Higgs 
					//candidate: continue!"<< std::endl;
					continue;
				}
				TopCandidate top = TopCandidate(bJet, wJet1, wJet2);
				/*if(top.deltaRjjW() < 100./top.ptW()) continue;
				if(top.deltaRjjW() > 220./top.ptW()) continue;
				if(top.deltaRWb() < 2.75-0.015*top.Pt()+2e-5*top.Pt()*top.Pt()) continue;
				if(top.deltaRWb() > 4.4-0.015*top.Pt()+2e-5*top.Pt()*top.Pt()) continue;*/
				tops.emplace_back(top);
			}
		}
	}
	//Sort Top candidates by xt, using a lambda expression
	std::sort(tops.begin(), tops.end(), [](const TopCandidate a, const TopCandidate b) 
	{
        //return b.Pt() < a.Pt();   
		return b.xt() > a.xt(); //Want smallest first!
    }); 
	if(m_debug) std::cout<<"TopCandidateProducer::produceResolved: returning "<< tops.size() <<" top candidates."<< std::endl;
	return tops;
}
