/***********************************************************************************************
*	JetProducer
*	A class to create Jets
*
*	David Wardrope, 29th November 2012
*
***********************************************************************************************/
#ifndef JETPRODUCER_H
#define JETPRODUCER_H

#include "Event.h"
#include "Jet.h"
#include <stdlib.h> 

extern bool m_debug;
//extern float m_minBWeight;
class JetProducer 
{
	public:
        JetProducer(Event& evt, const std::string& calib, bool trackJetBTagging, int btagWP=77):
				m_evt(evt), m_calib(calib), m_trackJetBTagging(trackJetBTagging), m_btagWP(btagWP)
		{ 
			//if(m_debug) std::cout<<"JetProducer("<< name <<", " <<")"<< std::endl;
			m_branchStem = "calojet_akt4_";
			setupBranches(); 
			setMinBTag(btagWP);
		}
        ~JetProducer(){
        }
		std::vector<Jet> produce(const float& ptMin, const float& etaMax, bool rejectNonBTagged, 
									const std::vector<Jet>& trackJets);
		void setMinBTag(const int& btagWP) { m_minBWeight = Jet::findMinBTag(btagWP); }
	private:
		void cleanUp();
		void fillArrays();
		void nullPtrs();
		void setupBranches();

		std::string m_branchStem;
		Event& m_evt;
		const std::string& m_calib;
		bool m_trackJetBTagging;
		int m_btagWP;
		float m_minBWeight;
		
		//TTree variables
		//int m_jet_n, m_otherjet_n;
		std::vector<float> *m_jet_pt, *m_jet_eta, *m_jet_phi, *m_jet_E, *m_jet_mv2c10, *m_jet_jvt;
		std::vector<float> *m_jet_pt_noMuCorr;
		std::vector<float> *m_jet_factorPtReco;
		//std::vector<float> *m_parton_pt, *m_parton_eta, *m_parton_phi, *m_parton_E;
		std::vector<float> *m_jet_mv2c10_SF;
		std::vector<int> *m_jet_passBadLoose, *m_jet_pdgId;
		std::vector<std::vector<int> > *m_jet_trackJetIdx, *m_jet_truthJetIdx, *m_jet_partIdx;
		//float m_jet_btag_sf[__nJets], m_jet_btag_sferr[__nJets], m_jet_btag_sfvarup[__nJets][10],m_jet_btag_sfvardn[__nJets][10];
		std::vector<float> *m_truthJet_pt, *m_truthJet_eta, *m_truthJet_phi, *m_truthJet_E;
		//std::vector<int> *m_jet_truthflav;
};
#endif
