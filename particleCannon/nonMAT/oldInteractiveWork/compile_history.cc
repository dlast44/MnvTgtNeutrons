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
    cout << "Usage: compile_history <output_directory_name_in_plots>" << endl;
    return 1;
  }

  TString output_dir = argv[1];

  int Emax = 1000;
  int Estep = 50;
  TString Elo;
  TString Ehi;

  TH1D* h_neut;
  TH1D* h_pi0;
  TH1D* h_pip; 
  TH1D* h_pim;

  TFile* input_neut = new TFile("../hists/Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0_history.root","READ");
  TFile* input_pi0 = new TFile("../hists/Run_126000_Target5_Pi0PlusMuon_PDG_111_Ek_lo_0.0_Ek_hi_5000.0_history.root","READ");
  TFile* input_pip = new TFile("../hists/Run_126000_Target5_PiPlusPlusMuon_PDG_211_Ek_lo_0.0_Ek_hi_5000.0_history.root","READ");
  TFile* input_pim = new TFile("../hists/Run_126000_Target5_PiMinusPlusMuon_PDG_-211_Ek_lo_0.0_Ek_hi_5000.0_history.root","READ");

  gStyle->SetOptStat(0);

  TCanvas* c1 = new TCanvas("c1","c1",1600,1200);
  TLegend* leg = new TLegend(0.7,0.7,0.9,0.9);

  for (int i=0; i < Emax/Estep; ++i){
    Elo = to_string(i*Estep);
    Ehi = to_string((i+1)*Estep);

    h_neut = (TH1D*)input_neut->Get("h_history_Elo_"+Elo+"_Ehi_"+Ehi);
    h_neut->SetLineColor(3);
    h_pi0 = (TH1D*)input_pi0->Get("h_history_Elo_"+Elo+"_Ehi_"+Ehi);
    h_pi0->SetLineColor(4);
    h_pip = (TH1D*)input_pip->Get("h_history_Elo_"+Elo+"_Ehi_"+Ehi);
    h_pip->SetLineColor(2);
    h_pim = (TH1D*)input_pim->Get("h_history_Elo_"+Elo+"_Ehi_"+Ehi);    
    h_pim->SetLineColor(6);
    if (i==0){
      leg->AddEntry(h_neut,"Neutrons","l");
      leg->AddEntry(h_pi0,"#pi^{0}","l");
      leg->AddEntry(h_pip,"#pi^{+}","l");
      leg->AddEntry(h_pim,"#pi^{-}","l");
    }

    c1->cd();
    h_pi0->Draw();
    h_pim->Draw("same");
    h_pip->Draw("same");
    h_neut->Draw("same");
    leg->Draw();

    c1->Print("../plots/"+output_dir+"/histories_Elo_"+Elo+"_Ehi_"+Ehi+".jpg");
    c1->Print("../plots/"+output_dir+"/histories_Elo_"+Elo+"_Ehi_"+Ehi+".pdf");
    c1->SetLogy();
    c1->Print("../plots/"+output_dir+"/histories_Elo_"+Elo+"_Ehi_"+Ehi+"_log.jpg");
    c1->Print("../plots/"+output_dir+"/histories_Elo_"+Elo+"_Ehi_"+Ehi+"_log.pdf");
    c1->SetLogy(0);
    c1->Clear();    
  }

  input_neut->Close();
  input_pi0->Close();
  input_pip->Close();
  input_pim->Close();

  return 0;
}
