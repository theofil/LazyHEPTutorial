// to compile this puppy, uncomment the last line in this file having the "int main()  ... "
// and run this in your command prompt 
// g++  simple.C -o simple.exe `root-config --glibs --cflags`

// to run it as ROOT scipt just exectute the line belwo in your command prompt
// root -l simple.C

// to compile it with ACLiC use
// root -l simple.C++

#include <iostream>

#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>


TTree *events;
TFile *fp;

using namespace std;

//Declaration of leaves types
Int_t           NJet;
Float_t         Jet_Px[10];
Float_t         Jet_Py[10];
Float_t         Jet_Pz[10];
Float_t         Jet_E[10];
Float_t         Jet_btag[10];
Bool_t          Jet_ID[10];
Int_t           NMuon;
Float_t         Muon_Px[3];
Float_t         Muon_Py[3];
Float_t         Muon_Pz[3];
Float_t         Muon_E[3];
Int_t           Muon_Charge[3];
Float_t         Muon_Iso[3];
Int_t           NElectron;
Float_t         Electron_Px[2];
Float_t         Electron_Py[2];
Float_t         Electron_Pz[2];
Float_t         Electron_E[2];
Int_t           Electron_Charge[2];
Float_t         Electron_Iso[2];
Int_t           NPhoton;
Float_t         Photon_Px[2];
Float_t         Photon_Py[2];
Float_t         Photon_Pz[2];
Float_t         Photon_E[2];
Float_t         Photon_Iso[2];
Float_t         MET_px;
Float_t         MET_py;
Float_t         MChadronicBottom_px;
Float_t         MChadronicBottom_py;
Float_t         MChadronicBottom_pz;
Float_t         MCleptonicBottom_px;
Float_t         MCleptonicBottom_py;
Float_t         MCleptonicBottom_pz;
Float_t         MChadronicWDecayQuark_px;
Float_t         MChadronicWDecayQuark_py;
Float_t         MChadronicWDecayQuark_pz;
Float_t         MChadronicWDecayQuarkBar_px;
Float_t         MChadronicWDecayQuarkBar_py;
Float_t         MChadronicWDecayQuarkBar_pz;
Float_t         MClepton_px;
Float_t         MClepton_py;
Float_t         MClepton_pz;
Int_t           MCleptonPDGid;
Float_t         MCneutrino_px;
Float_t         MCneutrino_py;
Float_t         MCneutrino_pz;
Int_t           NPrimaryVertices;
Bool_t          triggerIsoMu24;
Float_t         EventWeight;

void init()
{
   // Set branch addresses
   events->SetBranchAddress("NJet",&NJet);
   events->SetBranchAddress("Jet_Px",Jet_Px);
   events->SetBranchAddress("Jet_Py",Jet_Py);
   events->SetBranchAddress("Jet_Pz",Jet_Pz);
   events->SetBranchAddress("Jet_E",Jet_E);
   events->SetBranchAddress("Jet_btag",Jet_btag);
   events->SetBranchAddress("Jet_ID",Jet_ID);
   events->SetBranchAddress("NMuon",&NMuon);
   events->SetBranchAddress("Muon_Px",Muon_Px);
   events->SetBranchAddress("Muon_Py",Muon_Py);
   events->SetBranchAddress("Muon_Pz",Muon_Pz);
   events->SetBranchAddress("Muon_E",Muon_E);
   events->SetBranchAddress("Muon_Charge",Muon_Charge);
   events->SetBranchAddress("Muon_Iso",Muon_Iso);
   events->SetBranchAddress("NElectron",&NElectron);
   events->SetBranchAddress("Electron_Px",Electron_Px);
   events->SetBranchAddress("Electron_Py",Electron_Py);
   events->SetBranchAddress("Electron_Pz",Electron_Pz);
   events->SetBranchAddress("Electron_E",Electron_E);
   events->SetBranchAddress("Electron_Charge",Electron_Charge);
   events->SetBranchAddress("Electron_Iso",Electron_Iso);
   events->SetBranchAddress("NPhoton",&NPhoton);
   events->SetBranchAddress("Photon_Px",Photon_Px);
   events->SetBranchAddress("Photon_Py",Photon_Py);
   events->SetBranchAddress("Photon_Pz",Photon_Pz);
   events->SetBranchAddress("Photon_E",Photon_E);
   events->SetBranchAddress("Photon_Iso",Photon_Iso);
   events->SetBranchAddress("MET_px",&MET_px);
   events->SetBranchAddress("MET_py",&MET_py);
   events->SetBranchAddress("MChadronicBottom_px",&MChadronicBottom_px);
   events->SetBranchAddress("MChadronicBottom_py",&MChadronicBottom_py);
   events->SetBranchAddress("MChadronicBottom_pz",&MChadronicBottom_pz);
   events->SetBranchAddress("MCleptonicBottom_px",&MCleptonicBottom_px);
   events->SetBranchAddress("MCleptonicBottom_py",&MCleptonicBottom_py);
   events->SetBranchAddress("MCleptonicBottom_pz",&MCleptonicBottom_pz);
   events->SetBranchAddress("MChadronicWDecayQuark_px",&MChadronicWDecayQuark_px);
   events->SetBranchAddress("MChadronicWDecayQuark_py",&MChadronicWDecayQuark_py);
   events->SetBranchAddress("MChadronicWDecayQuark_pz",&MChadronicWDecayQuark_pz);
   events->SetBranchAddress("MChadronicWDecayQuarkBar_px",&MChadronicWDecayQuarkBar_px);
   events->SetBranchAddress("MChadronicWDecayQuarkBar_py",&MChadronicWDecayQuarkBar_py);
   events->SetBranchAddress("MChadronicWDecayQuarkBar_pz",&MChadronicWDecayQuarkBar_pz);
   events->SetBranchAddress("MClepton_px",&MClepton_px);
   events->SetBranchAddress("MClepton_py",&MClepton_py);
   events->SetBranchAddress("MClepton_pz",&MClepton_pz);
   events->SetBranchAddress("MCleptonPDGid",&MCleptonPDGid);
   events->SetBranchAddress("MCneutrino_px",&MCneutrino_px);
   events->SetBranchAddress("MCneutrino_py",&MCneutrino_py);
   events->SetBranchAddress("MCneutrino_pz",&MCneutrino_pz);
   events->SetBranchAddress("NPrimaryVertices",&NPrimaryVertices);
   events->SetBranchAddress("triggerIsoMu24",&triggerIsoMu24);
   events->SetBranchAddress("EventWeight",&EventWeight);
}


void simple()
{
    fp = TFile::Open("http://theofil.web.cern.ch/theofil/cmsod/files/data.root", "READ");
    events = (TTree*)fp->Get("events");
    init();
  
    unsigned int nEvents = events->GetEntriesFast();
    for(unsigned int i = 0 ; i<nEvents ; ++i)
    {
	events->GetEntry(i);
        cout << NMuon << ", " << NJet << endl;
    }
}

//int main(){simple (); return 0;}
