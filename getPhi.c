


void doThing2(TDirectory *dir,double low, double high) {
  TH1 *hist_tot;
  int first = 0;
  TList *keyList = dir->GetListOfKeys();
  keyList->Sort();

  TIter iter(keyList);
  
  vector<int> vDet;
  vector<double> vPhi;
  vector<double> vSum;
  vector<double> vTheta;

  

  while(TKey *key = (TKey*)iter.Next()) {
    TObject *obj = key->ReadObj();
    std::string name = obj->GetName();
    //std::string name("xtal68_0_1");
    if(first==0){
      hist_tot = (TH1*)obj;
    }
    TH1* hist = (TH1*)obj;
    if(first>0){
      hist_tot->Add(hist);
    }
    first++;

       

    

    /*size_t firstNumber = name.find_first_of("0123456789");
    size_t first_      = name.find_first_of("_");

    std::string sxtal = name.substr(firstNumber,first_-firstNumber);
    std::string sw1 = name.substr(first_+1,1);
    std::string sw2 = name.substr(first_+3,1);

    int xtal = stoi(sxtal);
    int w1   = stoi(sw1)-1;
    int w2   = stoi(sw2)-1;
    if(w1==w2) continue;

   

    if(low>high) std::swap(low,high);
    hist->GetXaxis()->SetRangeUser(low-100,high+100);

    GPeak *peak = PhotoPeakFit(hist,low,high);

    //cout << xtal << endl;
    //cout << w1 << endl;
    //cout << w2 << endl;

    TVector3 z(0,0,1);
    TVector3 v1 = TGretina::GetSegmentPosition(xtal,w1);
    TVector3 v2 = TGretina::GetSegmentPosition(xtal,w2);
    TVector3 v3 = v2-v1; 

    TVector3 rp  =  z.Cross(v1);
    TVector3 sp  = v1.Cross(v3);
    double phi   = rp.Angle(sp);  
    double theta = v1.Theta();

    //cout << xtal << "\t" << phi*TMath::RadToDeg() << "\t" << peak->GetSum() << endl;
    vDet.push_back(xtal);
    vTheta.push_back(theta*TMath::RadToDeg());
    vPhi.push_back(phi*TMath::RadToDeg());
    vSum.push_back(peak->GetSum());

    //cout << phi << endl;
    //cout << phi*180./3.14 << endl;*/
  }

  /*cout <<"\n\n";
  for(int i=0;i<vSum.size();i++) {
    cout << vDet.at(i) << "\t" << vTheta.at(i) << "\t" << vPhi.at(i) << "\t" << vSum.at(i) << endl;
  }*/
  cout << first << endl;
  new TCanvas;
  hist_tot->Draw();

}
