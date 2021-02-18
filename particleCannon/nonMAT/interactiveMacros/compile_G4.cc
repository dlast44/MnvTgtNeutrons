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
    cout << "Usage: compile_G4 <file_name_from_hists_down_minus_extension>" << endl;
    return 1;
  }

  TString name_root = argv[1];
  vector<TString> names = {"h_prot_angle_to_prim_v_parentID_US","h_prot_angle_to_prim_v_parentID_DS","h_prot_angle_to_vtx_line_v_parentID_US","h_prot_angle_to_vtx_line_v_parentID_DS","h_prot_angle_to_parent_v_parentID_US","h_prot_angle_to_parent_v_parentID_DS","h_prot_dist_to_vtx_v_parentID_US","h_prot_dist_to_vtx_v_parentID_DS","h_prot_z_dist_to_vtx_v_parentID_US","h_prot_z_dist_to_vtx_v_parentID_DS","h_prot_dist_to_vtx_v_prot_angle_to_vtx_US","h_prot_dist_to_vtx_v_prot_angle_to_vtx_DS"};

  //Currently leaving off h_prot_mom_v_BlobE_US/DS since they need some work.

  TH2D* hist2D;
  //TH1D* hist1D;

  TFile* input_file = new TFile("../hists/"+name_root+"_g4_plots.root","READ");

  gStyle->SetOptStat(0);

  TCanvas* c1 = new TCanvas("c1","c1",1600,1200);
  for (auto name:names){
    hist2D = (TH2D*)input_file->Get(name);
    hist2D->Draw("colz");
    c1->Print("../plots/"+name_root+"_"+name+".jpg");
    c1->Print("../plots/"+name_root+"_"+name+".pdf");
    c1->Clear();
  }

  input_file->Close();

  return 0;
}
