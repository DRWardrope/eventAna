#include "SampleHandler.h"
SampleHandler::SampleHandler()
{
	//Cross-sections are in fb.
	m_sampleList.insert(std::pair<int, SampleDetails>(301488, SampleDetails(301488, 79800, 439.433, 300, "RS_G_hh_bbbb_c10_M300")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301489, SampleDetails(301489, 99800, 632.898, 400, "RS_G_hh_bbbb_c10_M400")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301490, SampleDetails(301490, 94400, 297.106, 500, "RS_G_hh_bbbb_c10_M500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301491, SampleDetails(301491, 99800, 136.634, 600, "RS_G_hh_bbbb_c10_M600")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301492, SampleDetails(301492, 54800, 67.0785, 700, "RS_G_hh_bbbb_c10_M700")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301493, SampleDetails(301493, 70000, 35.1207, 800, "RS_G_hh_bbbb_c10_M800")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301494, SampleDetails(301494, 83000, 19.4264, 900, "RS_G_hh_bbbb_c10_M900")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301495, SampleDetails(301495, 100000, 11.213, 1000, "RS_G_hh_bbbb_c10_M1000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301496, SampleDetails(301496, 99800, 6.73515, 1100, "RS_G_hh_bbbb_c10_M1100")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301497, SampleDetails(301497, 99800, 4.17493, 1200, "RS_G_hh_bbbb_c10_M1200")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301498, SampleDetails(301498, 19800, 2.65644, 1300, "RS_G_hh_bbbb_c10_M1300")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301499, SampleDetails(301499, 99600, 1.73156, 1400, "RS_G_hh_bbbb_c10_M1400")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301500, SampleDetails(301500, 99400, 1.14861, 1500, "RS_G_hh_bbbb_c10_M1500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301501, SampleDetails(301501, 99800, 0.77772, 1600, "RS_G_hh_bbbb_c10_M1600")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301502, SampleDetails(301502, 15000, 0.37155, 1800, "RS_G_hh_bbbb_c10_M1800")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301503, SampleDetails(301503, 89800, 0.18508, 2000, "RS_G_hh_bbbb_c10_M2000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301504, SampleDetails(301504, 99800, 0.0827661, 2250, "RS_G_hh_bbbb_c10_M2250")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301505, SampleDetails(301505, -99, 0.0385532, 2500, "RS_G_hh_bbbb_c10_M2500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301506, SampleDetails(301506, 59600, 0.0185941, 2750, "RS_G_hh_bbbb_c10_M2750")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301507, SampleDetails(301507, 78000, 0.00922879, 3000, "RS_G_hh_bbbb_c10_M3000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301528, SampleDetails(301528, -99, 3926.24, "TTBarNonAllHad_1100m1300")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301529, SampleDetails(301529, -99, 1617.31, "TTBarNonAllHad_1300m1500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301530, SampleDetails(301530, -99, 718.018, "TTBarNonAllHad_1500m1700")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301531, SampleDetails(301531, -99, 431.859, "TTBarNonAllHad_1700m2000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(301532, SampleDetails(301532, -99, 257.23, "TTBarNonAllHad_2000m14000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(303722, SampleDetails(303722, -99, 2755.9, "TTBarAllHad_1100m1300")));
	m_sampleList.insert(std::pair<int, SampleDetails>(303723, SampleDetails(303723, -99, 1137.73, "TTBarAllHad_1300m1500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(303724, SampleDetails(303724, -99, 503.701, "TTBarAllHad_1500m1700")));
	m_sampleList.insert(std::pair<int, SampleDetails>(303725, SampleDetails(303725, -99, 305.113, "TTBarAllHad_1700m2000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(303726, SampleDetails(303726, -99, 180.424, "TTBarAllHad_2000m14000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(342619, SampleDetails(342619, 10708.3, 11.3459, "SM_HH")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343394, SampleDetails(343394, 99000, 963.832, 260, "H_hh_bbbb_M260")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343395, SampleDetails(343395, 100000, 1044.14, 300, "H_hh_bbbb_M300")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343396, SampleDetails(343396, 100000, 599.565, 400, "H_hh_bbbb_M400")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343397, SampleDetails(343397, 100000, 241.968, 500, "H_hh_bbbb_M500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343398, SampleDetails(343398, 100000, 114.04, 600, "H_hh_bbbb_M600")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343399, SampleDetails(343399, -99, 57.5633, 700, "H_hh_bbbb_M700")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343400, SampleDetails(343400, 93000, 30.437, 800, "H_hh_bbbb_M800")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343401, SampleDetails(343401, 100000, 16.7031, 900, "H_hh_bbbb_M900")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343402, SampleDetails(343402, 99000, 9.46051, 1000, "H_hh_bbbb_M1000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343403, SampleDetails(343403, 98000, 5.50841, 1100, "H_hh_bbbb_M1100")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343404, SampleDetails(343404, 60000, 3.28698, 1200, "H_hh_bbbb_M1200")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343405, SampleDetails(343405, 97000, 2.00518, 1300, "H_hh_bbbb_M1300")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343406, SampleDetails(343406, 99000, 1.24772, 1400, "H_hh_bbbb_M1400")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343407, SampleDetails(343407, 99000, 0.79036, 1500, "H_hh_bbbb_M1500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343408, SampleDetails(343408, 100000, 0.099, 1600, "H_hh_bbbb_M1600")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343409, SampleDetails(343409, 98000, 0.099, 1800, "H_hh_bbbb_M1800")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343410, SampleDetails(343410, 99000, 0.099, 2000, "H_hh_bbbb_M2000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343411, SampleDetails(343411, 99000, 0.099, 2250, "H_hh_bbbb_M2250")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343412, SampleDetails(343412, 99000, 0.099, 2500, "H_hh_bbbb_M2500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343413, SampleDetails(343413, 100000, 0.099, 2750, "H_hh_bbbb_M2750")));
	m_sampleList.insert(std::pair<int, SampleDetails>(343414, SampleDetails(343414, 100000, 0.099, 3000, "H_hh_bbbb_M3000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361021, SampleDetails(361021, -99, 5.26002e+10, "JZ1W")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361022, SampleDetails(361022, -99, 8.11357e+08, "JZ2W")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361023, SampleDetails(361023, -99, 8.45364e+06, "JZ3W")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361024, SampleDetails(361024, -99, 1.34992e+08, "JZ4W")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361025, SampleDetails(361025, -99, 4197.68, "JZ5W")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361026, SampleDetails(361026, -99, 241.942, "JZ6W")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361027, SampleDetails(361027, -99, 6.35887, "JZ7W")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361028, SampleDetails(361028, -99, 6.35478, "JZ8W")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361029, SampleDetails(361029, -99, 0.236819, "JZ9W")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361030, SampleDetails(361030, -99, 0.00705233, "JZ10W")));
	
	m_sampleList.insert(std::pair<int, SampleDetails>(361446, SampleDetails(361446, 79780000, 11.94e6*7.9667e-2, "Znunu_BFilt_Pt0_70")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361449, SampleDetails(361449, 4428600, 427.810e3*1.2605e-1, "Znunu_BFilt_Pt70_140")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361452, SampleDetails(361452, 4925900, 64.654e3*1.3695e-1, "Znunu_BFilt_Pt140_280")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361455, SampleDetails(361455, 1600000, 4.259e3*1.4748e-1, "Znunu_BFilt_Pt280_500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361458, SampleDetails(361458, 510000, 254.650*1.5626e-1, "Znunu_BFilt_Pt500_700")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361461, SampleDetails(361461, 4500, 56.072*1.3143e-1, "Znunu_BFilt_Pt700_1000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361464, SampleDetails(361464, 29980, 5.776*1.339e-1, "Znunu_BFilt_Pt1000_2000")));
	m_sampleList.insert(std::pair<int, SampleDetails>(361467, SampleDetails(361467, 9910, 23.457e-3*1.38e-2, "Znunu_BFilt_Pt2000_E_CMS")));
	//m_sampleList.insert(std::pair<int, SampleDetails>(36, SampleDetails(36, , , "Znunu_BFilt_Pt")));
	m_sampleList.insert(std::pair<int, SampleDetails>(393320, SampleDetails(393320, 51171.4, 6955.4, 130, "GGM_Hino_M130")));
	m_sampleList.insert(std::pair<int, SampleDetails>(393321, SampleDetails(393321, 53021, 3832.31, 150, "GGM_Hino_M150")));
	m_sampleList.insert(std::pair<int, SampleDetails>(393322, SampleDetails(393322, 52469.8, 1335.62, 200, "GGM_Hino_M200")));
    m_sampleList.insert(std::pair<int, SampleDetails>(393368, SampleDetails(393368, 4.99456e+04, 577.314, 250, "GGM_Hino_M250")));
	m_sampleList.insert(std::pair<int, SampleDetails>(393323, SampleDetails(393323, 51777.6, 284.855, 300, "GGM_Hino_M300")));
	m_sampleList.insert(std::pair<int, SampleDetails>(393324, SampleDetails(393324, 51441.1, 88.7325, 400, "GGM_Hino_M400")));
	m_sampleList.insert(std::pair<int, SampleDetails>(393325, SampleDetails(393325, -99, 33.8387, 500, "GGM_Hino_M500")));
	m_sampleList.insert(std::pair<int, SampleDetails>(393326, SampleDetails(393326, 49974.4, 14.6677, 600, "GGM_Hino_M600")));
	m_sampleList.insert(std::pair<int, SampleDetails>(393327, SampleDetails(393327, 50770.2, 3.46143, 800, "GGM_Hino_M800")));
	m_sampleList.insert(std::pair<int, SampleDetails>(410000, SampleDetails(410000, 4.93866e+07, 451695, "TTBarNonAllHad")));
	m_sampleList.insert(std::pair<int, SampleDetails>(410007, SampleDetails(410007, 6.95736, 380211, "TTBarAllHad")));
}

bool SampleHandler::isSignal(const int& dsid) const
{
	if(dsid >= 301488 && dsid <= 301507) return true; //RSG c=1.0
	if(dsid >= 343394 && dsid <= 343414) return true; //2HDM
	if(dsid == 342619) return true; //SM non-resonant
	if(dsid >= 393320 && dsid <= 393327) return true;
	return false;
}
