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
// $Id: OpNoviceActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file OpNoviceActionInitialization.cc
/// \brief Implementation of the OpNoviceActionInitialization class

#include "OpNoviceActionInitialization.hh"
#include "OpNovicePrimaryGeneratorAction.hh"
#include "OpNoviceRunAction.hh"

#include "OpNoviceEventAction.hh"
#include "OpNoviceTrackingAction.hh"
#include "OpNoviceSteppingAction.hh"
#include "OpNoviceStackingAction.hh"
#include "OpNoviceSteppingVerbose.hh"

#include "OpNoviceRecorderBase.hh"
#include "TDetector.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceActionInitialization::OpNoviceActionInitialization(OpNoviceRecorderBase* recorder,TDetector* detector)
: G4VUserActionInitialization(),fRecorder(recorder),fDetector(detector)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

OpNoviceActionInitialization::~OpNoviceActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceActionInitialization::BuildForMaster() const
{
	SetUserAction(new OpNoviceRunAction(fRecorder,fDetector));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void OpNoviceActionInitialization::Build() const
{
  SetUserAction(new OpNovicePrimaryGeneratorAction());
  SetUserAction(new OpNoviceStackingAction());
	
  SetUserAction(new OpNoviceRunAction(fRecorder,fDetector));
  SetUserAction(new OpNoviceEventAction(fRecorder));
  SetUserAction(new OpNoviceTrackingAction(fRecorder));	
  SetUserAction(new OpNoviceSteppingAction(fRecorder));


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose*
               OpNoviceActionInitialization::InitializeSteppingVerbose() const
{
	G4cout<<"OpNoviceActionInitialization::InitializeSteppingVerbose() was called"<<G4endl;
  return new OpNoviceSteppingVerbose();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
