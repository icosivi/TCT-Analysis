{

  //macro to get the "collected charge vs position" plot starting from a typical TCT 2d-scan of the collected charge
  //This macro was originally written for LGAD sensors, in order to define the "gain vs position" plot. This feature is therefore implemented  // as well
  //Author: FEDERICO SIVIERO
  //Created 3/29/2018



  //insert the inputs files: typical .rtct files from a Particulars TCT scan
  PSTCT stct2("examples/F_200.rtct",20,2);
  stct2.CorrectBaseLine(); 
  stct2.PrintInfo();

   PSTCT stct3("examples/F_400.rtct",20,2);
  stct3.CorrectBaseLine(); 
  stct3.PrintInfo();

   PSTCT stct4("examples/F_500.rtct",20,2);
  stct4.CorrectBaseLine(); 
  stct4.PrintInfo();

 


   TH2F *hist2=stct2.Draw(0,0,0,0,0,0,0,15);
   TH2F *hist3=stct3.Draw(0,0,0,0,0,0,0,15);
   TH2F *hist4=stct4.Draw(0,0,0,0,0,0,0,15);
 
   
   TH1F *charge2=new TH1F("isto","isto",40,0,40);
   TH1F *charge3=new TH1F("isto","isto",40,0,40);
   TH1F *charge4=new TH1F("isto","isto",40,0,40);
  
   // this cycle fill 3 1D-histograms projecting along the x-axis the original 2D-scan
   
  int bin=0;
  for(int i=0;i<140;i++){
    TH1D *h2=hist2->ProjectionX("p2",bin,bin+1);
    TH1D *h3=hist3->ProjectionX("p3",bin,bin+1);
    TH1D *h4=hist4->ProjectionX("p4",bin,bin+1);
    double integr2=h2->Integral();
    double integr3=h3->Integral();
    double integr4=h4->Integral();
    charge2->Fill(bin,integr2);
    charge3->Fill(bin,integr3);
    charge4->Fill(bin,integr4);
  
    bin=bin+1;
  }

 
 charge4->SetMarkerStyle(21);
 charge3->SetMarkerStyle(21);
 charge4->SetLineColor(kGreen);
 charge3->SetLineColor(kRed);
 charge4->SetMarkerColor(kGreen);
 charge3->SetMarkerColor(kRed);
 charge2->Draw("HIST LP");
 charge4->Draw("HIST LP SAME"); 
 charge3->Draw("HIST LP SAME");


 TLegend *leg=new TLegend(0.2,0.3,0.5,0.5);
 leg->AddEntry(charge2,"W3 200V");
 leg->AddEntry(charge3,"W3 400V");
 leg->AddEntry(charge4,"W3 500V");
 leg->Draw();


 //Uncomment to produce the "gain vs position" plot

 //qref is the reference charge to which normalize the previous plot in order to obtain the gain

 /*double qref2=(charge2->GetBinContent(4)+charge2->GetBinContent(5))/2;
   double qref3=(charge3->GetBinContent(4)+charge3->GetBinContent(5))/2;
   double qref4=(charge4->GetBinContent(4)+charge4->GetBinContent(5))/2;



  TH1F *Charge2=new TH1F("isto","isto",40,0,40);
  TH1F *Charge3=new TH1F("isto","isto",40,0,40);
  TH1F *Charge4=new TH1F("isto","isto",40,0,40);
 
  
//This cycle fills three histograms, equal to the previous ones but with the normalized charge, i.e. the gain of the device

  int bin2=0;
  for(int i=0;i<140;i++){
    
    double q2=charge2->GetBinContent(bin2);
    Charge2->Fill(bin2,(q2/qref2));
    double q3=charge3->GetBinContent(bin2);
    Charge3->Fill(bin2,(q3/qref3));
    double q4=charge4->GetBinContent(bin2);
    Charge4->Fill(bin2,(q4/qref4));
    
    bin2=bin2+1;
  }

//plot the gains 

 Charge4->SetMarkerStyle(21);
 Charge3->SetMarkerStyle(21);
 Charge4->SetLineColor(kGreen);
 Charge3->SetLineColor(kRed);
 Charge4->SetMarkerColor(kGreen);
 Charge3->SetMarkerColor(kRed);
 Charge2->Draw("HIST LP");
 Charge4->Draw("HIST LP SAME"); 
 Charge3->Draw("HIST LP SAME");





 */
  
}
