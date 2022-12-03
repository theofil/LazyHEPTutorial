// to compile this puppy, uncomment the last line in this file having the "int main()  ... "
// and run this in your command prompt 
// g++  analysis.C -o analysis.exe `root-config --glibs --cflags`

// to run it as ROOT scipt just exectute the line belwo in your command prompt
// root -l analysis.C

// to compile it with ACLiC use
// root -l analysis.C++

#include <iostream>
#include <string>

#include <TROOT.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH1I.h>
#include "TLegend.h"
#include "TLegendEntry.h"
#include "THStack.h"
#include "TCanvas.h"
#include "setTDRStyle.C"
#include "TTree.h"


using namespace std;

// configuration
#define nFiles  9
float fraction = 1; // fraction of events to proccess for each sample
string files[]  = {"data.root", "qcd.root", "wjets.root", "single_top.root", "ww.root", "ttbar.root",  "wz.root", "zz.root", "dy.root"};
string labels[] = {"Data", "QCD", "WJets", "Single Top", "WW", "TTbar", "WZ", "ZZ",  "DY"};
int colors[] =  {1,2,3,4,5,6,7,8,9};
string path = "http://theofil.web.cern.ch/theofil/cmsod/files/";

// pointers to file and tree objects
TFile *fp;
TTree *events;

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
Float_t         Photon_Px[3];
Float_t         Photon_Py[3];
Float_t         Photon_Pz[3];
Float_t         Photon_E[3];
Float_t         Photon_Iso[3];
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

// book histogram for each variable
TH1F *h_NMuon[nFiles];
THStack *hs_NMuon; 
TCanvas *c_NMuon;
TLegend *l_NMuon;

TH1F *h_mumuMass[nFiles];
THStack *hs_mumuMass; 
TCanvas *c_mumuMass;
TLegend *l_mumuMass;

void analysis()
{ 

//    for(auto color: colors)
//    {
//      cout << color << endl; 
//    }
//    cout << endl;
//    for(int i = 0; i < nFiles; ++i) 
//    {
//	cout << colors[i] << endl;
//    }
    setTDRStyle();
    gStyle->SetOptTitle(0);

    // create an array of samples, holding objects of the sample type
    for(int i = 0; i < nFiles; ++i) 
    {
        //printf("Adding file %s as %s with color %d \n", files[i].c_str(), labels[i].c_str(), colors[i]);
        cout << "Adding file " << files[i] << " as " << labels[i] << " with color " << colors[i] << endl; 
        string histoName;

        // create histograms and the histogram stack
        histoName = "h_NMuon_"+to_string(i);

        h_NMuon[i] = new TH1F( histoName.c_str(), histoName.c_str(), 5, 0, 5); 
        h_NMuon[i]->Sumw2();
        h_NMuon[i]->SetFillColor(colors[i]);
        h_NMuon[i]->SetLineColor(colors[i]);
        h_NMuon[i]->GetXaxis()->SetNdivisions(509);
        h_NMuon[i]->GetYaxis()->SetNdivisions(509);
        h_NMuon[i]->GetXaxis()->SetTitle("NMuon");
        h_NMuon[i]->GetYaxis()->SetTitle("events / bin");

        histoName = "h_mumuMass_"+to_string(i);

        h_mumuMass[i] = new TH1F( histoName.c_str(), histoName.c_str(), 100, 0, 200); 
        h_mumuMass[i]->Sumw2();
        h_mumuMass[i]->SetFillColor(colors[i]);
        h_mumuMass[i]->SetLineColor(colors[i]);
        h_mumuMass[i]->GetXaxis()->SetNdivisions(509);
        h_mumuMass[i]->GetYaxis()->SetNdivisions(509);
        h_mumuMass[i]->GetXaxis()->SetTitle("mumuMass");
        h_mumuMass[i]->GetYaxis()->SetTitle("events / bin");

    }
    hs_NMuon = new THStack("hs_NMuon","hs_NMuon");
    l_NMuon  = new TLegend(0.75, 0.47, 0.95, 0.89);

    hs_mumuMass = new THStack("hs_mumuMass","hs_mumuMass");
    l_mumuMass  = new TLegend(0.75, 0.47, 0.95, 0.89);

   
    for(int i = 0; i < nFiles; ++i) 
    {
//        if(i==1 || i==2 || i==3 || i == 4 || i==5 || i==6)continue;
	cout <<"opening "<< files[i] << " from path " << path << endl;   

     	fp = TFile::Open( (path + files[i]).c_str() , "READ");
	events = (TTree*)fp->Get("events");
	init();
  
	Long64_t nEvents = Long64_t(fraction*events->GetEntriesFast());
        cout <<"analyzing " << nEvents << " of " << labels[i] << endl;
         
	for(Long64_t j = 0 ; j<nEvents ; ++j)
	{
	    events->GetEntry(j);
            h_NMuon[i]->Fill(NMuon, EventWeight);
            
            // select only events with 2 muons and trigger fired
            if(NMuon>=2 && triggerIsoMu24 == true)
	    {
		float px1 = Muon_Px[0];
		float py1 = Muon_Py[0];
		float pz1 = Muon_Pz[0];
		float E1  = Muon_E[0];

		float px2 = Muon_Px[1];
		float py2 = Muon_Py[1];
		float pz2 = Muon_Pz[1];
		float E2  = Muon_E[1];
                
                float mumuMass2 = pow(E1 + E2, 2) - pow(px1 + px2, 2) - pow(py1 + py2, 2) - pow(pz1 + pz2, 2);
                float mumuMass = pow(mumuMass2, 0.5);

		h_mumuMass[i]->Fill(mumuMass, EventWeight);		
	    }
	}

        if(i!=0)
        { 
	    hs_NMuon->Add(h_NMuon[i]);
	    l_NMuon->AddEntry(h_NMuon[i], labels[i].c_str(), "fl");

	    hs_mumuMass->Add(h_mumuMass[i]);
	    l_mumuMass->AddEntry(h_mumuMass[i], labels[i].c_str(), "fl");
        }
        else
        {
	    l_NMuon->AddEntry(h_NMuon[i], labels[i].c_str(), "pe");

	    l_mumuMass->AddEntry(h_mumuMass[i], labels[i].c_str(), "pe");
	}
           
    }

    // draw NMuon histogram
    c_NMuon = new TCanvas("hs_NMuon");
    c_NMuon->cd();
    h_NMuon[0]->Draw("e1");
    hs_NMuon->Draw("hist same");    
    hs_NMuon->Draw("axis same");    
    l_NMuon->Draw("same");
    h_NMuon[0]->Draw("e1 same");
    c_NMuon->SetLogy();
    c_NMuon->SaveAs("NMuon.png");
    c_NMuon->SaveAs("NMuon.pdf");

    // draw mumuMass histogram
    c_mumuMass = new TCanvas("hs_mumuMass");
    c_mumuMass->cd();
    h_mumuMass[0]->Draw("e1");
    hs_mumuMass->Draw("hist same");    
    hs_mumuMass->Draw("axis same");    
    l_mumuMass->Draw("same");
    h_mumuMass[0]->Draw("e1 same");
    c_mumuMass->SetLogy();
    c_mumuMass->SaveAs("mumuMass.png");
    c_mumuMass->SaveAs("mumuMass.pdf");
}

//uncomment the line below if you want to compile analysis.C
//int main(){analysis (); return 0;}
