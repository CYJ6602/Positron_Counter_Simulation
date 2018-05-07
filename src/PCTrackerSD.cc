
#include "PCTrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"



PCTrackerSD::PCTrackerSD(G4String name) 
  : G4VSensitiveDetector(name),fHitsCollection(0)
    //fHitsCollection2(0),fHitsCollection3(0)
{
  G4String HCname;
  collectionName.insert(HCname="PositronCollection");
  //  collectionName.insert(HCname="PositronCollection2");
  //collectionName.insert(HCname="PositronCollection3");
}


PCTrackerSD::~PCTrackerSD() 
{}


void PCTrackerSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsCollection 
    = new PCTrackerHitsCollection(SensitiveDetectorName, collectionName[0]);

  /*
  fHitsCollection2 
    = new PCTrackerHitsCollection(SensitiveDetectorName, collectionName[1]);
  fHitsCollection3 
    = new PCTrackerHitsCollection(SensitiveDetectorName, collectionName[2]); 
  */
  
  // Add this collection in hce

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );
  
  /*
  
  G4int hcID;
  hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection1 );

  hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[1]);
  hce->AddHitsCollection( hcID, fHitsCollection2 );

  hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[2]);
  hce->AddHitsCollection( hcID, fHitsCollection3 ); 

  */
}


G4bool PCTrackerSD::ProcessHits(G4Step* aStep, 
                                     G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.) return false;
  
  PCTrackerHit* newHit = new PCTrackerHit();
  
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetPhysV(aStep->GetPreStepPoint()->GetTouchableHandle()
		    ->GetVolume());
  newHit->SetTime(aStep->GetPreStepPoint()->GetGlobalTime());

  fHitsCollection->insert( newHit );

 /*

  if(newHit->GetLogicV()->GetName()=="PositronCounter1")
  fHitsCollection1->insert( newHit );

  if(newHit->GetLogicV()->GetName()=="PositronCounter2")
  fHitsCollection2->insert( newHit );

  if(newHit->GetLogicV()->GetName()=="PositronCounter3")
  fHitsCollection3->insert( newHit );

  //newHit->Print();

  */
 
  return true;
}


void PCTrackerSD::EndOfEvent(G4HCofThisEvent*)
{
  // Done on EventAction
  /*

  // Reset the count,time for each event
  G4int PC1count = 0;
  G4double PC1time = 0.;
  //  if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the tracker : " << G4endl;
     for ( G4int i=0 ; i < nofHits ; i++ )
       {
	 PCTrackerHit *PCHit = (*fHitsCollection)[nofHits-1-i];
	 if(PCHit->GetLogicV()->GetName()=="Alelectrode")
	   {
	     PC1count=1;
	     PC1time=PCHit->GetTime();
	   }
       }
     G4cout << G4endl
	    << "PC1count = " << PC1count <<G4endl;
  //  }

  */
}

