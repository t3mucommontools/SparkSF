//simple macro to 1. open rootfiles produced by spark 2. take projections from TH2 efficiencies 3. plot data and MC on same canvas adding ratio

#include "TH1D.h"
#include <cmath>
#include <string>

void plotter_dR()
{
    TString IDname = "Tau3MuGlobal"; //"Tau3MuTracker";
    TString inputfile = "NUM_"+IDname+"_DEN_genTracks/NUM_"+IDname+"_DEN_genTracks_pair_dR.root";
    TString year = "2018";

    TFile *f = new TFile(inputfile);
    TH1D *eff_data = (TH1D*)f->Get("NUM_"+IDname+"_DEN_genTracks_pair_dR_efficiencyData");
    TH1D *eff_MC = (TH1D*)f->Get("NUM_"+IDname+"_DEN_genTracks_pair_dR_efficiencyMC");
    TH1D *eff_ratio;

    // Define the ratio plot
    eff_ratio = (TH1D*)eff_data->Clone("eff_ratio");
    eff_ratio->Sumw2(); 
    eff_ratio->Divide(eff_MC);
    eff_ratio->SetStats(0);

    //Drawing
    TCanvas *c2 = new TCanvas("c2","c2",150,10,990,990);
    gStyle->SetOptTitle(0);
    gStyle->SetOptStat(0);
    // Upper plot will be in pad1
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
    pad1->SetBottomMargin(0); // Upper and lower plot are joined
    pad1->SetGridx();      // Vertical grid
    pad1->Draw();          // Draw the upper pad: pad1
    pad1->cd();            // pad1 becomes the current pad
    eff_MC->GetYaxis()->SetRangeUser(0, 1.2);

    eff_MC->GetYaxis()->SetTitle("Efficiency");
    eff_MC->GetYaxis()->SetTitleSize(24);
    eff_MC->GetYaxis()->SetTitleFont(43);
    
    eff_data->SetStats(0);
    eff_data->SetLineColor(kBlack);
    eff_data->SetMarkerColor(kBlack);
    eff_data->SetMarkerStyle(20);

    eff_MC->SetStats(0);
    eff_MC->SetLineColor(kRed);
    eff_MC->SetMarkerColor(kRed);
    eff_MC->SetMarkerStyle(21);
    gStyle->SetLegendBorderSize(0);

    TLegend*leg = new TLegend(0.6,0.55,0.9,0.9);
    leg->SetHeader(IDname);
    leg->AddEntry(eff_MC,"MC","lpe");
    leg->AddEntry(eff_data,year+" data","lpe");
    gStyle->SetLegendBorderSize(0);
    leg->SetFillStyle(0);
    c2->Update();

    eff_MC->Draw("E");
    eff_data->Draw("E same");
    leg->Draw();
  
    // lower plot will be in pad2
    c2->cd();          // Go back to the main canvas before defining pad2
    TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);
    pad2->SetGridx(); // vertical grid
    pad2->SetGridy(); // vertical grid
    pad2->Draw();
    pad2->cd();       // pad2 becomes the current pad  
    eff_ratio->SetLineColor(kBlack);
    eff_ratio->GetYaxis()->SetTitle("Data/MC");
    eff_ratio->GetYaxis()->SetTitleSize(22);
    eff_ratio->GetYaxis()->SetTitleFont(43);
    eff_ratio->GetYaxis()->SetTitleOffset(1.25);
    eff_ratio->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    eff_ratio->GetYaxis()->SetLabelSize(15);
    
    // X axis ratio plot settings
    eff_ratio->GetXaxis()->SetTitle("dR(#mu, #mu)");
    eff_ratio->GetXaxis()->SetTitleSize(24);
    eff_ratio->GetXaxis()->SetTitleFont(43);
    eff_ratio->GetXaxis()->SetTitleOffset(3);
    eff_ratio->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
    eff_ratio->GetXaxis()->SetLabelSize(15);
    eff_ratio->GetYaxis()->SetRangeUser(0.7, 1.2);
    if(IDname=="Tau3MuTracker") eff_ratio->GetYaxis()->SetRangeUser(0.0, 2.0);
 
    eff_ratio->Draw("E");
    c2->cd();
    c2->Update();
    c2->SaveAs("plot/plot_"+year+"_"+IDname+"_dR.png");
}
