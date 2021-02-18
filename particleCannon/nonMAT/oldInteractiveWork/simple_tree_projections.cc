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
  if (argc != 5){
    cout << "Usage: simple_tree_projections <input_file_path> <output_file_path> <mass> <pdg>" << endl;
    return 1;
  }

  TString input_name = argv[1];
  TString output_name = argv[2];
  TString mass = argv[3];
  TString pdg = argv[4];
  TString branch;

  int lo_cut;
  int hi_cut;
  TString lo_cut_str;
  TString hi_cut_str;

  int BlobEBinSize = 100;
  int BlobEMax = 3000;

  TFile* input_file = new TFile(input_name,"READ");
  TTree* input_tree = (TTree*)input_file->Get("CCQENu");

  TFile* output_file = new TFile("../hists/"+output_name+"_projections.root","RECREATE");
  output_file->cd();
  for (int i=0; i < BlobEMax/BlobEBinSize; ++i){
    lo_cut = i*BlobEBinSize;
    hi_cut = lo_cut+BlobEBinSize;
    lo_cut_str = to_string(lo_cut);
    hi_cut_str = to_string(hi_cut);
    output_file->mkdir("BlobTotalE_Low_"+lo_cut_str+"_High_"+hi_cut_str);
  }
  //int marker = 0;

  cout << "File: " << input_name << " has " << input_tree->GetEntries() << " Events." << endl;
  
  ifstream inFile;
  inFile.open("PlottingBranches.txt");
  if (!inFile){
    cout << "PlottingBranches.txt did not succesfully open." << endl;
    return 2;
  }

  while (inFile >> branch){
    cout << branch << endl;
    output_file->cd();
    input_tree->Project("h_"+branch+"_NoSlicing","CCQENu_"+branch,"CCQENu_BlobMCPID == "+pdg);
    for (int i=0; i < BlobEMax/BlobEBinSize; ++i){
      lo_cut = i*BlobEBinSize;
      hi_cut = lo_cut+BlobEBinSize;
      lo_cut_str = to_string(lo_cut);
      hi_cut_str = to_string(hi_cut);
      output_file->cd("BlobTotalE_Low_"+lo_cut_str+"_High_"+hi_cut_str);
      input_tree->Project("h_"+branch+"_BlobTotalE_Low_"+lo_cut_str+"_High_"+hi_cut_str,"CCQENu_"+branch,"CCQENu_BlobMCPID == "+pdg+" && CCQENu_BlobTotalE >= "+lo_cut_str+" && CCQENu_BlobTotalE < "+hi_cut_str);
    }
  }

  output_file->Write();
  output_file->Close();

  inFile.close();
  return 0;
}
