//so far only supports floats and ints in the output tree
#include "HHTreeManager.h"
#include "Math/Boost.h"
#include "Math/Vector3D.h"
void HHTreeManager::bookTrees()
{
	m_includeTruthDetails = false;
	TTree* t = new TTree("Output", "");

	//Bare minimum variables:
	addBranch(t, "weight", "F");
	addBranch(t, "btrigSF", "F");
	addBranch(t, "HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25", "I");
	addBranch(t, "HLT_j100_2j55_bmv2c2060_split", "I");
	addBranch(t, "HLT_j225_bmv2c2060_split", "I");
	addBranch(t, "MEt", "F");
	addBranch(t, "deltaPhi4jMin", "F");
	addBranch(t, "meff", "F");
	addBranch(t, "meffLeps", "F");
	addBranch(t, "meffHC", "F");
	addBranch(t, "mtbmin", "F");
	addBranch(t, "jet_n", "I");
	addBranch(t, "electron_n", "I");
	addBranch(t, "muon_n", "I");
	addBranch(t, "eff70TagBits", "I");
	addBranch(t, "eff60TagBits", "I");
	addBranch(t, "eff50TagBits", "I");
	addBranch(t, "eff30TagBits", "I");

	addBranch(t, "hcand1_m", "F");
	addBranch(t, "hcand1_pt", "F");
	addBranch(t, "hcand1_dRjj", "F");
	addBranch(t, "hcand1_nTrackJets", "I");
	addBranch(t, "hcand1_jet1_pt", "F");
	addBranch(t, "hcand1_jet1_eta", "F");
	addBranch(t, "hcand1_jet1_trackMoverP", "F");
	addBranch(t, "hcand1_jet1_flav", "I");
	addBranch(t, "hcand1_jet1_muCorr", "I");
	addBranch(t, "hcand1_jet2_pt", "F");
	addBranch(t, "hcand1_jet2_eta", "F");
	addBranch(t, "hcand1_jet2_trackMoverP", "F");
	addBranch(t, "hcand1_jet2_flav", "I");
	addBranch(t, "hcand1_jet2_muCorr", "I");
	addBranch(t, "hcand1_tagBits", "I");

	addBranch(t, "hcand2_m", "F");
	addBranch(t, "hcand2_pt", "F");
	addBranch(t, "hcand2_dRjj", "F");
	addBranch(t, "hcand2_nTrackJets", "I");
	addBranch(t, "hcand2_jet1_pt", "F");
	addBranch(t, "hcand2_jet1_eta", "F");
	addBranch(t, "hcand2_jet1_trackMoverP", "F");
	addBranch(t, "hcand2_jet1_flav", "I");
	addBranch(t, "hcand2_jet1_muCorr", "I");
	addBranch(t, "hcand2_jet2_pt", "F");
	addBranch(t, "hcand2_jet2_eta", "F");
	addBranch(t, "hcand2_jet2_trackMoverP", "F");
	addBranch(t, "hcand2_jet2_flav", "I");
	addBranch(t, "hcand2_jet2_muCorr", "I");
	addBranch(t, "hcand2_tagBits", "I");

	addBranch(t, "hcand1_hcand2_dEta", "F");
	addBranch(t, "hcand1_hcand2_dR", "F");
	addBranch(t, "hcand1_hcand2_m", "F");
	addBranch(t, "hcand1_hcand2_pt", "F");
	addBranch(t, "hcand1_hcand2_scaledM", "F");

	addBranch(t, "Xt1", "F");
	addBranch(t, "mTop1", "F");
	addBranch(t, "mW1", "F");
	addBranch(t, "Dhh", "F");
	addBranch(t, "Rhh", "F");
	addBranch(t, "Xhh", "F");

	addBranch(t, "cosThetaStar", "F");
	addBranch(t, "cosTheta1", "F");
	addBranch(t, "cosTheta2", "F");
	addBranch(t, "Phi", "F");
	addBranch(t, "Phi1", "F");
	addBranch(t, "extraJet1_pt", "F");
	addBranch(t, "extraJet1_eta", "F");
	addBranch(t, "extraJet1_HH_dPhi", "F");

	addBranch(t, "electron_veto", "I");
	addBranch(t, "electron1_pt", "F");
	addBranch(t, "electron1_mt", "F");
	addBranch(t, "electron1_minDRj", "F");
	addBranch(t, "electron1_likelihood", "I");
	addBranch(t, "electron1_isolated", "I");
	addBranch(t, "muon_veto", "I");
	addBranch(t, "muon1_pt", "F");
	addBranch(t, "muon1_eta", "F");
	addBranch(t, "muon1_phi", "F");
	addBranch(t, "muon1_E", "F");
	addBranch(t, "muon1_mt", "F");
	addBranch(t, "muon1_minDRj", "F");
	addBranch(t, "muon1_quality", "I");
	addBranch(t, "muon1_isolated", "I");

	addBranch(t, "muon2_pt", "F");
	addBranch(t, "muon2_eta", "F");
	addBranch(t, "muon2_phi", "F");
	addBranch(t, "muon2_E", "F");

	addBranch(t, "dimuon_pt", "F");
	addBranch(t, "dimuon_eta", "F");
	addBranch(t, "dimuon_phi", "F");
	addBranch(t, "dimuon_E", "F");
	addBranch(t, "dimuon_m", "F");

	addBranch(t, "GCdR_min", "F");
	addBranch(t, "GCdR_max", "F");
	addBranch(t, "GCdR_diff", "F");
	addBranch(t, "GCdR_sum", "F");
	addBranch(t, "avgAbsHCJetEta", "F");

	book(t);
}

void HHTreeManager::book(TTree* tBase)
{
	std::cout<<"HHTreeManager::book(TTree* "<< tBase->GetName() <<"):"<< std::endl;
	std::cout<<"HHTreeManager::book: name = "<< m_name << std::endl;
	std::array<std::string, 2> tags{{"SemiTagged", "FullyTagged"}};
	for(auto tag: tags)
	{
		TTree* tCat = (TTree*) tBase->CloneTree(0);//m_name+std::string("_")+ (*var));
		tCat->SetName((m_name+std::string("_")+tag).c_str());
		tCat->SetTitle((m_name+std::string("_")+tag).c_str());
		tCat->SetDirectory(0);
		std::cout<<"Booking TTree "<< tCat->GetName() <<", title = "<< tCat->GetTitle() << std::endl;
		m_tree.insert(std::pair<std::string, TTree*>(tag, tCat));
	}
}

void HHTreeManager::fillTree(Event& event, const std::array<HiggsCandidate, 2>& hcands, const std::vector<Jet>& jets, 
							const std::vector<Electron>& electrons, const std::vector<Muon>& muons,
							const std::vector<TopCandidate>& tops)
{
	if(m_debug) std::cout<<"HHTreeManager::fillTree(Event "<< event.id() <<")"<< std::endl;		
	if(hcands[0].numTightTags() + hcands[1].numTightTags() < 2) return; // Skip events with fewer than 2 tight b-tags
	if(hcands[0].category() != 0 || hcands[1].category() != 0) return;
	if(calcRhh(hcands) > 75) return;
	//event level variables
	fillVar("event", event.number());//changed this to be the event number from the ntuple
	fillVar("eventNumber", event.eventNumber());
	fillVar("runNumber", event.runNumber());
	fillVar("weight", event.weight());
	if(event.isMC())
	{
		//std::cout<<"I'm apparently a MC event!"<< std::endl;
		fillVar("genWeight", event.weight("MC"));//*event.weight("Cross-section"));
		fillVar("xsWeight", event.weight("Cross-section"));
		fillVar("puWeight", event.weight("PileUp"));
		fillVar("btagWeight", event.weight("BTagSF"));
		fillVar("jvtEffSF", event.weight("JvtEffSF"));
		fillVar("btrigSF", event.weight("BTrigSF"));
	}else
	{
		fillVar("genWeight", 1.);
		fillVar("xsWeight", 1.);
		fillVar("puWeight", 1.);
		fillVar("btagWeight", 1.);
		fillVar("jvtEffSF", 1.);
		fillVar("btrigSF", 1.);
		
	}

	if(event.isMC())
	{
		fillVar("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25", event.triggerBits().triggered("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25_emul"));
		fillVar("HLT_j100_2j55_bmv2c2060_split", event.triggerBits().triggered("HLT_j100_2j55_bmv2c2060_split_emul"));
		fillVar("HLT_j225_bmv2c2060_split", event.triggerBits().triggered("HLT_j225_bmv2c2060_split_emul"));
	}else{
		fillVar("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25", event.triggerBits().triggered("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25"));
		fillVar("HLT_j100_2j55_bmv2c2060_split", event.triggerBits().triggered("HLT_j100_2j55_bmv2c2060_split"));
		fillVar("HLT_j225_bmv2c2060_split", event.triggerBits().triggered("HLT_j225_bmv2c2060_split"));
	}

	std::bitset<4> eff70TagBits;
	std::bitset<4> eff60TagBits;
	std::bitset<4> eff50TagBits;
	std::bitset<4> eff30TagBits;
	eff70TagBits.reset(); //sets all bits to 0
	eff60TagBits.reset(); //sets all bits to 0
	eff50TagBits.reset(); //sets all bits to 0
	eff30TagBits.reset(); //sets all bits to 0
	if(m_debug) std::cout<<"HHTreeManager::fillTree: obtained event weight."<< std::endl;		
	//Assume that there are two HiggsCandidates in the array that are ordered correctly (i.e. by pt).
	std::vector<Jet> hcandJets;
	float avgAbsHCJetEta = 0.;
	for(int i = 0; i < 2; ++i)
	{
		for(int j = 0; j < 2;  ++j)
		{
			Jet hcJet = hcands[i].getJetRef(j);
			hcandJets.push_back(hcJet);
			avgAbsHCJetEta += fabs(hcJet.eta());
		}
	}
	std::sort(hcandJets.begin(), hcandJets.end(), [](const Jet a, const Jet b) 
	{
        return b.Pt() < a.Pt();   
    }); 
	fillVar("avgAbsHCJetEta", 0.25*avgAbsHCJetEta);
	float dRMin = 5.;
	float dR2ndMin = 5.;
	for(std::vector<Jet>::const_iterator jet1 = hcandJets.begin(); jet1 != hcandJets.end(); ++jet1)
	{
		for(std::vector<Jet>::const_iterator jet2 = jet1+1; jet2 != hcandJets.end(); ++jet2)
		{
			//if(jet1 == jet2) continue; //jet1 can't equal jet2 because of iterator ranges
			float dR = jet1->deltaR(*jet2);
			if(dR < dRMin)
			{
				dRMin = dR;
				for(std::vector<Jet>::const_iterator jet3 = hcandJets.begin(); jet3 != hcandJets.end(); ++jet3)
				{
					if(jet3 == jet1 || jet3 == jet2) continue;
					for(std::vector<Jet>::const_iterator jet4 = jet3+1; jet4 != hcandJets.end(); ++jet4)
					{
						if(jet4 == jet3 || jet4 == jet2 || jet4 == jet1) continue;
						dR2ndMin = jet3->deltaR(*jet4);
					}
				}
			}
		}
	}
	fillVar("GCdR_min", dRMin);
	fillVar("GCdR_max", dR2ndMin);
	fillVar("GCdR_diff", dR2ndMin - dRMin);
	fillVar("GCdR_sum", dR2ndMin + dRMin);
	fillVar("MEt", event.met());
	//fillVar("MEt_phi", event.metphi());
	fillVar("deltaPhi4jMin", calcDPhi4jMin(hcands, event.metphi()));
	fillVar("TriggerBits", event.triggerBits().to_ulong());
	fillVar("meff", calcMEff(event.met(), jets));
	fillVar("meffLeps", calcMEff(event.met(), jets, muons, electrons));
	fillVar("meffHC", calcMEff(event.met(), hcandJets));
	fillVar("mtbmin", calcMtBMin(event.met(), event.metphi(), hcandJets));
	
	for(unsigned int i = 0; i < 2; ++i)
	{
		std::string branchPrefix = "hcand"+std::to_string(i+1)+"_";
		fillVar(branchPrefix + "m", hcands[i].M());
		fillVar(branchPrefix + "pt", hcands[i].Pt());
		fillVar(branchPrefix + "numTightTags", hcands[i].numTightTags());
		//fillVar(branchPrefix + "category", hcands[i].category());
		fillVar(branchPrefix + "TrackJetsDRjj", hcands[i].deltaRTrackJets());
		fillVar(branchPrefix + "truthHiggsId", hcands[i].truthHiggsId());
		if(hcands[i].category() < 2)
		{
			std::bitset<2> candTagBits; //These are Higgs-candidate-specific and will have fake content for four-tag events
			candTagBits.reset(); //sets all bits to 0
			for(int j = 0; j < 2; ++j)
			{
				fillP4(branchPrefix+"jet"+std::to_string(j+1)+"_", hcands[i].getJet(j)->p4(), false);
				//fillP4(branchPrefix+"jet"+std::to_string(j+1)+"_", hcands[i].getJet(j)->p4(), true);
				fillVar(branchPrefix+"jet"+std::to_string(j+1)+"_flav", hcands[i].getJet(j)->pdgId());
				fillVar(branchPrefix+"jet"+std::to_string(j+1)+"_muCorr", hcands[i].getJet(j)->isMuonCorrected());
				fillVar(branchPrefix+"jet"+std::to_string(j+1)+"_mv2c10", hcands[i].getJet(j)->mv2c10());
				if(hcands[i].getJet(j)->isBTagged()) candTagBits[j] = true;
				if(hcands[i].getJet(j)->isBTagged()) eff70TagBits[2*i+j] = true;
				if(hcands[i].getJet(j)->isBTagged(60)) eff60TagBits[2*i+j] = true;
				if(hcands[i].getJet(j)->isBTagged(50)) eff50TagBits[2*i+j] = true;
				if(hcands[i].getJet(j)->isBTagged(30)) eff30TagBits[2*i+j] = true;
				float trackJetM = 0.;
				float trackJetP = 0.;
				if(hcands[i].getJet(j)->matchedTrackJets().size() == 0) 
				{
					fillVar(branchPrefix+"jet"+std::to_string(j+1)+"_trackMoverP", -0.1);
				}else
				{
					for(auto trackjet :hcands[i].getJet(j)->matchedTrackJets())
					{
						trackJetM += trackjet.M();
						trackJetP += trackjet.P();
					}
					if(trackJetP == float(0)) 
					{
						fillVar(branchPrefix+"jet"+std::to_string(j+1)+"_trackMoverP", -0.1);
					}else
					{
						fillVar(branchPrefix+"jet"+std::to_string(j+1)+"_trackMoverP", trackJetM/trackJetP);
					}
				}
			}	
			if(hcands[0].numTightTags() == 2 && hcands[1].numTightTags() == 2)
			{
				//Randomly call one of the jets untagged.
				candTagBits[m_random2(m_gen)] = 0;
			}
			//std::cout<<"About to fill "<< branchPrefix <<"eff70TagBits with "<< candTagBits.to_ulong() << std::endl;
			fillVar(branchPrefix+"eff70TagBits", candTagBits.to_ulong());
		}
		if(m_debug) std::cout<<"HHTreeManager::fillTree: halfway through candidate "<< i <<"."<< std::endl;
		if(m_includeTruthDetails)
		{
			if(hcands[i].category() < 2)
			{
				fillP4(branchPrefix+"parton1_", hcands[i].getJet(0)->partonP4());
				fillP4(branchPrefix+"parton2_", hcands[i].getJet(1)->partonP4());
			}
			fillVar(branchPrefix+"dRbb", hcands[i].deltaRbb());
		}
		int nTrackJets = 0;
		if(m_debug) std::cout<<"HHTreeManager::fillTree: category = "<< hcands[i].category() << std::endl;
		if(hcands[i].category() == 0)
		{
			nTrackJets += hcands[i].getJet(0)->matchedTrackJets().size();
			nTrackJets += hcands[i].getJet(1)->matchedTrackJets().size();
		}else
		{
			nTrackJets = hcands[i].getFatJet()->matchedTrackJets().size();
		}
		fillVar(branchPrefix + "nTrackJets", nTrackJets);
		fillVar(branchPrefix + "dRjj", hcands[i].deltaR());
	}
	if(tops.empty()) 
	{
		fillVar("Xt1", -9); //There is certainly a top candidate, for there are at least four jets in the event.
		fillVar("mTop1", 9); //There is certainly a top candidate, for there are at least four jets in the event.
		fillVar("mW1", -9); //There is certainly a top candidate, for there are at least four jets in the event.
	}else
	{
		fillVar("Xt1", tops[0].xt()); //There is certainly a top candidate, for there are at least four jets in the event.
		fillVar("mW1", tops[0].mW());
		fillVar("mTop1", tops[0].M());
	}
	fillVar("eff70TagBits", eff70TagBits.to_ulong());
	fillVar("eff60TagBits", eff60TagBits.to_ulong());
	fillVar("eff50TagBits", eff50TagBits.to_ulong());
	fillVar("eff30TagBits", eff30TagBits.to_ulong());
	fillVar("hcand1_hcand2_dEta", fabs(hcands[0].Eta() - hcands[1].Eta()));
	fillVar("hcand1_hcand2_dR", hcands[0].deltaR(hcands[1]));
	fillVar("hcand1_hcand2_m", (hcands[0]+hcands[1]).M());
	fillVar("hcand1_hcand2_pt", (hcands[0]+hcands[1]).Pt());
	fillVar("hcand1_hcand2_ptReco_m", (hcands[0].ptRecoCorrP4()+hcands[1].ptRecoCorrP4()).M());
	fillVar("hcand1_hcand2_scaledM", scaleMass(hcands));
	fillVar("hcand1_hcand2_ptReco_scaledM", scalePtRecoCorrMass(hcands));
	if(m_includeTruthDetails) fillVar("hcand1_hcand2_partonM", (hcands[0].partonP4()+hcands[1].partonP4()).M());
	float xhh = calcXhh(hcands);
	fillVar("Xhh", xhh);
	fillVar("Dhh", calcDhh(hcands));
	fillVar("Rhh", calcRhh(hcands));
	fillJetInfo(hcands, jets);
	fillElectronInfo(event, hcands, electrons, jets);
	fillMuonInfo(event, hcands, muons, jets);
	if(hcands[0].category() < 2 && hcands[1].category() < 2) fillMelaAngles(hcands);

	std::string label = "Don't write"; 
	if(hcands[0].numTightTags() == 2 && hcands[1].numTightTags() == 2)
	{
		label = "FullyTagged";
		if(xhh < 1.6 && !event.isMC()) 
		{
			return; //BLIND ANALYSIS!
		}
	}else //Already checked (if I wanted to) if at least two tightly-tagged jets are present.
	{
		label = "SemiTagged";
	}
	if(label != "Don't write") findAndFillTree(label);
	if(label == "SemiTagged") 
	{
		if(hcands[0].category() == 1 && !hcands[0].fullyBTagged())
		{
			findAndFillTree(label);
		}else if(hcands[1].category() == 1 && !hcands[1].fullyBTagged())
		{
			findAndFillTree(label);
		}
	}
	if(m_debug) std::cout<<"HHTreeManager::fillTree: completed."<< std::endl;
}

float HHTreeManager::calcDhh(const std::array<HiggsCandidate, 2>& hcands)
{
	float thetaSR = 0.741947; //atan(110, 120);
	float theta = atan(hcands[1].M()/hcands[0].M()) - thetaSR;
	return fabs(sqrt(hcands[0].M()*hcands[0].M()+hcands[1].M()*hcands[1].M())*sin(theta)); 
}
float HHTreeManager::calcXhh(const std::array<HiggsCandidate, 2>& hcands)
{
	float dM0 = (hcands[0].M() - 120.0)/(0.1*hcands[0].M());
	float dM1 = (hcands[1].M() - 110.0)/(0.1*hcands[1].M());
	return sqrt(dM0*dM0 + dM1*dM1);
}
float HHTreeManager::calcRhh(const std::array<HiggsCandidate, 2>& hcands)
{
	float dM0 = (hcands[0].M() - 120.0);
	float dM1 = (hcands[1].M() - 110.0);
	return sqrt(dM0*dM0 + dM1*dM1);
}
float HHTreeManager::calcDPhi4jMin(const std::array<HiggsCandidate, 2>& hcands, const float& met_phi)
{
	float minDPhi = 99999.;
	for(int i = 0; i < 2; ++i)
	{
		if(hcands[i].category() == 0)
		{
			for(int j = 0; j < 2; ++j)
			{
				float dPhi = fabs(deltaPhi(hcands[i].getJet(j)->phi(), met_phi));
				if(dPhi < minDPhi) minDPhi = dPhi;
			}
		}else
		{
			float dPhi = fabs(deltaPhi(hcands[i].getFatJet()->phi(), met_phi));
			if(dPhi < minDPhi) minDPhi = dPhi;
		}
	}
	return minDPhi;
}

void HHTreeManager::fillJetInfo(const std::array<HiggsCandidate, 2>& hcands, const std::vector<Jet>& jets)
{
	fillVar("jet_n", jets.size());
	int bjet_n = 0;
	std::vector<Jet>::const_iterator extraJet1It = jets.end();
	std::vector<Jet>::const_iterator extraJet2It = jets.end();
	
	//Note that jet collection is sorted already
	assert(std::is_sorted(jets.begin(), jets.end(), [](const Jet a, const Jet b){ return b.Pt() < a.Pt();}));
	for(std::vector<Jet>::const_iterator jetIt = jets.begin(); jetIt != jets.end(); ++jetIt)
	{
		Jet jet = *jetIt;
		if(jet.isBTagged()) ++bjet_n;
		bool hcandJet = false;
		for(auto hcand: hcands)
		{
			if(hcand.category() == 0)
			{
				if(hcand.getJetIndex(0) == jet.index()) hcandJet = true;	
				if(hcand.getJetIndex(1) == jet.index()) hcandJet = true;	
			}else
			{
				if(hcand.getFatJet()->index() == jet.index() && hcand.getFatJet()->rParam() == float(0.4)) hcandJet = true;	
			}
		}	
		if(hcandJet) continue;
		if(extraJet1It == jets.end())
		{
			extraJet1It = jetIt;
		}else if(extraJet2It == jets.end())
		{
			extraJet2It = jetIt;
		}
	}
	fillVar("bjet_n", bjet_n);
	if(extraJet1It != jets.end())
	{
		fillP4("extraJet1_", extraJet1It->p4());
		ROOT::Math::PtEtaPhiEVector hhP4 = hcands[0].p4()+hcands[1].p4();
		fillVar("extraJet1_HH_dPhi", deltaPhi(hhP4.Phi(), extraJet1It->Phi()));
	}else{
		fillP4Dummy("extraJet1_");
		fillVar("extraJet1_HH_dPhi", -5.);
	}
	if(extraJet2It != jets.end())
	{
		fillP4("extraJet2_", extraJet2It->p4());
	}else{
		fillP4Dummy("extraJet2_");
	}
}
void HHTreeManager::fillElectronInfo(Event& event, const std::array<HiggsCandidate, 2>& hcands, const std::vector<Electron>& electrons, const std::vector<Jet>& jets)
{
	int electron_n = 0;
	assert(std::is_sorted(electrons.begin(), electrons.end(), [](const Electron a, const Electron b){ return b.Pt() < a.Pt();}));
	std::vector<Electron>::const_iterator leadElectron = electrons.end();
	float leadElectronMinDRHCj = 99.;
	int hcJId = -9;
	bool electronVeto = false;
	for(std::vector<Electron>::const_iterator electronIt = electrons.begin(); electronIt != electrons.end(); ++electronIt)
	{
		if(!electronIt->isolated()) continue;
		Electron electron = *electronIt;
		//From Kristian: electron LH: tight = 0, medium = 1, loose = 2
		if(electron.isolated() && electron.likelihood() >= 0 && electron.likelihood() <= 2) 
			electronVeto = true;
		++electron_n;
		leadElectron = electronIt; //First one to be found is leading, since vector is sorted
		break;
	} 
	fillVar("electron_n", electron_n);
	if(electron_n == 0)
	{
		fillVar("electron_veto", 0);
		fillP4Dummy("electron1_");
		fillVar("electron1_mt", -9);
		fillVar("electron1_likelihood", -9);
		fillVar("electron1_isolated", -9);
	}else
	{
		fillVar("electron_veto", electronVeto);
		fillP4("electron1_", *leadElectron);
		float mt = sqrt(leadElectron->pt()*event.met()*(1-cos(deltaPhi(leadElectron->phi(), event.metphi()))));
		fillVar("electron1_mt", mt);
		fillVar("electron1_likelihood", leadElectron->likelihood());
		fillVar("electron1_isolated", leadElectron->isolated());
		float minDRj = 99.;
		for(auto jet: jets)
		{
			float dR = jet.deltaR(*leadElectron);
			if(dR < minDRj)
			{
				minDRj = dR;
			}
		}
		fillVar("electron1_minDRj", minDRj);
	}
}
void HHTreeManager::fillMuonInfo(Event& event, const std::array<HiggsCandidate, 2>& hcands, const std::vector<Muon>& muons, const std::vector<Jet>& jets)
{
	assert(std::is_sorted(muons.begin(), muons.end(), [](const Muon a, const Muon b){ return b.Pt() < a.Pt();}));
	std::vector<Muon> extraMuons;
	bool muonVeto = false;
	for(std::vector<Muon>::const_iterator muonIt = muons.begin(); muonIt != muons.end(); ++muonIt)
	{
		if(!muonIt->isolated()) continue;
		Muon muon = *muonIt;
		if(muon.isolated() && muon.quality())
			muonVeto = true;
		bool hcandMuon = false;
		for(auto hcand: hcands)
		{
			std::array<int, 2> hcandMuonIds = hcand.muonCorrectionId();	
			if(muon.index() == hcandMuonIds[0]) hcandMuon = true;
			if(muon.index() == hcandMuonIds[1]) hcandMuon = true;
		}	
		if(hcandMuon) continue;
		extraMuons.push_back(muon);
	}
	fillVar("muon_n", extraMuons.size());

	if(extraMuons.size() == 0)
	{
		fillVar("muon_veto", 0);
		fillP4Dummy("muon1_");
		fillVar("muon1_mt", -9);
		fillVar("muon1_minDRj", -9);
		fillP4Dummy("muon2_");
		fillP4Dummy("dimuon_");
		fillVar("dimuon_m", -99.);
	}else
	{
		fillVar("muon_veto", muonVeto);
		fillP4("muon1_", extraMuons[0]);
		float mt = sqrt(extraMuons[0].pt()*event.met()*(1-cos(deltaPhi(extraMuons[0].phi(), event.metphi()))));
		fillVar("muon1_mt", mt);
		float minDRj = 99.;
		for(auto jet: jets)
		{
			float dR = jet.deltaR(extraMuons[0]);
			if(dR < minDRj) minDRj = dR;
		}
		fillVar("muon1_minDRj", minDRj);
			
		if(extraMuons.size() > 1)
		{
			fillP4("muon2_", extraMuons[1]);
			ROOT::Math::PtEtaPhiEVector dimuonP4 = extraMuons[0].p4()+extraMuons[1].p4();
			fillP4("dimuon_", dimuonP4);
			fillVar("dimuon_m", dimuonP4.M());
		}else
		{
			fillP4Dummy("muon2_");
			fillP4Dummy("dimuon_");
			fillVar("dimuon_m", -99.);
		}
	}
}
void HHTreeManager::fillMelaAngles(const std::array<HiggsCandidate, 2>& hcands)
{
	ROOT::Math::PtEtaPhiEVector q1_lab = hcands[0];
	ROOT::Math::PtEtaPhiEVector q2_lab = hcands[1];
	ROOT::Math::PtEtaPhiEVector q11_lab = hcands[0].getJet(0)->p4(); 
	ROOT::Math::PtEtaPhiEVector q12_lab = hcands[0].getJet(1)->p4();
	ROOT::Math::PtEtaPhiEVector q21_lab = hcands[1].getJet(0)->p4(); 
	ROOT::Math::PtEtaPhiEVector q22_lab = hcands[1].getJet(1)->p4();
	//For semi-merged candidates, the track-jets need to be corrected for the fact that they do not measure all
	//b-jets energy, but only the charged fraction.
	//Here, we assert that the sum of track-jets' momenta, appropriately corrected, is the Higgs boson momentum.
	if(hcands[0].category() == 1) 
	{
		//Could correct angles to better match calo-jet, but we choose not to.
		float sumTrackJets1112Pt = (hcands[0].getJet(0)->p4() + hcands[0].getJet(1)->p4()).Pt();
		float scale = hcands[0].Pt()/sumTrackJets1112Pt;
		q11_lab *= scale;
		q12_lab *= scale;
		q1_lab = q11_lab + q12_lab;
	}
	if(hcands[1].category() == 1)
	{
		float sumTrackJets2122Pt = (hcands[1].getJet(0)->p4() + hcands[1].getJet(1)->p4()).Pt();
		float scale = hcands[1].Pt()/sumTrackJets2122Pt;
		q21_lab *= scale;
		q22_lab *= scale;
		q2_lab = q21_lab + q22_lab;
	}
	ROOT::Math::PtEtaPhiEVector X_lab = q1_lab + q2_lab;
	ROOT::Math::Boost boostToXCM = ROOT::Math::Boost(X_lab.BoostToCM());
	ROOT::Math::PtEtaPhiEVector X_XRF = boostToXCM(X_lab);
	ROOT::Math::PtEtaPhiEVector q1_XRF = boostToXCM(q1_lab);
	ROOT::Math::PtEtaPhiEVector q2_XRF = boostToXCM(q2_lab);
	ROOT::Math::PtEtaPhiEVector q11_XRF = boostToXCM(q11_lab);
	ROOT::Math::PtEtaPhiEVector q12_XRF = boostToXCM(q12_lab);
	ROOT::Math::PtEtaPhiEVector q21_XRF = boostToXCM(q21_lab);
	ROOT::Math::PtEtaPhiEVector q22_XRF = boostToXCM(q22_lab);
	assert((q1_XRF+q2_XRF).Pt() < 1e-8); //Check that boost has been correctly calculated, since X_RF pt = 0.
	assert(std::abs((q11_XRF + q12_XRF).Pt() - q1_XRF.Pt()) < 1e-5); //Check only appropriate for resolved Higgs candidates
	assert(std::abs((q21_XRF + q22_XRF).Pt() - q2_XRF.Pt()) < 1e-5); 
	ROOT::Math::XYZVector nZ(0., 0., 1.);
	ROOT::Math::XYZVector n1  = q11_XRF.Vect().Cross(q12_XRF.Vect()).Unit();
	ROOT::Math::XYZVector n2  = q21_XRF.Vect().Cross(q22_XRF.Vect()).Unit();
	ROOT::Math::XYZVector nSC = nZ.Cross(q1_XRF.Vect()).Unit();
	//For discrimination purposes, take the absolute values of many of these variables...
	//fillVar("cosThetaStar", cos(q1_XRF.Theta()-nZ.Theta()));
	fillVar("cosThetaStar", fabs(cos(q1_XRF.Theta()-nZ.Theta())));
	//fillVar("Phi", fabs(sign(q1_XRF.Vect().Dot(n1.Cross(n2))) * acos(-n1.Dot(n2)))); 
	fillVar("Phi", fabs(acos(-n1.Dot(n2)))); 
	//fillVar("Phi1", sign(q1_XRF.Vect().Dot(n1.Cross(nSC))) * acos(n1.Dot(nSC)));
	fillVar("Phi1", fabs(acos(n1.Dot(nSC))));
	//For discrimination purposes, want |Phi1| - 0.5*pi
	ROOT::Math::Boost boostToH1CM = ROOT::Math::Boost(q1_lab.BoostToCM());
	ROOT::Math::Boost boostToH2CM = ROOT::Math::Boost(q2_lab.BoostToCM());
	ROOT::Math::PtEtaPhiEVector q11_H1RF = boostToH1CM(q11_lab);
	ROOT::Math::PtEtaPhiEVector q21_H2RF = boostToH2CM(q21_lab);
	fillVar("cosTheta1",  fabs(q1_XRF.Vect().Dot(q11_H1RF.Vect()) / sqrt(q1_XRF.Vect().Mag2()*q11_H1RF.Vect().Mag2())));
	fillVar("cosTheta2",  fabs(q2_XRF.Vect().Dot(q21_H2RF.Vect()) / sqrt(q2_XRF.Vect().Mag2()*q21_H2RF.Vect().Mag2())));
}
void HHTreeManager::fillP4(const std::string& branchStem, const ROOT::Math::PtEtaPhiEVector& p4, const bool absEta)
{
	fillVar(branchStem+"pt", p4.Pt());
	if(absEta)
	{
		fillVar(branchStem+"eta", fabs(p4.Eta()));
	}else
	{
		fillVar(branchStem+"eta", p4.Eta());
	}
	fillVar(branchStem+"phi", p4.Phi());
	fillVar(branchStem+"E", p4.E());
	fillVar(branchStem+"m", p4.M());
}
void HHTreeManager::fillP4Dummy(const std::string& branchStem)
{
	fillVar(branchStem+"pt", -99.);
	fillVar(branchStem+"eta", -99.);
	fillVar(branchStem+"phi", -99.);
	fillVar(branchStem+"E", -99.);
}
void HHTreeManager::fillVar(std::string varName, double value)
{
	//LUKE: add error message if this isn't found
	std::string type = m_typeMap[varName];
	if(type == "I")
	{
		m_varMap_I[varName] = value;
	}else if(type == "F")
	{
		m_varMap_F[varName] = value;
	}
}
void HHTreeManager::fillVar(std::string varName, std::vector<float> value)
{
	(m_varMap_V[varName]).clear();
	m_varMap_V[varName] = value;
}
void HHTreeManager::findAndFillTree(const std::string& variation)
{
	std::map<std::string, TTree*>::iterator it = m_tree.find(variation);
	if(it != m_tree.end())
	{
		it->second->Fill();
	}else{
		std::cout<<"HHTreeManager::findAndFillTree("<< variation <<"): couldn't find TTree for "<< variation << std::endl;
	}
}
void HHTreeManager::write(TFile* fOut)
{
	fOut->cd();
	std::cout<<"HHTreeManager::writeAll: Writing "<< m_tree.size() <<" trees."<< std::endl;
	for(std::map<std::string, TTree*>::iterator it = m_tree.begin(); it != m_tree.end(); ++it)
	{
		std::cout<<"HHTreeManager::writeAll: Writing "<< it->first ;
		std::cout<<": TTree name = "<< it->second->GetName() <<", title = "<< it->second->GetTitle() <<" address = "<< it->second <<std::endl;
		it->second->Write();
	}
}
void HHTreeManager::addBranch(TTree* t, std::string varName, std::string varType)
{
	m_typeMap[varName] = varType;
	if(varType == "I")
	{
		t->Branch(varName.c_str(), &(m_varMap_I[varName]), (varName + "/" + varType).c_str());
	}
	if(varType == "F")
	{
		t->Branch(varName.c_str(), &(m_varMap_F[varName]), (varName + "/" + varType).c_str());
	}
}
void HHTreeManager::addVectorBranch(TTree* t, std::string varName)
{
	m_typeMap[varName] = "V";
	t->Branch(varName.c_str(), &(m_varMap_V[varName]));
}
float HHTreeManager::deltaPhi(const float& phi1, const float& phi2)
{
	float dphi = phi1 - phi2;
	while(dphi >=  M_PI)
		dphi -= 2.*M_PI;
	while(dphi < -M_PI)
		dphi += 2.*M_PI;
	return dphi;
}
float HHTreeManager::deltaPhi(const ROOT::Math::PtEtaPhiEVector& vec1, const ROOT::Math::PtEtaPhiEVector& vec2)
{
	float dphi = vec1.Phi() - vec2.Phi();
	while(dphi >=  M_PI)
		dphi -= 2.*M_PI;
	while(dphi < -M_PI)
		dphi += 2.*M_PI;
	return dphi;
}
float HHTreeManager::deltaR(const ROOT::Math::PtEtaPhiEVector& vec1, const ROOT::Math::PtEtaPhiEVector& vec2)
{
	float deta = vec1.Eta() - vec2.Eta();
	float dphi = deltaPhi(vec1, vec2);
	return sqrt(deta*deta+dphi*dphi);
}
float HHTreeManager::scaleMass(const std::array<HiggsCandidate, 2>& hcands)
{
	ROOT::Math::PtEtaPhiEVector scaledH1 = (125./hcands[0].M())*hcands[0].p4();
	ROOT::Math::PtEtaPhiEVector scaledH2 = (125./hcands[1].M())*hcands[1].p4();
	return (scaledH1+scaledH2).M();
}
float HHTreeManager::scalePtRecoCorrMass(const std::array<HiggsCandidate, 2>& hcands)
{
	ROOT::Math::PtEtaPhiEVector scaledH1 = (125./hcands[0].ptRecoCorrP4().M())*hcands[0].ptRecoCorrP4();
	ROOT::Math::PtEtaPhiEVector scaledH2 = (125./hcands[1].ptRecoCorrP4().M())*hcands[1].ptRecoCorrP4();
	return (scaledH1+scaledH2).M();
}
float HHTreeManager::calcMEff(const float& met, const std::vector<Jet>& jets)
{
	float meff = met;
	for(auto jet: jets)
	{
		meff += jet.pt();
	}
	return meff;
}
float HHTreeManager::calcMEff(const float& met, const std::vector<Jet>& jets, const std::vector<Muon>& muons, const std::vector<Electron>& electrons)
{
	float meff = calcMEff(met, jets);
	for(auto elec: electrons)
	{
		if(!elec.isolated()) continue;
		meff += elec.pt();
	}
	for(auto muon: muons)
	{
		if(!muon.isolated()) continue;
		meff += muon.pt();
	}
	return meff;
}
float HHTreeManager::calcMtBMin(const float& met, const float& met_phi, const std::vector<Jet>& jets)
{
	float minMTB = 99999.;
	float mex = met*cos(met_phi);
	float mey = met*sin(met_phi);
	//for(auto jet: jets)
	long unsigned int maxNJets = 3;
	for(long unsigned int i = 0; i < std::min(jets.size(), maxNJets); ++i) //Assumes jets are sorted sensibly
	{
		Jet jet = jets[i];
		float jpx = jet.px();
		float jpy = jet.py();
		float mtb = sqrt((met + jet.pt())*(met + jet.pt()) - (mex + jpx)*(mex + jpx) - (mey + jpy)*(mey + jpy));
		if(mtb < minMTB) minMTB = mtb;
	}
	return minMTB;	
}
