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
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// Description: Test of Continuous Process G4Cerenkov
//              and RestDiscrete Process G4Scintillation
//              -- Generation Cerenkov Photons --
//              -- Generation Scintillation Photons --
//              -- Transport of optical Photons --
// Version:     5.0
// Created:     1996-04-30
// Author:      Juliet Armstrong
// mail:        gum@triumf.ca
//     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
#include "G4DigiManager.hh"
#include "G4UImanager.hh"

#include "OpNovicePhysicsList.hh"

#include "G4VModularPhysicsList.hh"
#include "OpNoviceDetectorConstruction.hh"
#include "OpNoviceDigitizer.hh"
#include "OpNoviceActionInitialization.hh"
#include "OpNoviceRecorderBase.hh"

#include "OpNoviceSteppingVerbose.hh"
#include "RootIO.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "TDetector.hh"
#include "TFile.h"


#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
namespace {
void PrintUsage() {
	G4cerr << " Usage: " << G4endl;
	G4cerr << " OpNovice [-m macro ] [-u UIsession] [-t nThreads] [-r seed] [-det detFileName]"
			<< G4endl;
	G4cerr << "   note: -t option is available only for multi-threaded mode."
			<< G4endl;
}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
	// Evaluate arguments
	//
	if ( argc > 9 ) {
		PrintUsage();
		return 1;
	}

	G4String macro;
	G4String session;
	std::string detFileName="";
#ifdef G4MULTITHREADED
	G4int nThreads = 0;
#endif


	G4long myseed = 345354;
	for ( G4int i=1; i<argc; i=i+2 ) {
		if      ( G4String(argv[i]) == "-m" ) macro   = argv[i+1];
		else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
		else if ( G4String(argv[i]) == "-r" ) myseed  = atoi(argv[i+1]);
		else if ( G4String(argv[i]) == "-det" ) detFileName  = argv[i+1];
		else if ( G4String(argv[i]) == "-t" ) {
#ifdef G4MULTITHREADED
			nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
			G4cout<<"Workign with "<<nThreads<<" threads "<<G4endl;
#else
			G4cout<<"This is not compiled with G4Multithread on, ignoring"<<G4endl;
#endif

		}

		else {
			PrintUsage();
			return 1;
		}
	}

	if (detFileName.length()==0){
		G4cerr<<"Error, you need to specify detector file name with -det "<<G4endl;
		return 1;
	}	

	/*Create the detectorLight*/
	TDetector *detectorLight=new TDetector(detFileName);

	// Choose the Random engine
	//
	G4Random::setTheEngine(new CLHEP::RanecuEngine);

	//my stepping verbose
	G4VSteppingVerbose::SetInstance(new OpNoviceSteppingVerbose);

	// Construct the default run manager
	//
#ifdef G4MULTITHREADED
	G4MTRunManager * runManager = new G4MTRunManager;
	if ( nThreads > 0 ) runManager->SetNumberOfThreads(nThreads);
#else
	G4RunManager * runManager = new G4RunManager;
#endif

	// Seed the random number generator manually
	G4Random::setTheSeed(myseed);

	// Set mandatory initialization classes
	//
	// Detector construction
	OpNoviceDetectorConstruction *detector=new OpNoviceDetectorConstruction(detectorLight);
	runManager-> SetUserInitialization(detector);




	// Physics list

	G4VModularPhysicsList* physicsList = new OpNovicePhysicsList();
	runManager->SetUserInitialization(physicsList);

	// runManager-> SetUserInitialization(new OpNovicePhysicsList());


	OpNoviceRecorderBase* recorder = NULL; //No recording is done in this example

	// User action initialization
	runManager->SetUserInitialization(new OpNoviceActionInitialization(recorder,detectorLight));


	// Initialize G4 kernel
	//
	runManager->Initialize();
#ifdef G4VIS_USE
	G4VisManager* visManager =0;
#endif

	// Get the pointer to the User Interface manager
	//
	G4UImanager* UImanager = G4UImanager::GetUIpointer(); 

	if ( macro.size() ) {
		// Batch mode
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command+macro);
	}
	else // Define UI session for interactive mode
	{

		// Initialize visualization
		//
#ifdef G4VIS_USE
		visManager= new G4VisExecutive;
		// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
		// G4VisManager* visManager = new G4VisExecutive("Quiet");
		visManager->Initialize();
#endif

#ifdef G4UI_USE
		G4UIExecutive * ui = new G4UIExecutive(argc,argv,session);
#ifdef G4VIS_USE
		UImanager->ApplyCommand("/control/execute vis.mac");
#else
		UImanager->ApplyCommand("/control/execute OpNovice.in");
#endif
		if (ui->IsGUI())
			UImanager->ApplyCommand("/control/execute gui.mac");
		ui->SessionStart();
		delete ui;
#endif
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	//                 owned and deleted by the run manager, so they should not
	//                 be deleted in the main() program !

#ifdef G4VIS_USE
	if (visManager) delete visManager;
#endif

	OpNoviceDigitizer *digitizer;
	digitizer=(OpNoviceDigitizer*)(G4DigiManager::GetDMpointer()->FindDigitizerModule("OpNoviceDetectorDigitizer"));




/*
	RootIO *fRootIO=RootIO::GetInstance();
	TFile *fRootFile=fRootIO->getFile();
	fRootFile->cd();
	fRootIO->saveDetector(detectorLight);
*/

	delete runManager;

	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
