#ifndef VLQCANDIDATE_H
#define VLQCANDIDATE_H
/*************************************************************************************************
 *
 * VLQCandidate: class to represent VLQ candidates
 * Holds copies of the constituent jet and constituent Higgs Candidate
 * Marco Montella mindlessly building on top of David Wardrope, November 2017
 *
 *************************************************************************************************/
#include <array>
#include <memory>
#include <bitset>
#include <stdio.h>
#include <iostream>
#include "Jet.h"
#include "HiggsCandidate.h"
#include "TRandom3.h"

class VLQCandidate: public Particle
{
	public:
		VLQCandidate(): Particle(-99), m_higgs(HiggsCandidate()), m_jet(Jet()), m_dRbH(-99){};

		VLQCandidate(const Jet& jet, const HiggsCandidate& higgs);

		float deltaR(const Particle&) const;
		float deltaRbH() const { return m_dRbH; };
		bool fullyBTagged() const { return m_fullyBTagged; };
		const HiggsCandidate* getHiggs() const{ return &m_higgs; };
		const Jet*		getJet() const { return &m_jet; };
		const Jet&		getJetRef() const { return m_jet; };
		int 			getJetIndex() const { return this->getJet()->index(); };
		void			setDRbH(float dRbH) { m_dRbH = dRbH; };
		bool			sharesJet(const VLQCandidate&) const;
		bool			sharesHiggs(const VLQCandidate&) const;

	protected:
		Jet m_jet;
		HiggsCandidate m_higgs;
		float m_dRbH;

		bool m_fullyBTagged, m_looselyBTagged;

};
#endif
