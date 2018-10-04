#include "SimpleHistSet.h"
#include "TGraphErrors.h"
#include "TKDE.h"
SimpleHistSet::SimpleHistSet(const std::string& nameIn)
{
	//std::cout<<"SimpleHistSet::SimpleHistSet("<< nameIn << std::endl;
	m_name = nameIn;
	h_count = createPlot("count", (nameIn+";;Number of Events").c_str(), 1, 0, 2);
	h_count_a = createPlot("count_a", (nameIn+";;Number of Events").c_str(), 1, 0, 2);
	h_count_b = createPlot("count_b", (nameIn+";;Number of Events").c_str(), 1, 0, 2);
	//int nBins = 140;
	int nBins = 56;
	//int nBins = 28;
	h_hh_M = createPlot("hh_M", (nameIn+";m_{HH} [GeV];Number of Events").c_str(), nBins, 200., 1600.);
	h_hh_M_a = createPlot("hh_M_a", (nameIn+";m_{HH} [GeV];Number of Events").c_str(), nBins, 200., 1600.);
	h_hh_M_b = createPlot("hh_M_b", (nameIn+";m_{HH} [GeV];Number of Events").c_str(), nBins, 200., 1600.);
	h_kde_hh_M = createPlot("kde_hh_M", (nameIn+";m_{HH} [GeV];Number of Events").c_str(), nBins, 200., 1600.);
	h_extraMuon1_mt = createPlot("extraMuon1_mt", (nameIn+";m_{T}(#mu, #slash{E}_{T}) [GeV];Number of Events").c_str(), 
									3, 0, 120);
	h_weight = createPlot("weight", (nameIn+";weight;Number of Events").c_str(), 100, -100, 100.);
	h_hh_flavourSum = createPlot("hh_flavourSum", (nameIn+";flavour sum;Number of Events").c_str(), 21, -0.5, 20.5);
	h_h1_h2_flavourSum = createPlot("h1_h2_flavourSum", 
								(nameIn+";Leading candidate's flavour sum; Subleading candidate's flavour sum;Number of Events").c_str(), 
								21, -0.5, 20.5, 21, -0.5, 20.5);
	int nMEtBins = 8;
	h_kde_MEt = createPlot("kde_MEt", (nameIn+";#slash{E}_{T} [GeV];Number of Events").c_str(), nMEtBins, 0., 800.);
	h_MEt = createPlot("MEt", (nameIn+";#slash{E}_{T} [GeV];Number of Events").c_str(), nMEtBins, 0., 800.);
	h_MEt_a = createPlot("MEt_a", (nameIn+";#slash{E}_{T} [GeV];Number of Events").c_str(), nMEtBins, 0., 800.);
	h_MEt_b = createPlot("MEt_b", (nameIn+";#slash{E}_{T} [GeV];Number of Events").c_str(), nMEtBins, 0., 800.);
	
	h_Xhh = createPlot("Xhh", (nameIn+";X_{hh};Number of Events").c_str(), nBins, 0., 1.6);
	h_Xhh_a = createPlot("Xhh_a", (nameIn+";X_{hh};Number of Events").c_str(), nBins, 0., 1.600);
	h_Xhh_b = createPlot("Xhh_b", (nameIn+";X_{hh};Number of Events").c_str(), nBins, 0., 1.600);

	float mHMin = 0.; float mHMax = 200.;
	int nMHBins = (mHMax - mHMin)/10;
	h_h1_h2_m = createPlot("h1_h2_m", (nameIn+";m_{H}^{lead} [GeV];m_{H}^{subl} [GeV]").c_str(), nMHBins, mHMin, mHMax, nMHBins, mHMin, mHMax); 
	h_avgMinPt_h1_h2_m = createPlot("avgMinPt_h1_h2_m", (nameIn+";m_{H}^{lead} [GeV];m_{H}^{subl} [GeV]").c_str(), nMHBins, mHMin, mHMax, nMHBins, mHMin, mHMax); 
	h_avgNJets_h1_h2_m = createPlot("avgNJets_h1_h2_m", (nameIn+";m_{H}^{lead} [GeV];m_{H}^{subl} [GeV]").c_str(), nMHBins, mHMin, mHMax, nMHBins, mHMin, mHMax); 
	h_hlt_2b35_2j35_h1_h2_m = createPlot("hlt_2b35_2j35_h1_h2_m", (nameIn+";m_{H}^{lead} [GeV];m_{H}^{subl} [GeV]").c_str(), nMHBins, mHMin, mHMax, nMHBins, mHMin, mHMax); 
	h_hlt_b75_3j75_h1_h2_m = createPlot("hlt_b75_3j75_h1_h2_m", (nameIn+";m_{H}^{lead} [GeV];m_{H}^{subl} [GeV]").c_str(), nMHBins, mHMin, mHMax, nMHBins, mHMin, mHMax); 
	h_hlt_j100_2b55_h1_h2_m = createPlot("hlt_j100_2b55_h1_h2_m", (nameIn+";m_{H}^{lead} [GeV];m_{H}^{subl} [GeV]").c_str(), nMHBins, mHMin, mHMax, nMHBins, mHMin, mHMax); 
	h_hlt_b225_h1_h2_m = createPlot("hlt_b225_h1_h2_m", (nameIn+";m_{H}^{lead} [GeV];m_{H}^{subl} [GeV]").c_str(), nMHBins, mHMin, mHMax, nMHBins, mHMin, mHMax); 
	h_hlt_j380_h1_h2_m = createPlot("hlt_j380_h1_h2_m", (nameIn+";m_{H}^{lead} [GeV];m_{H}^{subl} [GeV]").c_str(), nMHBins, mHMin, mHMax, nMHBins, mHMin, mHMax); 
	h_hlt_b80_xe60_h1_h2_m = createPlot("hlt_b80_xe60_h1_h2_m", (nameIn+";m_{H}^{lead} [GeV];m_{H}^{subl} [GeV]").c_str(), nMHBins, mHMin, mHMax, nMHBins, mHMin, mHMax); 
}
void SimpleHistSet::fillPlots(const SimpleEvent& event, const float& weight)
{
	h_count->Fill(1, weight);
	h_hh_M->Fill(event.hh_m, weight);
	h_extraMuon1_mt->Fill(event.extraMuon1_mt, weight);
	h_weight->Fill(weight);
	if(weight < 0)
	{
		d_neg_hh_M.push_back(event.hh_m);
		d_neg_MEt.push_back(event.met);
		d_neg_weight.push_back(weight);
	}else{
		d_pos_hh_M.push_back(event.hh_m);
		d_pos_MEt.push_back(event.met);
		d_pos_weight.push_back(weight);
	}
	h_hh_flavourSum->Fill(event.h1_j1_flav + event.h1_j2_flav + event.h2_j1_flav + event.h2_j2_flav, weight);
	h_h1_h2_flavourSum->Fill(event.h1_j1_flav + event.h1_j2_flav, event.h2_j1_flav + event.h2_j2_flav, weight);
	h_MEt->Fill(event.met, weight);
	h_Xhh->Fill(event.xhh, weight);

	h_h1_h2_m->Fill(event.h1_m, event.h2_m, weight);
	if(event.hlt_2b35_2j35) h_hlt_2b35_2j35_h1_h2_m->Fill(event.h1_m, event.h2_m, weight);
	if(event.hlt_b75_3j75) h_hlt_b75_3j75_h1_h2_m->Fill(event.h1_m, event.h2_m, weight);
	if(event.hlt_j100_2b55) h_hlt_j100_2b55_h1_h2_m->Fill(event.h1_m, event.h2_m, weight);
	if(event.hlt_b225) h_hlt_b225_h1_h2_m->Fill(event.h1_m, event.h2_m, weight);
	if(event.hlt_j380) h_hlt_j380_h1_h2_m->Fill(event.h1_m, event.h2_m, weight);
	if(event.hlt_b80_xe60) h_hlt_b80_xe60_h1_h2_m->Fill(event.h1_m, event.h2_m, weight);
	h_avgNJets_h1_h2_m->Fill(event.h1_m, event.h2_m, weight*event.jet_n);
	if(event.h1_j2_pt > event.h2_j2_pt) h_avgMinPt_h1_h2_m->Fill(event.h1_m, event.h2_m, weight*event.h2_j2_pt);
	else h_avgMinPt_h1_h2_m->Fill(event.h1_m, event.h2_m, weight*event.h2_j1_pt);
}
void SimpleHistSet::fillPlots(const SimpleEvent& event, const float& weight, const float& weightA, const float& weightB)
{
	fillPlots(event, weight);
	h_count_a->Fill(1, weightA);
	h_count_b->Fill(1, weightB);
	h_hh_M_a->Fill(event.hh_m, weightA);
	h_hh_M_b->Fill(event.hh_m, weightB);
	h_MEt_a->Fill(event.met, weightA);
	h_MEt_b->Fill(event.met, weightB);
	h_Xhh_a->Fill(event.xhh, weightA);
	h_Xhh_b->Fill(event.xhh, weightB);
}
void SimpleHistSet::writePlots()//TFile* f)
{
	//f->cd();
	makeKDEHistogram(h_kde_hh_M, h_hh_M, d_pos_hh_M, d_pos_weight, d_neg_hh_M, d_neg_weight);
	makeKDEHistogram(h_kde_MEt, h_MEt, d_pos_MEt, d_pos_weight, d_neg_MEt, d_neg_weight);
	zeroNegativeBins();
	for(auto pair: mapHists)
	{
//		if(pair.second->GetEntries() == 0) continue;
		pair.second->Write();
	}
}

void SimpleHistSet::makeKDEHistogram(std::shared_ptr<TH1D> h, std::shared_ptr<TH1D> hOrig,
					const std::vector<double>& posVar, std::vector<double>& posWeight, 
					const std::vector<double>& negVar, std::vector<double>& negWeight)
{
	std::cout<<"makeKDEHistogram("<< h->GetName() <<", "<< posVar.size() <<", "<< posWeight.size() 
									<<", "<< negVar.size() <<", "<< negWeight.size() <<")"<< std::endl;
	if(posVar.size() == 0) return; //No weights, so cannot construct KDE.

	float sumPosWeights = tameWeights(posWeight);
	float sumNegWeights = tameWeights(negWeight);

	TKDE* kde_pos_hhM = new TKDE(posVar.size(), &posVar.front(), &posWeight.front(), 
									h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
	TGraphErrors* gKDE_pos = kde_pos_hhM->GetGraphWithErrors(h->GetNbinsX(), h->GetXaxis()->GetXmin()+0.5*h->GetBinWidth(1),
															h->GetXaxis()->GetXmax()+0.5*h->GetBinWidth(1));
	TKDE* kde_neg_hhM = nullptr;
	TGraphErrors* gKDE_neg = nullptr;
	if(negVar.size() > 1)
	{
		std::cout<<"makeKDEHistogram: now considering negative weight events"<< std::endl;
		kde_neg_hhM = new TKDE(negVar.size(), &negVar.front(), &negWeight.front(), 
									h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
		gKDE_neg = kde_neg_hhM->GetGraphWithErrors(h->GetNbinsX(), h->GetXaxis()->GetXmin()+0.5*h->GetBinWidth(1),
															h->GetXaxis()->GetXmax()+0.5*h->GetBinWidth(1));
	}
	float totalKDEUnc = 0;
	for(int b = 0; b < h->GetNbinsX(); ++b)	
	{
		float content = sumPosWeights*gKDE_pos->GetY()[b];
		float error = sumPosWeights*gKDE_pos->GetErrorY(b);
		if(gKDE_neg) 
		{
			content += sumNegWeights*gKDE_neg->GetY()[b];
			error *= error;	
			error += sumNegWeights*sumNegWeights*gKDE_neg->GetErrorY(b)*gKDE_neg->GetErrorY(b);
			error = sqrt(error);
		}
		content *= h->GetBinWidth(1);
		error *= h->GetBinWidth(1);
		totalKDEUnc += error*error;
		h->SetBinContent(b+1, content);
		h->SetBinError(b+1, error);
	}
/*	float errScale = calculateTotalUnc(hOrig)/sqrt(totalKDEUnc);
	for(int b = 0; b < h->GetNbinsX(); ++b)	
	{
		h->SetBinError(b+1, errScale*h->GetBinError(b+1));
	}*/
	std::cout<<"makeKDEHistogram: ends."<<std::endl;
}
void SimpleHistSet::zeroNegativeBins()
{
	for(auto pair: mapHists)
	{
		for(int i = 0; i < pair.second->GetNbinsX()+1; ++i)
		{
			if(pair.second->GetBinContent(i) < 0.) 
			{
				pair.second->SetBinContent(i, 0.);
				pair.second->SetBinError(i, 0.);
			}
		}
	}
}
double SimpleHistSet::tameWeights(std::vector<double>& weights)
{
	double averageWeight = 0.;
	for(auto weight: weights) 
	{
		averageWeight += weight; 
	}
	averageWeight /= weights.size();
	double ssd = 0.;
	for(auto weight: weights) 
	{
		ssd += (weight-averageWeight)*(weight-averageWeight); 
	}
	ssd /= weights.size()-1;
	ssd = sqrt(ssd);
	for(unsigned int  i = 0; i < weights.size(); ++i)
	{
		if(fabs(weights[i]) > averageWeight + 3*ssd) 
		{
			float diff = weights[i] - averageWeight;
			weights[i] = averageWeight;
			averageWeight -= diff/weights.size();
		}
	}
	return averageWeight*weights.size();
}
float SimpleHistSet::calculateTotalUnc(std::shared_ptr<TH1D> h)
{
	float totalUnc = 0;
	for(int i = 0; i < h->GetNbinsX(); ++i)
	{
		totalUnc += h->GetBinError(i)*h->GetBinError(i);
	}
	return sqrt(totalUnc);
}
double SimpleHistSet::sumWeights(std::vector<double>& weights)
{
	double sumWeights = 0;
	for(auto weight: weights) 
	{
		sumWeights += weight; 
//		std::cout<<"sumWeights: "<< sumWeights << std::endl;
	}
	std::cout<<"sumWeights( size = "<< weights.size() <<"): found sum of weights = "<< sumWeights << std::endl;
	return sumWeights;
}

double SimpleHistSetCollection::sumWeights(const std::string& region)
{
	std::map<std::string, std::shared_ptr<SimpleHistSet> >::iterator it = mapSimpleHistSets.find(region);
	if(it == mapSimpleHistSets.end())
	{
		std::cout <<"SimpleHistSetCollection::sumWeights: could not find SimpleHistSet for region "<< region << std::endl;
		return -99;
	}else
	{
		return it->second->sumWeights();
	}	
}
