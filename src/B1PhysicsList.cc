//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B3PhysicsList.cc 102186 2017-01-09 13:35:29Z gcosmo $
//
/// \file B3PhysicsList.cc
/// \brief Implementation of the B3PhysicsList class

#include "B1PhysicsList.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
//#include "G4DecayPhysics.hh"
//#include "G4EmStandardPhysics.hh"
//#include "G4RadioactiveDecayPhysics.hh"

// physics for e-/e+
//#include "G4eBremsstrahlung.hh"
//#include "G4ePairProduction.hh"
//#include "G4eMultipleScattering.hh"
//#include "G4eplusAnnihilation.hh"
//#include "G4AnnihiToMuPair.hh"
//#include "G4eIonisation.hh"

// physics for gamma
//#include "G4PhotoElectricEffect.hh"
//#include "G4ComptonScattering.hh"
//#include "G4GammaConversionToMuons.hh"
//#include "G4GammaConversion.hh"

// physics for mu-/mu+
//#include "G4MuBremsstrahlung.hh"
//#include "G4MuPairProduction.hh"
//#include "G4MuMultipleScattering.hh"
//#include "G4MuIonisation.hh"

//physics for ion/alpha
#include "G4hMultipleScattering.hh"
#include "G4NuclearStopping.hh"
#include "G4ionIonisation.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicsListHelper.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PhysicsList::B1PhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  // Default physics
  //  RegisterPhysics(new G4DecayPhysics());

  // EM physics
  //  RegisterPhysics(new G4EmStandardPhysics());

  // Radioactive decay
  //  RegisterPhysics(new G4RadioactiveDecayPhysics());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PhysicsList::~B1PhysicsList()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  

void B1PhysicsList::ConstructProcess()
{
 
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  G4ParticleDefinition* p_alpha
    = particleTable->FindParticle(particleName="alpha");

  G4ParticleDefinition* p_electron
    = particleTable->FindParticle(particleName="e-");

  G4ParticleDefinition* p_positron
    = particleTable->FindParticle(particleName="e+");

  G4ParticleDefinition* p_gamma
    = particleTable->FindParticle(particleName="gamma");

  G4ParticleDefinition* p_muplus
    = particleTable->FindParticle(particleName="mu+");

  G4ParticleDefinition* p_muminus
    = particleTable->FindParticle(particleName="mu-");
  

  // Register PhysicsProcess for each Particle
  
  ph->RegisterProcess(new G4hMultipleScattering(),p_alpha);
  ph->RegisterProcess(new G4NuclearStopping(),p_alpha);
  G4ionIonisation* ionIoni = new G4ionIonisation();
  ionIoni->SetStepFunction(0.1,1.*um);
  ph->RegisterProcess(ionIoni, p_alpha);

  //  ph->RegisterProcess(new G4eBremsstrahlung(),p_electron);
  //  ph->RegisterProcess(new G4ePairProduction(),p_electron);
  //  ph->RegisterProcess(new G4eMultipleScattering(),p_electron);
  //  ph->RegisterProcess(new G4eplusAnnihilation(),p_electron);
  //  ph->RegisterProcess(new G4AnnihiToMuPair(),p_electron);
  //    G4eIonisation* eIoni = new G4eIonisation();
  //  eIoni->SetStepFunction(0.1,1.*um);
  //  ph->RegisterProcess(eIoni,p_electron);

    //  ph->RegisterProcess(new G4eBremsstrahlung(),p_positron);
    //  ph->RegisterProcess(new G4ePairProduction(),p_positron);
    //  ph->RegisterProcess(new G4eMultipleScattering(),p_positron);
    //  ph->RegisterProcess(new G4eplusAnnihilation(),p_positron);
    //  ph->RegisterProcess(new G4AnnihiToMuPair(),p_positron);
    //  ph->RegisterProcess(eIoni,p_positron);

  //  ph->RegisterProcess(new G4PhotoElectricEffect(),p_gamma);
  //  ph->RegisterProcess(new G4ComptonScattering(),p_gamma);
  //  ph->RegisterProcess(new G4GammaConversionToMuons(),p_gamma);
  //  ph->RegisterProcess(new G4GammaConversion(),p_gamma);  

  //  ph->RegisterProcess(new G4MuBremsstrahlung(),p_muplus);
  //  ph->RegisterProcess(new G4MuPairProduction(),p_muplus);
  //  ph->RegisterProcess(new G4MuMultipleScattering(),p_muplus);
  //  G4MuIonisation* muIoni = new G4MuIonisation();
  //  muIoni->SetStepFunction(0.1,1.*um);
  //  ph->RegisterProcess(muIoni,p_muplus);

  //  ph->RegisterProcess(new G4MuPairProduction(),p_muminus);
}
