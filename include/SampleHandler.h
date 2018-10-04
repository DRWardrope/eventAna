#ifndef SAMPLEHANDLER_H
#define SAMPLEHANDLER_H
#include <map>
#include <string>
struct SampleDetails
{
	SampleDetails() {};
	SampleDetails(const int& dsidIn, const float& totalWeightIn, const float& xsecIn, const std::string& nameIn): dsid(dsidIn), totalWeight(totalWeightIn), xsec(xsecIn), name(nameIn) {};
	SampleDetails(const int& dsidIn, const float& totalWeightIn, const float& xsecIn, const int& massIn, const std::string& nameIn):
					 dsid(dsidIn), totalWeight(totalWeightIn), xsec(xsecIn), mass(massIn), name(nameIn) {};
	int dsid = -99;
	float totalWeight = -99.;
	float xsec = -99;
	int mass = -99;
	std::string name = "";
};
class SampleHandler
{
	public:
		SampleHandler();
		SampleDetails operator[](int dsid) { return m_sampleList[dsid]; }		
		std::map<int, SampleDetails>::const_iterator begin() const { return m_sampleList.begin(); }
		std::map<int, SampleDetails>::const_iterator end() const { return m_sampleList.end(); }
		bool contains(const int& dsid) const { return m_sampleList.find(dsid) != m_sampleList.end(); }
		bool isSignal(const int& dsid) const;
	private:
		std::map<int, SampleDetails> m_sampleList;	
};
#endif
