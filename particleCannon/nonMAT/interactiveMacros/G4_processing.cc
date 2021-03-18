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

int main(int argc, char* argv[]) {
  TString location;
  TString tool;
  TString top_dir;
  TString name;
  TString tag;
  int pdg_in;

  if (argc < 5 || argc > 7){
    cout << "Usage: particle_history <tool_name> <top_dir> <file_name> <file_tag> <pdg_in, default 0> <location>" << endl;
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
  if (!isMin) output_file = new TFile("../../../../particleCannon/hists/"+name+"_g4_plots_"+tag+".root","RECREATE");
  else output_file = new TFile("/minerva/data/users/dlast/Moved_from_app/particle_cannon/hists/"+top_dir+"/"+tool+"/"+name+"_g4_plots_"+tag+".root","RECREATE");
  //TFile* output_file = new TFile("/minerva/data/users/dlast/Moved_from_app/particle_cannon/hists/"+top_dir+"/"+tool+"/"+name+"_g4_plots_small_sample_SL6.root","RECREATE");
  //TFile* output_file = new TFile("/minerva/data/users/dlast/Moved_from_app/particle_cannon/hists/"+top_dir+"/"+tool+"/"+name+"_g4_plots_KinE_cutoff_500MeV.root","RECREATE");

  output_file->cd();

  //Additional non-proton specific plots//

  //////////////////////////////////////PLOTS TO MAKE
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

  //TH1D* h_prot_parent_ID;

  //TH1D* h_prot_angle_to_prim_US;
  //TH1D* h_prot_angle_to_prim_DS;

  //TH1D* h_prot_angle_to_parent_US;
  //TH1D* h_prot_angle_to_parent_DS;

  //TH1D* h_prot_angle_to_vtx_line_US;
  //TH1D* h_prot_angle_to_vtx_line_DS;

  //Did parentage contain neutron? (Andrew GEANT neutrons)
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


  //  TH1D* h_prot_dist_to_vtx_US;
  //TH1D* h_prot_dist_to_vtx_DS;
  //TH1D* h_prot_z_dist_to_vtx_US;
  //TH1D* h_prot_z_dist_to_vtx_DS;

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

  //Blob Variables
  int nBlobs;
  int nBlobTracks;
  int nBlobIncTracks;
  int Blob_g4_ID[150];
  int Blob_g4_TopMCPID[150];
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

  vector<double> frac_vals;
  double total_Etrue;

  for (int i=0; i < input_tree->GetEntries(); ++i){
    g4_tracks.clear();
    cout << "Event: " << i << endl;
    input_tree->GetEvent(i);
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
    //Just here as a kinetic energy cutoff. Hard-coded.
    //if(g4_prim_part.KinE > 500.0) continue;
    TVector3 prim_p(g4_prim_part.Px,g4_prim_part.Py,g4_prim_part.Pz);

    G4_info g4_blob_part;
    int primary_blobs = 0;
    int proton_blobs = 0;

    //if (!pdg_in) cout << "No requirement for the particle identity of track ID 1." << endl;
    if (g4_prim_part.PDG != pdg_in){
      cout << "ISSUE WITH G4 Info (primary non-muon particle is not track with ID 1)" << endl;
      continue;
    }

    h_prim_KinE->Fill(g4_prim_part.KinE);
    h_prim_KinE_v_nblobs->Fill(nBlobs,g4_prim_part.KinE);
    h_prim_KinE_v_multiplicity->Fill(nTracks,g4_prim_part.KinE);
    h_prim_KinE_v_nBlobTracks->Fill(nBlobTracks,g4_prim_part.KinE);
    h_prim_KinE_v_nBlobIncTracks->Fill(nBlobIncTracks,g4_prim_part.KinE);
    
    for (int j=0; j < nBlobs; ++j){
      frac_vals = {Blob_EFrac_neut[j],Blob_EFrac_prot[j],Blob_EFrac_pi0[j],Blob_EFrac_pip[j],Blob_EFrac_pim[j],Blob_EFrac_photo[j],Blob_EFrac_el[j],Blob_EFrac_mu[j],Blob_EFrac_other[j],Blob_EFrac_non[j]};
      total_Etrue = accumulate(frac_vals.begin(), frac_vals.end(), 0.0);
      auto max_el = max_element(frac_vals.begin(), frac_vals.end());
      if (*max_el/total_Etrue < 0.5){
	continue;
      }

      g4_blob_part = g4_tracks[Blob_g4_ID[j]];

      //blob_KinE = g4_blob_part.KinE;
      if (Blob_g4_TopMCPID[j] != pdg_in){
	continue;
      }
      ++primary_blobs;

      bool US = false;
      if ((Blob_VtxDZ[j] + vtx[2] - mc_vtx[2])< 0.0) US=true;
      /*
      if (Blob_total_E[j] > 50){
	cout << "BLOB FROM: " << g4_blob_part.PDG << endl;
	cout << "BLOB TID: " << g4_blob_part.TrackID << endl;
	cout << "BLOB MAX Contrib. number" << std::distance(frac_vals.begin(),max_el) << endl;
      }
      */
      

      if(g4_blob_part.PDG == 2212 && distance(frac_vals.begin(),max_el) == 1){
	//cout << "PROTON IN BOTH TRACK AND MAX DEPOSIT" << endl;
	++proton_blobs;
	G4_info prot_parent = g4_tracks[g4_blob_part.ParentID];
	TVector3 p_prot(g4_blob_part.Px,g4_blob_part.Py,g4_blob_part.Pz);
	TVector3 p_prot_parent(prot_parent.Px,prot_parent.Py,prot_parent.Pz);
	TVector3 vtx_to_prot(prot_parent.FinX - mc_vtx[0],prot_parent.FinY - mc_vtx[1],prot_parent.FinZ - mc_vtx[2]);
	if (US){
	  h_prim_end_Z_to_prim_v_parentID_US->Fill(PDGbins[prot_parent.PDG],g4_prim_part.FinZ);
	  h_prot_angle_to_prim_v_parentID_US->Fill(PDGbins[prot_parent.PDG],p_prot.Angle(prim_p));
	  h_prot_angle_to_parent_v_parentID_US->Fill(PDGbins[prot_parent.PDG],p_prot.Angle(p_prot_parent));
	  h_prot_angle_to_vtx_line_v_parentID_US->Fill(PDGbins[prot_parent.PDG],p_prot.Angle(vtx_to_prot));
	  h_prot_dist_to_vtx_v_parentID_US->Fill(PDGbins[prot_parent.PDG],vtx_to_prot.Mag());
	  h_prot_z_dist_to_vtx_v_parentID_US->Fill(PDGbins[prot_parent.PDG],vtx_to_prot.Z());
	  h_prot_mom_v_BlobE_US->Fill(Blob_total_E[j],p_prot.Mag());
	  h_prot_dist_to_vtx_v_prot_angle_to_vtx_US->Fill(p_prot.Angle(vtx_to_prot),vtx_to_prot.Mag());
	  h_prim_KinE_v_prot_z_dist_to_vtx_US->Fill(vtx_to_prot.Z(),g4_prim_part.KinE);
	  h_prim_KinE_v_prot_mom_US->Fill(p_prot.Mag(),g4_prim_part.KinE);
	}
	else{
	  h_prim_end_Z_to_prim_v_parentID_DS->Fill(PDGbins[prot_parent.PDG],g4_prim_part.FinZ);
	  h_prot_angle_to_prim_v_parentID_DS->Fill(PDGbins[prot_parent.PDG],p_prot.Angle(prim_p));
	  h_prot_angle_to_parent_v_parentID_DS->Fill(PDGbins[prot_parent.PDG],p_prot.Angle(p_prot_parent));
	  h_prot_angle_to_vtx_line_v_parentID_DS->Fill(PDGbins[prot_parent.PDG],p_prot.Angle(vtx_to_prot));
	  h_prot_dist_to_vtx_v_parentID_DS->Fill(PDGbins[prot_parent.PDG],vtx_to_prot.Mag());
	  h_prot_z_dist_to_vtx_v_parentID_DS->Fill(PDGbins[prot_parent.PDG],vtx_to_prot.Z());
	  h_prot_mom_v_BlobE_DS->Fill(Blob_total_E[j],p_prot.Mag());
	  h_prot_dist_to_vtx_v_prot_angle_to_vtx_DS->Fill(p_prot.Angle(vtx_to_prot),vtx_to_prot.Mag());
	  h_prim_KinE_v_prot_z_dist_to_vtx_DS->Fill(vtx_to_prot.Z(),g4_prim_part.KinE);
	  h_prim_KinE_v_prot_mom_DS->Fill(p_prot.Mag(),g4_prim_part.KinE);
	}
      }

      /*
      if (pdg_in == 2112){
	++neutron_blobs;
	while (g4_blob_part.TrackID > 1){
	  if (g4_blob_part.PDG == 2112){
	    neutron_KinE = g4_blob_part.KinE;
	    break;
	  }
	  g4_blob_part=g4_tracks[g4_blob_part.ParentID];
	}
	if (!neutron_KinE) neutron_KinE = g4_prim_part.KinE;
      }
      else{	
	while (g4_blob_part.TrackID > 1){
	  if (g4_blob_part.PDG == 2112){
	    ++neutron_blobs;
	    neutron_KinE = g4_blob_part.KinE;
	    break;
	  }
	  g4_blob_part=g4_tracks[g4_blob_part.ParentID];
	}
      }
      if(neutron_KinE)h_neutron_KinE_made_blob->Fill(neutron_KinE);
    }


    cout << "Number of total blobs from primary particle: " << primary_blobs  << endl;
    cout << "Number of primary particle blobs that had neutrons at some point: "<< neutron_blobs  << endl;
    */
    }
    h_prim_KinE_v_nprot_blobs->Fill(proton_blobs,g4_prim_part.KinE);
    h_prim_KinE_v_n_nonprot_blobs->Fill(nBlobs-proton_blobs,g4_prim_part.KinE);
    if (proton_blobs > 0){
      h_prim_KinE_v_nblobs_w_prot->Fill(nBlobs,g4_prim_part.KinE);
      h_prim_KinE_v_multiplicity_w_prot->Fill(nTracks,g4_prim_part.KinE);
      h_prim_KinE_v_nBlobTracks_w_prot->Fill(nBlobTracks,g4_prim_part.KinE);
      h_prim_KinE_v_nBlobIncTracks_w_prot->Fill(nBlobIncTracks,g4_prim_part.KinE);
    }
  }


  TH2D* h_prim_KinE_v_nblobs_wo_prot = (TH2D*)h_prim_KinE_v_nblobs->Clone("h_prim_KinE_v_nblobs_wo_prot");
  h_prim_KinE_v_nblobs_wo_prot->Add(h_prim_KinE_v_nblobs_w_prot,-1.0);

  TH2D* h_prim_KinE_v_multiplicity_wo_prot = (TH2D*)h_prim_KinE_v_multiplicity->Clone("h_prim_KinE_v_multiplicity_wo_prot");
  h_prim_KinE_v_multiplicity_wo_prot->Add(h_prim_KinE_v_multiplicity_w_prot,-1.0);

  TH2D* h_prim_KinE_v_nBlobTracks_wo_prot = (TH2D*)h_prim_KinE_v_nBlobTracks->Clone("h_prim_KinE_v_nBlobTracks_wo_prot");
  h_prim_KinE_v_nBlobTracks_wo_prot->Add(h_prim_KinE_v_nBlobTracks_w_prot,-1.0);

  TH2D* h_prim_KinE_v_nBlobIncTracks_wo_prot = (TH2D*)h_prim_KinE_v_nBlobIncTracks->Clone("h_prim_KinE_v_nBlobIncTracks_wo_prot");
  h_prim_KinE_v_nBlobIncTracks_wo_prot->Add(h_prim_KinE_v_nBlobIncTracks_w_prot,-1.0);

  output_file->Write();
  input_file->Close();
  output_file->Close();

  return 0;
}
