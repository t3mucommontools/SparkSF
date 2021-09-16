//simple macro to 1. open rootfiles produced by spark 2. take projections from TH2 efficiencies 3. plot data and MC on same canvas adding ratio

#include "TH1D.h"
#include <cmath>
#include <string>

void plotter()
{
    TString IDname = "Tau3MuTracker"; //"Tau3MuGlobal";
    TString inputfile = "NUM_"+IDname+"_DEN_genTracks/NUM_"+IDname+"_DEN_genTracks_pt_abseta.root";
    TString year = "2018";

    int nbins = 4; //number of eta bins
    TH1D *eff_data[nbins];
    TH1D *eff_MC[nbins];
    TH1D *eff_ratio[nbins];


    TFile *f = new TFile(inputfile);
    TH2D *eff_data_2D = (TH2D*)f->Get("NUM_"+IDname+"_DEN_genTracks_pt_abseta_efficiencyData");
    TH2D *eff_MC_2D = (TH2D*)f->Get("NUM_"+IDname+"_DEN_genTracks_pt_abseta_efficiencyMC");

    if( eff_data_2D->GetNbinsY() != eff_MC_2D->GetNbinsY()){
        cout<<"error"<<endl;
        return;
    }
    nbins = eff_data_2D->GetNbinsY();
    Double_t edges[nbins];
    eff_data_2D->GetYaxis()->GetLowEdge(edges);

    for(int i=0; i<nbins; i++){
        cout<<"eta bin "<<i<<endl;
        TString eta_lable = "";
        std::ostringstream oss;
        oss << std::setprecision(2) << edges[i];
        std::string low_edge = oss.str();
        std::ostringstream oss2;
        oss2 << std::setprecision(2) << edges[i+1];
        std::string high_edge = oss2.str();
        if(i==nbins-1) eta_lable= "|#eta| #geq "+low_edge;
        else eta_lable= low_edge+" #leq |#eta| < "+high_edge;
        cout<<eta_lable<<endl;
        
        eff_data[i] = eff_data_2D->ProjectionX(Form("data_bin%d",i+1),i+1,i+1);
        eff_MC[i] = eff_MC_2D->ProjectionX(Form("MC_bin%d",i+1),i+1,i+1);
        eff_data[i]->GetXaxis()->SetRangeUser(1, 20);
        eff_MC[i]->GetXaxis()->SetRangeUser(1, 20);

        // Define the ratio plot
        eff_ratio[i] = (TH1D*)eff_data[i]->Clone(Form("eff_ratio%d",i));
        eff_ratio[i]->Sumw2(); 
        eff_ratio[i]->Divide(eff_MC[i]);
        eff_ratio[i]->SetStats(0);
        eff_ratio[i]->GetXaxis()->SetRangeUser(1, 20);

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
        eff_MC[i]->GetYaxis()->SetRangeUser(0, 1.2);

        eff_MC[i]->GetYaxis()->SetTitle("Efficiency");
        eff_MC[i]->GetYaxis()->SetTitleSize(24);
        eff_MC[i]->GetYaxis()->SetTitleFont(43);
    
        eff_data[i]->SetStats(0);
        eff_data[i]->SetLineColor(kBlack);
        eff_data[i]->SetMarkerColor(kBlack);
        eff_data[i]->SetMarkerStyle(20);

        eff_MC[i]->SetStats(0);
        eff_MC[i]->SetLineColor(kRed);
        eff_MC[i]->SetMarkerColor(kRed);
        eff_MC[i]->SetMarkerStyle(21);

        gStyle->SetLegendBorderSize(0);
        pad1->Update();
//        pad1->cd();            // pad1 becomes the current pad

        Double_t y_low = 0.0, y_high = 0.35;
        if(IDname=="Tau3MuTracker"){ y_low = 0.50; y_high = 0.85; }
        TLegend*leg = new TLegend(0.6,y_low,0.9,y_high);
        leg->SetHeader("#splitline{"+IDname+" }{"+eta_lable+" }");
        leg->AddEntry(eff_MC[i],"MC","lpe");
        leg->AddEntry(eff_data[i],year+" data","lpe");
        leg->SetFillStyle(0);

        eff_MC[i]->Draw("E");
        eff_data[i]->Draw("E same");
        leg->Draw();
  
        // lower plot will be in pad2
        c2->cd();          // Go back to the main canvas before defining pad2
        TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
        pad2->SetTopMargin(0);
        pad2->SetBottomMargin(0.2);
        pad2->SetGridx(); // vertical grid
        pad2->SetGridy(); // horizontal
        pad2->Draw();
        pad2->cd();       // pad2 becomes the current pad  
        eff_ratio[i]->SetLineColor(kBlack);
        eff_ratio[i]->GetYaxis()->SetTitle("Data/MC");
        eff_ratio[i]->GetYaxis()->SetTitleSize(22);
        eff_ratio[i]->GetYaxis()->SetTitleFont(43);
        eff_ratio[i]->GetYaxis()->SetTitleOffset(1.25);
        eff_ratio[i]->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        eff_ratio[i]->GetYaxis()->SetLabelSize(15);
        
        // X axis ratio plot settings
        eff_ratio[i]->GetXaxis()->SetTitle("p_{T} (GeV)");
        eff_ratio[i]->GetXaxis()->SetTitleSize(24);
        eff_ratio[i]->GetXaxis()->SetTitleFont(43);
        eff_ratio[i]->GetXaxis()->SetTitleOffset(3);
        eff_ratio[i]->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
        eff_ratio[i]->GetXaxis()->SetLabelSize(15);
 
        eff_ratio[i]->GetYaxis()->SetRangeUser(0.7, 1.2);
        if(IDname=="Tau3MuTracker") eff_ratio[i]->GetYaxis()->SetRangeUser(0.5, 5.0);
        eff_ratio[i]->Draw("E");
        c2->cd();
        c2->Update();
        c2->SaveAs(Form("plot/plot_"+year+"_"+IDname+"_bin%d.png",i+1));
    }
}
