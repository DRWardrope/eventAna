/***********************************************************************************************
*	ParticleProducer
*	A class to create Particle objects from AktNtuple contents and store in Event.h
*
*	David Wardrope, 29th November 2012
*
***********************************************************************************************/
#ifndef PARTICLEPRODUCER_H
#define PARTICLEPRODUCER_H

//#include "ProducerBase.h"
#include "Event.h"
#include "Particle.h"
#include <stdlib.h> 

extern bool m_debug;
//extern float m_minBWeight;
class ParticleProducer 
{
	public:
        ParticleProducer(Event& evt): m_evt(evt)
		{ 
			if(m_debug) std::cout<<"ParticleProducer()"<< std::endl;
			setupBranches(); 
		}
        ~ParticleProducer(){
        }
		std::vector<Particle> produce(int selPdgId=0);
		std::vector<Particle> produceByIndex(std::vector<int> indices);
	private:
		void cleanUp();
		void fillArrays();
		void nullPtrs();
		void setupBranches();

		Event& m_evt;
		
		//TTree variables
		std::vector<int> *m_part_pdgId;
		std::vector<float> *m_part_pt, *m_part_eta, *m_part_phi, *m_part_m;
};
#endif
