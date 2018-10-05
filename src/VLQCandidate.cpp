#include "VLQCandidate.h"
#include "TMath.h"

VLQCandidate::VLQCandidate(const Jet& jet, const HiggsCandidate& higgs): m_jet(jet), m_higgs(higgs), m_dRbH(-99)
{
  /// Setting Coordinates of VLQ 4-vector ///
  ROOT::Math::PtEtaPhiEVector VLQ_decay(jet + higgs);
  this->SetCoordinates(VLQ_decay.Pt(), VLQ_decay.Eta(), VLQ_decay.Phi(), VLQ_decay.E());
  m_dRbH = jet.deltaR(higgs);

  // setting the BTagging Status
  m_fullyBTagged = higgs.fullyBTagged() && jet.isBTagged();
}

float VLQCandidate::deltaR(const Particle& that) const
{
  float dPhi = this->deltaPhi(that);
  float dEta = this->Eta()-that.Eta();
  float deltaR = sqrt(dEta*dEta+dPhi*dPhi);

  return deltaR;
}

bool VLQCandidate::sharesJet(const VLQCandidate& other) const
{
  if (this->getJet()->index() == other.getJet()->index()) return true;
  else return false;
}

bool VLQCandidate::sharesHiggs(const VLQCandidate& other) const
{
  if (this->getHiggs()->index() == other.getHiggs()->index()) return true;
  else return false;
}
