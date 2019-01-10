#ifndef PCDetectorConstruction_h
#define PCDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"
#include "G4VisAttributes.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

class PCDetectorMessenger;

class PCDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    PCDetectorConstruction();
    virtual ~PCDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  G4LogicalVolume* GetColl() const { return fColl; }
  G4LogicalVolume* GetTube() const { return fTube; }
  G4LogicalVolume* GetCham() const { return fCham; }

   // Set methods

  void SetDefaults();

  void SetMirrorOn(G4bool );
  G4bool GetMirrorOn(){return fMirrorOn;}

  void SetStopperOn(G4bool );
  G4bool GetStopperOn(){return fStopperOn;}

  void SetAngle(G4double );
  G4double GetAngle(){return fAngle;}

  void SetThickness(G4double );
  G4double GetThickness(){return fThickness;}

  void SetPCPosition(G4bool ); // set far(0) or close(1) configuration
  G4bool GetPCPosition(){return fPCPosition;}

  void SetCsIType(G4bool );
  G4bool GetCsIType(){return fCsIType;}
   
  void SetDistToAxisX(G4double );
  void SetDistToAxisY(G4double );
  void SetDistToPC_Offset(G4double );

  G4double DistToAxisX, DistToAxisY, DistToPC_Offset;
  
private:

  void VisAttributes();
  
  G4VPhysicalVolume* ConstructDetector();

  G4LogicalVolume** fPC_logic;
  G4LogicalVolume* fTarget;
  G4LogicalVolume* world_logic;
  G4LogicalVolume* chamber_logic;
  G4LogicalVolume* innerVacuum_logic;
  G4LogicalVolume* chamber_hole1_logic;
  G4LogicalVolume* chamber_hole2_logic;
  G4LogicalVolume* chamber_hole3_logic;
  G4LogicalVolume* chamber_inner_cap_tube_logic;
  G4LogicalVolume* chamber_cap_tube_logic;
  G4LogicalVolume* chamber_cap1_logic;
  G4LogicalVolume* chamber_cap2_logic;
  G4LogicalVolume* chamber_cap3_logic;
  G4LogicalVolume* collimator_logic;
  G4LogicalVolume* colVacuum1_logic;
  G4LogicalVolume* colVacuum2_logic;
  G4LogicalVolume* colLead1_logic;
  G4LogicalVolume* colLead2_logic;
  G4LogicalVolume* beamtube11_logic;
  G4LogicalVolume* beamtube12_logic;
  G4LogicalVolume* beamtube2_logic;
  G4LogicalVolume* beamtube3_logic;
  G4LogicalVolume* beamtube41_logic;
  G4LogicalVolume* beamtube42_logic;
  G4LogicalVolume* beamtube43_logic;
  G4LogicalVolume* beamtube5_logic;
  G4LogicalVolume* beamtube6_logic;
  G4LogicalVolume* beamtubeVacuum11_logic;
  G4LogicalVolume* beamtubeVacuum12_logic;
  G4LogicalVolume* beamtubeVacuum2_logic;
  G4LogicalVolume* beamtubeVacuum3_logic;
  G4LogicalVolume* beamtubeVacuum41_logic;
  G4LogicalVolume* beamtubeVacuum42_logic;
  G4LogicalVolume* beamtubeVacuum43_logic;
  G4LogicalVolume* beamtubeVacuum5_logic;
  G4LogicalVolume* LeadShield_logic;
  G4LogicalVolume* Mirror_logic;
  G4LogicalVolume* AlStopper_logic;


  /// New beam-line description.
  G4LogicalVolume* tub1_logic;
  G4LogicalVolume* tub1_vacuum_logic;
  G4LogicalVolume* tub2_logic;
  G4LogicalVolume* tub2_vacuum_logic;
  G4LogicalVolume* tub3_logic;
  G4LogicalVolume* tub3_vacuum_logic;
  G4LogicalVolume* tub4_logic;
  G4LogicalVolume* tub4_vacuum_logic;
  G4LogicalVolume* tub5_logic;
  G4LogicalVolume* tub5_vacuum_logic;
  G4LogicalVolume* tub6_logic;
  G4LogicalVolume* tub6_vacuum_logic;
  G4LogicalVolume* tub7_logic;
  G4LogicalVolume* tub7_vacuum_logic;
  G4LogicalVolume* tub8_logic;
  G4LogicalVolume* tub8_vacuum_logic;
  G4LogicalVolume* tub9_logic;
  G4LogicalVolume* tub9_vacuum_logic;
  G4LogicalVolume* tub10_logic;
  G4LogicalVolume* tub10_vacuum_logic;
  G4LogicalVolume* tub11_logic;
  G4LogicalVolume* tub11_vacuum_logic;
  G4LogicalVolume* tub12_logic;
  G4LogicalVolume* tub12_vacuum_logic;
  G4LogicalVolume* tub13_logic;
  G4LogicalVolume* tub13_vacuum_logic;
  G4LogicalVolume* tub14_logic;
  G4LogicalVolume* tub14_vacuum_logic;
  G4LogicalVolume* tub15_logic;
  G4LogicalVolume* tub15_vacuum_logic;
  G4LogicalVolume* tub16_logic;
  G4LogicalVolume* tub16_vacuum_logic;
  G4LogicalVolume* tub17_logic;
  G4LogicalVolume* tub17_vacuum_logic;
  G4LogicalVolume* tub18_logic;
  G4LogicalVolume* tub18_vacuum_logic;
  G4LogicalVolume* vacChamEnter1_logic;
  G4LogicalVolume* vacChamEnter2_logic;
  G4LogicalVolume* vacChamEnter2vacuum_logic;
  G4LogicalVolume* tub20_logic;
  G4LogicalVolume* tub20_vacuum_logic;
  G4LogicalVolume* tub21a_logic;
  G4LogicalVolume* tub21a_vacuum_logic;
  G4LogicalVolume* tub21b_logic;
  G4LogicalVolume* tub21c_logic;
  G4LogicalVolume* tub22_logic;
  G4LogicalVolume* leadCol16_logic;
  G4LogicalVolume* leadCol12_logic;
  ///

  /// Target support description.
  G4LogicalVolume* ring_opravka_target_logic;
  G4LogicalVolume* ring_opravka_mirror_logic;
  G4LogicalVolume* target_support_ring_logic;
  G4LogicalVolume* target_support_ring_p1_logic;
  G4LogicalVolume* target_support_ring_p2_logic;
  G4LogicalVolume* target_support_ring_p3_logic;
  G4LogicalVolume* mirror_support_ring_logic;

  G4LogicalVolume* support_rings_table_logic;
  G4LogicalVolume* support_rings_rigidity_logic;
  G4LogicalVolume* bpm_base_logic;
  ///
  
  G4double fEndofTube;
  G4bool fMirrorOn;
  G4bool fStopperOn;
  G4bool fPCPosition;
  G4bool fCsIType;
  G4double fThickness;
  G4double fAngle;

  PCDetectorMessenger* fMessenger ;

protected:

  G4LogicalVolume* fTube;
  G4LogicalVolume* fColl;
  G4LogicalVolume* fCham;

  
  //  G4LogicalVolume* PC_logic1;
  //G4LogicalVolume* PC_logic2;
  //G4LogicalVolume* PC_logic3;
    
 
};


#endif

