

{

  GH1D g1(*((TH1*)_file0->Get("ne25_1613")));
  GH1D g2(*((TH1*)_file0->Get("ne25_1621")));
  GH1D g3(*((TH1*)_file0->Get("ne25_1701")));
  //GH1D g4(*((TH1*)_file0->Get("bg_511")));
  //GH1D g5(*((TH1*)_file0->Get("bg_1460")));
  

  //g1.Rebin(8); //from 8192 to 512
  //g2.Rebin(8);
  //g3.Rebin(8);
  //g4.Rebin(8);
  //g5.Rebin(8);
  

  TF1 *f1 = g1.ConstructTF1_shift();
  TF1 *f2 = g2.ConstructTF1_shift();
  TF1 *f3 = g3.ConstructTF1_shift();
  //TF1 *f4 = g4.ConstructTF1_shift();
  //TF1 *f5 = g5.ConstructTF1_shift();


  TF1 *f15 = new TF1("f15","1/(1 + TMath::Exp(-(x-150) / (80/2.355)))*[0]*TMath::Exp([1]*x)",0,8192);
  f15->SetParameter(0,400);
  f15->SetParLimits(0,0,1000000);
  f15->SetParameter(1,-0.0024774);
  //f15->SetParLimits(1,-0.0024774*1.11,-0.0024774*0.9);
  f15->SetParLimits(1,-0.0024774*10,-0.0024774*0.1);

  TF1 *f16 = new TF1("f16","1/(1 + TMath::Exp(-(x-150) / (80/2.355)))*[0]*TMath::Exp([1]*x)",0,8192);
  f16->SetParameter(0,200);
  f16->SetParLimits(0,0,1000000);
  f16->SetParameter(1,-0.000519906);
  //f16->SetParLimits(1,-0.000519906*1.11,-0.000519906*0.9);
  f16->SetParLimits(1,-0.000519906*10,-0.000519906*0.1);

  
  //GH1D gthing(*((TH1*)_file0->Get("Si25_exp")));
  //GH1D gthing2(*((TH1*)_file0->Get("Si25_exp2")));
  
  //gthing.Rebin(4); //from 2048 to 512
  //gthing.Draw();
  //gthing2.Rebin(4);
  //new TCanvas;
  //gthing2.Draw();

  TF1Sum sum;
  sum.AddTF1(f1);
  sum.AddTF1(f2);
  sum.AddTF1(f3);
  //sum.AddTF1(f4);
  //sum.AddTF1(f5);
  sum.AddTF1(f15);
  sum.AddTF1(f16);


  sum.GetFunc()->SetNpx(8192);
  sum.GetFunc()->SetParameter(0,0.1);
  sum.GetFunc()->SetParLimits(0,0.000001,100);
  sum.GetFunc()->SetParameter(1,0.0);
  sum.GetFunc()->SetParLimits(1,-2.0,2.0);

  sum.GetFunc()->SetParameter(2,0.1); 
  sum.GetFunc()->SetParLimits(2,0.000001,100);
  sum.GetFunc()->SetParameter(3,0.0);
  sum.GetFunc()->SetParLimits(3,-2.0,2.0);

  sum.GetFunc()->SetParameter(4,0.1);
  sum.GetFunc()->SetParLimits(4,0.000001,100);
  sum.GetFunc()->SetParameter(5,0.0);
  sum.GetFunc()->SetParLimits(5,-2.0,2.0);
  
  /*sum.GetFunc()->SetParameter(6,0.1);
  sum.GetFunc()->SetParLimits(6,0.000001,100);
  sum.GetFunc()->SetParameter(7,0.0);
  sum.GetFunc()->SetParLimits(7,-15.0,15.0);

  sum.GetFunc()->SetParameter(8,0.1);
  sum.GetFunc()->SetParLimits(8,0.000001,100);
  sum.GetFunc()->SetParameter(9,0.0);
  sum.GetFunc()->SetParLimits(9,-15.0,15.0);*/


  f1->Print("V");
  sum.Print("V");

  //std::cout << "Ndim: " << sum.GetNdim() << std::endl;
  //std::cout << "NDF: " << sum.GetNDF() << std::endl;
  std::cout << "NPar: " << sum.GetNpar() << std::endl;

  //sum.SetLineColor(kRed);
  //g4.Fit(&sum);

}
