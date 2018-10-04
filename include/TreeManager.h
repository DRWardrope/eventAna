#ifndef TREEMANAGER_H 
#define TREEMANAGER_H
#include <array>
#include <random>
#include "Event.h"
#include "HiggsCandidate.h"
#include "Electron.h"
#include "Muon.h"
#include "TopCandidate.h"
extern bool m_debug;

class TreeManager
{
	public:
		TreeManager(std::string name): m_name(name)
		{
			this->bookTrees();
			std::random_device rd;//Get random number generator seed from hardware device, if available
			m_gen = std::mt19937(rd());//Initialize random number generator.
			m_random2 = std::uniform_int_distribution<unsigned int>(0,1);
		}
		//Implement the following destructor to delete the TTree* held by the class.
		//I should really either follow the "Rule of Three" or use std::unique_ptr.
		~TreeManager()
		{
			for(std::map<std::string, TTree*>::iterator it = m_tree.begin(); it != m_tree.end(); ++it)
			{
				std::cout<<"TreeManager::~TreeManager: Deleting "<< it->first ;
				std::cout<<": TTree name = "<< it->second->GetName() <<", title = "<< it->second->GetTitle() <<" address = "<< it->second <<std::endl;
				delete it->second;	
			}
		}
		void findAndFillTree(const std::string&);
		void fillCutFlow(std::map<std::string, double>, std::string);
		void fillTree(Event& event, const std::array<HiggsCandidate, 2>&, const std::vector<Jet>&, 
						const std::vector<Electron>&, const std::vector<Muon>&, const std::vector<TopCandidate>&);
		void setName(const std::string& name){ m_name = name; }
		void write(TFile* fOut);
	private:
		void addBranch(TTree*, std::string, std::string);
		void addVectorBranch(TTree*, std::string);
		void book(TTree*);
		void bookTrees();
		float calcDhh(const std::array<HiggsCandidate, 2>&);
		float calcRhh(const std::array<HiggsCandidate, 2>&);
		float calcXhh(const std::array<HiggsCandidate, 2>&);
		float calcDPhi4jMin(const std::array<HiggsCandidate, 2>& hcands, const float& met_phi);
		float calcMEff(const float& met, const std::vector<Jet>&);
		float calcMEff(const float& met, const std::vector<Jet>&, const std::vector<Muon>&, const std::vector<Electron>&);
		//float calcMEffHC(const float& met, const std::array<HiggsCandidate, 2>&);
		float calcMtBMin(const float& met, const float& met_phi, const std::vector<Jet>&);
		bool containsBaseline(std::vector<std::string>);
		void fillElectronInfo(Event& event, const std::array<HiggsCandidate, 2>&, const std::vector<Electron>&, const std::vector<Jet>&);
		void fillJetInfo(const std::array<HiggsCandidate, 2>&, const std::vector<Jet>&);
		void fillMelaAngles(const std::array<HiggsCandidate, 2>&);
		void fillMuonInfo(Event& event, const std::array<HiggsCandidate, 2>&, const std::vector<Muon>&, const std::vector<Jet>&);
		void fillP4(const std::string&, const ROOT::Math::PtEtaPhiEVector&, const bool absEta=false);
		void fillP4Dummy(const std::string&);
		void fillVar(std::string, double);
		void fillVar(std::string, std::vector<float>);
		template <typename T> int sign(T val){ return (T(0) < val) - (val < T(0)); }
		float deltaPhi(const float& phi1, const float& phi2);
		float deltaPhi(const ROOT::Math::PtEtaPhiEVector& phi1, const ROOT::Math::PtEtaPhiEVector& phi2);
		float deltaR(const ROOT::Math::PtEtaPhiEVector& phi1, const ROOT::Math::PtEtaPhiEVector& phi2);
		float scaleMass(const std::array<HiggsCandidate, 2>&);
		float scaleMEt(const float& met, const float& met_phi, const std::array<HiggsCandidate,2>&);
		float scalePtRecoCorrMass(const std::array<HiggsCandidate, 2>&);

		std::string m_name;
		std::vector<float> m_branches;
		std::vector<int> m_branches_I;
		std::vector<float> m_branches_F;
		std::map<std::string, TTree*> m_tree;
		std::map<std::string, TTree*>::iterator mIt_tree;

		std::map<std::string, int > m_varMap_I;
		std::map<std::string, float > m_varMap_F;
		std::map<std::string, std::string > m_typeMap;
		std::map<std::string, std::vector<float> > m_varMap_V;
		bool m_includeTruthDetails;
		std::mt19937 m_gen;
		std::uniform_int_distribution<unsigned int> m_random2;
};
#endif
