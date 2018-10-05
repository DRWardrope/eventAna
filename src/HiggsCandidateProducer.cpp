#include "HiggsCandidateProducer.h"
#include <algorithm>
#include <array>
#include <cmath>

//This first function produces resolved Higgs boson candidates, then chooses the best 0, 1 or 2 of these
std::vector<HiggsCandidate> HiggsCandidateProducer::produceResolved(const std::vector<Jet>& jets, const std::vector<Muon>& muons, const float& ptMin, const float& ptMax)
{
	std::vector<HiggsCandidate> hcands;
	if(m_debug) std::cout<<"HiggsCandidateProducer::produceResolved("<< jets.size() <<" jets,"<< ptMin <<"):" << std::endl;
	//Jets are pt-ordered by JetProducer.
	std::vector<Jet> bTagged, nonBTagged;
	for(std::vector<Jet>::const_iterator jetIt = jets.begin(); jetIt < jets.end(); ++jetIt)
	{
		if(jetIt->isBTagged()) bTagged.emplace_back(*jetIt);	
		else nonBTagged.emplace_back(*jetIt);	
	}
	int nTopUpJets = 4-bTagged.size();
	
	if(nTopUpJets > 0)
	{
		std::shuffle(nonBTagged.begin(), nonBTagged.end(), m_ranGen);
		if(nTopUpJets > nonBTagged.size()) nTopUpJets = nonBTagged.size();
		for(std::vector<Jet>::const_iterator jetIt = nonBTagged.begin(); jetIt < nonBTagged.begin()+nTopUpJets; ++jetIt)
		{
			bTagged.emplace_back(*jetIt);
		}	
	}
	for(std::vector<Jet>::const_iterator jetIt = bTagged.begin(); jetIt < bTagged.end(); ++jetIt)
	{
		const Jet& jet = *jetIt;
		std::shared_ptr<Jet> leadingTrackJet1 = jet.matchedTrackJets().size() > 0 ? std::make_shared<Jet>(jet.matchedTrackJets().at(0)) : nullptr; 
		//Only consider jets with indices above current jet1 iterator. Avoids considering both 0+1 and 1+0.
		for(std::vector<Jet>::const_iterator jet2It = jetIt+1; jet2It < bTagged.end(); ++jet2It)
		{
			const Jet& jet2 = *jet2It;
			HiggsCandidate hcand = HiggsCandidate(jet, jet2);
			if(hcand.Pt() < ptMin) continue;
			if(hcand.Pt() > ptMax) continue;
			std::shared_ptr<Jet> leadingTrackJet2 = jet2.matchedTrackJets().size() > 0 ? std::make_shared<Jet>(jet2.matchedTrackJets().at(0)) : nullptr;
			if(leadingTrackJet1 && leadingTrackJet2)
			{
				hcand.setDRTrackJets(leadingTrackJet1->deltaR(*leadingTrackJet2));
			}else
			{
				hcand.setDRTrackJets(-99.);
			}
			hcand.setPtRecoCorrP4(jet.ptRecoFactor()*jet.p4()+jet2.ptRecoFactor()*jet2.p4());
			
			hcands.push_back(hcand);
		}	
	}
	std::sort(hcands.begin(), hcands.end(), [](const HiggsCandidate a, const HiggsCandidate b) 
	{
        return b.Pt() < a.Pt();   
		//return b.jetPtScalarSum() < a.jetPtScalarSum();
    }); 
	//The ambiguity resolver is used to pick which Higgs candidate to select, in cases that share a jet.
	std::function<bool(const HiggsCandidate&, const HiggsCandidate&)> ambResolver;
	ambResolver = &highestMV2c20; //
	//The best pair chooser returns a score, allowing the best pair of Higgs candidates to be picked.
	std::function<float(const HiggsCandidate&, const HiggsCandidate&)> bestPairChooser;
	//bestPairChooser = &calcMV2c20Score; //
	bestPairChooser = &calcDhhScore; //Alternatively, choose on the basis of Dhh.
	//resolveAmbiguities finds the best 0, 1 or 2 Higgs candidates with no overlapping jets.
	std::vector<HiggsCandidate> uniqueHCands = resolveAmbiguities(hcands, bestPairChooser, ambResolver);	
	//No need to sort, because already done and resolveAmbiguities is supposed to preserve relative order of candidates
	std::sort(uniqueHCands.begin(), uniqueHCands.end(), [](const HiggsCandidate a, const HiggsCandidate b) 
	{
        //return b.Pt() < a.Pt();   
		return b.jetPtScalarSum() < a.jetPtScalarSum();
    }); 
	if(m_debug) std::cout<<"HiggsCandidateProducer::produceResolved: returning "<< uniqueHCands.size() <<" uniqueHCands."<< std::endl;
	return uniqueHCands;
}
//This second function produces boosted candidates
std::vector<HiggsCandidate> HiggsCandidateProducer::produceMerged(const std::vector<Jet>& caloJets, const std::vector<Muon>& muons, 
															const float& ptMin)
{
	std::vector<HiggsCandidate> hcands;
	if(m_debug) std::cout<<"HiggsCandidateProducer::produceMerged("<< caloJets.size() <<" jets, "<< ptMin 
							<<"):" << std::endl;
	for(auto caloJet: caloJets)
	{
		if(caloJet.M() < 50.) continue; //This is applied in FatJetProducer, but not JetProducer. Therefore needs to be checked here.
		if(caloJet.matchedTrackJets().size() < 1) continue; //Only form Higgs candidates if there is a track-jet
		HiggsCandidate hcand(caloJet);
		if(hcand.Pt() < ptMin) continue;
		if(m_debug) std::cout<<"HiggsCandidateProducer::produce: calo-jet pt = "<< caloJet.pt() <<", HiggsCandidate pt = "<< hcand.pt() << std::endl;
		if(m_debug) std::cout<<"hcand after muon-in-jet correction pt = "<< hcand.pt() <<" GeV"<< std::endl;
		hcands.push_back(hcand);
	}
	//Sort Higgs candidates by pt, using a lambda expression
	std::sort(hcands.begin(), hcands.end(), [](const HiggsCandidate a, const HiggsCandidate b) 
	{
        return b.Pt() < a.Pt();   
    }); 
	if(m_debug) std::cout << "HiggsCandidateProducer::produceMerged: returning "<< hcands.size() <<" hcands." << std::endl;
	return hcands;
}
//resolveAmbiguities is designed for the resolved case, where the list of HiggsCandidates may feature candidates that share jets
//The best pair of resolved candidates that do not share jets is searched for. If such a pair cannot be found, the best single candidate is returned.
std::vector<HiggsCandidate> HiggsCandidateProducer::resolveAmbiguities(const std::vector<HiggsCandidate>& hcands, 
												std::function<float(const HiggsCandidate&, const HiggsCandidate&)>& bestPairChooser,
												std::function<bool(const HiggsCandidate&, const HiggsCandidate&)>& ambResolver)
{
	if(hcands.size() <= 1) return hcands; //No ambiguity to resolve, simply return the vector.
	std::vector<HiggsCandidate> bestPair = chooseBestPair(hcands, bestPairChooser);
	if(bestPair.size() > 0) return bestPair;
	else //Did not find a pair of Higgs candidates that did not share a jet. Therefore, simply return the best single candidate.
	{	
		std::vector<HiggsCandidate> sortedHCands = hcands;
		std::sort(sortedHCands.begin(), sortedHCands.end(), ambResolver);	
		std::vector<HiggsCandidate> unambigHCands;
		unambigHCands.push_back(sortedHCands.at(0));//Shouldn't need if, bestH1 should always be defined...
		return unambigHCands;
	}
}
//bestPairChooser is used for the resolved case.
std::vector<HiggsCandidate> HiggsCandidateProducer::chooseBestPair(const std::vector<HiggsCandidate>& hcands, 
												std::function<float(const HiggsCandidate&, const HiggsCandidate&)>& bestPairChooser)
{
	if(hcands.size() <= 1) return hcands; //No choice to make, simply return the vector.
	float bestScore = 9999.;
	HiggsCandidate bestH1, bestH2; //Will have index = -99, unless set equal to real candidate.
	for(std::vector<HiggsCandidate>::const_iterator hcand1 = hcands.begin(); hcand1 != hcands.end(); ++hcand1)
	{
		for(std::vector<HiggsCandidate>::const_iterator hcand2 = hcand1 + 1; hcand2 != hcands.end(); ++hcand2)
		{ 
			if(hcand1->sharesJet(*hcand2)) continue; //Relevant for resolved analysis.
			float mhh = (hcand1->p4() + hcand2->p4()).M();
			//Chicago values: these would be better expressed in terms of the pt of the candidates.
			float h1DRjjMin = 0.;
			float h1DRjjMax = 1.00;//0.9967394;
			float h2DRjjMin = 0.;
			float h2DRjjMax = 1.05;//1.047049
			if(mhh < 1250)
			{
				h1DRjjMin = 360/mhh-0.500;
				h1DRjjMax = 655/mhh+0.500; 
				h2DRjjMin = 235/mhh;
				h2DRjjMax = 875/mhh + 0.35;
			}
			if(hcand1->deltaR() > h1DRjjMax) continue; 
			if(hcand2->deltaR() > h2DRjjMax) continue; 
			if(hcand1->deltaR() < h1DRjjMin) continue; 
			if(hcand2->deltaR() < h2DRjjMin) continue; 
			float score = bestPairChooser(*hcand1, *hcand2);
			if(score < bestScore)
			{
				bestScore = score;
				bestH1 = *hcand1;
				bestH2 = *hcand2;
			}
		}
	}
	std::vector<HiggsCandidate> bestHCands;
	if(bestH1.index() != -99)  //Can only ever find pairs of candidates...
	{
		bestHCands.push_back(bestH1);
		bestHCands.push_back(bestH2);
	}
	return bestHCands;
}

//The following functions can be used to pick the "best" pair of Higgs candidates. These are defined so the best candidates have scores closest to zero.
float calcMV2c20Score(const HiggsCandidate& h1, const HiggsCandidate& h2)
{
	return 4. - h1.mv2c10Sum() - h2.mv2c10Sum();//Mv2c20 ranges from -1 to 1, with 1 being most b-jet-like. Therefore perfect event would have sum = 4.
}
float calcDhhScore(const HiggsCandidate& h1, const HiggsCandidate& h2)
{
	float thetaSR = 0.741947; //atan(110, 120);
	
	float lead_m = h1.M();
	float subl_m = h2.M();

	if(h1.jetPtScalarSum() < h2.jetPtScalarSum())
	{
		lead_m = h2.M();
		subl_m = h1.M();
	}
	float theta = atan(subl_m/lead_m) - thetaSR;
	return fabs(sqrt(h1.M()*h1.M()+h2.M()*h2.M())*sin(theta));
}
bool highestMV2c20(const HiggsCandidate& h1, const HiggsCandidate& h2)
{
	return h1.mv2c10Sum() > h2.mv2c10Sum();
}
std::shared_ptr<Jet> HiggsCandidateProducer::findLeadingTrackJet(const Jet& jet, const std::vector<Jet>& trackJets)
{
	//std::cout <<"HiggsCandidateProducer::findLeadingTrackJet( jet "<<jet.index()<<", no. track-jets = "<< trackJets.size() <<")"<< std::endl;
	std::shared_ptr<Jet> leadingTrackJet = nullptr;
	//for(int tjIdx: jet.trackJetIdx())
	const std::vector<int>& tjIndices = jet.trackJetIdx();
	//std::cout<<"Jet "<< jet.index() <<" was associated to "<< tjIndices.size() <<" track-jets"<< std::endl;
	for(std::vector<int>::const_iterator tjIdxIt = tjIndices.begin(); tjIdxIt != tjIndices.end(); ++tjIdxIt)
	{
		int tjIdx = *tjIdxIt;
		//std::cout<<"HiggsCandidateProducer: matched track-jet idx = "<< tjIdx << std::endl;
		if(tjIdx >= trackJets.size())
		{
			//std::cout<<"Track-jet "<< tjIdx <<" was not selected by the analysis."<< std::endl;
			continue;//Track-jets are ordered in pt, with those below threshold removed.
		}
		std::shared_ptr<Jet> trackJet = std::make_shared<Jet>(trackJets.at(tjIdx));
		if(leadingTrackJet)
		{
		//	std::cout<<"HiggsCandidateProducer: leading track-jet has pt = "<< leadingTrackJet->pt() <<" GeV."<< std::endl;
			if(trackJet->pt() > leadingTrackJet->pt()) leadingTrackJet = trackJet;
		}else{
			leadingTrackJet = trackJet;
		//	std::cout<<"HiggsCandidateProducer: first matching track-jet, set as leading."<< std::endl;
		}
		//std::cout<<"HiggsCandidateProducer: jogging on..."<< std::endl;
	}	
	return leadingTrackJet;
}
