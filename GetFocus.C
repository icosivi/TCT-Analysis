{ 
  //  Macro for automatic focus finding procedure using Particulars TCT setup
  // You need to perform a scan that outputs a <example>.rtct file before using this macro
  // This macro was originally written in Ljubljana by Particulars, I modified it during my thesis work
  // Author: FEDERICO SIVIERO    ( created 3-15-2018)


  //How to run this macro: open a terminal and enter ROOT
  // gSystem->Load("TCTAnalyse.sl")       You need to download the TCTAnalyse libraries http://www.particulars.si/TCTAnalyse/
  // .X GetFocus("<path-to-input-file>/<input-file>.rtct")


  
  Int_t k=0;
  Int_t numO,numS;
  MeasureWF *wf[300];               // waveforms = reserve 300 
  TGraph *cc[300];                  // charge collection graph = reserve 300 
  Float_t width[300],pos[300];      // array of FWHM values and transition positions
  Float_t optical_axis_co[300];     // optical axis coordinate
  Float_t Os;                       // Optical axis step 


  PSTCT meas("<path-to-input-file>/<input-file>.rtct",20,2);       // measurement file
   //The first parameter is to set the scale such that signal start at t=0;
   //The second parameter is defined by byte order in float

  //  Uncomment for reflection corrections 
  /* meas.RefC=-0.1276; 
   meas.RefTs=0;
   meas.RefTe=26.2;*/


  meas.CorrectBaseLine();   // Baseline correction 
  meas.PrintInfo();         // Information about the read data 
  
  //------ USER INTERVENTION REQUIRED -----------------------------------
  //select the basic parameters of the analysis 
  Int_t step=2;                  //   select step factor (1=2um units; 2=1um units)
  Int_t optic_axis=2;            //   select optic axis (0=x,1=y,2=z)
  Int_t scanning_axis=0;         //   select scanning axis (0=x,1=y,2=z)
  Float_t LowLim=0;              //   low limit for fit   
  Float_t HiLim=40;              //   high limit for fit
  Float_t FWHM=10;               //   expected FWHM 
  Float_t Level=8;               //   start of flat level

  

  //------USER INTERVENTION - NOT REQUIRED -----------------------------
  //set scanning parameters 
  switch(optic_axis)
    {
    case 0: numO=meas.Nx-1; Os=meas.dx; break; //x-axis
    case 1: numO=meas.Ny-1; Os=meas.dy; break; //y-axis
    case 2: numO=meas.Nz-1; Os=meas.dz; break; //z-axis
    }
  switch(scanning_axis)
    {
    case 0: numS=meas.Nx-1; break; //x-axis
    case 1: numS=meas.Ny-1; break; //y-axis
    case 2: numS=meas.Nz-1; break; //z-axis
    }

  //--------------------------------------------------------------------
  //scan over the measurements and select waveforms

  for(int j=0;j<=numO;j++)
     {
       switch(optic_axis)
	 {
	 case 0: wf[j]=meas.Projection(1,scanning_axis,j,0,0,0,0,numS); break; //CHOOSE THE RIGTH CHANNEL IN 1st parameter!!!
	 case 1: wf[j]=meas.Projection(1,scanning_axis,0,j,0,0,0,numS); break; // 0=Ch[1]; 1=Ch[2]; etc..
	 case 2: wf[j]=meas.Projection(1,scanning_axis,0,0,j,0,0,numS); break;
	 } 
       
       wf[j]->SetTemperatures(meas.T);
       wf[j]->ScaleHisto(-0.001); //use any value to set the scale
       wf[j]->CorrectBaseLine(1); //correct base line - not needed
       cc[j]=wf[j]->CCE(0,10);   //integrate the charge in time window 0-80 ns
     }

  //--------------------------------------------------------------------
  //define fit function
  
  TF1 *ff=new TF1("ff","TMath::Erfc((x-[0])/[1])*[2]+[3]",LowLim,HiLim); 
  ff->SetParameter(0,Level); 
  ff->SetParameter(1,FWHM); 
  ff->SetParameter(2,8); 
  ff->SetParameter(3,4);
  //ff->SetParameter(4,1);  

  //-----------------------------------------------------
  //fitting

  float fw;
  double zz;
  
  for(int j=0;j<=numO;j++)
   {
     cc[j]->Fit("ff","R");
     cc[j]->GetFunction("ff")->SetLineColor(j%8+1); 
     width[j]=ff->GetParameter(1)*2.35/TMath::Sqrt(2);
     pos[j]=ff->GetParameter(0);
     optical_axis_co[j]=j*Os;

     if(j==0){ fw=width[j];
       zz=optical_axis_co[j];
     }else{
     if(width[j]<fw){  
       fw=width[j];
       zz=optical_axis_co[j];
     }
     }//
   }
  //-----------------------------------------------------

  
 //plot graphs at different positions along optical axis

  for(int j=1;j<=numO;j++)
    {
      cc[j]->SetLineColor(j%8+1);
      if(j==1)
	{
	  cc[j]->Draw("AL");
	   cc[j]->GetHistogram()->SetTitle("scanning axis vs collected charge");
	  cc[j]->GetHistogram()->GetXaxis()->SetTitle("scanning axis [#mum]");
	}
      else cc[j]->Draw("L");
    }

  //-----------------------------------------------------

  
 //draw the gaussian beam profile 
 TCanvas c2;
 TGraph *FWHMg=new TGraph(numO+1,optical_axis_co,width);
 FWHMg->SetMarkerStyle(21); 
 FWHMg->Draw("APL");
 FWHMg->GetHistogram()->SetTitle("Gaussian beam profile");
 FWHMg->GetHistogram()->GetXaxis()->SetTitle("optical distance [#mum]");
 FWHMg->GetHistogram()->GetYaxis()->SetTitle("FWHM [#mum]");
 
  //-----------------------------------------------------

 
  // Find the missalignment between z and optical axis 
 TCanvas c3;
 TGraph *POSg=new TGraph(numO+1,optical_axis_co,pos);
 POSg->SetMarkerStyle(21);
 POSg->Draw("APL");
 POSg->GetHistogram()->GetXaxis()->SetTitle("optical distance [#mum]");
 POSg->GetHistogram()->GetYaxis()->SetTitle("position of the edge [#mum]");
 POSg->Fit("pol1");

  //-----------------------------------------------------
  //-----------------------------------------------------

 if(step==1){
 double fw2=fw*2;
 cout<<"FWHM min:"<<fw2<<"um"<<endl;
 cout<<"corresponding z:"<<zz<<endl;
 }else{
 double fw2=fw;
 cout<<"FWHM min:"<<fw2<<"um"<<endl;
 cout<<"corresponding z:"<<zz<<endl;
 }

}
