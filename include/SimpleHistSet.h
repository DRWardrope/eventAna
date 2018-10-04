/**************************************************************************************************
*
*	SimpleSimpleHistSet
*	Defines a set of histograms for the statistical analysis.  
*	No functions are provided to manipulate the sets of histograms or to compare them.
*	David Wardrope, 14th March 2017
*
**************************************************************************************************/
#ifndef SIMPLEHISTSET_H
#define SIMPLEHISTSET_H
#include "SimpleEvent.h"
#include "TH1D.h"

class SimpleHistSetCollection;

class SimpleHistSet
{
	public:
		SimpleHistSet()
		{
			m_name = "Uninitialized";
		}
		SimpleHistSet(const std::string& nameIn);
		float calculateTotalUnc(std::shared_ptr<TH1D> h);
		std::shared_ptr<TH1D> createPlot(const std::string& histName, const std::string& title, 
											const int& nBins, const float& xMin, const float& xMax)
		{
			std::shared_ptr<TH1D> h = std::make_shared<TH1D>((m_name+"_"+histName).c_str(), title.c_str(), nBins, xMin, xMax);
			mapHists.insert(std::pair<std::string, std::shared_ptr<TH1D> >(histName, h));
			return h;
		}
		std::shared_ptr<TH2D> createPlot(const std::string& histName, const std::string& title, 
											const int& nBinsX, const float& xMin, const float& xMax,
											const int& nBinsY, const float& yMin, const float& yMax)
		{
			std::shared_ptr<TH2D> h = std::make_shared<TH2D>((m_name+"_"+histName).c_str(), title.c_str(), nBinsX, xMin, xMax, nBinsY, yMin, yMax);
			map2DHists.insert(std::pair<std::string, std::shared_ptr<TH2D> >(histName, h));
			return h;
		}
		void fillPlots(const SimpleEvent& event, const float& weight);
		void fillPlots(const SimpleEvent& event, const float& weight, const float& weightA, const float& weightB);
		int numEntries() { return d_pos_weight.size() + d_neg_weight.size(); } 
		void makeKDEHistogram(std::shared_ptr<TH1D> h, std::shared_ptr<TH1D> hOrig, const std::vector<double>& posVar, std::vector<double>& posWeight, 
					const std::vector<double>& negVar, std::vector<double>& negWeight);
		double tameWeights(std::vector<double>&);
		double sumWeights(std::vector<double>& weights);
		double sumWeights() { return sumWeights(d_pos_weight) + sumWeights(d_neg_weight); } 
		void writePlot(const std::string& key)
		{
			std::map<std::string, std::shared_ptr<TH1D> >::iterator hIt = mapHists.find(key);
			if(hIt != mapHists.end())
			{
				hIt->second->Write();
			}else
			{
				std::map<std::string, std::shared_ptr<TH2D> >::iterator hIt2D = map2DHists.find(key);
				if(hIt2D != map2DHists.end())
				{
					hIt2D->second->Write();
				}else
				{
					std::cout<<"HistSet::writePlot: couldn't find "<< key << std::endl;
				}
			}
		}
		void writePlots();//(TFile* f);
		void zeroNegativeBins();
	private:
		std::string m_name;
		std::shared_ptr<TH1D> h_count, h_count_a, h_count_b;
		std::shared_ptr<TH1D> h_hh_M, h_kde_hh_M, h_hh_M_a, h_hh_M_b;
		std::shared_ptr<TH1D> h_MEt, h_MEt_a, h_MEt_b, h_kde_MEt;
		std::shared_ptr<TH1D> h_Xhh, h_Xhh_a, h_Xhh_b;
		std::shared_ptr<TH1D> h_extraMuon1_mt;
		std::shared_ptr<TH1D> h_hh_flavourSum;
		std::shared_ptr<TH2D> h_h1_h2_flavourSum;
		std::shared_ptr<TH1D> h_weight;
		std::shared_ptr<TH2D> h_h1_h2_m, h_avgMinPt_h1_h2_m, h_avgNJets_h1_h2_m;
		std::shared_ptr<TH2D> h_hlt_2b35_2j35_h1_h2_m, h_hlt_b75_3j75_h1_h2_m, h_hlt_j100_2b55_h1_h2_m, h_hlt_b225_h1_h2_m, h_hlt_j380_h1_h2_m, h_hlt_b80_xe60_h1_h2_m;
		std::vector<double> d_pos_hh_M, d_neg_hh_M; 
		std::vector<double> d_pos_MEt, d_neg_MEt; 
		std::vector<double> d_pos_weight, d_neg_weight;
		std::map<std::string, std::shared_ptr<TH1D> > mapHists;
		std::map<std::string, std::shared_ptr<TH2D> > map2DHists;
};

class SimpleHistSetCollection
{
	public:
		SimpleHistSetCollection(const std::string& nameIn, const std::string& seln): m_name(nameIn), m_seln(seln)
		{
			signal = createSimpleHistSet(m_name, m_seln, "signal");
			controlReg1 = createSimpleHistSet(m_name, m_seln, "controlReg1");
			controlReg2 = createSimpleHistSet(m_name, m_seln, "controlReg2");
			valReg1 = createSimpleHistSet(m_name, m_seln, "valReg1");
			valReg2 = createSimpleHistSet(m_name, m_seln, "valReg2");
			superReg = createSimpleHistSet(m_name, m_seln, "superReg");//SuperRegion is SR+CR1+CR2
		}
		std::shared_ptr<SimpleHistSet> createSimpleHistSet(const std::string& hsName, const std::string& selnName, 
															const std::string& regionName)
		{
			std::shared_ptr<SimpleHistSet> hs = std::make_shared<SimpleHistSet>(SimpleHistSet(hsName+"_"+selnName+"_"+regionName));
			mapSimpleHistSets.insert(std::pair<std::string, std::shared_ptr<SimpleHistSet> >(regionName, hs));
			return hs;
		}
		void fillPlots(const SimpleEvent& event, const float& weight)
		{
			if(event.inSuperRegion()) superReg->fillPlots(event, weight);
			if(event.inSignalRegion()) signal->fillPlots(event, weight);
			else if(event.inControlRegion1()) controlReg1->fillPlots(event, weight);
			else if(event.inControlRegion2()) controlReg2->fillPlots(event, weight);

			if(event.inValRegion1()) valReg1->fillPlots(event, weight);
			else if(event.inValRegion2()) valReg2->fillPlots(event, weight);
		}
		void fillPlots(const SimpleEvent& event, const float& weight, const float& weightA, const float& weightB)
		{
			if(event.inSuperRegion()) superReg->fillPlots(event, weight, weightA, weightB);
			if(event.inSignalRegion()) signal->fillPlots(event, weight, weightA, weightB);
			else if(event.inControlRegion1()) controlReg1->fillPlots(event, weight, weightA, weightB);
			else if(event.inControlRegion2()) controlReg2->fillPlots(event, weight, weightA, weightB);

			if(event.inValRegion1()) valReg1->fillPlots(event, weight, weightA, weightB);
			else if(event.inValRegion2()) valReg2->fillPlots(event, weight, weightA, weightB);
		}
		double sumWeights(const std::string& region); 
		void writePlot(const std::string& key)
		{
			zeroNegativeBins();
			for(auto pair: mapSimpleHistSets)
			{
				pair.second->writePlot(key);
			}
		}
		void writePlots()
		{
			zeroNegativeBins();
			for(auto pair: mapSimpleHistSets)
			{
				pair.second->writePlots();
			}
		}
		void zeroNegativeBins()
		{
			for(auto pair: mapSimpleHistSets)
			{
				pair.second->zeroNegativeBins();
			}
		}
	private:
		std::string m_name, m_seln;
		std::map<std::string, std::shared_ptr<SimpleHistSet> > mapSimpleHistSets;
		std::shared_ptr<SimpleHistSet> superReg, controlReg1, controlReg2, valReg1, valReg2, signal;
};	
#endif
