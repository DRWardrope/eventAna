#include "TriggerBits.h"

TriggerBits::TriggerBits(std::map<std::string, bool> trigList)
{
	m_bits.reset(); //sets all bits to 0/
	int i = 0;
	for(auto trig: trigList)
	{
		m_trigNames.push_back(trig.first);
		m_bits.set(i, trig.second);
		++i;	
	}
}
std::string TriggerBits::pathsTriggered(int intBits) const
{
	std::bitset<32> bits(intBits);
	std::string path;
	for(int i = 0; i < 32; ++i)
	{
		if(bits.test(i)) //Has this trigger fired?
		{
			if(m_trigNames.size() < i) //Has this trigger been named?
			{
				if(!path.empty()) path.append(" && ");
				path.append(m_trigNames[i]); 
			}
		}
	}
	return path;
}
bool TriggerBits::triggered(const std::string& path) const
{
	for(unsigned int i = 0; i < m_trigNames.size(); ++i)
	{
		if(path == m_trigNames.at(i))
		{
			return m_bits[i];
		}
	}
	std::cout<<"TriggerBits::triggered: path "<< path <<" was not found. Return false."<< std::endl;
	return false;
}
