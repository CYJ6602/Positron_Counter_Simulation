
#ifndef MCTrackerSD_h
#define MCTrackerSD_h 1

#include "G4VSensitiveDetector.hh"

#include "MCTrackerHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class MCTrackerSD : public G4VSensitiveDetector
{
  public:
    MCTrackerSD(G4String name);
    virtual ~MCTrackerSD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
  MCTrackerHitsCollection* fHitsCollection;
 
};


#endif
