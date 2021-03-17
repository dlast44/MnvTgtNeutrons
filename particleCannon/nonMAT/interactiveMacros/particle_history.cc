//some standard C++ includes
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <numeric>
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
  TString tool;
  TString top_dir;
  TString name;
  int pdg_in;

  if (argc < 4 || argc > 5){
    cout << "Usage: particle_history <tool_name> <top_dir> <file_name> <pdg_in, default 0>" << endl;
    cout << "<tool_name> is CCQENu or MAD" << endl;
    return 1;
  }
  else if (argc == 4){
    tool = argv[1];
    top_dir = argv[2];
    name = argv[3];
    pdg_in = 0;
  }
  else{
    tool = argv[1];
    top_dir = argv[2];
    name = argv[3];
    pdg_in = atoi(argv[4]);
  }

  TString tree_name = "";
  if (tool == "CCQENu") tree_name = tool;
  else if (tool == "MAD") tree_name = "MasterAnaDev";
  else{
    cout << "NEED tool to be either CCQENu or MAD" << endl;
    return 2;
  }

  TFile* input_file = new TFile("/minerva/data/users/dlast/merged_PC_files/"+top_dir+"/"+tool+"/"+name+".root","READ");
  TTree* input_tree = (TTree*)input_file->Get(tree_name);

  TFile* output_file = new TFile("../hists/"+top_dir+"/"+tool+"/"+name+"_history.root","RECREATE");

  int Emax = 1500;
  int Estep = 30;
  int FG_Emax = 100;
  int FG_Estep = 10;

  TString Elo;
  TString Ehi;

  double EFrac_neut[150];
  double EFrac_prot[150];
  double EFrac_pi0[150];
  double EFrac_pip[150];
  double EFrac_pim[150];
  double EFrac_photo[150];
  double EFrac_el[150];
  double EFrac_mu[150];
  double EFrac_other[150];
  double EFrac_non[150];


  vector<double> frac_vals;
  double total_Etrue;

  int primary_pdg[150];
  int n_blobs;
  double total_E[150];
  double Blob_VtxDZ[150];

  double vtx[4];
  double mc_vtx[4];

  output_file->cd();
  TH2D* h_history_US_FG = new TH2D("h_history_US_FG","Blob Energy Vs. Particle which mostly contributed to Blob Energy (Row Normalized, Target Region);;Blob Energy [MeV]",11,0,11,FG_Emax/FG_Estep,0,FG_Emax);
  TH2D* h_history_US = new TH2D("h_history_US","Blob Energy Vs. Particle which mostly contributed to Blob Energy (Row Normalized, Target Region);;Blob Energy [MeV]",11,0,11,Emax/Estep,0,Emax);
  TH2D* h_history_DS_FG = new TH2D("h_history_DS_FG","Blob Energy Vs. Particle which mostly contributed to Blob Energy (Row Normalized, Tracker Region);;Blob Energy [MeV]",11,0,11,FG_Emax/FG_Estep,0,FG_Emax);
  TH2D* h_history_DS = new TH2D("h_history_DS","Blob Energy Vs. Particle which mostly contributed to Blob Energy (Row Normalized, Tracker Region);;Blob Energy [MeV]",11,0,11,Emax/Estep,0,Emax);

  h_history_US->GetXaxis()->SetBinLabel(1,"n");
  h_history_US->GetXaxis()->SetBinLabel(2,"p");
  h_history_US->GetXaxis()->SetBinLabel(3,"#pi^{0}");
  h_history_US->GetXaxis()->SetBinLabel(4,"#pi^{+}");
  h_history_US->GetXaxis()->SetBinLabel(5,"#pi^{-}");
  h_history_US->GetXaxis()->SetBinLabel(6,"#gamma");
  h_history_US->GetXaxis()->SetBinLabel(7,"e");
  h_history_US->GetXaxis()->SetBinLabel(8,"#mu");
  h_history_US->GetXaxis()->SetBinLabel(9,"Other");
  h_history_US->GetXaxis()->SetBinLabel(10,"NonPart");
  h_history_US->GetXaxis()->SetBinLabel(11,"Mixed");

  h_history_US_FG->GetXaxis()->SetBinLabel(1,"n");
  h_history_US_FG->GetXaxis()->SetBinLabel(2,"p");
  h_history_US_FG->GetXaxis()->SetBinLabel(3,"#pi^{0}");
  h_history_US_FG->GetXaxis()->SetBinLabel(4,"#pi^{+}");
  h_history_US_FG->GetXaxis()->SetBinLabel(5,"#pi^{-}");
  h_history_US_FG->GetXaxis()->SetBinLabel(6,"#gamma");
  h_history_US_FG->GetXaxis()->SetBinLabel(7,"e");
  h_history_US_FG->GetXaxis()->SetBinLabel(8,"#mu");
  h_history_US_FG->GetXaxis()->SetBinLabel(9,"Other");
  h_history_US_FG->GetXaxis()->SetBinLabel(10,"NonPart");
  h_history_US_FG->GetXaxis()->SetBinLabel(11,"Mixed");

  h_history_DS->GetXaxis()->SetBinLabel(1,"n");
  h_history_DS->GetXaxis()->SetBinLabel(2,"p");
  h_history_DS->GetXaxis()->SetBinLabel(3,"#pi^{0}");
  h_history_DS->GetXaxis()->SetBinLabel(4,"#pi^{+}");
  h_history_DS->GetXaxis()->SetBinLabel(5,"#pi^{-}");
  h_history_DS->GetXaxis()->SetBinLabel(6,"#gamma");
  h_history_DS->GetXaxis()->SetBinLabel(7,"e");
  h_history_DS->GetXaxis()->SetBinLabel(8,"#mu");
  h_history_DS->GetXaxis()->SetBinLabel(9,"Other");
  h_history_DS->GetXaxis()->SetBinLabel(10,"NonPart");
  h_history_DS->GetXaxis()->SetBinLabel(11,"Mixed");

  h_history_DS_FG->GetXaxis()->SetBinLabel(1,"n");
  h_history_DS_FG->GetXaxis()->SetBinLabel(2,"p");
  h_history_DS_FG->GetXaxis()->SetBinLabel(3,"#pi^{0}");
  h_history_DS_FG->GetXaxis()->SetBinLabel(4,"#pi^{+}");
  h_history_DS_FG->GetXaxis()->SetBinLabel(5,"#pi^{-}");
  h_history_DS_FG->GetXaxis()->SetBinLabel(6,"#gamma");
  h_history_DS_FG->GetXaxis()->SetBinLabel(7,"e");
  h_history_DS_FG->GetXaxis()->SetBinLabel(8,"#mu");
  h_history_DS_FG->GetXaxis()->SetBinLabel(9,"Other");
  h_history_DS_FG->GetXaxis()->SetBinLabel(10,"NonPart");
  h_history_DS_FG->GetXaxis()->SetBinLabel(11,"Mixed");

  /*
  vector<TH1D*> h_history;
  for (int i=0; i < Emax/Estep; ++i){
    Elo = to_string(i*Estep);
    Ehi = to_string((i+1)*Estep);
    h_history.push_back(new TH1D("h_history_Elo_"+Elo+"_Ehi_"+Ehi,"Direct Particle Parent "+Elo+" MeV < Blob E < "+Ehi+" MeV;;Fraction",11,0,11));
    h_history.at(i)->GetXaxis()->SetBinLabel(1,"n");
    h_history.at(i)->GetXaxis()->SetBinLabel(2,"p");
    h_history.at(i)->GetXaxis()->SetBinLabel(3,"#pi^{0}");
    h_history.at(i)->GetXaxis()->SetBinLabel(4,"#pi^{+}");
    h_history.at(i)->GetXaxis()->SetBinLabel(5,"#pi^{-}");
    h_history.at(i)->GetXaxis()->SetBinLabel(6,"#gamma");
    h_history.at(i)->GetXaxis()->SetBinLabel(7,"e");
    h_history.at(i)->GetXaxis()->SetBinLabel(8,"#mu");
    h_history.at(i)->GetXaxis()->SetBinLabel(9,"Other");
    h_history.at(i)->GetXaxis()->SetBinLabel(10,"NonPart");
    h_history.at(i)->GetXaxis()->SetBinLabel(11,"Mixed");
  }
  */

  input_tree->SetBranchAddress(tree_name+"_vtx",&vtx);
  input_tree->SetBranchAddress("mc_vtx",&mc_vtx);

  input_tree->SetBranchAddress(tree_name+"_BlobTopMCPID",&primary_pdg);
  input_tree->SetBranchAddress(tree_name+"_BlobTopMCPID_sz",&n_blobs);

  input_tree->SetBranchAddress(tree_name+"_BlobTotalE",&total_E);
  input_tree->SetBranchAddress(tree_name+"_Blob2DVtxDZ",&Blob_VtxDZ);

  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Neutron",&EFrac_neut);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Proton",&EFrac_prot);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Pi0",&EFrac_pi0);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_PiPlus",&EFrac_pip);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_PiMinus",&EFrac_pim);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Photon",&EFrac_photo);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Election",&EFrac_el);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Muon",&EFrac_mu);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Others",&EFrac_other);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_NonParticle",&EFrac_non);

  int nentries = input_tree->GetEntries();
  int n100 = nentries/100;
  int n = 0;

  for (int i=0; i < nentries; ++i){
    input_tree->GetEntry(i);

    if (i%n100==0){
      cout << n << "%" << endl;
      n += 1;
    }

    for (int j=0; j < n_blobs; ++j){
      if (primary_pdg[j] != pdg_in || total_E[j] > Emax) continue;
      frac_vals = {EFrac_neut[j],EFrac_prot[j],EFrac_pi0[j],EFrac_pip[j],EFrac_pim[j],EFrac_photo[j],EFrac_el[j],EFrac_mu[j],EFrac_other[j],EFrac_non[j]};
      total_Etrue = accumulate(frac_vals.begin(), frac_vals.end(), 0.0);
      auto max_el = max_element(frac_vals.begin(), frac_vals.end());

      /*
      if (i==0){
	cout << "{ ";
	for (unsigned int k=0; k < frac_vals.size();++k){
	  cout << frac_vals.at(k) << ", ";
	}
	cout << "}" << endl;
	cout << total_Etrue << endl;
	cout << max_el-frac_vals.begin() << endl;
      }
      */


      if (*max_el/total_Etrue > 0.5){
	if((Blob_VtxDZ[j]+vtx[2]-mc_vtx[2]) < 0){
	  if(total_E[j] <= 100.0) h_history_US_FG->Fill(max_el-frac_vals.begin(),total_E[j]);
	  h_history_US->Fill(max_el-frac_vals.begin(),total_E[j]);
	}
	else{
	  if(total_E[j] <= 100.0) h_history_DS_FG->Fill(max_el-frac_vals.begin(),total_E[j]);
	  h_history_DS->Fill(max_el-frac_vals.begin(),total_E[j]);
	}
      }
      else{
	if((Blob_VtxDZ[j]+vtx[2]-mc_vtx[2]) < 0){
	  if(total_E[j] <= 100.0) h_history_US_FG->Fill(10,total_E[j]);
	  h_history_US->Fill(10,total_E[j]);
	}
	else{
	  if(total_E[j] <= 100.0) h_history_DS_FG->Fill(10,total_E[j]);
	  h_history_DS->Fill(10,total_E[j]);
	}
      }
    }
  }


  double scaler;

  for (unsigned int i=0; i < Emax/Estep; ++i){
    if (h_history_US->Integral(1,11,i,i) == 0) continue;
    scaler = 1.0/(h_history_US->Integral(1,11,i,i));
    for (unsigned int j=1;j<12;++j)h_history_US->SetBinContent(j,i,h_history_US->GetBinContent(j,i)*scaler);
  }
  for (unsigned int i=0; i < FG_Emax/FG_Estep+1; ++i){
    if (h_history_US_FG->Integral(1,11,i,i) == 0) continue;
    scaler = 1.0/(h_history_US_FG->Integral(1,11,i,i));
    for (unsigned int j=1;j<12;++j)h_history_US_FG->SetBinContent(j,i,h_history_US_FG->GetBinContent(j,i)*scaler);
  }
  for (unsigned int i=0; i < Emax/Estep; ++i){
    if (h_history_DS->Integral(1,11,i,i) == 0) continue;
    scaler = 1.0/(h_history_DS->Integral(1,11,i,i));
    for (unsigned int j=1;j<12;++j)h_history_DS->SetBinContent(j,i,h_history_DS->GetBinContent(j,i)*scaler);
  }
  for (unsigned int i=0; i < FG_Emax/FG_Estep +1; ++i){
    if (h_history_DS_FG->Integral(1,11,i,i) == 0) continue;
    scaler = 1.0/(h_history_DS_FG->Integral(1,11,i,i));
    for (unsigned int j=1;j<12;++j)h_history_DS_FG->SetBinContent(j,i,h_history_DS_FG->GetBinContent(j,i)*scaler);
  }

  h_history_US_FG->GetZaxis()->SetRangeUser(0,1);
  h_history_US->GetZaxis()->SetRangeUser(0,1);
  h_history_DS_FG->GetZaxis()->SetRangeUser(0,1);
  h_history_DS->GetZaxis()->SetRangeUser(0,1);

  input_file->Close();

  output_file->Write();
  output_file->Close();

  return 0;
}
