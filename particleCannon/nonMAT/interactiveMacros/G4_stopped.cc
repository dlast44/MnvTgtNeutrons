//some standard C++ includes
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
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

  TFile* input_file = new TFile("/minerva/data/users/dlast/merged_PC_files/"+top_dir+"/"+tool+"/"+name,"READ");
  TTree* input_tree = (TTree*)input_file->Get(tree_name);

  TFile* output_file = new TFile("../hists/"+top_dir+"/"+tool+"/"+name+"_g4_plots.root","RECREATE");

  output_file->cd();
  TH1D* h_nBlobs_tot_for_stopped = new TH1D("h_nBlobs_tot_for_stopped","Total No. of Blobs for Events where Primary Particle Stopped;No. Blobs;Entries",100,0,100);
  //TH1D* h_nBlobs_prim_for_stopped = new TH1D("h_nBlobs_prim_for_stopped","No. of Blobs from Primary for Events where Primary Particle Stopped;No. Blobs;Entries",100,0,100);
  //TH1D* h_nBlobs_n_for_stopped = new TH1D("h_nBlobs_prim_for_stopped","No. of Blobs from Neutron for Events where Primary Particle Stopped;No. Blobs;Entries",100,0,100);

  TH1D* h_deltamag = new TH1D("h_deltamag","#Delta|#vec{p}| for closest-in-energy secondary with less energy;#Delta|#vec{p}|[MeV];Entries",100,-5000,0);
  TH1D* h_deltap = new TH1D("h_deltap","|#Delta#vec{p}| for closest-in-energy secondary with less energy;|#Delta#vec{p}|[MeV];Entries",100,0,5000);
  TH1D* h_deltapx = new TH1D("h_deltapx","#Delta p_{x} for closest-in-energy secondary with less energy;#Delta p_{x}[MeV];Entries",100,-1000,1000);
  TH1D* h_deltapy = new TH1D("h_deltapy","#Delta p_{y} for closest-in-energy secondary with less energy;#Delta p_{y}[MeV];Entries",100,-1000,1000);
  TH1D* h_deltapz = new TH1D("h_deltapz","#Delta p_{z} for closest-in-energy secondary with less energy;#Delta p_{z}[MeV];Entries",100,-1000,1000);
  TH1D* h_deltamag_norm = new TH1D("h_deltamag_norm","#Delta|#vec{p}|/|#vec{p}_{i}| for closest-in-energy secondary with less energy;#Delta|#vec{p}|/|#vec{p}_{i}|;Entries",120,-1.1,0.1);
  TH1D* h_deltap_norm = new TH1D("h_deltap_norm","|#Delta#vec{p}|/|#vec{p}_{i}| for closest-in-energy secondary with less energy;|#Delta#vec{p}|/|#vec{p}_{i}|;Entries",120,-0.2,2.2);
  TH1D* h_deltapx_norm = new TH1D("h_deltapx_norm","#Delta p_{x}/|#vec{p}_{i}| for closest-in-energy secondary with less energy;#Delta p_{x}/|#vec{p}_{i}|;Entries",110,-2.2,2.2);
  TH1D* h_deltapy_norm = new TH1D("h_deltapy_norm","#Delta p_{y}/|#vec{p}_{i}| for closest-in-energy secondary with less energy;#Delta p_{y}/|#vec{p}_{i}|;Entries",110,-2.2,2.2);
  TH1D* h_deltapz_norm = new TH1D("h_deltapz_norm","#Delta p_{z}/|#vec{p}_{i}| for closest-in-energy secondary with less energy;#Delta p_{z}/|#vec{p}_{i}|;Entries",110,-2.2,2.2);
  TH1D* h_piz_norm = new TH1D("h_piz_norm","p_{iz}/|#vec{p}_{i}| for stopping primary neutrons;p_{iz}/|#vec{p}_{i}|;Entries",220,-1.1,1.1);
  TH1D* h_piz_norm_ALL = new TH1D("h_piz_norm_All","p_{iz}/|#vec{p}_{i}| for all primary neutrons;p_{iz}/|#vec{p}_{i}|;Entries",220,-1.1,1.1);
  TH1D* h_transmission = new TH1D("h_transmission","Fraction of primary particles which interact in the target versus their p_{iz}/|#vec{p}_{i}|;p_{iz}/|#vec{p}_{i}|;fraction",220,-1.1,1.1);
  TH1D* h_dist_to_interaction = new TH1D("h_dist_to_interaction","Distance to interaction of primary particle;Dist.[mm];Entries",100,0,100);

  TH2D* h_KinE_v_nBlobs_tot_for_stopped = new TH2D("h_KinE_v_nBlobs_tot_for_stopped","Primary Kinetic Energy vs. No. Blobs for Events where Primary Particle Stopped;No. Blobs;Kinetic Energy [MeV]",100,0,100,100,0,5000);

  TH2D* h_deltamag_norm_v_pi = new TH2D("h_deltamag_norm_v_pi","#Delta|#vec{p}|/|#vec{p}_{i}| vs. |#vec{p}_{i}| for closest-in-energy secondary with less energy;|#vec{p}_{i}|[MeV];#Delta|#vec{p}|/|#vec{p}_{i}|",60,0,6000,12,-1.1,0.1);
  TH2D* h_deltap_norm_v_pi = new TH2D("h_deltap_norm_v_pi","|#Delta#vec{p}|/|#vec{p}_{i}| vs. |#vec{p}_{i}| for closest-in-energy secondary with less energy;|#vec{p}_{i}|[MeV];|#Delta#vec{p}|/|#vec{p}_{i}|",60,0,6000,12,-0.2,2.2);

  TH2D* h_deltamag_norm_v_piz_norm = new TH2D("h_deltamag_norm_v_piz_norm","#Delta|#vec{p}|/|#vec{p}_{i}| vs. p_{iz}/|#vec{p}_{i}| for closest secondary in enegry;p_{iz}/|#vec{p}_{i}|;#Delta|#vec{p}|/|#vec{p}_{i}|",220,-1.1,1.1,12,-1.1,0.1);
  TH2D* h_deltap_norm_v_piz_norm = new TH2D("h_deltap_norm_v_piz_norm","|#Delta#vec{p}|/|#vec{p}_{i}| vs. p_{iz}/|#vec{p}_{i}| for closest secondary in enegry;p_{iz}/|#vec{p}_{i}|;#Delta|#vec{p}|/|#vec{p}_{i}|",220,-1.1,1.1,12,-0.2,2.2);
  //TH1D* h_el_v_inel;
  //TH1D* h_KinE_stopped;
  //TH2D* h_el_v_inel_KinE = new TH2D("h_el_v_inel_2D_KinE","Kinetic Energy vs. N Outgoing same as primary",100,0,100,1000,0,5000);

  int max=6750;
  int size;
  int pdg[max];
  int ID[max];
  int parentID[max];
  int prim_pdg[max];
  int proc[max];
  double fin_x[max];
  double fin_y[max];
  double fin_z[max];
  double fin_t[max];
  double KE[max];
  double px[max];
  double py[max];
  double pz[max];

  int n_blobs_tot;
  int n_blobs_n;
  int primary_pdg[150];

  double MCvtx[4];

  input_tree->SetBranchAddress("G4_PDG",&pdg);
  input_tree->SetBranchAddress("G4_PDG_sz",&size);
  input_tree->SetBranchAddress("G4_TrackID",&ID);
  input_tree->SetBranchAddress("G4_ParentTrackID",&parentID);
  input_tree->SetBranchAddress("G4_Primary_PDG",&prim_pdg);
  input_tree->SetBranchAddress("G4_Process",&proc);
  input_tree->SetBranchAddress("G4_FinalX",&fin_x);
  input_tree->SetBranchAddress("G4_FinalY",&fin_y);
  input_tree->SetBranchAddress("G4_FinalZ",&fin_z);
  input_tree->SetBranchAddress("G4_FinalT",&fin_t);
  input_tree->SetBranchAddress("G4_InitKinE",&KE);
  input_tree->SetBranchAddress("G4_InitPx",&px);
  input_tree->SetBranchAddress("G4_InitPy",&py);
  input_tree->SetBranchAddress("G4_InitPz",&pz);

  input_tree->SetBranchAddress(tree_name+"_BlobMCPID_sz",&n_blobs_tot);
  input_tree->SetBranchAddress(tree_name+"_BlobTopMCPID",&primary_pdg);

  input_tree->SetBranchAddress("mc_vtx",&MCvtx); //X,Y,Z,T in that order.

  unordered_map<int, G4_info> tracks;

  for (int i=0; i < input_tree->GetEntries(); ++i){
    tracks.clear();
    cout << "Event: " << i << endl;
    input_tree->GetEvent(i);
    TVector3 vtx(MCvtx[0],MCvtx[1],MCvtx[2]);
    TVector3 dist;
    if (size > max || size <= 0){
      cout << "UH OH" << endl;
      cout << size << endl;
      continue;
    }

    for (int j=0; j < size;++j){
      G4_info track = {pdg[j],ID[j],parentID[j],prim_pdg[j],proc[j],fin_x[j],fin_y[j],fin_z[j],fin_t[j],KE[j],px[j],py[j],pz[j]};
      tracks[track.TrackID] = track;
    }
    
    G4_info prim_part = tracks[1];
    if (prim_part.PDG != pdg_in) continue;
    TVector3 PrimP(prim_part.Px,prim_part.Py,prim_part.Pz);
    TVector3 PrimR(prim_part.FinX,prim_part.FinY,prim_part.FinZ);
    h_piz_norm_ALL->Fill(PrimP.Z()/PrimP.Mag());
    if (fabs(prim_part.FinZ-5776.0) > 6 /*|| prim_part.KinE > 500*/) continue;

    dist = PrimR-vtx;
    h_dist_to_interaction->Fill(dist.Mag());
    h_nBlobs_tot_for_stopped->Fill(n_blobs_tot);
    h_KinE_v_nBlobs_tot_for_stopped->Fill(n_blobs_tot,prim_part.KinE);
    cout << "Primary Stopped in target with Kinetic E: " << prim_part.KinE << endl;

    //bool pure_inel = false;
    int same_particle_type_out = 0;
    double min_diff = 100000000.0;
    TVector3 DP(-10000000,-10000000,-10000000);
    TVector3 P(-10000000,-10000000,-10000000);
    //G4_info outgoing_part = {-999,-999,-999,-999,-999,-999,-999,-999,-999,-999,-999,-999,-999};
    for (auto t:tracks){
      if (t.second.ParentID == 1 && t.second.PDG == pdg_in){
	TVector3 mom(t.second.Px,t.second.Py,t.second.Pz);
	//TVector3 deltap = mom-PrimP;
	if (t.second.KinE > 5)++same_particle_type_out;
	if (prim_part.KinE - t.second.KinE < 0){
	  cout << "ACCELERATED?" << endl;
	  continue;
	}
	if (prim_part.KinE - t.second.KinE < min_diff){
	  min_diff = prim_part.KinE - t.second.KinE;
	  //outgoing_part = t.second;
	  P = mom;
	}
      }
    }
    if (min_diff != 100000000.0){
      cout << "Lowest Diff in Kin. E in MeV: " << min_diff << endl;
      DP = P-PrimP;
    }
    //cout << "n same particle type: " << same_particle_type_out << endl;

    h_piz_norm->Fill(PrimP.Z()/PrimP.Mag());
    h_transmission->Fill(PrimP.Z()/PrimP.Mag());
    
    if (same_particle_type_out > 0){
      h_deltamag->Fill(P.Mag()-PrimP.Mag());
      h_deltap->Fill(DP.Mag());
      h_deltapx->Fill(DP.X());
      h_deltapy->Fill(DP.Y());
      h_deltapz->Fill(DP.Z());
      h_deltamag_norm->Fill((P.Mag()-PrimP.Mag())/PrimP.Mag());
      h_deltap_norm->Fill(DP.Mag()/PrimP.Mag());
      h_deltapx_norm->Fill(DP.X()/PrimP.Mag());
      h_deltapy_norm->Fill(DP.Y()/PrimP.Mag());
      h_deltapz_norm->Fill(DP.Z()/PrimP.Mag());

      h_deltamag_norm_v_pi->Fill(PrimP.Mag(),(P.Mag()-PrimP.Mag())/PrimP.Mag());
      h_deltap_norm_v_pi->Fill(PrimP.Mag(),DP.Mag()/PrimP.Mag());
      h_deltamag_norm_v_piz_norm->Fill(PrimP.Z()/PrimP.Mag(),(P.Mag()-PrimP.Mag())/PrimP.Mag());
      h_deltap_norm_v_piz_norm->Fill(PrimP.Z()/PrimP.Mag(),DP.Mag()/PrimP.Mag());

    }
    //h_el_v_inel_KinE->Fill(same_particle_type_out,prim_part.KinE);
  }
  h_transmission->Divide(h_piz_norm_ALL);
  output_file->Write();
  input_file->Close();
  output_file->Close();

  return 0;
}
