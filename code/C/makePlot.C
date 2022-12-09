// written by Konstantinos Theofilatos 27.11.2022 
// run it as ROOT scipt in CINT (ROOT's interpreter) 
// [needs internet connection to download the data] 
//
//    root makePlot.C
//
// --- other options
// a) compile it with ACLiC use
//      root makePlot.C++
//
// b) compile with g++, uncomment the last line in this file having the "int main()  ... "
//    and run this in your command prompt 
//      g++  makePlot.C -o makePlot.exe `root-config --glibs --cflags`
//
// This program produces a histogram of the muon multiplicity 
// as well as the output below
//
// Data :  469384.0 ± 685.1 
// MC   :  324461.4 ± 55461.7 
// ###########################
// QCD :  79160.5 ± 55457.2 
// WJets :  209555.9 ± 689.2 
// Single Top :  311.6 ± 4.4 
// WW :  229.9 ± 3.7 
// TTbar :  1001.8 ± 16.2 
// WZ :  69.9 ± 1.3 
// ZZ :  16.9 ± 0.4 
// DY :  34114.8 ± 145.6 

#include <iostream>
#include <stdio.h>
#include <string>

#include <TROOT.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH1I.h>
#include "TLegend.h"
#include "TLegendEntry.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TStyle.h"


using namespace std;

// book histogram
int nBins       = 7;
float xMin      = 0;
float xMax      = 7;
string plotName = "NMuon"; // don't put special symbols and spaces here
string ylabel   = "events / bin";
string xlabel   = "number of muons";
bool setLogy    = true; // log scale for y-axis

// configuration
#define nFiles  9
float fraction = 1; // fraction of events to proccess for each sample
string files[]  = {"data.root", "qcd.root", "wjets.root", "single_top.root", "ww.root", "ttbar.root",  "wz.root", "zz.root", "dy.root"};
string labels[] = {"Data", "QCD", "WJets", "Single Top", "WW", "TTbar", "WZ", "ZZ",  "DY"};
int fillColors[] ={  1   , 6    , 3      , 4           , 9   , 2      , 7   , 8   , 5};
int lineColors[] ={  1   , 6    , 3      , 4           , 9   , 2      , 7   , 8   , 5};
string path = "http://theofil.web.cern.ch/theofil/cmsod/files/";

// pointers to TFile and TTrees
TFile *fp[nFiles];
TTree *events[nFiles];

// variables stored in the root tree, you can access them during the event loop
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

// pointers to histograms, stack, canvas and legenend
TH1F *histo[nFiles];  
THStack *hs; 
TCanvas *can;
TLegend *leg;
TH1F   *h_mc; //last one is for the sum of all MCs


// setTDRStyle and init declared here, implementated in the end of this file
void setTDRStyle(); 
void init(TTree *events); 


// store weighted event counts and their error
Double_t counts[nFiles+1]; // last is for all MCs summed
Double_t countsError[nFiles+1]; 

// this function is executed when running from prompt: root makeHist.C
void makePlot()
{    
    // set style for the plots
    setTDRStyle();
    gStyle->SetOptTitle(0);

    // create histogram stack, legend and canvas, set style
    hs       = new THStack("hs","hs"); // histogram stack
    leg      = new TLegend(0.75, 0.47, 0.95, 0.89); // numbers indicate xMin, yMin, xMax, yMax of the legend box
    can      = new TCanvas("can","can");
    
    // create histogram that will be the sum of all MCs 
    h_mc = new TH1F("h_mc", "h_mc", nBins, xMin, xMax);
    h_mc->Sumw2();
    h_mc->SetFillColor(kWhite);
    h_mc->SetLineColor(kBlack);
    h_mc->SetLineWidth(2);
    h_mc->GetXaxis()->SetTitle(xlabel.c_str());
    h_mc->GetYaxis()->SetTitle(ylabel.c_str());

    // loop over files (samples) -- i index
    for(int i = 0; i < nFiles; ++i) 
    {
        // open root file i, "get" the root ttrees from inside 
	cout <<"opening "<< files[i] << " from path " << path << endl;   

     	fp[i] = TFile::Open( (path + files[i]).c_str() , "READ"); 
	events[i] = (TTree*)fp[i]->Get("events"); 
	init(events[i]); 
  

        // create TH1F histogram for the file i
        auto histoName = (plotName + "_" + to_string(i)).c_str();
        histo[i] = new TH1F(histoName, histoName, nBins, xMin, xMax);
        histo[i]->Sumw2();
        histo[i]->SetFillColor(fillColors[i]);
        histo[i]->SetLineColor(lineColors[i]);
        histo[i]->GetXaxis()->SetNdivisions(509);
        histo[i]->GetYaxis()->SetNdivisions(509);
        histo[i]->GetXaxis()->SetTitle(xlabel.c_str());
        histo[i]->GetYaxis()->SetTitle(ylabel.c_str());

        // if i!= 0 add histogram to the MC histogram stack, else treat it as data (i == 0)
        if (i != 0){hs->Add(histo[i]); leg->AddEntry(histo[i], labels[i].c_str(), "fl");}
	if (i == 0){histo[i]->SetFillStyle(0); histo[i]->SetFillColor(0);leg->AddEntry(histo[i], labels[i].c_str(), "pe");}

        // see how many entries (unweighted events) the tree has
	Long64_t nEvents = Long64_t(fraction*events[i]->GetEntriesFast());
        cout <<"analyzing " << nEvents << " unweighted events of " << labels[i] << endl;
        
        // --------- the magic starts here --------- // 
        // event loop -- j index
	for(Long64_t j = 0 ; j<nEvents ; ++j)
	{
	    events[i]->GetEntry(j); 

            // by default the event will be rejected, unless it qualifies the desired selection below
            bool selection = false;
            float var = 0;

            // define selection (analysis cuts) and the variable to be filled in the histogram
            if(triggerIsoMu24) 
            {
                selection = true; 
                var       = NMuon;
            }

            // apply selection and fill the histogram with the computed variable taking into account the event weight
            if(selection) histo[i]->Fill(var, EventWeight);
	         
	} 
        // --------- the magic ends here --------- // 

       if(i !=0 ) h_mc->Add(histo[i]); // add histo to h_mc if it's not data histo

    } // end of file loop

    // draw and save plot
    can->cd();
    histo[0]->Draw("e1");
    hs->Draw("hist same");    
    h_mc->Draw("hist same");
    leg->Draw("same");
    histo[0]->Draw("e1 same");
    histo[0]->Draw("axis same");
    if(setLogy) can->SetLogy();
    auto fileName_png = (plotName + ".png").c_str();
    auto fileName_pdf = (plotName + ".pdf").c_str();
    can->SaveAs(fileName_pdf);
    can->SaveAs(fileName_png);


   // print summary statistics for data and the sum of all MCs
   cout << "###########################" <<endl;
   int i = 0;
   counts[i] = histo[i]->IntegralAndError(0, histo[i]->GetNbinsX()+1, countsError[i]); // number of weighted events and their error
   printf("%s :  %2.1f ± %2.1f \n", labels[i].c_str(), counts[i], countsError[i]);
   
   i = nFiles;
   counts[i] = h_mc->IntegralAndError(0, h_mc->GetNbinsX()+1, countsError[i]); // number of weighted events and their error
   printf("%s :  %2.1f ± %2.1f \n", "MC  ", counts[i], countsError[i]);
  
   cout << "###########################" <<endl;
   // print out statistics for each MC
   for (int i = 1; i < nFiles; ++i) 
   {
	countsError[i]=0;
	counts[i] = histo[i]->IntegralAndError(0, histo[i]->GetNbinsX()+1, countsError[i]); // number of weighted events and their error
	printf("%s :  %2.1f ± %2.1f \n", labels[i].c_str(), counts[i], countsError[i]);
   }

}
































//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////    you can ignore all lines of code below this line        //////// 
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// setTDRStyle for the plots
void setTDRStyle() 
{
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

  // For the Pad:
  tdrStyle->SetPadBorderMode(0);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);

  tdrStyle->SetEndErrorSize(2);
  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);
  tdrStyle->SetMarkerSize(1.2);

  //For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

  //For the date:
  tdrStyle->SetOptDate(0);

  // For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);

  // Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);

  // For the Global title:

  //  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);

  // For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);

  // For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

  // For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

  // Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(0);
  tdrStyle->SetOptLogz(0);

  tdrStyle->cd();

  // --- set nice colors in 2D

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  // --- set default Sumw2
  TH1::SetDefaultSumw2(true);
}

void init(TTree *events)
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

//uncomment the line below if you want to compile makePlot.C
//int main(){makePlot (); return 0;}
