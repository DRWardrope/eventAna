/**************************************************************************************************
*
* TriggerBits:
*	Class to hold trigger information efficiently for storage in TTree
*	David Wardrope, 6th December 2016
**************************************************************************************************/


#ifndef TRIGGERBITS_H
#define TRIGGERBITS_H
#include <iostream>
#include <bitset>
#include <map>
#include <vector>
class TriggerBits
{
	public:
		TriggerBits(): m_trigNames()
		{
			m_bits.reset(); //sets all bits to 0
		}
		TriggerBits(std::vector<std::string> trigList): m_trigNames(trigList)
		{
			m_bits.reset(); //sets all bits to 0
		}
		TriggerBits(std::map<std::string, bool> trigList);
		std::vector<std::string> names() { return m_trigNames; };
		std::string pathsTriggered(int) const;
		void set(int id, bool dec) { m_bits[id] = dec; }
		void setBits(const std::bitset<32>& bits){ m_bits = bits; }
		void setBits(const unsigned long& bits){ m_bits = bits; }
		bool triggered() const { return m_bits.any(); }
		bool triggered(const std::string& path) const;
		
		unsigned long to_ulong() const { return m_bits.to_ulong(); }
	private:
		std::vector<std::string> m_trigNames;
		std::bitset<32> m_bits;
};
#endif
