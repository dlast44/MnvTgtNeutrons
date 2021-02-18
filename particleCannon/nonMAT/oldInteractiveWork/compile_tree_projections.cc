//some standard C++ includes
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>

//some ROOT includes
#include "TInterpreter.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TSystemDirectory.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TLegend.h"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 2){
    cout << "Usage: compile_tree_projections <output_directory_name_in_plots>" << endl;
    return 1;
  }

  TString output_dir = argv[1]+TString("/");
  TString branch;

  /* Matches arguments in simple_tree_projections
  int BlobEBinSize = 100;
  int BlobEMax = 3000;
  */

  int Emax = 3000;
  int Estep = 100;
  TString Elo;
  TString Ehi;

  TH1D* h_neut;
  TH1D* h_pi0;
  TH1D* h_pip; 
  TH1D* h_pim;

  vector<TH1D*> hists;

  double n_neut;
  double n_pi0;
  double n_pip;
  double n_pim;

  double max_neut;
  double max_pi0;
  double max_pip;
  double max_pim;

  vector<double> maxes;

  TFile* input_neut = new TFile("../hists/Target5_Neutrons_TruthSelected_projections.root","READ");
  TFile* input_pi0 = new TFile("../hists/Target5_Pi0s_TruthSelected_projections.root","READ");
  TFile* input_pip = new TFile("../hists/Target5_PiPluses_TruthSelected_projections.root","READ");
  TFile* input_pim = new TFile("../hists/Target5_PiMinuses_TruthSelected_projections.root","READ");

  gStyle->SetOptStat(0);

  TCanvas* c1 = new TCanvas("c1","c1",1600,1200);
  TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);

  for (int i=0; i < Emax/Estep; ++i){
    Elo = to_string(i*Estep);
    Ehi = to_string((i+1)*Estep);

    ifstream inFile;
    inFile.open("PlottingBranches.txt");
    if (!inFile){
      cout << "PlottingBranches.txt did not succesfully open." << endl;
      return 2;
    }

    while (inFile >> branch){
      h_neut = (TH1D*)input_neut->Get("BlobTotalE_Low_"+Elo+"_High_"+Ehi+"/h_"+branch+"_BlobTotalE_Low_"+Elo+"_High_"+Ehi);
      h_neut->SetLineColor(3);
      n_neut = h_neut->Integral();
      h_pi0 = (TH1D*)input_pi0->Get("BlobTotalE_Low_"+Elo+"_High_"+Ehi+"/h_"+branch+"_BlobTotalE_Low_"+Elo+"_High_"+Ehi);
      h_pi0->SetLineColor(4);
      n_pi0 = h_pi0->Integral();
      h_pip = (TH1D*)input_pip->Get("BlobTotalE_Low_"+Elo+"_High_"+Ehi+"/h_"+branch+"_BlobTotalE_Low_"+Elo+"_High_"+Ehi);
      h_pip->SetLineColor(2);
      n_pip = h_pip->Integral();
      h_pim = (TH1D*)input_pim->Get("BlobTotalE_Low_"+Elo+"_High_"+Ehi+"/h_"+branch+"_BlobTotalE_Low_"+Elo+"_High_"+Ehi);
      h_pim->SetLineColor(6);
      n_pim = h_pim->Integral();

      if (n_neut != 0.0){
	if (n_pi0 != 0.0) h_pi0->Scale(n_neut/n_pi0);
	if (n_pip != 0.0) h_pip->Scale(n_neut/n_pip);
	if (n_pim != 0.0) h_pim->Scale(n_neut/n_pim);
      }
      else if (n_pi0 != 0.0){
	if (n_pip != 0.0) h_pip->Scale(n_pi0/n_pip);
	if (n_pim != 0.0) h_pim->Scale(n_pi0/n_pim);
      }
      else if (n_pip!=0.0){
	if (n_pim != 0.0) h_pim->Scale(n_pip/n_pim);
      }

      max_neut = h_neut->GetMaximum();
      max_pi0 = h_pi0->GetMaximum();
      max_pip = h_pip->GetMaximum();
      max_pim = h_pim->GetMaximum();

      leg->AddEntry(h_neut,"Neutrons","l");
      leg->AddEntry(h_pi0,"#pi^{0}","l");
      leg->AddEntry(h_pip,"#pi^{+}","l");
      leg->AddEntry(h_pim,"#pi^{-}","l");
      
      c1->cd();

      if ((n_pim + n_pi0 + n_pip + n_neut) != 0.0) {
	hists = {h_pim, h_pi0, h_pip, h_neut};
	maxes = {max_pim, max_pi0, max_pip, max_neut};
	auto max_index = max_element(maxes.begin(),maxes.end()) - maxes.begin();
	hists.at(max_index)->Draw();
	for (unsigned int i=0;i<hists.size();++i){
	  if (i == max_index) continue;
	  hists.at(i)->Draw("same");
	}
      }

      leg->Draw();
      
      c1->Print("../plots/"+output_dir+branch+"_Elo_"+Elo+"_Ehi_"+Ehi+".png");
      c1->Print("../plots/"+output_dir+branch+"_Elo_"+Elo+"_Ehi_"+Ehi+".pdf");
      c1->SetLogy();
      c1->Print("../plots/"+output_dir+branch+"_Elo_"+Elo+"_Ehi_"+Ehi+"_log.png");
      c1->Print("../plots/"+output_dir+branch+"_Elo_"+Elo+"_Ehi_"+Ehi+"_log.pdf");
      c1->SetLogy(0);
      c1->Clear();
      leg->Clear();
    }
  }

  input_neut->Close();
  input_pi0->Close();
  input_pip->Close();
  input_pim->Close();

  return 0;
}
