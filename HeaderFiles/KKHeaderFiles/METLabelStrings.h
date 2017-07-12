#ifndef MET_LABEL_STRINGS_H
#define MET_LABEL_STRINGS_H

#include <map>

typedef std::map<TString, TString> labelMap;
void SetStringKey_StSMap_MET(labelMap &mapVartoLabel/*, bool isSmear*/) {
	TString stringSmear = ""; // \todo need to enable GetStringSmear(isSmear, false);
	TString stringSmearLabel = ""; // need to enable = GetStringSmear(isSmear, true);

	TString stringMT2ll = "M_{T2}(ll)";
	TString stringMT2lblb = "M_{T2}(lb)(lb)";
	TString stringMT2lblb_ZeroBLepMass = "M^{m(b,l)=0}_{T2}(lb)(lb)";
	TString stringMT2bb_ZMET = "M_{T2}^{#slash{W}}(bb)";
	TString stringKT2_Top = "#kappa_{T2}^{top}";

	TString stringMETSig1DHack = "#slash{E}^{2}_{T} / H_{T}";
	TString stringMETSig2DHack = "(#slash{E}_{#parallel} / #sigma_{#slash{E}_{#parallel}}) #oplus (#slash{E}_{#perp} / #sigma_{#slash{E}_{#perp}})";
	TString stringMETSig2DHack_Par = "#slash{E}_{#parallel} / #sigma_{#slash{E}_{#parallel}}";
	TString stringMETSig2DHack_Perp = "#slash{E}_{#perp} / #sigma_{#slash{E}_{#perp}}";
	TString stringMETSig2DTrue = "#slash{E}_{T} Sig";

	mapVartoLabel["RawMETSig"]    = "RawMET Significance";

	mapVartoLabel[stringSmear + "MT2ll"]        = stringSmearLabel + stringMT2ll + " [GeV]";
	mapVartoLabel[stringSmear + "MT2ll80"]      = stringSmearLabel + stringMT2ll + " > 80 GeV";
	mapVartoLabel[stringSmear + "MT2ll90"]      = stringSmearLabel + stringMT2ll + " > 90 GeV";
	mapVartoLabel[stringSmear + "MT2ll100"]     = stringSmearLabel + stringMT2ll + " > 100 GeV";
	mapVartoLabel[stringSmear + "MT2ll110"]     = stringSmearLabel + stringMT2ll + " > 110 GeV";
	mapVartoLabel[stringSmear + "MT2ll120"]     = stringSmearLabel + stringMT2ll + " > 120 GeV";
	mapVartoLabel[stringSmear + "MT2lblb"]      = stringSmearLabel + stringMT2lblb + " [GeV]";
	mapVartoLabel[stringSmear + "MT2lblb_ZeroBLepMass"]      = stringSmearLabel + stringMT2lblb_ZeroBLepMass + " [GeV]";
	mapVartoLabel[stringSmear + "MT2bb_ZMET"]   = stringSmearLabel + stringMT2bb_ZMET + " [GeV]";
	mapVartoLabel[stringSmear + "KT2_Top"]   = stringSmearLabel + stringKT2_Top + " [GeV]";

	mapVartoLabel[stringSmear + "MET"]          = stringSmearLabel + "#slash{E}_{T} [GeV]";
	mapVartoLabel[stringSmear + "METX"]         = stringSmearLabel + "#slash{E}_{x} [GeV]";
	mapVartoLabel[stringSmear + "METY"]         = stringSmearLabel + "#slash{E}_{y} [GeV]";
	mapVartoLabel[stringSmear + "METPhi"]       = stringSmearLabel + "#slash{E}_{T} #phi [rad]";
	mapVartoLabel[stringSmear + "TTBarPt"]      = stringSmearLabel + " t#bar{t} p_{T} [GeV]";

	mapVartoLabel[stringSmear + "UPar"]         = stringSmearLabel + "u_{#parallel} [GeV]";
	mapVartoLabel[stringSmear + "UParPlusqT"]   = stringSmearLabel + "u_{#parallel} + q_{T}^{Z} [GeV]";
	mapVartoLabel[stringSmear + "UParDivqT"]   = stringSmearLabel +  "-#frac{u_{#parallel}}{q_{T}^{Z}}";
	mapVartoLabel[stringSmear + "UPerp"]        = stringSmearLabel + "u_{#perp}  [GeV]";
	mapVartoLabel["SumEt"] = "#Sigma E_{T} [GeV]";
	mapVartoLabel["SumEtSubqT"] = "#Sigma E_{T} - q_{T}^{Z} [GeV]";

	mapVartoLabel[stringSmear + "METSig1DHack"] = stringSmearLabel + stringMETSig1DHack;
	mapVartoLabel[stringSmear + "METSig1DHack_AllJets"] = stringSmearLabel + stringMETSig1DHack;

	mapVartoLabel[stringSmear + "METSig2DHack"] = stringSmearLabel + stringMETSig2DHack;
	mapVartoLabel[stringSmear + "METSig2DHack_AllJets"] = stringSmearLabel + stringMETSig2DHack;
	mapVartoLabel[stringSmear + "METSig2DHackFull"] = stringSmearLabel + stringMETSig2DHack;

	mapVartoLabel[stringSmear + "METSig2DHack_Par"] = stringSmearLabel + stringMETSig2DHack_Par;
	mapVartoLabel[stringSmear + "METSig2DHack_Par_AllJets"] = stringSmearLabel + stringMETSig2DHack_Par;
	mapVartoLabel[stringSmear + "METSig2DHack_ParFull"] = stringSmearLabel + stringMETSig2DHack_Par;

	mapVartoLabel[stringSmear + "METSig2DHack_Perp"] = stringSmearLabel + stringMETSig2DHack_Perp;
	mapVartoLabel[stringSmear + "METSig2DHack_Perp_AllJets"] = stringSmearLabel + stringMETSig2DHack_Perp;
	mapVartoLabel[stringSmear + "METSig2DHack_PerpFull"] = stringSmearLabel + stringMETSig2DHack_Perp;

	mapVartoLabel[stringSmear + "METSig2DTrue"] = stringSmearLabel + stringMETSig2DTrue;

}


void SetStringKey_StSMap_Composite(labelMap &mapVartoLabel) {
    for (int iSmear = 0; iSmear < 2; ++iSmear) {
        SetStringKey_StSMap_MET(mapVartoLabel/*, iSmear*/);
    }
}

#endif // MET_LABEL_STRINGS_H

