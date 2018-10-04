/***********************************************************************************************
*	ElectronProducer
*	A class to create Jets
*
*	David Wardrope, 21st April 2017
*
***********************************************************************************************/
#ifndef ELECTRONPRODUCER_H
#define ELECTRONPRODUCER_H

#include "Event.h"
#include "Electron.h"
#include <stdlib.h> 

extern bool m_debug;
class ElectronProducer 
{
	public:
        ElectronProducer(Event& evt):m_evt(evt)
		{ 
			setupBranches(); 
		}
        ~ElectronProducer(){}
		std::vector<Electron> produce(const float& ptMin = -99., bool reqPassMedium=false);
	private:
		void cleanUp();
		void fillArrays();
		void nullPtrs();
		void setupBranches();

		Event& m_evt;
		std::vector<float> *m_electron_pt, *m_electron_eta, *m_electron_phi, *m_electron_E;
		std::vector<int> *m_electron_likelihood, *m_electron_isolated;
};
#endif
