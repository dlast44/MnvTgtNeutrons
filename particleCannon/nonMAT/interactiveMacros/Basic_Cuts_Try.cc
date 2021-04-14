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
#include <unordered_map>
#include <bitset>

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

struct G4_info{
  int PDG;
  int TrackID;
  int ParentID;
  int Prim_PDG;
  int Process;
  double FinX;
  double FinY;
  double FinZ;
  double FinT;
  double KinE;
  double Px;
  double Py;
  double Pz;
};

//This function makes some basic blob-level discrimination on variables and characterizes the blobs with a bit array of passing (1) or failing (0) a
//presumed cut. These are first-pass type cuts and are very preliminary.

int main(int argc, char* argv[]) {
  TString location;
  TString tool;
  TString top_dir;
  TString name;
  TString tag;
  int pdg_in;

  if (argc < 5 || argc > 7){
    cout << "Usage: Basic_Cuts_Try.exe <tool_name> <top_dir> <file_name> <file_tag> <pdg_in, default 0> <location>" << endl;
    cout << "<tool_name> is CCQENu or MAD" << endl;
    return 1;
  }
  else if (argc == 5){
    tool = argv[1];
    top_dir = argv[2];
    name = argv[3];
    tag = argv[4];
    pdg_in = 0;
    location = "minerva";
  }
  else if (argc == 6){
    tool = argv[1];
    top_dir = argv[2];
    name = argv[3];
    tag = argv[4];
    pdg_in = atoi(argv[5]);
    location = "minerva";
  }
  else{
    tool = argv[1];
    top_dir = argv[2];
    name = argv[3];
    tag = argv[4];
    pdg_in = atoi(argv[5]);
    location = argv[6];
  }
  cout << "pdg in is: " << pdg_in << endl;
  
  TString tree_name = "";
  if (tool == "CCQENu") tree_name = tool;
  else if (tool == "MAD") tree_name = "MasterAnaDev";
  else{ 
    cout << "NEED tool to be either CCQENu or MAD" << endl;
    return 2;
  }

  bool isMin = true;
  if (location == "personal") isMin = false;
  else if (location != "minerva"){
    cout << "No location other than Minerva GPVMS or personal machine currently supported in this script." << endl;
    return 3;
  }

  TFile* input_file;
  if (!isMin) input_file = new TFile("/mnt/Windows/Users/lastd/SWforPhysResearch/Minerva_Tuples/merged_PC_files/"+name+".root","READ");
  else input_file = new TFile("/minerva/data/users/dlast/merged_PC_files/"+top_dir+"/"+tool+"/"+name+".root","READ");
  TTree* input_tree = (TTree*)input_file->Get(tree_name);

  TFile* output_file;
  if (!isMin) output_file = new TFile("../../../../particleCannon/hists/"+name+"_cuts_plots_"+tag+".root","RECREATE");
  else output_file = new TFile("/minerva/data/users/dlast/Moved_from_app/particle_cannon/hists/"+top_dir+"/"+tool+"/"+name+"_cuts_plots_"+tag+".root","RECREATE");

  output_file->cd();


  unordered_map<int,int> PDGbins;
  PDGbins[2112] = 2;
  PDGbins[2212] = 3;
  PDGbins[111] = 4;
  PDGbins[211] = 5;
  PDGbins[-211] = 6;
  PDGbins[22] = 7;
  PDGbins[11] = 8;
  PDGbins[-11] = 8;
  PDGbins[13] = 9;
  PDGbins[-13] = 9;
  PDGbins[-999] = 1;
  
  TH2D* h_prim_KinE_v_parentID_passed_all_cuts_US = new TH2D("h_prim_KinE_v_parentID_passed_all_cuts_US","Primary Particle Kinetic Energy versus Blob Parent for blobs which passed cuts (Not Tracker);;Kinetic Energy[MeV]",10,0,10,50,0,5000);
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(3,"n");
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(4,"p");
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(9,"e");
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(10,"#mu");
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(1,"Other");
  h_prim_KinE_v_parentID_passed_all_cuts_US->GetXaxis()->SetBinLabel(2,"Multi");
  TH2D* h_prim_KinE_v_parentID_passed_all_cuts_DS = new TH2D("h_prim_KinE_v_parentID_passed_all_cuts_DS","Primary Particle Kinetic Energy versus Blob Parent for blobs which passed cuts (Tracker);;Kinetic Energy[MeV]",10,0,10,50,0,5000);
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(3,"n");
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(4,"p");
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(9,"e");
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(10,"#mu");
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(1,"Other");
  h_prim_KinE_v_parentID_passed_all_cuts_DS->GetXaxis()->SetBinLabel(2,"Multi");

  TH2D* h_prim_KinE_v_BlobE_passed_all_cuts_US = new TH2D("h_prim_KinE_v_BlobE_passed_all_cuts_US","Primary Particle Kinetic Energy versus Blob Parent for blobs which passed cuts (Not Tracker);Blob Energy[MeV];Kinetic Energy[MeV]",50,0,500,50,0,5000);
  TH2D* h_prim_KinE_v_BlobE_passed_all_cuts_DS = new TH2D("h_prim_KinE_v_BlobE_passed_all_cuts_DS","Primary Particle Kinetic Energy versus Blob Parent for blobs which passed cuts (Tracker);Blob Energy[MeV];Kinetic Energy[MeV]",50,0,500,50,0,5000);

  TH2D* h_prim_KinE_v_n_3D_blobs = new TH2D("h_prim_KinE_v_n_3D_blobs","Primary Particle Kinetic Energy versus No. 3D Blobs in Event;No.;Kinetic Energy[MeV]",5,0,5,50,0,5000);
  TH2D* h_prim_KinE_v_n_passed_all_cuts_US = new TH2D("h_prim_KinE_v_n_passed_all_cuts_US","Primary Particle Kinetic Energy versus No. Passed All Cuts in Tracker;No.;Kinetic Energy[MeV]",5,0,5,50,0,5000);
  TH2D* h_prim_KinE_v_n_passed_all_cuts_DS = new TH2D("h_prim_KinE_v_n_passed_all_cuts_DS","Primary Particle Kinetic Energy versus No. Passed All Cuts not in Tracker;No.;Kinetic Energy[MeV]",5,0,5,50,0,5000);
  TH2D* h_prim_KinE_v_n_passed_all_cuts = new TH2D("h_prim_KinE_v_n_passed_all_cuts","Primary Particle Kinetic Energy versus No. Passed All Cuts;No.;Kinetic Energy[MeV]",5,0,5,50,0,5000);
  
  TH2D* h_n_3D_blobs_v_n_passed_all_cuts = new TH2D("h_n_3D_blobs_v_n_passed_all_cuts","Per Event: No. 3D Blobs vs. No. Passed All Cuts (All Regions);No.;No.",5,0,5,5,0,5);
  TH2D* h_n_passed_all_cuts_US_v_n_passed_all_cuts_DS = new TH2D("h_n_passed_all_cuts_US_v_n_passed_all_cuts_DS","Per Event: No. Passed All Cuts not in Tracker vs. No. Passed All Cuts in Tracker;No.;No.",5,0,5,5,0,5);


  /*
  TH2D*  h_prim_end_Z_to_prim_v_parentID_US = new TH2D("h_prim_end_Z_to_prim_v_parentID_US","Angle between blob-producing proton momentum and the primary non-muon particle which led to it vs. its parent  particle type (Target);;Primary Final Z [mm]",10,0,10,1000,4000,8000);
  h_prim_end_Z_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(3,"n");
  h_prim_end_Z_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(4,"p");
  h_prim_end_Z_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prim_end_Z_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prim_end_Z_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prim_end_Z_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prim_end_Z_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(9,"e");
  h_prim_end_Z_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(10,"#mu");
  h_prim_end_Z_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(1,"Other");
  TH2D*  h_prim_end_Z_to_prim_v_parentID_DS = new TH2D("h_prim_end_Z_to_prim_v_parentID_DS","Angle between blob-producing proton momentum and the primary non-muon particle which led to it vs. its parent particle type (Tracker);;Primary Final Z [mm]",10,0,10,1000,4000,8000);
  h_prim_end_Z_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(3,"n");
  h_prim_end_Z_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(4,"p");
  h_prim_end_Z_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prim_end_Z_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prim_end_Z_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prim_end_Z_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prim_end_Z_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(9,"e");
  h_prim_end_Z_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(10,"#mu");
  h_prim_end_Z_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(1,"Other");

  TH2D*  h_prot_angle_to_prim_v_parentID_US = new TH2D("h_prot_angle_to_prim_v_parentID_US","Angle between blob-producing proton momentum and the primary non-muon particle which led to it vs. its parent  particle type (Target);;Angle[rad]",10,0,10,160,0,3.2);
  h_prot_angle_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(3,"n");
  h_prot_angle_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(4,"p");
  h_prot_angle_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_angle_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_angle_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_angle_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_angle_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(9,"e");
  h_prot_angle_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_angle_to_prim_v_parentID_US->GetXaxis()->SetBinLabel(1,"Other");
  TH2D*  h_prot_angle_to_prim_v_parentID_DS = new TH2D("h_prot_angle_to_prim_v_parentID_DS","Angle between blob-producing proton momentum and the primary non-muon particle which led to it vs. its parent particle type (Tracker);;Angle[rad]",10,0,10,160,0,3.2);
  h_prot_angle_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(3,"n");
  h_prot_angle_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(4,"p");
  h_prot_angle_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_angle_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_angle_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_angle_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_angle_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(9,"e");
  h_prot_angle_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_angle_to_prim_v_parentID_DS->GetXaxis()->SetBinLabel(1,"Other");

  TH2D*  h_prot_angle_to_vtx_line_v_parentID_US = new TH2D("h_prot_angle_to_vtx_line_v_parentID_US","Angle between blob-producing proton momentum and a line from the vertex to the particle's start (Target);;Angle[rad]",10,0,10,160,0,3.2); 
  h_prot_angle_to_vtx_line_v_parentID_US->GetXaxis()->SetBinLabel(3,"n");
  h_prot_angle_to_vtx_line_v_parentID_US->GetXaxis()->SetBinLabel(4,"p");
  h_prot_angle_to_vtx_line_v_parentID_US->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_angle_to_vtx_line_v_parentID_US->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_angle_to_vtx_line_v_parentID_US->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_angle_to_vtx_line_v_parentID_US->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_angle_to_vtx_line_v_parentID_US->GetXaxis()->SetBinLabel(9,"e");
  h_prot_angle_to_vtx_line_v_parentID_US->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_angle_to_vtx_line_v_parentID_US->GetXaxis()->SetBinLabel(1,"Other");
 TH2D*  h_prot_angle_to_vtx_line_v_parentID_DS = new TH2D("h_prot_angle_to_vtx_line_v_parentID_DS","Angle between blob-producing proton momentum and a line from the vertex to the particle's start (Tracker);;Angle[rad]",10,0,10,160,0,3.2);
  h_prot_angle_to_vtx_line_v_parentID_DS->GetXaxis()->SetBinLabel(3,"n");
  h_prot_angle_to_vtx_line_v_parentID_DS->GetXaxis()->SetBinLabel(4,"p");
  h_prot_angle_to_vtx_line_v_parentID_DS->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_angle_to_vtx_line_v_parentID_DS->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_angle_to_vtx_line_v_parentID_DS->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_angle_to_vtx_line_v_parentID_DS->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_angle_to_vtx_line_v_parentID_DS->GetXaxis()->SetBinLabel(9,"e");
  h_prot_angle_to_vtx_line_v_parentID_DS->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_angle_to_vtx_line_v_parentID_DS->GetXaxis()->SetBinLabel(1,"Other");

  TH2D* h_prot_angle_to_parent_v_parentID_US = new TH2D("h_prot_angle_to_parent_v_parentID_US","Angle between blob-producign proton mom. and its parent's mom. vs. parent particle type (Target);;Angle[rad]",10,0,10,160,0,3.2);
  h_prot_angle_to_parent_v_parentID_US->GetXaxis()->SetBinLabel(3,"n");
  h_prot_angle_to_parent_v_parentID_US->GetXaxis()->SetBinLabel(4,"p");
  h_prot_angle_to_parent_v_parentID_US->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_angle_to_parent_v_parentID_US->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_angle_to_parent_v_parentID_US->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_angle_to_parent_v_parentID_US->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_angle_to_parent_v_parentID_US->GetXaxis()->SetBinLabel(9,"e");
  h_prot_angle_to_parent_v_parentID_US->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_angle_to_parent_v_parentID_US->GetXaxis()->SetBinLabel(1,"Other");
  TH2D* h_prot_angle_to_parent_v_parentID_DS = new TH2D("h_prot_angle_to_parent_v_parentID_DS","Angle between blob-producign proton mom. and its parent's mom. vs. parent particle type (Tracker);;Angle[rad]",10,0,10,160,0,3.2);
  h_prot_angle_to_parent_v_parentID_DS->GetXaxis()->SetBinLabel(3,"n");
  h_prot_angle_to_parent_v_parentID_DS->GetXaxis()->SetBinLabel(4,"p");
  h_prot_angle_to_parent_v_parentID_DS->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_angle_to_parent_v_parentID_DS->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_angle_to_parent_v_parentID_DS->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_angle_to_parent_v_parentID_DS->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_angle_to_parent_v_parentID_DS->GetXaxis()->SetBinLabel(9,"e");
  h_prot_angle_to_parent_v_parentID_DS->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_angle_to_parent_v_parentID_DS->GetXaxis()->SetBinLabel(1,"Other");

  TH2D* h_prot_dist_to_vtx_v_parentID_US = new TH2D("h_prot_dist_to_vtx_v_parentID_US","Distance from blob-producing proton initial position to true vertex location vs. prot. parent particle type (Target);;Dist.[mm]",10,0,10,300,0,3000);
  h_prot_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(3,"n");
  h_prot_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(4,"p");
  h_prot_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(9,"e");
  h_prot_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(1,"Other");
  TH2D* h_prot_dist_to_vtx_v_parentID_DS = new TH2D("h_prot_dist_to_vtx_v_parentID_DS","Distance from blob-producing proton initial position to true vertex location vs. prot. parent particle type (Tracker);;Dist.[mm]",10,0,10,300,0,3000);
  h_prot_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(3,"n");
  h_prot_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(4,"p");
  h_prot_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(9,"e");
  h_prot_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(1,"Other");
  TH2D* h_prot_z_dist_to_vtx_v_parentID_US = new TH2D("h_prot_z_dist_to_vtx_v_parentID_US","Z Distance from blob-producing proton initial position to true vertex location vs. prot. parent particle type (Target);;Dist.[mm]",10,0,10,110,-1000,100);
  h_prot_z_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(3,"n");
  h_prot_z_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(4,"p");
  h_prot_z_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_z_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_z_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_z_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_z_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(9,"e");
  h_prot_z_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_z_dist_to_vtx_v_parentID_US->GetXaxis()->SetBinLabel(1,"Other");
  TH2D* h_prot_z_dist_to_vtx_v_parentID_DS = new TH2D("h_prot_z_dist_to_vtx_v_parentID_DS","Z Distance from blob-producing proton initial position to true vertex location vs. prot. parent particle type (Tracker);;Dist.[mm]",10,0,10,110,-100,1000);
  h_prot_z_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(3,"n");
  h_prot_z_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(4,"p");
  h_prot_z_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(5,"#pi^{0}");
  h_prot_z_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(6,"#pi^{+}");
  h_prot_z_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(7,"#pi^{-}");
  h_prot_z_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(8,"#gamma");
  h_prot_z_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(9,"e");
  h_prot_z_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(10,"#mu");
  h_prot_z_dist_to_vtx_v_parentID_DS->GetXaxis()->SetBinLabel(1,"Other");

  TH2D* h_prot_dist_to_vtx_v_prot_angle_to_vtx_US = new TH2D("h_prot_dist_to_vtx_v_prot_angle_to_vtx_US","Blob-producing proton initial distance from vertex vs. angle between proton momentum and line to vertex (Target);Angle [rad];Dist. [mm]",160,0,3.2,300,0,3000);
  TH2D* h_prot_dist_to_vtx_v_prot_angle_to_vtx_DS = new TH2D("h_prot_dist_to_vtx_v_prot_angle_to_vtx_DS","Blob-producing proton initial distance from vertex vs. angle between proton momentum and line to vertex (Tracker);Angle [rad];Dist.[mm]",160,0,3.2,300,0,3000);

  TH2D* h_prot_mom_v_BlobE_US = new TH2D("h_prot_mom_v_BlobE_US","Momentum for blob-producing protons vs. Blob Energy (Target);Blob E [MeV];Momentum [MeV]",150,0,1500,1000,0,5000);
  TH2D* h_prot_mom_v_BlobE_DS = new TH2D("h_prot_mom_v_BlobE_DS","Momentum for blob-producing protons vs. Blob Energy (Tracker);Blob E [MeV];Momentum [MeV]",150,0,1500,1000,0,5000);

  //TH1D* h_prot_mom_US;
  //TH1D* h_prot_mom_DS;
  //Any others?


  //Plots I'm adding starting on 03/01/2021
  TH2D* h_prim_KinE_v_prot_z_dist_to_vtx_US = new TH2D("h_prim_KinE_v_prot_z_dist_to_vtx_US","Primary Particle kin. E vs. blob-producing proton Z dist. to vtx. (Target);Z Dist. [mm];Kinetic Energy [MeV]",110,-1000,100,50,0,5000);
  TH2D* h_prim_KinE_v_prot_z_dist_to_vtx_DS = new TH2D("h_prim_KinE_v_prot_z_dist_to_vtx_DS","Primary Particle kin. E vs. blob-producing proton Z dist. to vtx. (Tracker);Z Dist. [mm];Kinetic Energy [MeV]",110,-100,1000,50,0,5000);

  TH2D* h_prim_KinE_v_prot_mom_US = new TH2D("h_prim_KinE_v_prot_mom_US","Primary Particle kin. E vs. blob-producing proton momentum (Target);Proton p [MeV];Kinetic Energy [MeV]",50,0,5000,50,0,5000);
  TH2D* h_prim_KinE_v_prot_mom_DS = new TH2D("h_prim_KinE_v_prot_mom_DS","Primary Particle kin. E vs. blob-producing proton momentum (Tracker);Proton p [MeV];Kinetic Energy [MeV]",50,0,5000,50,0,5000);

  TH2D* h_prim_KinE_v_nprot_blobs = new TH2D("h_prim_KinE_v_nprot_blobs","Primary Particle kin. E vs. No. of blobs produced by protons;No.;Kinetic Energy [MeV]",10,0,10,50,0,5000);

  TH2D* h_prim_KinE_v_multiplicity = new TH2D("h_prim_KinE_v_multiplicity","Primary Particle Kin. E vs. No. Tracks at vertex;No.;Kinetic Energy [MeV]",10,0,10,50,0,5000);
  TH2D* h_prim_KinE_v_nBlobTracks = new TH2D("h_prim_KinE_v_nBlobTracks","Primary Particle Kin. E vs. No. Blobs with a fit Track;No.;Kinetic Energy [MeV]",10,0,10,50,0,5000);
  TH2D* h_prim_KinE_v_nBlobIncTracks = new TH2D("h_prim_KinE_v_nBlobIncTracks","Primary Particle Kin. E vs. No. Tracks fit across all blob clusters;No.;Kinetic Energy [MeV]",10,0,10,50,0,5000);

  TH2D* h_prim_KinE_v_nblobs = new TH2D("h_prim_KinE_v_nblobs","Primary Particle kin. E vs. No. of blobs;No.;Kinetic Energy [MeV]",25,0,25,50,0,5000);

  TH2D* h_prim_KinE_v_multiplicity_w_prot = new TH2D("h_prim_KinE_v_multiplicity_w_prot","Primary Particle Kin. E vs. No. Tracks at vertex (Evt. has prot. blob);No.;Kinetic Energy [MeV]",10,0,10,50,0,5000);
  TH2D* h_prim_KinE_v_nBlobTracks_w_prot = new TH2D("h_prim_KinE_v_nBlobTracks_w_prot","Primary Particle Kin. E vs. No. Blobs with a fit Track (Evt. has prot. blob);No.;Kinetic Energy [MeV]",10,0,10,50,0,5000);
  TH2D* h_prim_KinE_v_nBlobIncTracks_w_prot = new TH2D("h_prim_KinE_v_nBlobIncTracks_w_prot","Primary Particle Kin. E vs. No. Tracks fit across all blob clusters (Evt. has prot. blob);No.;Kinetic Energy [MeV]",10,0,10,50,0,5000);

  TH2D* h_prim_KinE_v_nblobs_w_prot = new TH2D("h_prim_KinE_v_nblobs_w_prot","Primary Particle kin. E vs. No. of blobs (Evt. has prot. blob);No.;Kinetic Energy [MeV]",25,0,25,50,0,5000);

  TH1D* h_prim_KinE = new TH1D("h_prim_KinE","Primary Particle Kin. E;Kinetic Energy [MeV];Events",100,0,5000);

  //Plots I'm adding on 03/18/2021... Might want to break this code apart or at least organize it better... These are in as quick a response as possible to the suggestions by Christopher on 03/17/2021...
  TH2D* h_prim_KinE_v_n_nonprot_blobs = new TH2D("h_prim_KinE_v_n_nonprot_blobs","Primary Particle kin. E vs. No. of non-prot blobs;No.;Kinetic Energy [MeV]",25,0,25,50,0,5000);
  //TH2D* h_prim_KinE_v_blob_in_first_plane = TH2D("h_prim_KinE_v_blob_in_first_plane","Primary Particle kin. E vs. Bool of blob in first plane.;Blob In First  Plane?;Kinetic Energy [MeV]",2,0,2,50,0,5000);
  */  

  double vtx[4];
  double mc_vtx[4];
  int nTracks;

  input_tree->SetBranchAddress(tree_name+"_vtx",&vtx);
  input_tree->SetBranchAddress("mc_vtx",&mc_vtx);
  input_tree->SetBranchAddress("multiplicity",&nTracks);

  //G4 Variables
  int g4_max=6750; //Might not work for neutrino MC, be careful.
  int g4_size;
  int g4_pdg[g4_max];
  int g4_ID[g4_max];
  int g4_parentID[g4_max];
  int g4_prim_pdg[g4_max];
  int g4_proc[g4_max];
  double g4_fin_x[g4_max];
  double g4_fin_y[g4_max];
  double g4_fin_z[g4_max];
  double g4_fin_t[g4_max];
  double g4_KE[g4_max];
  double g4_px[g4_max];
  double g4_py[g4_max];
  double g4_pz[g4_max];

  input_tree->SetBranchAddress("G4_PDG",&g4_pdg);
  input_tree->SetBranchAddress("G4_PDG_sz",&g4_size);
  input_tree->SetBranchAddress("G4_TrackID",&g4_ID);
  input_tree->SetBranchAddress("G4_ParentTrackID",&g4_parentID);
  input_tree->SetBranchAddress("G4_Primary_PDG",&g4_prim_pdg);
  input_tree->SetBranchAddress("G4_Process",&g4_proc);
  input_tree->SetBranchAddress("G4_FinalX",&g4_fin_x);
  input_tree->SetBranchAddress("G4_FinalY",&g4_fin_y);
  input_tree->SetBranchAddress("G4_FinalZ",&g4_fin_z);
  input_tree->SetBranchAddress("G4_FinalT",&g4_fin_t);
  input_tree->SetBranchAddress("G4_InitKinE",&g4_KE);
  input_tree->SetBranchAddress("G4_InitPx",&g4_px);
  input_tree->SetBranchAddress("G4_InitPy",&g4_py);
  input_tree->SetBranchAddress("G4_InitPz",&g4_pz);

  unordered_map<int, G4_info> g4_tracks;
  G4_info empty_track = {-999,-999,-999,-999,-999,-999,-999,-999,-999,-999,-999,-999,-999};
  
  //Blob Variables
  int nBlobs;
  int nBlobTracks;
  int nBlobIncTracks;
  int Blob_g4_ID[150];
  int Blob_g4_TopMCPID[150];
  int Blob_ID[150];
  int Blob_ClusID[1500];
  int Blob_nClus[150];
  int Blob_Is3D[150];
  double Blob_total_E[150];
  double Blob_EFrac_neut[150];
  double Blob_EFrac_prot[150];
  double Blob_EFrac_pi0[150];
  double Blob_EFrac_pip[150];
  double Blob_EFrac_pim[150];
  double Blob_EFrac_photo[150];
  double Blob_EFrac_el[150];
  double Blob_EFrac_mu[150];
  double Blob_EFrac_other[150];
  double Blob_EFrac_non[150];
  double Blob_VtxDZ[150];
  double Blob_X[150];
  double Blob_Y[150];
  double Blob_Z[150];
  double Blob_EndX[150];
  double Blob_EndY[150];
  double Blob_EndZ[150];
  //double Blob_ZPerCluster[1500];
  
  input_tree->SetBranchAddress(tree_name+"_BlobMCTrackID_sz",&nBlobs);
  input_tree->SetBranchAddress(tree_name+"_BlobMCTrackID",&Blob_g4_ID);
  input_tree->SetBranchAddress(tree_name+"_EvtHasNBlobTracks",&nBlobTracks);
  input_tree->SetBranchAddress(tree_name+"_EvtHasNBlobIncTracks",&nBlobIncTracks);
  input_tree->SetBranchAddress(tree_name+"_BlobTopMCPID",&Blob_g4_TopMCPID);
  input_tree->SetBranchAddress(tree_name+"_BlobTotalE",&Blob_total_E);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Neutron",&Blob_EFrac_neut);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Proton",&Blob_EFrac_prot);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Pi0",&Blob_EFrac_pi0);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_PiPlus",&Blob_EFrac_pip);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_PiMinus",&Blob_EFrac_pim);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Photon",&Blob_EFrac_photo);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Election",&Blob_EFrac_el);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Muon",&Blob_EFrac_mu);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_Others",&Blob_EFrac_other);
  input_tree->SetBranchAddress(tree_name+"_MCEnergyFrac_NonParticle",&Blob_EFrac_non);
  input_tree->SetBranchAddress(tree_name+"_Blob2DVtxDZ",&Blob_VtxDZ);
  //input_tree->SetBranchAddress(tree_name+"_BlobZPerCluster",&Blob_ZPerCluster);
  input_tree->SetBranchAddress(tree_name+"_BlobClusID",&Blob_ClusID);
  input_tree->SetBranchAddress(tree_name+"_BlobID",&Blob_ID);
  input_tree->SetBranchAddress(tree_name+"_BlobIs3D",&Blob_Is3D);
  input_tree->SetBranchAddress(tree_name+"_BlobNClusters",&Blob_nClus);
  input_tree->SetBranchAddress(tree_name+"_BlobBegX",&Blob_X);
  input_tree->SetBranchAddress(tree_name+"_BlobBegY",&Blob_Y);
  input_tree->SetBranchAddress(tree_name+"_BlobBegZ",&Blob_Z);
  input_tree->SetBranchAddress(tree_name+"_BlobEndX",&Blob_EndX);
  input_tree->SetBranchAddress(tree_name+"_BlobEndY",&Blob_EndY);
  input_tree->SetBranchAddress(tree_name+"_BlobEndZ",&Blob_EndZ);
  
  vector<double> frac_vals;
  double total_Etrue;

  int bad_vtx;

  int entries=input_tree->GetEntries();
  //Short Test Run hard code... should make input at some point
  //entries = 100;

  //N Cuts needs to be equal here to later
  bitset<6> passedDS{"111111"};
  bitset<6> passedUS{"111101"};
    
  for (int i=0; i < entries; ++i){
    int n3D=0;
    int nPassedUS=0;
    int nPassedDS=0;
    int nPassedAO=0;
    int nPassedTJ=0;
    
    g4_tracks.clear();
    cout << "" << endl;
    cout << "Event: " << i << endl;
    input_tree->GetEvent(i);

    //cout << nBlobTracks << endl;

    TVector3 vert(vtx[0],vtx[1],vtx[2]);
    cout << "VertexPos: " << vert.X() << ", " << vert.Y() << ", " << vert.Z() << endl;

    //Skip events with vertex not in expected location of Tgt. 5 using definition from latest version of Jorge's code. For now, not requiriing the vertex be "in apothem".
    if (vert.Z() < 5756.71 || vert.Z() > 5801.24){
      cout << "Skipping Bad Vertex" << endl;
      ++bad_vtx;
      continue;
    }
    
    if (g4_size > g4_max || g4_size <= 0){
      cout << "UH OH. If the following number is 0, there is no (or too much) G4 info for this event for some reason. Maybe CCQENu some funny pre-G4 cut. Skipping Event." << endl;
      cout << g4_size << endl;
      cout << "" << endl;
      continue;
    }
    
    for (int j=0; j < g4_size;++j){
      G4_info g4_track = {g4_pdg[j],g4_ID[j],g4_parentID[j],g4_prim_pdg[j],g4_proc[j],g4_fin_x[j],g4_fin_y[j],g4_fin_z[j],g4_fin_t[j],g4_KE[j],g4_px[j],g4_py[j],g4_pz[j]};
      g4_tracks[g4_track.TrackID] = g4_track;
    }
    G4_info g4_prim_part = g4_tracks[1];

    if (g4_prim_part.PDG != pdg_in){
      cout << "PDG didn't match input... just a warning..." << endl;  
    }
    
    for (int j=0; j < nBlobs; ++j){
      //Current Blob-level "Cuts" And their bit number/object defintion
      //passedTJ. Passes Tejin's selection (leaving as default 1 if 3D for now. Tejin requires 3D, at least 100mm downstream of vertex, at least 100mm away from muon and outside 15deg muon cone... energy cut seems to be implied, but I have yet to find it in the documentation, it's Q^2 dependent so I think I don't wish to apply that yet...)
      //passedAO. Passes Andrew's selection (he selects different clusters, so this remains to be seen whether this is particularly useful...)
      //5. Overlaps with either scintillator plane immediately adjacent to target
      //4. Magnitude of z dist. (as calculated from beginning position) < 100 mm. most (~80%) of the way from tgt. 5 to tgt. 4 according to my z dist prot plots.
      //3. Energy < 50MeV
      //2. Angle of "blob direction" w.r.t. "blob flight path" NOT between 0.2 and 0.7 radians based on tracker region plots for proton angle. (should be OK for target region blobs too...)
      //   Define blob direction to be -blob flight path if beg and end position are the same (should only be non3D blobs...)
      //1. Non-Tracker blob. Define that either blob beg z or end z isn't in tracker region
      //0. !BlobIs3D

      int passedAO=0;//Currently require 3D as well...
      int passedTJ=0;//Make 1 if 3D...
      bitset<6> cuts{"000000"};

      if (Blob_Is3D[j]){
	cuts.flip(0);
	if (abs(Blob_Z[j]-vert.Z()) < 1500.0 && Blob_total_E[j] > 1.5){
	  passedAO = 1;
	  ++nPassedAO;
	}
	passedTJ = 1;
	++nPassedTJ;
	++n3D;
      }
      
      TVector3 BlobStart;
      TVector3 BlobEnd;
      if (abs(Blob_Z[j]-vert.Z()) > abs(Blob_EndZ[j]-vert.Z())){
	cout << "Blob Direction is backwards..." << endl;
	BlobStart.SetXYZ(Blob_EndX[j],Blob_EndY[j],Blob_EndZ[j]);
	BlobEnd.SetXYZ(Blob_X[j],Blob_Y[j],Blob_Z[j]);
      }
      else{
	BlobStart.SetXYZ(Blob_X[j],Blob_Y[j],Blob_Z[j]);
	BlobEnd.SetXYZ(Blob_EndX[j],Blob_EndY[j],Blob_EndZ[j]);
      }

      if (5990.0 < BlobStart.Z()  && BlobStart.Z() < 8340.0 && 5990.0 < BlobEnd.Z() && BlobEnd.Z() < 8340.0) cuts.flip(1);
      
      TVector3 BlobDirection = BlobEnd-BlobStart;
      TVector3 BlobFlightPath = BlobStart-vert;
      if (BlobDirection.Mag() == 0){
	if (BlobFlightPath.Mag() == 0){
	  cout << "Blob Is 0 size and vertex... Removing from 3D count and not using... though I doubt this line ever happens..." << endl;
	  if (Blob_Is3D[j]){
	    if (passedAO){
	      --nPassedAO;
	    }
	    if (passedTJ){
	      --nPassedTJ;
	    }
	    --n3D;
	  }
	  continue;
	}
	BlobDirection = BlobFlightPath;
      }
      else if (BlobFlightPath.Mag() == 0){
	BlobFlightPath = BlobDirection;
      }

      double BlobAngle = BlobFlightPath.Angle(BlobDirection);

      if (BlobAngle < 0.7 && BlobAngle > 0.2) cuts.flip(2);

      if (Blob_total_E[j] >= 20.0) cuts.flip(3);

      if (abs(BlobFlightPath.Z()) >= 100.0) cuts.flip(4);

      if (((BlobStart.Z()>5801.24 && BlobEnd.Z()>5801.24)||(BlobStart.Z()<5756.71 && BlobEnd.Z()<5756.71))) cuts.flip(5);
      
      cout << "Blob: " << j << " has cut profile: " << cuts << endl;
      /* Verification Info
      cout << "Is3D: " << Blob_Is3D[j] << endl;
      cout << "StartPos: " << BlobStart.X() << ", " << BlobStart.Y() << ", " << BlobStart.Z() << endl;
      cout << "EndPos: " << BlobEnd.X() << ", " << BlobEnd.Y() << ", " << BlobEnd.Z() << endl;
      cout << "Direction: " << BlobDirection.X() << ", " << BlobDirection.Y() << ", " << BlobDirection.Z() << endl;
      cout << "FlightPath: " << BlobFlightPath.X() << ", " << BlobFlightPath.Y() << ", " << BlobFlightPath.Z() << endl;
      cout << "Angle: " << BlobAngle << endl;
      cout << "Energy: " << Blob_total_E[j] << endl;
      */

      G4_info g4_blob_part;
      
      frac_vals = {Blob_EFrac_neut[j],Blob_EFrac_prot[j],Blob_EFrac_pi0[j],Blob_EFrac_pip[j],Blob_EFrac_pim[j],Blob_EFrac_photo[j],Blob_EFrac_el[j],Blob_EFrac_mu[j],Blob_EFrac_other[j],Blob_EFrac_non[j]};
      total_Etrue = accumulate(frac_vals.begin(), frac_vals.end(), 0.0);
      auto max_el = max_element(frac_vals.begin(), frac_vals.end());
      if (*max_el/total_Etrue < 0.5){
	g4_blob_part = empty_track;
      }
      else g4_blob_part = g4_tracks[Blob_g4_ID[j]];

      if (cuts==passedUS){
	++nPassedUS;
	h_prim_KinE_v_parentID_passed_all_cuts_US->Fill(PDGbins[g4_blob_part.PDG],g4_prim_part.KinE);
	h_prim_KinE_v_BlobE_passed_all_cuts_US->Fill(Blob_total_E[j],g4_prim_part.KinE);
      }
      else if (cuts==passedDS){
	++nPassedDS;
	h_prim_KinE_v_parentID_passed_all_cuts_DS->Fill(PDGbins[g4_blob_part.PDG],g4_prim_part.KinE);
	h_prim_KinE_v_BlobE_passed_all_cuts_DS->Fill(Blob_total_E[j],g4_prim_part.KinE);
      }

    }

    cout << "No. 3D Blobs... " << n3D << endl;
    cout << "No. Passed US Blobs... " << nPassedUS << endl;	
    cout << "No. Passed DS Blobs... " << nPassedDS << endl;
    
    h_prim_KinE_v_n_3D_blobs->Fill(n3D,g4_prim_part.KinE);
    h_prim_KinE_v_n_passed_all_cuts_US->Fill(nPassedUS,g4_prim_part.KinE);
    h_prim_KinE_v_n_passed_all_cuts_DS->Fill(nPassedDS,g4_prim_part.KinE);

    h_n_3D_blobs_v_n_passed_all_cuts->Fill(nPassedDS+nPassedUS,n3D);
    h_n_passed_all_cuts_US_v_n_passed_all_cuts_DS->Fill(nPassedDS,nPassedUS);
    
  }
  h_prim_KinE_v_n_passed_all_cuts->Add(h_prim_KinE_v_n_passed_all_cuts_US);
  h_prim_KinE_v_n_passed_all_cuts->Add(h_prim_KinE_v_n_passed_all_cuts_DS);
      
  cout << "" << endl;
  cout << "% Bad Vertex: " << bad_vtx/entries*100.0 << endl;
  
  output_file->Write();
  input_file->Close();
  output_file->Close();

  return 0;
}
