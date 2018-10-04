#include "HiggsCandidateProducer.h"
#include <algorithm>
#include <array>
#include <cmath>

//This first function produces resolved candidates
std::vector<HiggsCandidate> HiggsCandidateProducer::produceResolved(const std::vector<Jet>& jets, const std::vector<Muon>& muons, const float& ptMin, const float& ptMax)
{
	std::vector<HiggsCandidate> hcands;
	if(m_debug) std::cout<<"HiggsCandidateProducer::produceResolved("<< jets.size() <<" jets,"<< ptMin <<"):" << std::endl;
	//Jets are pt-ordered by JetProducer.
	//std::cout<< jets.size() <<" jets to play with."<< std::endl;
	std::vector<Jet> bTagged, nonBTagged;
	for(std::vector<Jet>::const_iterator jetIt = jets.begin(); jetIt < jets.end(); ++jetIt)
	{
		if(jetIt->isBTagged()) bTagged.emplace_back(*jetIt);	
		else nonBTagged.emplace_back(*jetIt);	
	}
	int nTopUpJets = 4-bTagged.size();
	
	//std::cout<<"Number of b-tagged jets = "<< bTagged.size() <<", therefore "<< nTopUpJets << " top-up jets are required."<< std::endl;
	if(nTopUpJets > 0)
	{
		std::shuffle(nonBTagged.begin(), nonBTagged.end(), m_ranGen);
		if(nTopUpJets > nonBTagged.size()) nTopUpJets = nonBTagged.size();
		for(std::vector<Jet>::const_iterator jetIt = nonBTagged.begin(); jetIt < nonBTagged.begin()+nTopUpJets; ++jetIt)
		{
			bTagged.emplace_back(*jetIt);
		}	
	}
	//std::cout<<"There are now "<< bTagged.size() <<" jets considered as b-tagged jets."<< std::endl;
	//std::sort(mostBTagged.begin(), mostBTagged.end(), [](const Jet a, const Jet b){ return b.mv2c10() < a.mv2c10(); });
	//std::cout<< "sorted the jets by mv2c10."<< std::endl;
	//std::cout<<"Now consider the first "<< stop <<" jets"<< std::endl;
	for(std::vector<Jet>::const_iterator jetIt = bTagged.begin(); jetIt < bTagged.end(); ++jetIt)
	{
		const Jet& jet = *jetIt;
	//	std::cout<<"Jet 1 = "<< jet.p4() <<", mv2c10 = "<< jet.mv2c10() << std::endl;
		//if(m_trackJetBTagging && jet.matchedTrackJets().size() < 1) continue;
		//if(!jet.isLooseBTagged()) continue;
		//if(jet.matchedTrackJets().size() > 1) continue; //ensures orthogonality with cat 1, but not 2. 
		std::shared_ptr<Jet> leadingTrackJet1 = jet.matchedTrackJets().size() > 0 ? std::make_shared<Jet>(jet.matchedTrackJets().at(0)) : nullptr; //findLeadingTrackJet(jet, trackJets);
		//Only consider jets with indices above current jet1 iterator. Avoids considering both 0+1 and 1+0.
		for(std::vector<Jet>::const_iterator jet2It = jetIt+1; jet2It < bTagged.end(); ++jet2It)
		{
			const Jet& jet2 = *jet2It;
			//std::cout<<"\tJet 2 = "<< jet2.p4() <<", mv2c10 = "<< jet2.mv2c10() << std::endl;
			//if(m_trackJetBTagging && jet2.matchedTrackJets().size() < 1) continue; 
			//if(!jet2.isLooseBTagged()) continue;
		//	if (jet2.matchedTrackJets().size() > 1) continue; 
			//if(jet.deltaR(jet2) > 2.0) continue;
			HiggsCandidate hcand = HiggsCandidate(jet, jet2);
		//	muonInJetCorrectionResolved(hcand, muons);
			if(hcand.Pt() < ptMin) continue;
			if(hcand.Pt() > ptMax) continue;
		//	if(hcand.getJet(0)->Pt() < 40.) continue;	 //eta requirement already applied
		//	if(hcand.getJet(1)->Pt() < 40.) continue;	 //eta requirement already applied
			std::shared_ptr<Jet> leadingTrackJet2 = jet2.matchedTrackJets().size() > 0 ? std::make_shared<Jet>(jet2.matchedTrackJets().at(0)) : nullptr;//findLeadingTrackJet(jet2, trackJets);
			if(leadingTrackJet1 && leadingTrackJet2)
			{
				hcand.setDRTrackJets(leadingTrackJet1->deltaR(*leadingTrackJet2));
			}else
			{
			//	std::cout<<"Couldn't find track-jets"<< std::endl;
				hcand.setDRTrackJets(-99.);
			}
			hcand.setPtRecoCorrP4(jet.ptRecoFactor()*jet.p4()+jet2.ptRecoFactor()*jet2.p4());
			
			//calculateXt(hcand, jets);
			hcands.push_back(hcand);
		}	
	}
	std::sort(hcands.begin(), hcands.end(), [](const HiggsCandidate a, const HiggsCandidate b) 
	{
        return b.Pt() < a.Pt();   
		//return b.jetPtScalarSum() < a.jetPtScalarSum();
    }); 
	//The ambiguity resolver is used to pick which Higgs candidate of the two considered is best. Used here to decide between candidates that share a jet.
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
															const float& ptMin, const float& ptSplit)
{
	std::vector<HiggsCandidate> hcands;
	if(m_debug) std::cout<<"HiggsCandidateProducer::produceMerged("<< caloJets.size() <<" jets, "<< ptMin <<", "<< ptSplit 
							<<"):" << std::endl;
	for(auto caloJet: caloJets)
	{
		if(caloJet.M() < 50.) continue; //This is applied in FatJetProducer, but not JetProducer. Therefore needs to be checked here.
		if(caloJet.matchedTrackJets().size() < 1) continue; //Only form Higgs candidates if there is a track-jet
		HiggsCandidate hcand(caloJet);
		if(hcand.Pt() < ptMin) continue;
		//std::sort(matchedTrackJets.begin(), matchedTrackJets.end(), [](const Jet a, const Jet b) {return b.Pt() < a.Pt();} ); 
		if(m_debug) std::cout<<"HiggsCandidateProducer::produce: calo-jet pt = "<< caloJet.pt() <<", HiggsCandidate pt = "<< hcand.pt() << std::endl;
		//muonInJetCorrectionBoosted(hcand, muons);
		if(m_debug) std::cout<<"hcand after muon-in-jet correction pt = "<< hcand.pt() <<" GeV"<< std::endl;
	//	if(hcand.category() == 1 && hcand.Pt() < semiPtMin) continue;	 //eta requirement already applied
	//	else if(hcand.category() == 2 && hcand.Pt() < fullyPtMin) continue;	 //eta requirement already applied
		//calculateXt(hcand,caloJets);
		hcands.push_back(hcand);
	}
	//The ambiguity resolver is used to pick which Higgs candidate of the two considered is best. Used here to decide between candidates that share a jet.
	//The best pair chooser returns a score, allowing the best pair of Higgs candidates to be picked.
	std::function<float(const HiggsCandidate&, const HiggsCandidate&)> bestPairChooser;
	bestPairChooser = &calcMV2c20Score; //
	//bestPairChooser = &calcDhhScore; //Alternatively, choose on the basis of Dhh.
	std::vector<HiggsCandidate> uniqueHCands = chooseBestPair(hcands, bestPairChooser);	
	//Sort Higgs candidates by pt, using a lambda expression
	std::sort(hcands.begin(), hcands.end(), [](const HiggsCandidate a, const HiggsCandidate b) 
	{
        return b.Pt() < a.Pt();   
    }); 
	/*std::cout<<"HiggsCandidateProducer::produce: list of boosted candidates:"<< std::endl;
	for(auto hcand: hcands)
	{
		std::cout<< hcand.index()<<", category = "<< hcand.category() <<"; "; 
	}
	std::cout<<"\n----------------------------------------"<< std::endl;*/
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
//bestPairChooser is 
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
			//Chicago values
			float h1DRjjMin = 0.;
			float h1DRjjMax = 1.00;//0.9967394;
			float h2DRjjMin = 0.;
			float h2DRjjMax = 1.05;//1.047049
			if(mhh < 1250)
			{
				h1DRjjMin = 360/mhh-0.5000000;
				h1DRjjMax = 655/mhh+0.5000000; //652.863/m4j+0.474449
				h2DRjjMin = 235/mhh;//235.242/mhh+0.0162996;
				h2DRjjMax = 875/mhh + 0.35;//874.890/mhh+0.347137; 
			}
			if(hcand1->deltaR() > h1DRjjMax) continue; 
			if(hcand2->deltaR() > h2DRjjMax) continue; 
			if(hcand1->deltaR() < h1DRjjMin) continue; 
			if(hcand2->deltaR() < h2DRjjMin) continue; 
			//float maxDRjj = 0.05+(694/mhh);*/
			/*float maxDRjj = 0.52+(2.27e+5/(mhh*mhh));
			if(hcand1->deltaR() > maxDRjj) continue; 
			if(hcand2->deltaR() > maxDRjj) continue; 
			//float minDRjj = 0.28+(130/mhh);
			float minDRjj = 0.36+(4.47e+4/(mhh*mhh));
			if(hcand1->deltaR() < minDRjj) continue; 
			if(hcand2->deltaR() < minDRjj) continue; */
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

void HiggsCandidateProducer::calculateXt(HiggsCandidate& hcand, const std::vector<Jet>& jets)
{
	float xt = 20.;
	if(hcand.category() == 0)
	{	
		const Jet& jet0 = *(hcand.getJet(0));
		const Jet& jet1 = *(hcand.getJet(1));
		for(auto jet : jets)
		{
			if(jet == jet0) continue;
			if(jet == jet1) continue;
			if(jet.deltaR(hcand) > 1.5) continue;
			float mW = 0.;
			if(jet0.mv2c10() > jet1.mv2c10())
			{
				mW = (jet + jet1).M();
			}else{
				mW = (jet + jet0).M();
			}
			float mt = (jet + jet0 + jet1).M();
			float thisXt = sqrt((mt-172.5)*(mt-172.5)/(0.01*mt*mt) + (mW-80.4)*(mW-80.4)/(0.01*mW*mW));
			if(xt < 0 || thisXt < xt) xt = thisXt;
		}
	}else{
		const Jet& fatjet = *(hcand.getFatJet());
		if(fatjet.rParam() > 0.4)
		{
			float mt = fatjet.M();
			xt = sqrt((mt-172.5)*(mt-172.5)/(0.01*mt*mt));
		}else{
			for(auto jet : jets)
			{
				if(jet == fatjet) continue;
				if(jet.deltaR(hcand) > 1.5) continue;
				float mt = (jet + fatjet).M();
				float thisXt = sqrt((mt-172.5)*(mt-172.5)/(0.01*mt*mt));
				if(xt < 0 || thisXt < xt) xt = thisXt;
			}
		}
	}
	hcand.setXt(xt);
}
void HiggsCandidateProducer::muonInJetCorrectionResolved(HiggsCandidate& hcand, const std::vector<Muon>& muons)
{
	if(muons.size() == 0) return;
	if(hcand.category() != 0)
	{
		std::cout<<"ERROR! HiggsCandidateProducer::muonInJetCorrectionResolved: incorrect category of HiggsCandidate passed."<< std::endl;
	}
	std::array<std::vector<Muon>::const_iterator, 2> closestMuons = {muons.end(), muons.end()};
	std::array<float, 2> minDRs = {0.4, 0.4};

	for(std::vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon)
	{
//		std::cout<<"Muon "<< muon->index() << std::endl;
		//Find the jet that is best matched to muon, then see if this is the closest muon.
		//If not, give the second-best jet a chance.
		std::array<float, 2> dR = {99., 99.};
		for(int i = 0; i < 2; ++i) //consider both track-jets/calo-jets
		{
			if(!(hcand.getJet(i)->isBTagged())) continue;
			dR[i] = muon->deltaR(hcand.getJetRef(i));
			//std::cout<<"dR["<< i <<"] = "<< dR[i] <<" < minDRs["<< i <<"] = "<< minDRs[i] << std::endl;

			if(dR[0] < dR[1])
			{
				if(dR[0] < minDRs[0])
				{
					minDRs[0] = dR[0];
					closestMuons[0] = muon;
				}else if(dR[1] < minDRs[1]){
					minDRs[1] = dR[1];
					closestMuons[1] = muon;
				}
			}else{
				if(dR[1] < minDRs[1])
				{
					minDRs[1] = dR[1];
					closestMuons[1] = muon;
				}else if(dR[0] < minDRs[0]){
					minDRs[0] = dR[0];
					closestMuons[0] = muon;
				}
			}
		}
	}

	for(int i = 0; i < 2; ++i)
	{
		if(closestMuons[i] == muons.end()) continue;
		ROOT::Math::PtEtaPhiEVector jetP4 = hcand.getJet(i)->p4() + (1-closestMuons[i]->eLoss()/closestMuons[i]->E())*closestMuons[i]->p4();
		hcand.setJetCoordinates(i, jetP4);
		hcand.setMuonCorrectionId(i, closestMuons[i]->index());
	}
}
void HiggsCandidateProducer::muonInJetCorrectionBoosted(HiggsCandidate& hcand, const std::vector<Muon>& muons)
{
	if(muons.size() == 0 || !hcand.fullyBTagged()) return;
	if(hcand.category() < 1)
	{
		std::cout<<"ERROR! HiggsCandidateProducer::muonInJetCorrectionBoosted: incorrect category of HiggsCandidate passed."<< std::endl;
	}
	std::vector<Muon>::const_iterator closestMuon = muons.end();
	float minDR = 0.4;

	for(std::vector<Muon>::const_iterator muon = muons.begin(); muon != muons.end(); ++muon)
	{
		float dR = muon->deltaR(hcand);
		if(dR < minDR)
		{
			minDR = dR;
			closestMuon = muon;
		}
	}

	if(closestMuon == muons.end()) return;
	ROOT::Math::PtEtaPhiEVector jetP4 = hcand.getFatJet()->p4() + (1-closestMuon->eLoss()/closestMuon->E())*closestMuon->p4();	
	hcand.setFatJetCoordinates(jetP4);
	hcand.setMuonCorrectionId(0, closestMuon->index());
}
/*
		if(m_debug)
		{
			std::cout<<"HiggsCandidateProducer:muonInJetCorrection: closestMuons["<< i <<"] address = "<< closestMuons[i] << std::endl;
		}
		if(closestMuons[i])
		{
			if(m_debug) 
			{
				std::cout<<"HiggsCandidateProducer:muonInJetCorrection: closestMuons["<< i <<"] = "<< closestMuons[i]->p4() << std::endl;
			}
			if(hcand.category() == 0)
			{
				if(m_debug) 
				{
					std::cout<<"HiggsCandidateProducer:muonInJetCorrection: jet["<< i <<"] = "<< hcand.getJet(i)->p4() << std::endl;
				}
			}else
			{
				if(m_debug)
				{
					std::cout<<"HiggsCandidateProducer:muonInJetCorrection: hcand = "<< hcand.p4() << std::endl;
					if(hcand.category() == 1) 
					{
						std::cout<<"HiggsCandidateProducer:muonInJetCorrection: hcand jet= "<< hcand.getJet(i)->p4() << std::endl;
					}
					std::cout<<"HiggsCandidateProducer:muonInJetCorrection: dR(hcand, muon) = "<< minDRs[i] << std::endl;;// closestMuons[i]->deltaR(hcand.getJetRef(i)) << std::endl;
					 std::cout<<"HiggsCandidateProducer:muonInJetCorrection: hcand m = "<< hcand.mass() <<", hcand pt = "<< hcand.pt() 
							<<", fat-jet pt = "<< hcand.getFatJet()->pt() <<", muon pt = "<< closestMuons[i]->pt() << std::endl;
				}
				//Add muon p4 to jet, but remove the E lost by the muon in the calorimeter.
				if(m_debug)
				{
					std::cout<<"HiggsCandidateProducer:muonInJetCorrection: after correction hcand = "<< hcand.p4() << std::endl;
					 std::cout<<"HiggsCandidateProducer:muonInJetCorrection: hcand m = "<< hcand.mass() <<", hcand pt = "<< hcand.pt() 
						<<", fat-jet pt = "<< hcand.getFatJet()->pt() <<", muon pt = "<< closestMuons[i]->pt() << std::endl;
					if(hcand.mass() > 150.) std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<< std::endl;
				}
				//if(m_debug) std::cout<<"After correction, hcand m = "<< hcand.mass() <<", fatjet pt = "<< jetP4.pt() <<", hcand pt = "<< hcand.pt() << std::endl;
			}
		}
	}*/

//The following functions can be used to pick the "best" pair of Higgs candidates. These are defined so the best candidates have scores closest to zero.
float calcMV2c20Score(const HiggsCandidate& h1, const HiggsCandidate& h2)
{
	return 4. - h1.mv2c10Sum() - h2.mv2c10Sum();//Mv2c20 ranges from -1 to 1, with 1 being most b-jet-like. Therefore perfect event would have sum = 4.
}
float calcDhhScore(const HiggsCandidate& h1, const HiggsCandidate& h2)
{
//	float thetaSR = 0.74776; //atan(115, 124);
	float thetaSR = 0.741947; //atan(110, 120);
	float theta = atan(h2.M()/h1.M()) - thetaSR;
	//Now that low pt Higgs candidates are acceptable, order candidates by sum of jet pt.
	/*if(h1.category()== 0 && h2.category() == 0)
	{
		if((h1.getJet(0)->Pt() + h1.getJet(1)->Pt()) < (h2.getJet(0)->Pt() + h2.getJet(1)->Pt()))
		{
			theta = atan(h1.M()/h2.M()) - thetaSR; 
		}
	}*/
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
