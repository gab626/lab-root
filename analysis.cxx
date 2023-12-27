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
  TH1F* h3 = (TH1F*)file->Get("h3");
  TF1* f1 = new TF1("f1", "[0]", 0, 2 * M_PI);
  TF1* f2 = new TF1("f2", "[0]", 0, M_PI);
  TF1* f3 = new TF1("f3", "[0] * exp(-[1] * x)", 0, 7);
  f1->SetParameter(0, h1->GetMean());
  f2->SetParameter(0, h2->GetMean());
  f3->SetParameters(700000, h3->GetMean());
  h1->Fit("f1", "RQ");
  h2->Fit("f2", "RQ");
  h3->Fit("f3", "RQ");
  TF1* fit1 = h1->GetFunction("f1");
  TF1* fit2 = h2->GetFunction("f2");
  TF1* fit3 = h3->GetFunction("f3");

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
  TH1F* msum1 = new TH1F("msum1", "All particles", 200, 0.7, 1.1);
  TH1F* msum2 = new TH1F("msum2", "pi-K couples", 200, 0.7, 1.1);
  msum1->SetDirectory(0);
  msum2->SetDirectory(0);
  m5->SetDirectory(0);
  msum1->Add(m2, m1, 1, -1);
  msum2->Add(m4, m3, 1, -1);

  TCanvas* c1 = new TCanvas("c1", "All particles", 600, 500);
  msum1->Draw();
  TCanvas* c2 = new TCanvas("c2", "pi-K couples", 600, 500);
  msum2->Draw();
  TCanvas* c3 = new TCanvas("c3", "K* mass", 600, 500);
  m5->Draw();

  file->Close();
}