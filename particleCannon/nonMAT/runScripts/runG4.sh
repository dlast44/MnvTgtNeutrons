#!/bin/bash

#./G4_processing /minerva/data/users/dlast/merged_PC_files/Pre_MAD_Broken_Neutrons/GEANTINFO_Run_126000_Target5_PiMinusPlusMuon_PDG_-211_Ek_lo_0.0_Ek_hi_5000.0_nFiles_416.root Pre_MAD_Broken_Neutrons/Run_126000_Target5_PiMinusPlusMuon_PDG_-211_Ek_lo_0.0_Ek_hi_5000.0 -211 > pim_ph_latest.txt &

#./G4_processing /minerva/data/users/dlast/merged_PC_files/Pre_MAD_Broken_Neutrons/GEANTINFO_Run_126000_Target5_PiPlusPlusMuon_PDG_211_Ek_lo_0.0_Ek_hi_5000.0_nFiles_417.root Pre_MAD_Broken_Neutrons/Run_126000_Target5_PiPlusPlusMuon_PDG_211_Ek_lo_0.0_Ek_hi_5000.0 211 > pip_ph_latest.txt &

#./G4_processing /minerva/data/users/dlast/merged_PC_files/Pre_MAD_Broken_Neutrons/GEANTINFO_Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0_nFiles_429.root Pre_MAD_Broken_Neutrons/Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0 2112 > neutron_ph_latest.txt &

#./G4_processing /minerva/data/users/dlast/merged_PC_files/Pre_MAD_Broken_Neutrons/GEANTINFO_Run_126000_Target5_Pi0PlusMuon_PDG_111_Ek_lo_0.0_Ek_hi_5000.0_nFiles_413.root Pre_MAD_Broken_Neutrons/Run_126000_Target5_Pi0PlusMuon_PDG_111_Ek_lo_0.0_Ek_hi_5000.0 111 > pi0_ph_latest.txt &

#./G4_processing CCQENu Fixed_Neutrons Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0_nFiles_427_07_21_2020 2112 > CCQENu_neut_g4_process_latest.txt &

#./G4_processing CCQENu Fixed_Neutrons Run_126000_Target5_PiMinusPlusMuon_PDG_-211_Ek_lo_0.0_Ek_hi_5000.0_nFiles_415_07_21_2020 -211 > CCQENu_pim_g4_process_latest.txt &

#./G4_processing CCQENu Fixed_Neutrons Run_126000_Target5_PiPlusPlusMuon_PDG_211_Ek_lo_0.0_Ek_hi_5000.0_nFiles_405_07_21_2020 211 > CCQENu_pip_g4_process_latest.txt &

#./G4_processing CCQENu Fixed_Neutrons Run_126000_Target5_Pi0PlusMuon_PDG_111_Ek_lo_0.0_Ek_hi_5000.0_nFiles_413_07_21_2020 111 > CCQENu_pi0_g4_process_latest.txt &

#../interactiveMacros/G4_processing.exe MAD Fixed_Neutrons Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0_nFiles_425_07_21_2020 03_01_2021_130PM 2112 > MAD_neut_g4_process_latest.txt &

#../interactiveMacros/G4_processing.exe MAD Fixed_Neutrons Run_126000_Target5_PiPlusPlusMuon_PDG_211_Ek_lo_0.0_Ek_hi_5000.0_nFiles_422_07_21_2020 03_01_2021_130PM 211 > MAD_pip_g4_process_latest.txt &

../interactiveMacros/G4_processing.exe MAD Fixed_Neutrons Run_126000_Target5_PiMinusPlusMuon_PDG_-211_Ek_lo_0.0_Ek_hi_5000.0_nFiles_414_07_21_2020 03_03_2021_330PM -211 > MAD_pim_g4_process_latest.txt &

../interactiveMacros/G4_processing.exe MAD Fixed_Neutrons Run_126000_Target5_Pi0PlusMuon_PDG_111_Ek_lo_0.0_Ek_hi_5000.0_nFiles_414_07_21_2020 03_03_2021_330PM 111 > MAD_pi0_g4_process_latest.txt &
