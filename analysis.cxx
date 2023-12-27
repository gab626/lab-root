#include <cmath>
#include <iostream>

void analysis() {
  TFile* file = new TFile("simulation.root");

  gStyle->SetOptStat(2210);
  gStyle->SetOptFit(1111);
  //  Terminal output and fitting
  TH1F* h[4];
  TH1::AddDirectory(kFALSE);
  h[0] = (TH1F*)file->Get("h0");
  for (int i = 1; i < 8; i++) {
    std::cout << "Particle type " << i << ": " << h[0]->GetBinContent(i)
              << " +/- " << h[0]->GetBinError(i) << '\n';
  }

  h[1] = (TH1F*)file->Get("h1");
  h[2] = (TH1F*)file->Get("h2");
  h[3] = (TH1F*)file->Get("h3");
  TF1* f1 = new TF1("f1", "pol0", 0, 2 * M_PI);
  TF1* f2 = new TF1("f2", "pol0", 0, M_PI);
  TF1* f3 = new TF1("f3", "expo", 0, 7);  // = exp([0] + [1] * x)
  f1->SetParameter(0, h[1]->GetMean());
  f2->SetParameter(0, h[2]->GetMean());
  f3->SetParameters(std::log(h[3]->GetMaximum()), h[3]->GetMean());
  h[1]->Fit("f1", "RQ");
  h[2]->Fit("f2", "RQ");
  h[3]->Fit("f3", "RQ");
  TF1* fit1 = h[1]->GetFunction("f1");
  TF1* fit2 = h[2]->GetFunction("f2");
  TF1* fit3 = h[3]->GetFunction("f3");

  std::cout << "Azimuthal fit parameter value: " << fit1->GetParameter(0)
            << " +/- " << fit1->GetParError(0) << '\n'
            << "Chi / NDF = " << fit1->GetChisquare() / fit1->GetNDF() << '\n'
            << "Probability = " << fit1->GetProb() << '\n';
  std::cout << "Polar fit parameter value: " << fit2->GetParameter(0) << " +/- "
            << fit2->GetParError(0) << '\n'
            << "Chi / NDF = " << fit2->GetChisquare() / fit2->GetNDF() << '\n'
            << "Probability = " << fit2->GetProb() << '\n';
  std::cout << "Impulse fit parameter 0 value: " << fit3->GetParameter(0)
            << " +/- " << fit3->GetParError(0) << '\n'
            << "Impulse fit parameter 1 value: " << fit3->GetParameter(1)
            << " +/- " << fit3->GetParError(1) << '\n'
            << "Chi / NDF = " << fit3->GetChisquare() / fit3->GetNDF() << '\n'
            << "Probability = " << fit3->GetProb() << '\n';

  TH1F* m1 = (TH1F*)file->Get("m1");
  TH1F* m2 = (TH1F*)file->Get("m2");
  TH1F* m3 = (TH1F*)file->Get("m3");
  TH1F* m4 = (TH1F*)file->Get("m4");
  TH1F* m5 = (TH1F*)file->Get("m5");
  TH1F* msum1 = new TH1F("msum1", "From all particles", 200, 0.7, 1.1);
  TH1F* msum2 = new TH1F("msum2", "From pi-K couples", 200, 0.7, 1.1);
  msum1->SetDirectory(0);
  msum2->SetDirectory(0);
  m5->SetDirectory(0);
  msum1->Add(m2, m1, 1, -1);
  msum2->Add(m4, m3, 1, -1);

  TF1* g1 = new TF1("g1", "gaus", 0.7, 1.1);
  TF1* g2 = new TF1("g2", "gaus", 0.7, 1.1);
  g1->SetParameters(msum1->GetMaximum(), msum1->GetMean(), msum1->GetRMS());
  g2->SetParameters(msum2->GetMaximum(), msum2->GetMean(), msum2->GetRMS());
  msum1->Fit("g1", "RQ");
  msum2->Fit("g2", "RQ");
  TF1* gauss1 = msum1->GetFunction("g1");
  TF1* gauss2 = msum2->GetFunction("g2");

  std::cout << "K* FROM ALL PARTICLES\n"
            << "Mass = " << gauss1->GetParameter(1) << '\n'
            << "Width = " << gauss1->GetParameter(2) << '\n'
            << "Chi / NDF = " << gauss1->GetChisquare() / gauss1->GetNDF()
            << '\n'
            << "Probability = " << gauss1->GetProb() << '\n';
  std::cout << "K* FROM PI-K COUPLES\n"
            << "Mass = " << gauss2->GetParameter(1) << '\n'
            << "Width = " << gauss2->GetParameter(2) << '\n'
            << "Chi / NDF = " << gauss2->GetChisquare() / gauss2->GetNDF()
            << '\n'
            << "Probability = " << gauss2->GetProb() << '\n';
  //  Canvas and cosmetics
  TCanvas* histo =
      new TCanvas("histo", "Particles, angles and impulse", 600, 500);
  histo->Divide(2, 2);
  h[0]->SetFillColor(4);
  h[1]->SetFillColor(2);
  h[2]->SetFillColor(2);
  h[3]->SetFillColor(3);
  for (int i = 0; i < 4; i++) {
    histo->cd(i + 1);
    h[i]->Draw();
  }

  TCanvas* invmass =
      new TCanvas("invmass", "Invariant mass histograms", 600, 500);
  invmass->Divide(3, 1);
  invmass->cd(1);
  m5->Draw();
  invmass->cd(2);
  msum1->Draw();
  invmass->cd(3);
  msum2->Draw();

  file->Close();
}

// rivedere msum1, rivedere titoli istogrammi mettere titoli negli assi, vedere
// valori xy, scegliere colori carini, fit m5?, rivedere nome parametri e box
// statistica, cambiare marker in invmass
