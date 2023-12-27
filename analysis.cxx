#include <cmath>
#include <iostream>

void setStyle() {}

void analysis() {
  TFile* file = new TFile("simulation.root");

  TH1F* h0 = (TH1F*)file->Get("h0");

  for (int i = 1; i < 8; i++) {
    std::cout << "Particles " << i << ": " << h0->GetBinContent(i) << " +/- "
              << h0->GetBinError(i) << '\n';
  }

  TH1F* h1 = (TH1F*)file->Get("h1");
  TH1F* h2 = (TH1F*)file->Get("h2");
  TF1* f1 = new TF1("f1", "[0]", 0, 2 * M_PI);
  TF1* f2 = new TF1("f2", "[0]", 0, M_PI);
  f1->SetParameter(0, h1->GetEntries() / h1->GetNbinsX());
  f2->SetParameter(0, h2->GetEntries() / h2->GetNbinsX());
  // f1->SetParLimits(0, 0, 2 * M_PI); SBAGLIATI (vanno implementati?)
  // f2->SetParLimits(0, 0, M_PI);
  h1->Fit("f1", "Q", 0, 2 * M_PI);
  h2->Fit("f2", "Q", 0, M_PI);

  std::cout << "Azimuthal fit parameter value: " << f1->GetParameter(0) << '\n'
            << "Chi / NDF = " << f1->GetChisquare() / f1->GetNDF() << '\n'
            << "Probability = " << f1->GetProb() << '\n';
  std::cout << "Polar fit parameter value: " << f2->GetParameter(0) << '\n'
            << "Chi / NDF = " << f2->GetChisquare() / f2->GetNDF() << '\n'
            << "Probability = " << f2->GetProb() << '\n';

  file->Close();
}

/* TCanvas* c1 = new TCanvas("c1", "Histograms", 200, 10, 1000, 1000);
  c1->Divide(3, 2);
  for (int i = 0; i < 6; i++) {
    c1->cd(i + 1);
    h[i]->Draw();
  }

  TCanvas* c2 = new TCanvas("c2", "Invariant mass", 200, 10, 1000, 1000);
  c2->Divide(3, 2);
  for (int i = 0; i < 6; i++) {
    c2->cd(i + 1);
    m[i]->Draw();
  } */