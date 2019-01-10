
#include "PCDetectorConstruction.hh"
#include "PCDetectorMessenger.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4VisAttributes.hh"

#include "MCTrackerSD.hh"
#include "PCTrackerSD.hh"
#include "G4SDManager.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleWithEnergyFilter.hh"


PCDetectorConstruction::PCDetectorConstruction()
  : G4VUserDetectorConstruction(),
    fPC_logic(NULL),
    fTube(NULL), fColl(NULL), fCham(NULL)
{
  world_logic = NULL;
  fTarget = NULL;  
  chamber_logic = NULL;
  innerVacuum_logic= NULL;
  chamber_hole1_logic= NULL;
  chamber_hole2_logic= NULL;
  chamber_hole3_logic= NULL;
  chamber_inner_cap_tube_logic= NULL;
  chamber_cap_tube_logic= NULL;
  chamber_cap1_logic= NULL;
  chamber_cap2_logic= NULL;
  chamber_cap3_logic = NULL;
  collimator_logic= NULL;
  colVacuum1_logic= NULL;
  colVacuum2_logic= NULL;
  colLead1_logic= NULL;
  colLead2_logic= NULL;
  beamtube11_logic= NULL;
  beamtube12_logic= NULL;
  beamtube2_logic= NULL;
  beamtube3_logic= NULL;
  beamtube41_logic= NULL;
  beamtube42_logic= NULL;
  beamtube43_logic= NULL;
  beamtube5_logic= NULL;
  beamtube6_logic= NULL;
  beamtubeVacuum11_logic= NULL;
  beamtubeVacuum12_logic= NULL;
  beamtubeVacuum2_logic= NULL;
  beamtubeVacuum3_logic= NULL;
  beamtubeVacuum41_logic= NULL;
  beamtubeVacuum42_logic= NULL;
  beamtubeVacuum43_logic= NULL;
  beamtubeVacuum5_logic= NULL;
  Mirror_logic= NULL;
  AlStopper_logic= NULL;
  
  SetDefaults();
  
  fMessenger = new PCDetectorMessenger(this);
  
  fPC_logic = new G4LogicalVolume*[3];
}


PCDetectorConstruction::~PCDetectorConstruction()
{
  delete [] fPC_logic ;
  delete fTarget ;
  delete fMessenger;
  delete fTube;
  delete fColl;
  delete fCham;
}


G4VPhysicalVolume* PCDetectorConstruction::Construct()
{
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //Define materials
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  G4String name,symbol;
  G4double a,z,density,pressure,temperature,fraction;
  G4int ncomp,natoms;
  a= 12.01*g/mole;
  G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);
  a= 18.998*g/mole;
  G4Element* elN = new G4Element(name="Nitrogen", symbol="N", z=7., a);
  a= 1.01*g/mole;
  G4Element* elH = new G4Element(name="Hydrogen", symbol="H", z=1., a);
  a= 16.00*g/mole;
  G4Element* elO = new G4Element(name="Oxygen", symbol="O", z=8., a);

  G4Material* Al = nist->FindOrBuildMaterial("G4_Al");
  // G4Material* Al = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  // G4Material* Al2O3 = nist->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  
  a= 26.98*g/mole;
  G4Element* elAl = new G4Element(name="Aluminum",symbol="Al",z=13.,a);
  G4Material* Air = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* Pb = nist->FindOrBuildMaterial("G4_Pb");
  G4Material* CsI = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  G4Material* Tl = nist->FindOrBuildMaterial("G4_Tl");
   
  //  pressure = 0.0001*atmosphere ;
  //  density = 0.0012*g/cm3;
  density = 2.376e-15*g/cm3;
  pressure = 2.0e-7*bar;
  temperature = 288.*kelvin;
  G4Material* vacuum = new G4Material(name="vacuum", density, ncomp=1,
				      kStateGas,temperature,pressure);
  vacuum->AddMaterial(Air,fraction=100*perCent);
 
  density = 1.39*g/cm3;
  G4Material* Mylar = new G4Material(name="Mylar", density, ncomp=3);
  Mylar->AddElement(elO,2);
  Mylar->AddElement(elC,5);
  Mylar->AddElement(elH,4);
    
  density = 2.7*g/cm3;
  G4Material* Albuild = new G4Material(name="Albuild",density,ncomp=1);
  Albuild->AddElement(elAl, natoms=1);

  density = 4.51*g/cm3 ;
  G4Material* targetMaterial = new G4Material(name="CsI_Tl", density, ncomp = 2);
  targetMaterial->AddMaterial(CsI,fraction=99.999*perCent);
  targetMaterial->AddMaterial(Tl,fraction=0.001*perCent);

  G4Material* Fe = nist->FindOrBuildMaterial("G4_Fe");
  G4Material* Cr = nist->FindOrBuildMaterial("G4_Cr");
  G4Material* Ni = nist->FindOrBuildMaterial("G4_Ni");
  G4Material* Mn = nist->FindOrBuildMaterial("G4_Mn");
  G4Material* Si = nist->FindOrBuildMaterial("G4_Si");
  G4Material* N  = nist->FindOrBuildMaterial("G4_N" );
  G4Material* C  = nist->FindOrBuildMaterial("G4_C" );
  G4Material* P  = nist->FindOrBuildMaterial("G4_P" );
  G4Material* S  = nist->FindOrBuildMaterial("G4_S" );

  // G4double density;
  G4int ncomponents;
  // G4double fraction;
  /// https://www.makeitfrom.com/material-properties/AISI-304-S30400-Stainless-Steel
  density=7.8*g/cm3;
  G4Material* S30400 = new G4Material(name="S30400", density, ncomp=9);
  S30400->AddMaterial(Fe, fraction=70.25*perCent );
  S30400->AddMaterial(Cr, fraction=19.*perCent   );
  S30400->AddMaterial(Ni, fraction= 9.25*perCent );
  S30400->AddMaterial(Mn, fraction= 1.*perCent   );
  S30400->AddMaterial(Si, fraction= 0.375*perCent);
  S30400->AddMaterial(N,  fraction= 0.05*perCent );
  S30400->AddMaterial(C,  fraction= 0.04*perCent );
  S30400->AddMaterial(P,  fraction= 0.02*perCent );
  S30400->AddMaterial(S,  fraction= 0.015*perCent);

  density = 1.032*g/cm3;
  G4Material* Polystyrene = new G4Material(name="Polystyrene", density, ncomp=2);
  Polystyrene->AddElement(elC, 19);
  Polystyrene->AddElement(elH, 21);
   
  // Option to switch on/off checking of volumes overlaps
 
  G4bool checkOverlaps = true; // it's not important
 
  // World
  G4double worldX  = 100.*cm, worldY = 300.*cm, worldZ = 250.*cm;
  
  G4Box* world_solid = new G4Box("World", 0.5*worldX, 0.5*worldY, 0.5*worldZ);      
  world_logic =  new G4LogicalVolume(world_solid, Air,"World");                                    
  G4VPhysicalVolume* world_physics = new G4PVPlacement(0, G4ThreeVector(), world_logic, "World", 0, false, 0, checkOverlaps);      
    
  // define vacuum chamber
  G4double chamberX = 288.*mm, chamberY = 365*mm, chamberZ = 310*mm;
  G4double gapX = 15.*mm, gapY = 15.*mm, gapZ = 15.*mm ;
  G4double innerchamberX = chamberX-2.*gapX, innerchamberY = chamberY-2.*gapY, innerchamberZ = chamberZ-2.*gapZ;
  
  G4VSolid* chamber_solid = new G4Box("Vacuum_Chamber", 0.5*chamberX, 0.5*chamberY, 0.5*chamberZ);
  /// chamber_logic = new G4LogicalVolume(chamber_solid, Al, "Vacuum_Chamber");
  chamber_logic = new G4LogicalVolume(chamber_solid, S30400, "Vacuum_Chamber");

  fCham = chamber_logic;
  
  G4VSolid* innerVacuum_solid = new G4Box("Inner_Vacuum", 0.5*innerchamberX, 0.5*innerchamberY, 0.5*innerchamberZ);
  innerVacuum_logic = new G4LogicalVolume(innerVacuum_solid, vacuum, "Inner_Vacuum");

  G4double hole1_Radius = 50.*mm, hole1_Z = gapY+0.1*mm ; // hole for collimator
  G4double hole2_Radius = 75.*mm, hole2_Z = gapY+0.1*mm ; // hole for opposite side of collimator
  G4double hole3_Radius = 50.*mm, hole3_Z = gapZ+0.1*mm ; // hole for direction to Positron Counters

  G4VSolid* chamber_hole1_solid = new G4Tubs("Chamber_Hole1", 0., hole1_Radius, 0.5*hole1_Z, 0., 360.*degree);
  G4VSolid* chamber_hole2_solid = new G4Tubs("Chamber_Hole2", 0., hole2_Radius, 0.5*hole2_Z, 0., 360.*degree);
  G4VSolid* chamber_hole3_solid = new G4Tubs("Chamber_Hole3", 0., hole3_Radius, 0.5*hole3_Z, 0., 360.*degree);  

  chamber_hole1_logic = new G4LogicalVolume(chamber_hole1_solid, vacuum, "Chamber_Hole1");
  chamber_hole2_logic = new G4LogicalVolume(chamber_hole2_solid, vacuum, "Chamber_Hole2");
  chamber_hole3_logic = new G4LogicalVolume(chamber_hole3_solid, vacuum, "Chamber_Hole3");
  
  //  G4double hole2_RadiusMin = 60.*mm, hole2_RadiusMax = 75.*cm, hole2_Z = 96.*mm ; // Al_hole at the end
  
  G4double cap_Radius = 100.*mm, cap_Z = 21.*mm ;
  G4double cap_tube_Radius = hole2_Radius - 15.*mm, cap_tube_Z = 96.* mm;
  
  G4VSolid* chamber_cap1_solid = new G4Tubs("Chamber_Cap1", hole2_Radius, cap_Radius, 0.5*(cap_Z+0.001*mm), 0., 360.*degree);
  G4VSolid* chamber_cap2_solid = new G4Tubs("Chamber_Cap2", 0., cap_Radius, 0.5*(cap_Z+0.001*mm), 0., 360.*degree);
  G4VSolid* chamber_cap_tube_solid = new G4Tubs("Chamber_Cap_Tube", cap_tube_Radius, hole2_Radius, 0.5*cap_tube_Z, 0., 360.*degree);
  G4VSolid* chamber_inner_cap_tube_solid = new G4Tubs("Chamber_Inner_Cap_Tube", 0., hole2_Radius, 0.5*(cap_tube_Z+0.001*mm), 0., 360.*degree);

  /// chamber_cap1_logic = new G4LogicalVolume(chamber_cap1_solid, Al ,"Chamber_Cap1") ;
  chamber_cap1_logic = new G4LogicalVolume(chamber_cap1_solid, S30400 ,"Chamber_Cap1") ;
  /// chamber_cap2_logic = new G4LogicalVolume(chamber_cap2_solid, Al ,"Chamber_Cap2") ;
  chamber_cap2_logic = new G4LogicalVolume(chamber_cap2_solid, S30400 ,"Chamber_Cap2") ;
  /// chamber_cap_tube_logic = new G4LogicalVolume(chamber_cap_tube_solid, Al ,"Chamber_Cap_Tube") ;
  chamber_cap_tube_logic = new G4LogicalVolume(chamber_cap_tube_solid, S30400 ,"Chamber_Cap_Tube") ;
  chamber_inner_cap_tube_logic = new G4LogicalVolume(chamber_inner_cap_tube_solid, vacuum ,"Chamber_Inner_Cap_Tube") ;

  G4double chamber_cap3_Radius = hole3_Radius + 15.*mm, chamber_cap3_Z = 15.*mm;
  G4VSolid* chamber_cap3_solid = new G4Tubs("Chamber_Cap3", hole3_Radius, chamber_cap3_Radius, 0.5*chamber_cap3_Z, 0.,360.*degree);
  /// chamber_cap3_logic = new G4LogicalVolume(chamber_cap3_solid, Al, "Chamber_Cap3");
  chamber_cap3_logic = new G4LogicalVolume(chamber_cap3_solid, S30400, "Chamber_Cap3");

  

// define collimator
  G4double colRadius = 34.*mm, colZ = 165.*mm;
  G4double innercolRadius = 30.*mm, innercolZ = 140.*mm; 
  G4double innercolRadius2 = 16.5*mm, innercolZ2 = 25.001*mm;//add 0.001 to escape the layer problem
  G4double col_LeadRadiusMin1 = 6.*mm, col_LeadRadiusMin2 = 8.3*mm, col_LeadZ = 50.*mm ;

  G4VSolid* collimator_solid = new G4Tubs("Collimator", 0., colRadius, 0.5*colZ, 0., 360.*degree);
  /// collimator_logic = new G4LogicalVolume(collimator_solid, Al, "Collimator");
  collimator_logic = new G4LogicalVolume(collimator_solid, S30400, "Collimator");

  fColl = collimator_logic;
  
  G4VSolid* colVacuum1_solid = new G4Tubs("Collimator_Vacuum1", 0., innercolRadius, 0.5*innercolZ, 0., 360.*degree);
  colVacuum1_logic = new G4LogicalVolume(colVacuum1_solid, vacuum, "Collimator_Vacuum1");
  
  G4VSolid* colVacuum2_solid = new G4Tubs("Collimator_Vacuum2", 0., innercolRadius2, 0.5*innercolZ2, 0., 360.*degree);
  colVacuum2_logic = new G4LogicalVolume(colVacuum2_solid, vacuum, "Collimator_Vacuum2");

  G4VSolid* colLead1_solid=new G4Tubs("Collimator_Lead1", col_LeadRadiusMin1, innercolRadius, 0.5*col_LeadZ, 0., 360.*degree);
  colLead1_logic = new G4LogicalVolume(colLead1_solid, Pb, "Collimator_Lead1");
  
  G4VSolid* colLead2_solid=new G4Tubs("Collimator_Lead1", col_LeadRadiusMin2, innercolRadius, 0.5*col_LeadZ, 0., 360.*degree);
  colLead2_logic = new G4LogicalVolume(colLead2_solid, Pb, "Collimator_Lead2");


  //define Muon beam tube from 1 to 6...
  G4double smallestD = 112.5*mm; // second tube Diameter
  
  G4double tubeRadiusMin11 = colRadius, tubeRadiusMax11= 195./2.*mm, tubeZ11= 21.*mm ;
  G4double tubeRadiusMin12 = smallestD/2.*mm, tubeRadiusMax12 = tubeRadiusMax11, tubeZ12=tubeZ11;
  G4double tubeRadiusMin2 = (smallestD/2.-10.)*mm, tubeRadiusMax2= smallestD/2.*mm, tubeZ2= (123.75+21.)*mm; // initially, z2 set to 123.75mm
  G4double tubeRadiusMin3 = tubeRadiusMax2, tubeRadiusMax3= 200./2.*mm, tubeZ3=21.*2.*mm;
  G4double tubeRadiusMin41 = 85.*mm, tubeRadiusMax41= 265./2.*mm, tubeZ41= 83.5*mm; // divide biggest part for three region
  G4double tubeRadiusMin42 = 85.0*mm, tubeRadiusMax42= tubeRadiusMax41, tubeZ42= 25.*mm; // initially set to 156.?? mm
  G4double tubeRadiusMin43 = 130.7*mm, tubeRadiusMax43= tubeRadiusMax41, tubeZ43= (83.5+20.)*mm;
  G4double tubeRadiusMin5 = 100.*mm, tubeRadiusMax5= 261.4/2.*mm, tubeZ5= 1000.*mm; // exposured tubeZ5 = 75.mm... MinRadius is the size of vacuum in the beam line
  G4double tubeRadiusMin6 = tubeRadiusMax5, tubeRadiusMax6= 500.*mm, tubeZ6= 1000.*mm; // MaxRadius is assumed, Pb composed

 

  G4VSolid* beamtube11_solid = new G4Tubs("BeamTube11", 0., tubeRadiusMax11, 0.5*tubeZ11, 0., 360.*degree);
  G4VSolid* beamtubeVacuum11_solid = new G4Tubs("BeamTube11_Vacuum", 0., tubeRadiusMin11, 0.5*tubeZ11, 0., 360.*degree);
  /// beamtube11_logic = new G4LogicalVolume(beamtube11_solid, Al, "BeamTube11");
  beamtube11_logic = new G4LogicalVolume(beamtube11_solid, S30400, "BeamTube11");
  beamtubeVacuum11_logic = new G4LogicalVolume(beamtubeVacuum11_solid, vacuum, "BeamTube11_Vacuum");

  G4VSolid* beamtube12_solid = new G4Tubs("BeamTube12", 0., tubeRadiusMax12, 0.5*tubeZ12, 0., 360.*degree);
  G4VSolid* beamtubeVacuum12_solid = new G4Tubs("BeamTube12_Vacuum", 0., tubeRadiusMin12, 0.5*tubeZ12, 0., 360.*degree);
  /// beamtube12_logic = new G4LogicalVolume(beamtube12_solid, Al, "BeamTube12");
  beamtube12_logic = new G4LogicalVolume(beamtube12_solid, S30400, "BeamTube12");
  beamtubeVacuum12_logic = new G4LogicalVolume(beamtubeVacuum12_solid, vacuum, "BeamTube12_Vacuum");

  G4VSolid* beamtube2_solid = new G4Tubs("BeamTube2", 0., tubeRadiusMax2, 0.5*tubeZ2, 0., 360.*degree);
  G4VSolid* beamtubeVacuum2_solid = new G4Tubs("BeamTube2_Vacuum", 0., tubeRadiusMin2, 0.5*tubeZ2, 0., 360.*degree);
  /// beamtube2_logic = new G4LogicalVolume(beamtube2_solid, Al, "BeamTube2");
  beamtube2_logic = new G4LogicalVolume(beamtube2_solid, S30400, "BeamTube2");
  beamtubeVacuum2_logic = new G4LogicalVolume(beamtubeVacuum2_solid, vacuum, "BeamTube2_Vacuum");

  fTube = beamtube2_logic;

  G4VSolid* beamtube3_solid = new G4Tubs("BeamTube3", 0., tubeRadiusMax3, 0.5*tubeZ3, 0., 360.*degree);
  G4VSolid* beamtubeVacuum3_solid = new G4Tubs("BeamTube3_Vacuum", 0., tubeRadiusMin3, 0.5*tubeZ3, 0., 360.*degree);
  /// beamtube3_logic = new G4LogicalVolume(beamtube3_solid, Al, "BeamTube3");
  beamtube3_logic = new G4LogicalVolume(beamtube3_solid, S30400, "BeamTube3");
  beamtubeVacuum3_logic = new G4LogicalVolume(beamtubeVacuum3_solid, vacuum, "BeamTube3_Vacuum");

  G4VSolid* beamtube41_solid = new G4Tubs("BeamTube41", 0., tubeRadiusMax41, 0.5*tubeZ41, 0., 360.*degree);
  G4VSolid* beamtubeVacuum41_solid = new G4Tubs("BeamTube41_Vacuum", 0., tubeRadiusMin41, 0.5*tubeZ41, 0., 360.*degree);
  /// beamtube41_logic = new G4LogicalVolume(beamtube41_solid, Al, "BeamTube41");
  beamtube41_logic = new G4LogicalVolume(beamtube41_solid, S30400, "BeamTube41");
  beamtubeVacuum41_logic = new G4LogicalVolume(beamtubeVacuum41_solid, vacuum, "BeamTube41_Vacuum");

  G4VSolid* beamtube42_solid = new G4Tubs("BeamTube42", 0., tubeRadiusMax42, 0.5*tubeZ42, 0., 360.*degree);
  G4VSolid* beamtubeVacuum42_solid = new G4Tubs("BeamTube42_Vacuum", 0., tubeRadiusMin42, 0.5*tubeZ42, 0., 360.*degree);
  /// beamtube42_logic = new G4LogicalVolume(beamtube42_solid, Al, "BeamTube42");
  beamtube42_logic = new G4LogicalVolume(beamtube42_solid, S30400, "BeamTube42");
  beamtubeVacuum42_logic = new G4LogicalVolume(beamtubeVacuum42_solid, vacuum, "BeamTube42_Vacuum");

  G4VSolid* beamtube43_solid = new G4Tubs("BeamTube43", 0., tubeRadiusMax43, 0.5*tubeZ43, 0., 360.*degree);
  G4VSolid* beamtubeVacuum43_solid = new G4Tubs("BeamTube43_Vacuum", 0., tubeRadiusMin43, 0.5*tubeZ43, 0., 360.*degree);
  /// beamtube43_logic = new G4LogicalVolume(beamtube43_solid, Al, "BeamTube43");
  beamtube43_logic = new G4LogicalVolume(beamtube43_solid, S30400, "BeamTube43");
  beamtubeVacuum43_logic = new G4LogicalVolume(beamtubeVacuum43_solid, vacuum, "BeamTube43_Vacuum");

  G4VSolid* beamtube5_solid = new G4Tubs("BeamTube5", 0., tubeRadiusMax5, 0.5*tubeZ5, 0., 360.*degree);
  G4VSolid* beamtubeVacuum5_solid = new G4Tubs("BeamTube5_Vacuum", 0., tubeRadiusMin5, 0.5*tubeZ5, 0., 360.*degree);
  /// beamtube5_logic = new G4LogicalVolume(beamtube5_solid, Al, "BeamTube5");
  beamtube5_logic = new G4LogicalVolume(beamtube5_solid, S30400, "BeamTube5");
  beamtubeVacuum5_logic = new G4LogicalVolume(beamtubeVacuum5_solid, vacuum, "BeamTube5_Vacuum");

  G4VSolid* beamtube6_solid = new G4Tubs("BeamTube6", tubeRadiusMin6, tubeRadiusMax6, 0.5*tubeZ6, 0., 360.*degree);
  //  G4VSolid* beamtubeVacuum6_solid = new G4Tubs("BeamTube6_Vacuum", 0., tubeRadiusMin6, 0.5*tubeZ6, 0., 360.*degree);
  beamtube6_logic = new G4LogicalVolume(beamtube6_solid, Pb, "BeamTube6");
  //  G4LogicalVolume* beamtubeVacuum6_logic = new G4LogicalVolume(beamtubeVacuum6_solid, vacuum, "BeamTube6_Vacuum");

  // define targets(CsI(Tl) crystal, foil), Al_stopper, Mirror
  G4double targetRadius, halfThickness;
  G4double crystalXY = 20.*mm;
  
  G4double StopperXY = 200.*mm ; // It's not sure
  G4double StopperZ = 4.*mm ;
  
  G4VSolid* Foil_solid = new G4Tubs("CsI_Tl", 0., targetRadius = 60.*mm, halfThickness = 0.5*fThickness, 0., 360.*degree);
  G4LogicalVolume* Foil_logic = new G4LogicalVolume(Foil_solid, targetMaterial, "CsI_Tl");

  G4VSolid* Crystal_solid = new G4Box("CsI_Tl", 0.5*crystalXY, 0.5*crystalXY, 0.5*fThickness);
  G4LogicalVolume* Crystal_logic = new G4LogicalVolume(Crystal_solid, targetMaterial, "CsI_Tl");

  if(fCsIType == 0)
    fTarget = Foil_logic;
  else
    fTarget = Crystal_logic;

  G4VSolid* Mirror_solid = new G4Tubs("Mirror", 0., targetRadius = 60.*mm, halfThickness = 0.5*2.9*um, 0., 360.*degree);
  Mirror_logic = new G4LogicalVolume(Mirror_solid, Mylar, "Mirror"); // Ignore the Al(50nm)
  
  G4VSolid* AlStopper_solid = new G4Box("Al_Stopper", 0.5*StopperXY, 0.5*StopperXY, 0.5*StopperZ);
  AlStopper_logic = new G4LogicalVolume(AlStopper_solid, Al, "Al_Stopper");

  //define Lead shield
  G4double LeadShieldX = 50.*mm, LeadShieldY = 100.*mm, LeadShieldZ = 200.*mm ;
  G4VSolid* LeadShield_solid = new G4Box("LeadShield", 0.5*LeadShieldX, 0.5*LeadShieldY, 0.5*LeadShieldZ);
  LeadShield_logic = new G4LogicalVolume(LeadShield_solid, Pb, "LeadShield");
   
  //define supporting frame

  G4double AlsupportingX = 30.*mm, AlsupportingY = 10.5/6.1*chamberY, AlsupportingZ= 1600.*mm;
  G4VSolid* Alsupporting_solid = new G4Box("AlSupporting", 0.5*AlsupportingX, 0.5*AlsupportingY, 0.5*AlsupportingZ);
  G4LogicalVolume* Alsupporting_logic = new G4LogicalVolume(Alsupporting_solid, Al, "AlSupporting");
  

  
  // Arrange each part in physical world

  // Order of arrangement is { Chamber -> Collimator -> MuonBeamTube -> Target -> Mirror, Al_Stoppers -> Lead shield }

  // center of Mirror is the center of the world_logic : (0,0,0)


  // set the chamber
  G4double MirrorToTarget = -55.*mm ; // y-direction
  G4double MirrorToChamberCenterZ = -35.*mm ; // z-direction, 120(dist from mirror to side of chamber) - 0.5*chamberZ
  G4double MirrorToChamberCenterX = 25.*mm ; // x-direction
  G4double MirrorToChamberCenterY = (-13.-7.5)*mm ; // y-direction, -7.5mm is added to set the collimator's end is beamtube11's end

  fEndofTube = MirrorToChamberCenterY - 0.5*chamberY - tubeZ11 - tubeZ2 - tubeZ41 - tubeZ5; // Used in PrimaryGenerator
  
  G4double DistToHole1X = -1.*MirrorToChamberCenterX ; // x-direction
  G4double DistToHole1Z = -1.*MirrorToChamberCenterZ ; // z-direction
  G4double DistToHole1Y = -0.5*chamberY + 0.5*hole1_Z ; // y-direction

  G4double DistToHole2X = -1.*MirrorToChamberCenterX ;
  G4double DistToHole2Y = 0.5*chamberY - 0.5*hole2_Z ;
  G4double DistToHole2Z = -23.*mm ; // derive from the picture only
  
  G4double DistToHole3Y = -1.*MirrorToChamberCenterY ; // y-direction
  G4double DistToHole3Z = 0.5*chamberZ -0.5*hole3_Z ; // z-direction
  G4double DistToHole3X = -1.*MirrorToChamberCenterX ; // x-direction, height of hole3 is assumed to be same with Mirror

  G4double MirrorToCapTubeCenterZ = MirrorToChamberCenterZ + DistToHole2Z ;
  G4double MirrorToCapTubeCenterY = MirrorToChamberCenterY + 0.5*chamberY + 0.5*cap_tube_Z ;
  G4double MirrorToCapTubeCenterX = MirrorToChamberCenterX + DistToHole2X ; // = 0

  G4double MirrorToCap3CenterX = MirrorToChamberCenterX + DistToHole3X ;
  G4double MirrorToCap3CenterY = MirrorToChamberCenterY + DistToHole3Y ;
  G4double MirrorToCap3CenterZ = MirrorToChamberCenterZ + DistToHole3Z + 0.5*hole3_Z + 0.5*chamber_cap3_Z ;

  G4RotationMatrix* xRot = new G4RotationMatrix();
  xRot->rotateX(90.*degree); // rotation matrix of hole1, hole2, collimator, beamtube

  new G4PVPlacement(0, G4ThreeVector(MirrorToChamberCenterX,MirrorToChamberCenterY,MirrorToChamberCenterZ), chamber_logic, "Vacuum_Chamber", world_logic, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(0,0,0), innerVacuum_logic, "Inner_Vacuum", chamber_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(DistToHole1X,DistToHole1Y,DistToHole1Z), chamber_hole1_logic, "Chamber_Hole1", chamber_logic, false, 0, checkOverlaps);
  
  new G4PVPlacement(xRot, G4ThreeVector(DistToHole2X,DistToHole2Y,DistToHole2Z), chamber_hole2_logic, "Chamber_Hole2", chamber_logic, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(DistToHole3X,DistToHole3Y,DistToHole3Z), chamber_hole3_logic, "Chamber_Hole3", chamber_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(MirrorToCapTubeCenterX,MirrorToCapTubeCenterY,MirrorToCapTubeCenterZ), chamber_inner_cap_tube_logic, "Chamber_Inner_Cap_Tube", world_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(MirrorToCapTubeCenterX,MirrorToCapTubeCenterY,MirrorToCapTubeCenterZ), chamber_cap_tube_logic, "Chamber_Cap_Tube", world_logic, false, 0, checkOverlaps);
  
  new G4PVPlacement(xRot, G4ThreeVector(MirrorToCapTubeCenterX,MirrorToCapTubeCenterY + 0.5*cap_tube_Z - 0.5* cap_Z ,MirrorToCapTubeCenterZ), chamber_cap1_logic, "Chamber_Cap1", world_logic, false, 0, checkOverlaps);

  new G4PVPlacement(xRot, G4ThreeVector(MirrorToCapTubeCenterX,MirrorToCapTubeCenterY + 0.5*cap_tube_Z + 0.5* cap_Z ,MirrorToCapTubeCenterZ), chamber_cap2_logic, "Chamber_Cap2", world_logic, false, 0, checkOverlaps);

  new G4PVPlacement(0, G4ThreeVector(MirrorToCap3CenterX,MirrorToCap3CenterY,MirrorToCap3CenterZ), chamber_cap3_logic, "Chamber_Cap3", world_logic, false, 0, checkOverlaps);
  

  /// New beam-line description.
  /// G4double zz00 = 1136.7*mm+34.55*mm+55.*mm+9.2*mm;
  G4double zz00 = 1136.7*mm+34.55*mm+9.2*mm+5.2*mm;

  G4VSolid* tub1_solid = new G4Tubs("tub1", 173.8*mm, 177.8*mm, 0.5*162.0*mm, 0., 360.*degree);
  tub1_logic = new G4LogicalVolume(tub1_solid, S30400, "tub1");
  tub1_logic->SetVisAttributes( G4VisAttributes(G4Colour(75/255.,0/255.,130/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,386.0*mm-zz00,0), tub1_logic, "tub1", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub1_vacuum_solid = new G4Tubs("tub1_vacuum", 0.*mm, 173.8*mm, 0.5*162.0*mm, 0., 360.*degree);
  tub1_vacuum_logic = new G4LogicalVolume(tub1_vacuum_solid, vacuum, "tub1_vacuum");
  tub1_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,386.0*mm-zz00,0), tub1_vacuum_logic, "tub1_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub2_solid = new G4Tubs("tub2", 132.0*mm, 177.8*mm, 0.5*3.0*mm, 0., 360.*degree);
  tub2_logic = new G4LogicalVolume(tub2_solid, S30400, "tub2");
  tub2_logic->SetVisAttributes( G4VisAttributes(G4Colour(173/255.,255/255.,47/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,468.5*mm-zz00,0), tub2_logic, "tub2", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub2_vacuum_solid = new G4Tubs("tub2_vacuum", 0.*mm, 132.0*mm, 0.5*3.0*mm, 0., 360.*degree);
  tub2_vacuum_logic = new G4LogicalVolume(tub2_vacuum_solid, vacuum, "tub2_vacuum");
  tub2_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,468.5*mm-zz00,0), tub2_vacuum_logic, "tub2_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub3_solid = new G4Tubs("tub3", 130.653*mm, 176.0*mm, 0.5*3.0*mm, 0., 360.*degree);
  tub3_logic = new G4LogicalVolume(tub3_solid, S30400, "tub3");
  tub3_logic->SetVisAttributes( G4VisAttributes(G4Colour(199/255.,21/255.,133/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,471.5*mm-zz00,0), tub3_logic, "tub3", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub3_vacuum_solid = new G4Tubs("tub3_vacuum", 0.*mm, 130.653*mm, 0.5*3.0*mm, 0., 360.*degree);
  tub3_vacuum_logic = new G4LogicalVolume(tub3_vacuum_solid, vacuum, "tub3_vacuum");
  tub3_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,471.5*mm-zz00,0), tub3_vacuum_logic, "tub3_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub4_solid = new G4Tubs("tub4", 130.653*mm, 133.653*mm, 0.5*59.0*mm, 0., 360.*degree);
  tub4_logic = new G4LogicalVolume(tub4_solid, S30400, "tub4");
  tub4_logic->SetVisAttributes( G4VisAttributes(G4Colour(85/255.,107/255.,47/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,502.5*mm-zz00,0), tub4_logic, "tub4", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub4_vacuum_solid = new G4Tubs("tub4_vacuum", 0.*mm, 130.653*mm, 0.5*59.0*mm, 0., 360.*degree);
  tub4_vacuum_logic = new G4LogicalVolume(tub4_vacuum_solid, vacuum, "tub4_vacuum");
  tub4_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,502.5*mm-zz00,0), tub4_vacuum_logic, "tub4_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub5_solid = new G4Tubs("tub5", 130.653*mm, 165.0*mm, 0.5*13.0*mm, 0., 360.*degree);
  tub5_logic = new G4LogicalVolume(tub5_solid, S30400, "tub5");
  tub5_logic->SetVisAttributes( G4VisAttributes(G4Colour(255/255.,20/255.,147/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,538.5*mm-zz00,0), tub5_logic, "tub5", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub5_vacuum_solid = new G4Tubs("tub5_vacuum", 0.*mm, 130.653*mm, 0.5*13.0*mm, 0., 360.*degree);
  tub5_vacuum_logic = new G4LogicalVolume(tub5_vacuum_solid, vacuum, "tub5_vacuum");
  tub5_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,538.5*mm-zz00,0), tub5_vacuum_logic, "tub5_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub6_solid = new G4Tubs("tub6", 124.968*mm, 164.968*mm, 0.5*19.0*mm, 0., 360.*degree);
  tub6_logic = new G4LogicalVolume(tub6_solid, S30400, "tub6");
  tub6_logic->SetVisAttributes( G4VisAttributes(G4Colour(30/255.,144/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,554.5*mm-zz00,0), tub6_logic, "tub6", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub6_vacuum_solid = new G4Tubs("tub6_vacuum", 0.*mm, 124.968*mm, 0.5*19.0*mm, 0., 360.*degree);
  tub6_vacuum_logic = new G4LogicalVolume(tub6_vacuum_solid, vacuum, "tub6_vacuum");
  tub6_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,554.5*mm-zz00,0), tub6_vacuum_logic, "tub6_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub7_solid = new G4Tubs("tub7", 124.968*mm, 131.968*mm, 0.5*4.0*mm, 0., 360.*degree);
  tub7_logic = new G4LogicalVolume(tub7_solid, S30400, "tub7");
  tub7_logic->SetVisAttributes( G4VisAttributes(G4Colour(147/255.,112/255.,219/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,566.0*mm-zz00,0), tub7_logic, "tub7", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub7_vacuum_solid = new G4Tubs("tub7_vacuum", 0.*mm, 124.968*mm, 0.5*4.0*mm, 0., 360.*degree);
  tub7_vacuum_logic = new G4LogicalVolume(tub7_vacuum_solid, vacuum, "tub7_vacuum");
  tub7_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,566.0*mm-zz00,0), tub7_vacuum_logic, "tub7_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub8_solid = new G4Tubs("tub8", 108.15*mm, 132.0*mm, 0.5*5.0*mm, 0., 360.*degree);
  tub8_logic = new G4LogicalVolume(tub8_solid, S30400, "tub8");
  tub8_logic->SetVisAttributes( G4VisAttributes(G4Colour(152/255.,251/255.,152/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,570.5*mm-zz00,0), tub8_logic, "tub8", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub8_vacuum_solid = new G4Tubs("tub8_vacuum", 0.*mm, 108.15*mm, 0.5*5.0*mm, 0., 360.*degree);
  tub8_vacuum_logic = new G4LogicalVolume(tub8_vacuum_solid, vacuum, "tub8_vacuum");
  tub8_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,570.5*mm-zz00,0), tub8_vacuum_logic, "tub8_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub9_solid = new G4Tubs("tub9", 101.0*mm, 114.156*mm, 0.5*29.7*mm, 0., 360.*degree);
  tub9_logic = new G4LogicalVolume(tub9_solid, S30400, "tub9");
  tub9_logic->SetVisAttributes( G4VisAttributes(G4Colour(221/255.,160/255.,211/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,587.85*mm-zz00,0), tub9_logic, "tub9", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub9_vacuum_solid = new G4Tubs("tub9_vacuum", 0.*mm, 101.0*mm, 0.5*29.7*mm, 0., 360.*degree);
  tub9_vacuum_logic = new G4LogicalVolume(tub9_vacuum_solid, vacuum, "tub9_vacuum");
  tub9_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,587.85*mm-zz00,0), tub9_vacuum_logic, "tub9_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub10_solid = new G4Tubs("tub10", 108.446*mm, 132.296*mm, 0.5*5.3*mm, 0., 360.*degree);
  tub10_logic = new G4LogicalVolume(tub10_solid, S30400, "tub10");
  tub10_logic->SetVisAttributes( G4VisAttributes(G4Colour(64/255.,224/255.,208/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,605.35*mm-zz00,0), tub10_logic, "tub10", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub10_vacuum_solid = new G4Tubs("tub10_vacuum", 0.*mm, 108.446*mm, 0.5*5.3*mm, 0., 360.*degree);
  tub10_vacuum_logic = new G4LogicalVolume(tub10_vacuum_solid, vacuum, "tub10_vacuum");
  tub10_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,605.35*mm-zz00,0), tub10_vacuum_logic, "tub10_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub11_solid = new G4Tubs("tub11", 125.0*mm, 132.0*mm, 0.5*107.5*mm, 0., 360.*degree);
  tub11_logic = new G4LogicalVolume(tub11_solid, S30400, "tub11");
  tub11_logic->SetVisAttributes( G4VisAttributes(G4Colour(75/255.,0/255.,130/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,661.75*mm-zz00,0), tub11_logic, "tub11", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub11_vacuum_solid = new G4Tubs("tub11_vacuum", 0.*mm, 125.0*mm, 0.5*107.5*mm, 0., 360.*degree);
  tub11_vacuum_logic = new G4LogicalVolume(tub11_vacuum_solid, vacuum, "tub11_vacuum");
  tub11_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,661.75*mm-zz00,0), tub11_vacuum_logic, "tub11_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub12_solid = new G4Tubs("tub12", 80.0*mm, 170.0*mm, 0.5*5.0*mm, 0., 360.*degree);
  tub12_logic = new G4LogicalVolume(tub12_solid, S30400, "tub12");
  tub12_logic->SetVisAttributes( G4VisAttributes(G4Colour(173/255.,255/255.,47/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,718.0*mm-zz00,0), tub12_logic, "tub12", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub12_vacuum_solid = new G4Tubs("tub12_vacuum", 0.*mm, 80.0*mm, 0.5*5.0*mm, 0., 360.*degree);
  tub12_vacuum_logic = new G4LogicalVolume(tub12_vacuum_solid, vacuum, "tub12_vacuum");
  tub12_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,718.0*mm-zz00,0), tub12_vacuum_logic, "tub12_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub13_solid = new G4Tubs("tub13", 81.0*mm, 177.0*mm, 0.5*19.5*mm, 0., 360.*degree);
  tub13_logic = new G4LogicalVolume(tub13_solid, S30400, "tub13");
  tub13_logic->SetVisAttributes( G4VisAttributes(G4Colour(199/255.,21/255.,133/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,730.25*mm-zz00,0), tub13_logic, "tub13", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub13_vacuum_solid = new G4Tubs("tub13_vacuum", 0.*mm, 81.0*mm, 0.5*19.5*mm, 0., 360.*degree);
  tub13_vacuum_logic = new G4LogicalVolume(tub13_vacuum_solid, vacuum, "tub13_vacuum");
  tub13_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,730.25*mm-zz00,0), tub13_vacuum_logic, "tub13_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub14_solid = new G4Tubs("tub14", 170.0*mm, 177.0*mm, 0.5*63.0*mm, 0., 360.*degree);
  tub14_logic = new G4LogicalVolume(tub14_solid, S30400, "tub14");
  tub14_logic->SetVisAttributes( G4VisAttributes(G4Colour(85/255.,107/255.,47/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,771.5*mm-zz00,0), tub14_logic, "tub14", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub14_vacuum_solid = new G4Tubs("tub14_vacuum", 0.*mm, 170.0*mm, 0.5*63.0*mm, 0., 360.*degree);
  tub14_vacuum_logic = new G4LogicalVolume(tub14_vacuum_solid, vacuum, "tub14_vacuum");
  tub14_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,771.5*mm-zz00,0), tub14_vacuum_logic, "tub14_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub15_solid = new G4Tubs("tub15", 55.5*mm, 209.0*mm, 0.5*25.0*mm, 0., 360.*degree);
  tub15_logic = new G4LogicalVolume(tub15_solid, S30400, "tub15");
  tub15_logic->SetVisAttributes( G4VisAttributes(G4Colour(255/255.,20/255.,147/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,815.5*mm-zz00,0), tub15_logic, "tub15", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub15_vacuum_solid = new G4Tubs("tub15_vacuum", 0.*mm, 55.5*mm, 0.5*25.0*mm, 0., 360.*degree);
  tub15_vacuum_logic = new G4LogicalVolume(tub15_vacuum_solid, vacuum, "tub15_vacuum");
  tub15_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,815.5*mm-zz00,0), tub15_vacuum_logic, "tub15_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub16_solid = new G4Tubs("tub16", 55.5*mm, 104.0*mm, 0.5*40.0*mm, 0., 360.*degree);
  tub16_logic = new G4LogicalVolume(tub16_solid, S30400, "tub16");
  tub16_logic->SetVisAttributes( G4VisAttributes(G4Colour(30/255.,144/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,848.0*mm-zz00,0), tub16_logic, "tub16", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub16_vacuum_solid = new G4Tubs("tub16_vacuum", 0.*mm, 55.5*mm, 0.5*40.0*mm, 0., 360.*degree);
  tub16_vacuum_logic = new G4LogicalVolume(tub16_vacuum_solid, vacuum, "tub16_vacuum");
  tub16_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,848.0*mm-zz00,0), tub16_vacuum_logic, "tub16_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub17_solid = new G4Tubs("tub17", 55.5*mm, 65.5*mm, 0.5*72.5*mm, 0., 360.*degree);
  tub17_logic = new G4LogicalVolume(tub17_solid, S30400, "tub17");
  tub17_logic->SetVisAttributes( G4VisAttributes(G4Colour(147/255.,112/255.,219/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,904.25*mm-zz00,0), tub17_logic, "tub17", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub17_vacuum_solid = new G4Tubs("tub17_vacuum", 0.*mm, 55.5*mm, 0.5*72.5*mm, 0., 360.*degree);
  tub17_vacuum_logic = new G4LogicalVolume(tub17_vacuum_solid, vacuum, "tub17_vacuum");
  tub17_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,904.25*mm-zz00,0), tub17_vacuum_logic, "tub17_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub18_solid = new G4Tubs("tub18", 55.5*mm, 104.0*mm, 0.5*20.0*mm, 0., 360.*degree);
  tub18_logic = new G4LogicalVolume(tub18_solid, S30400, "tub18");
  tub18_logic->SetVisAttributes( G4VisAttributes(G4Colour(152/255.,251/255.,152/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,950.5*mm-zz00,0), tub18_logic, "tub18", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub18_vacuum_solid = new G4Tubs("tub18_vacuum", 0.*mm, 55.5*mm, 0.5*20.0*mm, 0., 360.*degree);
  tub18_vacuum_logic = new G4LogicalVolume(tub18_vacuum_solid, vacuum, "tub18_vacuum");
  tub18_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,950.5*mm-zz00,0), tub18_vacuum_logic, "tub18_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* vacChamEnter1_solid = new G4Tubs("vacChamEnter1", 62.0*mm, 109.0*mm, 0.5*12.2*mm, 0., 360.*degree);
  vacChamEnter1_logic = new G4LogicalVolume(vacChamEnter1_solid, S30400, "vacChamEnter1");
  vacChamEnter1_logic->SetVisAttributes( G4VisAttributes(G4Colour(221/255.,160/255.,211/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,966.6*mm-zz00,0), vacChamEnter1_logic, "vacChamEnter1", world_logic, false, 0, checkOverlaps);

  G4VSolid* vacChamEnter2_solid = new G4Tubs("vacChamEnter2", 36.0*mm, 109.0*mm, 0.5*9.8*mm, 0., 360.*degree);
  vacChamEnter2_logic = new G4LogicalVolume(vacChamEnter2_solid, S30400, "vacChamEnter2");
  vacChamEnter2_logic->SetVisAttributes( G4VisAttributes(G4Colour(64/255.,224/255.,208/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,977.6*mm-zz00,0), vacChamEnter2_logic, "vacChamEnter2", world_logic, false, 0, checkOverlaps);

  G4VSolid* vacChamEnter2vacuum_solid = new G4Tubs("vacChamEnter2vacuum", 0.0*mm, 62.0*mm, 0.5*2.2*mm, 0., 360.*degree);
  vacChamEnter2vacuum_logic = new G4LogicalVolume(vacChamEnter2vacuum_solid, vacuum, "vacChamEnter2vacuum");
  vacChamEnter2vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(75/255.,0/255.,130/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,961.6*mm-zz00,0), vacChamEnter2vacuum_logic, "vacChamEnter2vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub20_solid = new G4Tubs("tub20", 30.0*mm, 61.0*mm, 0.5*10.0*mm, 0., 360.*degree);
  tub20_logic = new G4LogicalVolume(tub20_solid, S30400, "tub20");
  tub20_logic->SetVisAttributes( G4VisAttributes(G4Colour(173/255.,255/255.,47/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,967.7*mm-zz00,0), tub20_logic, "tub20", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub20_vacuum_solid = new G4Tubs("tub20_vacuum", 0.*mm, 30.0*mm, 0.5*10.0*mm, 0., 360.*degree);
  tub20_vacuum_logic = new G4LogicalVolume(tub20_vacuum_solid, vacuum, "tub20_vacuum");
  tub20_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,967.7*mm-zz00,0), tub20_vacuum_logic, "tub20_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub21a_solid = new G4Tubs("tub21a", 30.0*mm, 35.0*mm, 0.5*9.8*mm, 0., 360.*degree);
  tub21a_logic = new G4LogicalVolume(tub21a_solid, S30400, "tub21a");
  tub21a_logic->SetVisAttributes( G4VisAttributes(G4Colour(199/255.,21/255.,133/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,977.6*mm-zz00,0), tub21a_logic, "tub21a", world_logic, false, 0, checkOverlaps);
  G4VSolid* tub21a_vacuum_solid = new G4Tubs("tub21a_vacuum", 0.*mm, 30.0*mm, 0.5*9.8*mm, 0., 360.*degree);
  tub21a_vacuum_logic = new G4LogicalVolume(tub21a_vacuum_solid, vacuum, "tub21a_vacuum");
  tub21a_vacuum_logic->SetVisAttributes( G4VisAttributes(G4Colour(240/255.,255/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot, G4ThreeVector(0,977.6*mm-zz00,0), tub21a_vacuum_logic, "tub21a_vacuum", world_logic, false, 0, checkOverlaps);

  G4VSolid* tub21b_solid = new G4Tubs("tub21b", 30.0*mm, 35.0*mm, 0.5*15.0*mm, 0., 360.*degree);
  tub21b_logic = new G4LogicalVolume(tub21b_solid, S30400, "tub21b");
  tub21b_logic->SetVisAttributes( G4VisAttributes(G4Colour(85/255.,107/255.,47/255.,0.3)) );
  // new G4PVPlacement(xRot,
  new G4PVPlacement(0,
          // G4ThreeVector(0,990.0*mm-zz00,0),
          G4ThreeVector(0,0,0),
          tub21b_logic, "tub21b", chamber_hole1_logic, false, 0, checkOverlaps);

  G4VSolid* tub21c_solid = new G4Tubs("tub21c", 30.0*mm, 35.0*mm, 0.5*105.0*mm, 0., 360.*degree);
  tub21c_logic = new G4LogicalVolume(tub21c_solid, S30400, "tub21c");
  tub21c_logic->SetVisAttributes( G4VisAttributes(G4Colour(255/255.,20/255.,147/255.,0.3)) );
  new G4PVPlacement(xRot,
          // G4ThreeVector(0,1050.0*mm-zz00,0),
          G4ThreeVector(0-MirrorToChamberCenterX,1050.0*mm-zz00-MirrorToChamberCenterY,0-MirrorToChamberCenterZ),
          tub21c_logic, "tub21c", innerVacuum_logic, false, 0, checkOverlaps);

  G4VSolid* tub22_solid = new G4Tubs("tub22", 16.0*mm, 35.0*mm, 0.5*25.0*mm, 0., 360.*degree);
  tub22_logic = new G4LogicalVolume(tub22_solid, S30400, "tub22");
  tub22_logic->SetVisAttributes( G4VisAttributes(G4Colour(30/255.,144/255.,255/255.,0.3)) );
  new G4PVPlacement(xRot,
          // G4ThreeVector(0,1115.0*mm-zz00,0), tub22_logic,
          G4ThreeVector(0-MirrorToChamberCenterX,1115.0*mm-zz00-MirrorToChamberCenterY,0-MirrorToChamberCenterZ), tub22_logic,
          "tub22", innerVacuum_logic, false, 0, checkOverlaps);


  G4VSolid* leadCol16_solid = new G4Tubs("leadCol16", 8.136*mm, 30.0*mm, 0.5*50.0*mm, 0., 360.*degree);
  leadCol16_logic = new G4LogicalVolume(leadCol16_solid, Pb, "leadCol16");
  leadCol16_logic->SetVisAttributes( G4VisAttributes(G4Colour(85/255.,107/255.,47/255.,0.3)) );
  // new G4PVPlacement(xRot, G4ThreeVector(0,1027.5*mm-zz00,0), leadCol16_logic, "leadCol16", world_logic, false, 0, checkOverlaps);
  // new G4PVPlacement(0, G4ThreeVector(MirrorToChamberCenterX,MirrorToChamberCenterY,MirrorToChamberCenterZ), chamber_logic, "Vacuum_Chamber", world_logic, false, 0, checkOverlaps);
  new G4PVPlacement(xRot,
          // G4ThreeVector(0,1027.5*mm-zz00,0),
          G4ThreeVector(0-MirrorToChamberCenterX,1027.5*mm-zz00-MirrorToChamberCenterY,0-MirrorToChamberCenterZ),
          leadCol16_logic, "leadCol16", innerVacuum_logic, false, 0, checkOverlaps);

  G4VSolid* leadCol12_solid = new G4Tubs("leadCol12", 6.136*mm, 30.0*mm, 0.5*50.0*mm, 0., 360.*degree);
  leadCol12_logic = new G4LogicalVolume(leadCol12_solid, Pb, "leadCol12");
  leadCol12_logic->SetVisAttributes( G4VisAttributes(G4Colour(255/255.,20/255.,147/255.,0.3)) );
  // new G4PVPlacement(xRot, G4ThreeVector(0,1077.5*mm-zz00,0), leadCol12_logic, "leadCol12", world_logic, false, 0, checkOverlaps);
  // new G4PVPlacement(xRot, G4ThreeVector(0,1077.5*mm-zz00,0), leadCol12_logic, "leadCol12", innerVacuum_logic, false, 0, checkOverlaps);
  new G4PVPlacement(xRot,
          G4ThreeVector(0-MirrorToChamberCenterX,1077.5*mm-zz00-MirrorToChamberCenterY,0-MirrorToChamberCenterZ),
          leadCol12_logic, "leadCol12", innerVacuum_logic, false, 0, checkOverlaps);


  ///
  
  // set the Collimator, BeamTube
  G4double frameThickness = 8.*mm ; // thickness of frame of Foil, Mirror
  G4double MirrorToCollimator = MirrorToTarget - 0.5*frameThickness - 0.5*colZ ; // y-direction
  G4double MirrorToBeamTube11 = MirrorToChamberCenterY - 0.5*chamberY - 0.5*tubeZ11 ; // y-direction
  G4double MirrorToBeamTube12 = MirrorToBeamTube11 - 0.5*tubeZ11 - 0.5*tubeZ12 ; 
  G4double MirrorToBeamTube2 = MirrorToBeamTube12 + 0.5*tubeZ12 - 0.5*tubeZ2 ; // + 0.5 tubeZ12 since there is some intersection
  G4double MirrorToBeamTube3 = MirrorToBeamTube2 - 0.5*tubeZ2 + 0.5*tubeZ3 ; // + 0.5 tubeZ3 since there is some intersection
  G4double MirrorToBeamTube41 = MirrorToBeamTube3 - 0.5*tubeZ3 - 0.5*tubeZ41 ;
  G4double MirrorToBeamTube42 = MirrorToBeamTube41 - 0.5*tubeZ41 - 0.5*tubeZ42 ;
  G4double MirrorToBeamTube43 = MirrorToBeamTube42 - 0.5*tubeZ42 - 0.5*tubeZ43 ;
  G4double MirrorToBeamTube5 = MirrorToBeamTube43 - 0.5*tubeZ43 - 0.5*tubeZ5 ;
  G4double MirrorToBeamTube6 = MirrorToBeamTube5;  //MirrorToBeamTube5 - 0.5*tubeZ5 - 0.5*tubeZ6 ;

  G4double CollCenterToVacuum1 = 0.5*innercolZ - 0.5*colZ ; // y-direction
  G4double CollCenterToVacuum2 = 0.5*colZ - 0.5*innercolZ2 ; // y-direction
  
  G4double CollToLead1 = 0.5*innercolZ - 0.5*col_LeadZ ; // y-direction
  G4double CollToLead2 = 0.5*innercolZ - 1.5*col_LeadZ ; // y-direction

  //  G4double CollToLead1 = 0.5*colZ - (innercolZ2 + 0.5*col_LeadZ) ; // y-direction
  //  G4double CollToLead2 = 0.5*colZ - (innercolZ2 + 1.5*col_LeadZ) ; // y-direction

  /// new G4PVPlacement(xRot, G4ThreeVector(0.,MirrorToCollimator,0.), collimator_logic, "Collimator", world_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,CollCenterToVacuum1), colVacuum1_logic, "Collimator_Vacuum1", collimator_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,CollCenterToVacuum2), colVacuum2_logic, "Collimator_Vacuum2", collimator_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,CollToLead1), colLead1_logic, "Collimator_Lead1", colVacuum1_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,CollToLead2), colLead2_logic, "Collimator_Lead2", colVacuum1_logic, false, 0, checkOverlaps);

  /// new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube11,0), beamtube11_logic, "BeamTube11", world_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum11_logic, "BeamTube11_Vacuum", beamtube11_logic, false, 0, checkOverlaps);

  /// new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube12,0), beamtube12_logic, "BeamTube12", world_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum12_logic, "BeamTube12_Vacuum", beamtube12_logic, false, 0, checkOverlaps);
  
  /// new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube2,0), beamtube2_logic, "BeamTube2", world_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum2_logic, "BeamTube2_Vacuum", beamtube2_logic, false, 0, checkOverlaps);
  
  /// new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube3,0), beamtube3_logic, "BeamTube3", world_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum3_logic, "BeamTube3_Vacuum", beamtube3_logic, false, 0, checkOverlaps);

  /// new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube41,0), beamtube41_logic, "BeamTube41", world_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum41_logic, "BeamTube41_Vacuum", beamtube41_logic, false, 0, checkOverlaps);

  /// new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube42,0), beamtube42_logic, "BeamTube42", world_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum42_logic, "BeamTube42_Vacuum", beamtube42_logic, false, 0, checkOverlaps);

  /// new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube43,0), beamtube43_logic, "BeamTube43", world_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum43_logic, "BeamTube43_Vacuum", beamtube43_logic, false, 0, checkOverlaps);

  /// new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube5,0), beamtube5_logic, "BeamTube5", world_logic, false, 0, checkOverlaps);
  /// new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum5_logic, "BeamTube5_Vacuum", beamtube5_logic, false, 0, checkOverlaps);

  /// new G4PVPlacement(xRot, G4ThreeVector(0,MirrorToBeamTube6,0), beamtube6_logic, "BeamTube6", world_logic, false, 0, checkOverlaps);
  //  new G4PVPlacement(0, G4ThreeVector(0,0,0), beamtubeVacuum6_logic, "BeamTube6_Vacuum", beamtube6_logic, false, 0, checkOverlaps);


  //set the Target, Mirror, Al_Stopper

  G4RotationMatrix* xRot2 = new G4RotationMatrix();
  xRot2->rotateX(45.*degree); // rotation matrix of Mirror, AlStopper

  G4RotationMatrix* xRot_CsI = new G4RotationMatrix();
  xRot_CsI->rotateX(fAngle);
  
  /// Target centre.
  G4double tcx = -MirrorToChamberCenterX, tcy = -MirrorToChamberCenterY+MirrorToTarget, tcz = -MirrorToChamberCenterZ;
  if(fCsIType == 0) // foil
    {
        if(fMirrorOn == true)
	  {

	    new G4PVPlacement(xRot2,
                G4ThreeVector(tcx, tcy+62.8584*mm, tcz+2.8284*mm),
                Mirror_logic, "Mirror", innerVacuum_logic, false, 0, checkOverlaps);
	
	    new G4PVPlacement(xRot,
                G4ThreeVector(tcx, tcy, tcz),
                fTarget, "CsI_Tl", innerVacuum_logic, false, 0, checkOverlaps);

        /// Add the ring to which the foil is glued.
        G4VSolid* ring_opravka_target_solid = new G4Tubs("ring_opravka_target", 60.*mm, 65.*mm, 0.5*4.0*mm, 0., 360.*degree);
        ring_opravka_target_logic = new G4LogicalVolume(ring_opravka_target_solid, Al, "ring_opravka_target");
        ring_opravka_target_logic->SetVisAttributes( G4VisAttributes(G4Colour(255/255.,20/255.,147/255.,0.6)) );
        new G4PVPlacement(xRot,
                // G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY+MirrorToTarget-4./2*mm,-MirrorToChamberCenterZ),
                G4ThreeVector(tcx,tcy-4./2*mm,tcz),
                ring_opravka_target_logic, "ring_opravka_target", innerVacuum_logic, false, 0, checkOverlaps);

        /// Add the target support ring.
        G4VSolid* target_support_ring_p1_tub_solid = new G4Tubs("target_support_ring_tub_p1", 61.*mm, 69.*mm, 0.5*4.0*mm, 0., 360.*degree);
        G4VSolid* target_support_ring_p1_boxframe_solid = new G4Box("target_support_ring_boxframe_p1", .5*134.*mm, .5*134.*mm, .5*7.0*mm);
        G4VSolid* target_support_ring_p1_solid = new G4IntersectionSolid("target_support_ring_p1",
                target_support_ring_p1_tub_solid, target_support_ring_p1_boxframe_solid);

        G4VSolid* target_support_ring_p2_tub_solid = new G4Tubs("target_support_ring_tub_p2", 65.*mm, 69.*mm, 0.5*7.0*mm, 0., 360.*degree);
        G4VSolid* target_support_ring_p2_box_solid = new G4Box("target_support_ring_box_p2", .5*30.*mm, 69.*mm, 0.5*7.1*mm);
        G4VSolid* target_support_ring_p2_tubbox_solid = new G4SubtractionSolid("target_support_ring_p2_tubbox",
                target_support_ring_p2_tub_solid, target_support_ring_p2_box_solid);
        G4VSolid* target_support_ring_p2_boxframe_solid = new G4Box("target_support_ring_boxframe_p2", .5*134.*mm, .5*134.*mm, .5*7.0*mm);
        G4VSolid* target_support_ring_p2_solid = new G4IntersectionSolid("target_support_ring_p2",
                target_support_ring_p2_tubbox_solid, target_support_ring_p2_boxframe_solid);

        G4VSolid* target_support_ring_p3_tub_solid = new G4Tubs("target_support_ring_tub_p3", 65.*mm, 69.*mm, 0.5*6.0*mm, 0., 360.*degree);
        G4VSolid* target_support_ring_p3_box_solid = new G4Box("target_support_ring_box_p3", .5*134.*mm, .5*32.9848*mm, 0.5*6.0*mm);
        G4VSolid* target_support_ring_p3_solid = new G4IntersectionSolid("target_support_ring_p3",
                target_support_ring_p3_tub_solid, target_support_ring_p3_box_solid);

        G4VSolid* target_support_ring_p1p2_solid = new G4UnionSolid("target_support_ring_p1p2",
                target_support_ring_p1_solid, target_support_ring_p2_solid,
                0, G4ThreeVector(0., 0., -(4.+7.)/2*mm) );
        G4VSolid* target_support_ring_p1p2p3_solid = new G4UnionSolid("target_support_ring_p1p2p3",
                target_support_ring_p1p2_solid, target_support_ring_p3_solid,
                0, G4ThreeVector(0., 0., (4.+6.)/2*mm) );
        target_support_ring_logic = new G4LogicalVolume(target_support_ring_p1p2p3_solid, Al, "target_support_ring");
        target_support_ring_logic->SetVisAttributes( G4VisAttributes(G4Colour(0, 0, 1., 0.4)) );
        new G4PVPlacement(xRot,
                // G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY+MirrorToTarget+4./2*mm,-MirrorToChamberCenterZ),
                G4ThreeVector(tcx,tcy+4./2*mm,tcz),
                target_support_ring_logic, "target_support_ring", innerVacuum_logic, false, 0, checkOverlaps);

        /// Add the ring to which the mirror foil is glued.
        ring_opravka_mirror_logic = new G4LogicalVolume(ring_opravka_target_solid, Al, "ring_opravka_mirror");
        ring_opravka_mirror_logic->SetVisAttributes( G4VisAttributes(G4Colour(255/255.,20/255.,147/255.,0.6)) );
        new G4PVPlacement(xRot2,
	            // G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY+4.*mm,-MirrorToChamberCenterZ),
	            G4ThreeVector(tcx,tcy+64.2426*mm,tcz+4.2426*mm),
                ring_opravka_mirror_logic, "ring_opravka_mirror", innerVacuum_logic, false, 0, checkOverlaps);
        /// Add the mirror support ring.
        mirror_support_ring_logic = new G4LogicalVolume(target_support_ring_p1p2p3_solid, Al, "mirror_support_ring");
        mirror_support_ring_logic->SetVisAttributes( G4VisAttributes(G4Colour(0, 0, 1., 0.4)) );
        G4RotationMatrix* xRot3 = new G4RotationMatrix();
        xRot3->rotateX((45.+180.)*degree); // rotation matrix of the Mirror support ring.
        new G4PVPlacement(xRot3,
	            G4ThreeVector(tcx,tcy+61.4142*mm,tcz+1.4142*mm),
                target_support_ring_logic, "mirror_support_ring", innerVacuum_logic, false, 0, checkOverlaps);

        /// Add a support rings' table.
        G4VSolid* support_rings_table_solid = new G4Box("support_rings_table", .5*8.*mm, .5*60.*mm, 0.5*160.0*mm);
        support_rings_table_logic = new G4LogicalVolume(support_rings_table_solid, Al, "support_rings_table");
        support_rings_table_logic->SetVisAttributes( G4VisAttributes(G4Colour(255/255.,20/255.,147/255.,0.6)) );
        new G4PVPlacement(xRot,
                G4ThreeVector(tcx-71.*mm, tcy+47.8*mm, tcz),
                support_rings_table_logic, "support_rings_table", innerVacuum_logic, false, 0, checkOverlaps);
        /// Add a support rings' rigidity.
        G4VSolid* support_rings_rigidity_solid = new G4Box("support_rings_rigidity", .5*4.*mm, .5*20.*mm, 0.5*72.0*mm);
        support_rings_rigidity_logic = new G4LogicalVolume(support_rings_rigidity_solid, Al, "support_rings_rigidity");
        support_rings_rigidity_logic->SetVisAttributes( G4VisAttributes(G4Colour(255/255.,20/255.,147/255.,0.6)) );
        new G4PVPlacement(xRot,
                G4ThreeVector(tcx+69.*mm, tcy+30.*mm, tcz),
                support_rings_rigidity_logic, "support_rings_rigidity", innerVacuum_logic, false, 0, checkOverlaps);
        /// Add a BPM base.
        G4VSolid* bpm_base_solid = new G4Box("bpm_base", .5*5.*mm, .5*200.*mm, 0.5*248.0*mm);
        bpm_base_logic = new G4LogicalVolume(bpm_base_solid, Al, "bpm_base");
        bpm_base_logic->SetVisAttributes( G4VisAttributes(G4Colour(255/255.,20/255.,147/255.,0.6)) );
        new G4PVPlacement(xRot,
                G4ThreeVector(tcx-87.5*mm, tcy+73.8*mm, tcz),
                bpm_base_logic, "bpm_base", innerVacuum_logic, false, 0, checkOverlaps);
	  }
	else
	  {
	    new G4PVPlacement(xRot_CsI, G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY,
					      -MirrorToChamberCenterZ), fTarget, "CsI_Tl", innerVacuum_logic,
			      false, 0, checkOverlaps); // if no mirror, foil should be at the mirror position
	  }
    }

  else // fCsIType == 1 : crystal
    {
      new G4PVPlacement(xRot_CsI, G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY+MirrorToTarget,
						-MirrorToChamberCenterZ), fTarget, "CsI_Tl", innerVacuum_logic,
			false, 0, checkOverlaps);

      if(fMirrorOn == true)
	new G4PVPlacement(xRot2, G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY,
					       -MirrorToChamberCenterZ), Mirror_logic, "Mirror",
			  innerVacuum_logic, false, 0, checkOverlaps);
    }
  
  if(fStopperOn == true)
    {
      if(fAngle == 26.*degree)
	new G4PVPlacement(xRot_CsI, G4ThreeVector(-MirrorToChamberCenterX,-MirrorToChamberCenterY+10.*1.414,
					       -MirrorToChamberCenterZ), AlStopper_logic, "Al_Stopper",
			  innerVacuum_logic, false, 0, checkOverlaps);
	
      else
	new G4PVPlacement(xRot2,
            G4ThreeVector(tcx+33, tcy+81.3741, tcz+11.1528),
            AlStopper_logic, "Al_Stopper", innerVacuum_logic, false, 0, checkOverlaps);
       
    }
   

  // set the Alsupporting frame

  new G4PVPlacement(0, G4ThreeVector(-(119.+62.+30./2.)*mm, 0.75/10.5*AlsupportingY, 0), Alsupporting_logic, "AlSupporting", world_logic, false, 0, checkOverlaps);
  

  // set the LeadShield[i] i=0 to 5 (chamberX(height)/LeadShieldX = 288/50 ~ 5.xx) 

  G4double MirrorToLeadX[6] ; // only height(X) of each LeadShield are different
  G4double MirrorToLeadY = MirrorToChamberCenterY - chamber_cap3_Radius - 0.5*LeadShieldY ;
  G4double MirrorToLeadZ = MirrorToChamberCenterZ + 0.5*chamberZ + 0.5*LeadShieldZ ;
  
  for(G4int i=0 ; i!=6 ; i++)
    {
      MirrorToLeadX[i] = MirrorToChamberCenterX - 0.5*chamberX + (-0.5+i)*LeadShieldX ;
           
      new G4PVPlacement(0, G4ThreeVector(MirrorToLeadX[i],MirrorToLeadY,MirrorToLeadZ), LeadShield_logic, "LeadShield", world_logic, false, 0, checkOverlaps);

    }

  
  //  new G4PVPlacement(0, G4ThreeVector(,,), chamber_hole1_logic, "Chamber_Hole1", chamber_logic, false, 0, checkOverlaps);

 
  
  //define Positron Counters and Arrange each one

  G4VSolid* PC_solid1 = new G4Box("PCsolid1",0.5*35.*mm,0.5*30.*mm,0.5*5.*mm);
  G4VSolid* PC_solid2 = new G4Box("PCsolid2",0.5*55.*mm,0.5*60.*mm,0.5*5.*mm);
  G4VSolid* PC_solid3 = new G4Box("PCsolid3",0.5*65.*mm,0.5*60.*mm,0.5*10.*mm);

  G4double DistToPC1;
  
  if(fPCPosition == 0) // close  
    DistToPC1 = (135+32.5)*mm + DistToPC_Offset ; // (far) 802.mm +/- alpha // (close) = 135 + 32.5
  else
    DistToPC1 = 802.*mm + DistToPC_Offset;

  G4double DistToPC2 = DistToPC1 + 40.*mm ;
  G4double DistToPC3 = DistToPC2 + 43.*mm ;

  //  DistToAxisY = MirrorToChamberCenterY - 40.*mm ; // vertical dist to Axis of PCs from center of Mylar in y-direction
  //  DistToAxisX = 0.*mm ;
  //in x-direction, axis is assumed to be at same height with the center of Mylar(Mirror)  
  fPC_logic[0] = new G4LogicalVolume(PC_solid1,Polystyrene,"PositronCounter");
  new G4PVPlacement(0,G4ThreeVector(DistToAxisX,DistToAxisY,DistToPC1),fPC_logic[0],"PositronCounter1", world_logic,false,0,checkOverlaps);
 
  fPC_logic[1]= new G4LogicalVolume(PC_solid2,Polystyrene,"PositronCounter");
  new G4PVPlacement(0,G4ThreeVector(DistToAxisX+35./2*mm-55./2*mm,DistToAxisY-30./2*mm+60./2*mm,DistToPC2),fPC_logic[1],"PositronCounter2", world_logic,false,0,checkOverlaps);

  fPC_logic[2]= new G4LogicalVolume(PC_solid3,Polystyrene,"PositronCounter");
  new G4PVPlacement(0,G4ThreeVector(DistToAxisX+35./2*mm-65./2*mm,DistToAxisY-30./2*mm+60./2*mm,DistToPC3),fPC_logic[2],"PositronCounter3", world_logic,false,0,checkOverlaps);
  

  //setting the Visualization attributes, world to invisible
  world_logic->SetVisAttributes (G4VisAttributes::GetInvisible());

  /*
  G4VisAttributes* ChamberVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
  chamber_logic->SetVisAttributes(ChamberVisAtt);
  */
  
  //
  //always return the physical World
  //

  VisAttributes();
  
  return world_physics;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void PCDetectorConstruction::ConstructSDandField()
{
  // Set PositronCounters as Sensitive Detectors
  PCTrackerSD* PC = new PCTrackerSD("PC");
  // PCTrackerSD* PC2 = new PCTrackerSD("/PC2");
  //PCTrackerSD* PC3 = new PCTrackerSD("/PC3");

  G4SDManager* sdman = G4SDManager::GetSDMpointer();
  sdman->AddNewDetector(PC);
  SetSensitiveDetector("PositronCounter",PC, true);

   /*
   sdman->AddNewDetector(PC2);
    PC_logic2->SetSensitiveDetector(PC2);
  sdman->AddNewDetector(PC3);
    PC_logic3->SetSensitiveDetector(PC3);
   */

  G4SDParticleWithEnergyFilter* positronFilter = new G4SDParticleWithEnergyFilter("positronFilter");
  positronFilter->add("e+");
  positronFilter->SetKineticEnergy(1.0,60.0*MeV);
  PC->SetFilter(positronFilter);

  //  PC2->SetFilter(positronFilter);
  //  PC3->SetFilter(positronFilter);


  // Set Target as a Sensitive Detector to count the muons which hit target
  MCTrackerSD* MC = new MCTrackerSD("MC");
  sdman->AddNewDetector(MC);
  SetSensitiveDetector("CsI_Tl",MC,true);

  G4SDParticleWithEnergyFilter* muonFilter = new G4SDParticleWithEnergyFilter("muonFilter");
  muonFilter->add("mu+");
  muonFilter->SetKineticEnergy(0.01,5.*MeV);
  MC->SetFilter(muonFilter);
  

}

void PCDetectorConstruction::VisAttributes(){

   G4VisAttributes* default_va = new G4VisAttributes(G4Colour(1,1,1,0.3));
  chamber_logic->SetVisAttributes(default_va);

  G4VisAttributes* dim_va = new G4VisAttributes(G4Colour(0,0,0,0.1));
  innerVacuum_logic->SetVisAttributes(dim_va);
  chamber_hole1_logic->SetVisAttributes(dim_va);
  chamber_hole2_logic->SetVisAttributes(dim_va);
  chamber_hole3_logic->SetVisAttributes(dim_va);
  chamber_inner_cap_tube_logic->SetVisAttributes(dim_va);

  G4VisAttributes* cap_va = new G4VisAttributes(G4Colour(1,1,1,0.1));
  chamber_cap_tube_logic->SetVisAttributes(cap_va);
  chamber_cap1_logic->SetVisAttributes(cap_va);
  chamber_cap2_logic->SetVisAttributes(cap_va);

  G4VisAttributes* coll_va = new G4VisAttributes(G4Colour(1,1,1,0.5));
  collimator_logic->SetVisAttributes(coll_va);
  colVacuum1_logic->SetVisAttributes(dim_va);
  colVacuum2_logic->SetVisAttributes(dim_va);

  G4VisAttributes* coll_lead_va = new G4VisAttributes(G4Colour(0,0,0,0.15));
  colLead1_logic->SetVisAttributes(coll_lead_va);
  colLead2_logic->SetVisAttributes(coll_lead_va);

  beamtube11_logic->SetVisAttributes(default_va);
  beamtube12_logic->SetVisAttributes(default_va);
  beamtube2_logic->SetVisAttributes(default_va);
  beamtube3_logic->SetVisAttributes(default_va);
  beamtube41_logic->SetVisAttributes(default_va);
  beamtube42_logic->SetVisAttributes(default_va);
  beamtube43_logic->SetVisAttributes(default_va);
  beamtube5_logic->SetVisAttributes(default_va);
  beamtube6_logic->SetVisAttributes(default_va);

  beamtubeVacuum11_logic->SetVisAttributes(dim_va);
  beamtubeVacuum12_logic->SetVisAttributes(dim_va);
  beamtubeVacuum2_logic->SetVisAttributes(dim_va);
  beamtubeVacuum3_logic->SetVisAttributes(dim_va);
  beamtubeVacuum41_logic->SetVisAttributes(dim_va);
  beamtubeVacuum42_logic->SetVisAttributes(dim_va);
  beamtubeVacuum43_logic->SetVisAttributes(dim_va);
  beamtubeVacuum5_logic->SetVisAttributes(dim_va);

  G4VisAttributes* lead_va = new G4VisAttributes(G4Colour(1,1,1,0.5));
  LeadShield_logic->SetVisAttributes(lead_va);
  
  G4VisAttributes* target_va = new G4VisAttributes(G4Colour(1,0,0,0.9));
  fTarget->SetVisAttributes(target_va);

  G4VisAttributes* mirror_va = new G4VisAttributes(G4Colour(0,1,0,0.9));
  Mirror_logic->SetVisAttributes(mirror_va);

  G4VisAttributes* stopper_va = new G4VisAttributes(G4Colour(0,0,1,0.9));
  AlStopper_logic->SetVisAttributes(stopper_va);

  G4VisAttributes* PC_va = new G4VisAttributes(G4Colour(1,0,0,.5));
  fPC_logic[0]->SetVisAttributes(PC_va);
  fPC_logic[1]->SetVisAttributes(PC_va);
  fPC_logic[2]->SetVisAttributes(PC_va);
  
}


void PCDetectorConstruction::SetDefaults()
{
  fMirrorOn = true;
  fStopperOn = true;

  fPCPosition = 0; // close configuration
  fCsIType = 0; // foil

  fThickness = 5.*um; // 5um foil is default
  fAngle = 90.*degree; // actually, no rotation

  DistToAxisX = 0.;
  DistToAxisY = -35.;
  DistToPC_Offset = 0.;
}


void PCDetectorConstruction::SetDistToAxisX(G4double dist)
{
  DistToAxisX = dist ;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PCDetectorConstruction::SetDistToAxisY(G4double dist)
{
  DistToAxisY = dist;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PCDetectorConstruction::SetDistToPC_Offset(G4double dist)
{
  DistToPC_Offset = dist;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PCDetectorConstruction::SetMirrorOn(G4bool b)
{
  fMirrorOn = b;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PCDetectorConstruction::SetStopperOn(G4bool b)
{
  fStopperOn = b;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PCDetectorConstruction::SetPCPosition(G4bool b)
{
  fPCPosition = b; // 0 for close, 1 for far
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PCDetectorConstruction::SetCsIType(G4bool b)
{
  fCsIType = b;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PCDetectorConstruction::SetAngle(G4double angle)
{
  fAngle = angle;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PCDetectorConstruction::SetThickness(G4double t)
{
  fThickness = t;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}
