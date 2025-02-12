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
// $Id: OpNoviceDetectorSD.hh 73915 2013-09-17 07:32:26Z gcosmo $
//
/// \file optical/OpNovice/include/OpNoviceDetectorSD.hh
/// \brief Definition of the OpNoviceDetectorSD class
//
//
#ifndef OpNoviceDetectorSD_h
#define OpNoviceDetectorSD_h 1

#include "G4DataVector.hh"
#include "G4VSensitiveDetector.hh"
#include "OpNoviceDetectorHit.hh"
#include "OpNoviceDigitizer.hh"
#include "OpNoviceDigi.hh"
#include "G4SystemOfUnits.hh"

#include <vector>


class G4Step;
class G4HCofThisEvent;
class OpNoviceDigitizer;

class OpNoviceDetectorSD : public G4VSensitiveDetector
{

  public:

    OpNoviceDetectorSD(G4String name);
    virtual ~OpNoviceDetectorSD();
 
    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
 
    //A version of processHits that keeps aStep constant
    //G4bool ProcessHits_constStep(const G4Step* ,G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    void DrawAll();
    void PrintAll();
 
    //Initialize the arrays to store pmt possitions
  /*  inline void InitPMTs(G4int nPMTs){
      if(fPMTPositionsX)delete fPMTPositionsX;
      if(fPMTPositionsY)delete fPMTPositionsY;
      if(fPMTPositionsZ)delete fPMTPositionsZ;
      fPMTPositionsX=new G4DataVector(nPMTs);
      fPMTPositionsY=new G4DataVector(nPMTs);
      fPMTPositionsZ=new G4DataVector(nPMTs);
    }

    //Store a pmt position
    void SetPmtPositions(const std::vector<G4ThreeVector>& positions);
*/
  private:

    OpNoviceDetectorHitsCollection* fDetectorHitCollection;
    OpNoviceDigitizer* fDigitizer;
    OpNoviceDigitsCollection* fDC;

    G4DataVector* fPMTPositionsX;
    G4DataVector* fPMTPositionsY;
    G4DataVector* fPMTPositionsZ;



};

#endif
