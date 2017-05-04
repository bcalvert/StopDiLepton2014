#include "TFile.h"
#include "TH2F.h"
#include "TObjArray.h"
#include "TList.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "math.h"
#include <iostream>

using namespace std;

// ######################################

vector<TGraph*> getContours(TH2F* h_, float level);
TH2F* smooth(TH2F* h);
float getNeighboursAverage(TH2F* h, int i, int j);

// ######################################

vector<TGraph*> getContours(TH2F* h_, float level)
{
  // Clone the input

  TH2F* h = (TH2F*) h_->Clone();

  // Set empty bins at arbitrary value 100 to not create artifical contour

  for (int i = 1 ; i <= h->GetNbinsX() ; i++)
    for (int j = 1 ; j <= h->GetNbinsY() ; j++)
      {
        int bin = h->GetBin(i,j);
        if (h->GetBinContent(bin) == 0)
	  h->SetBinContent(bin,100);
      }

  // Get the contours from root

  double levels[1];
  levels[0] = level;
  h->SetContour(1,levels);
  TCanvas* c = new TCanvas("whatever","",800,600);
  h->Draw("CONT Z LIST");
  c->Paint();
  c->Update();
  TList* list = (TList*) ((TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours"))->At(0);

  // Copy the TGraph and wrap everything in a vector<TGraph*>

  vector<TGraph*> contours;
  for (int g = 0 ; g < list->GetSize() ; g++)
    {
      TGraph* graph =  (TGraph*) list->At(g);
      int    n = graph->GetN();

      // Ignore too small contours,
      // likely to be only noise because of a 1 or 2 bins only
      if (n <= 5) continue;

      float* x = new float[n];
      float* y = new float[n];

      double* graphX = graph->GetX();
      double* graphY = graph->GetY();

      for (int i = 0 ; i < n ; i++)
        {
	  x[i] = graphX[i];
	  y[i] = graphY[i];
        }

      contours.push_back(new TGraph(n,x,y));
    }

  cout << "Number of contours found : " << contours.size() << endl;

  return contours;
}

TH2F* smooth(TH2F* h)
{
  // Clone the input

  TH2F* s = (TH2F*) h->Clone();

  // For each bin, get the average with neighbours, and set this as the new value
  // in the output

  for (int i = 1 ; i <= s->GetNbinsX() ; i++)
    for (int j = 1 ; j <= s->GetNbinsY() ; j++)
      {
        float avg = getNeighboursAverage(h,i,j);
        s->SetBinContent(s->GetBin(i,j),avg);
      }

  return s;
}

float getNeighboursAverage(TH2F* h, int i, int j)
{
  // Get content in the neighbours

  float content_center = h->GetBinContent(i,j);
  float content_up     = h->GetBinContent(i,j+1);
  float content_down   = h->GetBinContent(i,j-1);
  float content_left   = h->GetBinContent(i-1,j);
  float content_right  = h->GetBinContent(i+1,j);

  // If we're looking at a bin with zero value, ignore it

  if (content_center == 0) return 0;

  // Compute the average by considering only non-null neighboors and ignoring those
  // with a too large variation with respect to the center

  float average = content_center;
  int neighboors = 0;
  if ((content_up    != 0.0) && (content_up    > 0.5 * content_center) && (content_up    < 1.5 * content_center)) { average += content_up;    neighboors++; }
  if ((content_down  != 0.0) && (content_down  > 0.5 * content_center) && (content_down  < 1.5 * content_center)) { average += content_down;  neighboors++; }
  if ((content_left  != 0.0) && (content_left  > 0.5 * content_center) && (content_left  < 1.5 * content_center)) { average += content_left;  neighboors++; }
  if ((content_right != 0.0) && (content_right > 0.5 * content_center) && (content_right < 1.5 * content_center)) { average += content_right; neighboors++; }

  // Return the average

  average = average/(neighboors+1);

  return average;

}




