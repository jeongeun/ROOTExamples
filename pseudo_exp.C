#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif
//https://root.cern.ch/root/htmldoc/guides/users-guide/Histograms.html#random-numbers-and-histograms

void pseudo_exp(){

  using namespace TMath;
  gSystem->Load("libDelphes");

  //TFile *file = TFile::Open("Wprime_m1000_phase2_pu200_0.root");
  const char *inputfile ="Wprime_m1000_phase2_pu200_0.root";
  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputfile);

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();
  cout << "numOfEntries = " << numberOfEntries << endl;
  // Get pointers to branches used in this analysis
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");

  // Book histogram
  TH1 *h_ePT = new TH1F("Electron_PT","Electron_PT", 100,0,1000);
  TH1 *h_MT = new TH1F("MT","MT", 100,0,5000);

  TLorentzVector *elevec = new TLorentzVector();
  TLorentzVector *metvec = new TLorentzVector();

  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    treeReader->ReadEntry(entry);

    Electron *electron;
    if(branchElectron->GetEntries() > 0)
    {
      // Take first (leading) electron
      electron = (Electron *) branchElectron->At(0);
      h_ePT->Fill(electron->PT);
    }
    MissingET *met;
    // Take missingET
    met = (MissingET *) branchMissingET->At(0);

    elevec->SetPtEtaPhiE(electron->PT, electron->Eta, electron->Phi, electron->PT);      
    metvec->SetPtEtaPhiE(met->MET, met->Eta, met->Phi, met->MET);   

    double MT = Sqrt(2*(electron->PT)*(met->MET)*(1-Cos(Abs(metvec->DeltaPhi(*elevec)))));   
    // Plot their transverse mass
    h_MT->Fill(MT);
  }

  double ePTsum = h_ePT->Integral(); cout << "ePT = " << ePTsum << endl;
  double MTsum = h_MT->Integral(); cout << "MT = " << MTsum << endl;

  // normalization
  double xseclumi = 10000; //This is just an example xsec * lumi  (not the true value)
  h_ePT->Scale(xseclumi/ePTsum);
  h_MT->Scale(xseclumi/MTsum);

  //random generation
  TH1 *h_pseudo_ePT = new TH1F("Random Electron_PT","Electron_PT (pseudo)", 100,0,1000);
  h_pseudo_ePT->FillRandom(h_ePT, 10000);

  TH1 *h_pseudo_MT = new TH1F("Random MT","MT (pseudo)", 100,0,5000);
  h_pseudo_MT->FillRandom(h_MT, 10000);


  // Draw MT Plot
  TCanvas* c1 = new TCanvas();
  c1->SetLogy();
  gStyle->SetOptStat(0);

  h_MT->SetLineColor(kBlue);
  h_MT->Draw("HIST");

  h_pseudo_MT->SetLineColor(kBlack);
  h_pseudo_MT->SetMarkerStyle(20);
  h_pseudo_MT->Draw("EP SAME");

  TLegend* leg1 = new TLegend(0.6,0.7,0.9,0.9);
  leg1->AddEntry(h_MT, "MT MC (Wprime 1 TeV)", "l");
  leg1->AddEntry(h_pseudo_MT, "Pseudo Data (20000)", "pe");
  leg1->Draw();

  // Draw ePT Plot
  TCanvas* c2 = new TCanvas();
  h_ePT->SetLineColor(kBlue);
  h_ePT->Draw("HIST");

  h_pseudo_ePT->SetLineColor(kBlack);
  h_pseudo_ePT->SetMarkerStyle(20);
  h_pseudo_ePT->Draw("EP SAME");

  TLegend* leg2 = new TLegend(0.6,0.7,0.9,0.9);
  leg2->AddEntry(h_ePT, "Electron PT MC (Wprime 1 TeV)", "l");
  leg2->AddEntry(h_pseudo_ePT, "Pseudo Data (15000)", "pe");
  leg2->Draw();

}
