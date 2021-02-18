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
    cout << "Usage: compile_history <file_name_from_hists_down_minus_extension>" << endl;
    return 1;
  }

  TString name_root = argv[1];
  vector<TString> names = {"h_history_US","h_history_US_FG","h_history_DS","h_history_DS_FG"};

  TH2D* hist;

  TFile* input_file = new TFile("../hists/"+name_root+"_history.root","READ");

  gStyle->SetOptStat(0);

  TCanvas* c1 = new TCanvas("c1","c1",1600,1200);
  for (auto name:names){
    hist = (TH2D*)input_file->Get(name);
    hist->Draw("colz");
    c1->Print("../plots/"+name_root+"_"+name+".jpg");
    c1->Print("../plots/"+name_root+"_"+name+".pdf");
    c1->Clear();
  }

  input_file->Close();

  return 0;
}
