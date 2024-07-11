#include<GH1D.h>

#include<cstdio>

#include<TSpectrum.h>
#include<TString.h>
#include<TVirtualPad.h>
#include<TBox.h>

#include<TF1.h>
#include<fstream>

GH1D::GH1D() : TH1D(), fOriginal(0) { }

GH1D::GH1D(std::string name,int nbinsx,const double *xbins) :
  GH1D(name.c_str(),name.c_str(),nbinsx,xbins)  { }

GH1D::GH1D(std::string name,int nbinsx,const float  *xbins) :
  GH1D(name.c_str(),name.c_str(),nbinsx,xbins)  { }

GH1D::GH1D(std::string name,int nbinsx,double xlow, double xhigh) : 
  GH1D(name.c_str(),name.c_str(),nbinsx,xlow,xhigh)  { }

GH1D::GH1D(const char *name,const char *title,int nbinsx,const double *xbins) :
  TH1D(name,title,nbinsx,xbins), fOriginal(0), fParent(0)   { 
  Init();
}
 
GH1D::GH1D(const char *name,const char *title,int nbinsx,const float  *xbins) :
  TH1D(name,title,nbinsx,xbins), fOriginal(0), fParent(0)   { 
  Init();
} 

GH1D::GH1D(const char *name,const char *title,int nbinsx,double xlow, double xhigh) :
  TH1D(name,title,nbinsx,xlow,xhigh), fOriginal(0), fParent(0)    { 
  Init();
}

//GH1D::GH1D(const GH1D &h1d);
GH1D::GH1D(const TH1D &h1d) :
  TH1D(h1d), fOriginal(0), fParent(0)    { 
  Init();
} 

GH1D::GH1D(const TH1F &h1f) :
   fOriginal(0), fParent(0)    {  
  h1f.Copy(*this); 
  Init();
} 

GH1D::GH1D(const TVectorD &v) :
  TH1D(v), fOriginal(0), fParent(0)    { 
  Init();
}

GH1D::~GH1D() { 
  //printf("GH1D deleted\n"); fflush(stdout);  
  if(fOriginal)
    delete fOriginal;
} 

void GH1D::Init() { 
  this->SetBit(kNoTitle);
}


void GH1D::Paint(Option_t *opt) {
  //printf("\t-in gh1d paint.\n");
  //fflush(stdout);
  TString sopt(opt);
  bool drawFunctions=false;
  if(!sopt.Length()) {
    if(this->GetSumw2()->GetSize()) {
      sopt.Append("hist");
      drawFunctions=true;
    }
  }
  TH1D::Paint(sopt.Data());
  if(drawFunctions) {
    TIter iter(this->GetListOfFunctions());
    while(TObject *obj = iter.Next()) {
      //if(obj->InheritsFrom(TF1::Class() || TLine::Class())
      if((obj->InheritsFrom("TF1")) || (obj->InheritsFrom("TLine")))
        obj->Paint("lsame");
    }
  }
}

void GH1D::Draw(Option_t *opt) {
  //printf("GH1D Draw!\n"); fflush(stdout);
  TH1D::Draw(opt);
  if(gPad) {
    //gPad->Modified(); 
    gPad->Update();
    TBox *frame = (TBox*)gPad->GetListOfPrimitives()->FindObject("TFrame");
    if(frame)
      frame->SetBit(TBox::kCannotMove); 
  }
  return; 
}

TH1* GH1D::DrawCopy(Option_t *opt, const char *name_postfix) const {
  //printf("GH1D Draw!\n"); fflush(stdout);
  return TH1D::DrawCopy(opt,name_postfix);
}

TH1* GH1D::DrawNormalized(Option_t *opt, double norm) const {
  //printf("GH1D Draw!\n"); fflush(stdout);
  return TH1D::DrawNormalized(opt,norm);
} 


TH1* GH1D::Rebin(int ngroup,const char *newname,const double *xbins) {
  TString sname(newname);
  //find the current viewing range
  //double xlow = GetXaxis()->GetBinLowEdge(GetXaxis()->GetFirst());
  //double xup  = GetXaxis()->GetBinUpEdge(GetXaxis()->GetLast());

  if(sname.Length()==0) {
    // we are not going to return a new histogram
    // so lets copy the current histogram to allow us to unbin latter...
    if(!fOriginal) {
      fOriginal = new TH1D();  
      fOriginal->SetDirectory(0);
      fOriginalBins = GetNbinsX();
      dynamic_cast<TH1D*>(this)->Copy(*fOriginal);
      fOriginal->SetName(Form("_%s_copy",this->GetName()));
      fOriginal->SetDirectory(0);
    }
  }
  TH1 *temp = TH1D::Rebin(ngroup,newname,xbins);
  //printf("Rebinng:\n");
  //printf("current  bins = %i\n",GetNbinsX());  
  //printf("original bins = %i\n\n",GetNbinsOriginal());  
  //if(sname.Length()==0) 
  //  GetXaxis()->SetRangeUser(xlow,xup);
  return temp;
}

void GH1D::Unbin(int ngroup) {
  //printf("unbin called!\n");

  if(!fOriginal) return;
  //double xlow = GetXaxis()->GetBinLowEdge(GetXaxis()->GetFirst());
  //double xup  = GetXaxis()->GetBinUpEdge(GetXaxis()->GetLast());
  std::string fname = this->GetName();
  //printf("Unbinng:\n");
  //printf("name: %s\n",fname.c_str());
  //printf("current  bins = %i\n",GetNbinsX());  
  //printf("original bins = %i\n\n",GetNbinsOriginal());  

  int oldBins = GetNbinsX();

  TDirectory *current = this->GetDirectory();
  fOriginal->Copy(*(dynamic_cast<TH1D*>(this)));
  this->SetName(fname.c_str());
  //printf("name2 = %s\n",this->GetName());
  this->SetDirectory(current);
  //printf("name3 = %s\n",this->GetName());

  if(ngroup>0) {
    int aim = oldBins*ngroup;
    //printf("aim = %i\n",aim);
    if(aim<GetNbinsOriginal()) {
      while(GetNbinsX()>(aim+2)) {
        //printf("\tcurrent = %i\n",GetNbinsX());
        Rebin(ngroup);
      }
    }
  }
}


TH1* GH1D::ShowBackground(int niter,Option_t* opt) {
  return TH1D::ShowBackground(niter,opt);
}

void GH1D::Background() {
  double x1=sqrt(-1);
  double x2=sqrt(-1);
  if(this->GetXaxis()) {
    x1 = this->GetXaxis()->GetBinLowEdge(this->GetXaxis()->GetFirst());
    x2 = this->GetXaxis()->GetBinUpEdge(this->GetXaxis()->GetLast());
  }

  if(this->TestBit(GH1D::kBackgroundRemoved)) {
    //put the background back...
    std::string fname = this->GetName();
    TDirectory *current = this->GetDirectory();
    fOriginal->Copy(*(dynamic_cast<TH1D*>(this)));
    this->SetName(fname.c_str());
    //printf("name2 = %s\n",this->GetName());
    this->SetDirectory(current);

  } else {
    //remove the background...
    if(!fOriginal) {
      fOriginal = new TH1D();  
      fOriginal->SetDirectory(0);
      fOriginalBins = GetNbinsX();
      dynamic_cast<TH1D*>(this)->Copy(*fOriginal);
      fOriginal->SetName(Form("_%s_copy",this->GetName()));
      fOriginal->SetDirectory(0);
    }
    this->GetXaxis()->UnZoom();
    TH1 *bg = TSpectrum::StaticBackground(this,12,"");
    this->Add(bg,-1); 
    this->SetBit(GH1D::kBackgroundRemoved);
  }
  if(x1==x1) {
    //printf("%s I AM HERE!\n",GetName());
    this->GetXaxis()->SetRangeUser(x1,x2);
  }
}

double GH1D::FitEval(double *dim,double *par) {

  double x = dim[0];
  //printf("par = 0x%08x\n",par); fflush(stdout);
  double scale = par[0];

  double binNum = GetXaxis()->FindBin(x); //gHist->GetBin() does not respect rebinning.

  double nBins = GetNbinsX();
  double kevPerBin = GetXaxis()->GetXmax()/nBins;
  double curBinX = GetBinCenter(binNum);
  double nextBinX = GetBinCenter(binNum+1);
  double prevBinX = GetBinCenter(binNum-1);

  if (x > prevBinX && x <= curBinX){
    double leftDiff = x - prevBinX;
    double rightDiff = curBinX - x;

    leftDiff = 1.0 - leftDiff/(double)kevPerBin;   //These numbers are now less than 1
    rightDiff = 1.0 - rightDiff/(double)kevPerBin; //and a measure of how close it is to that bin
    double binContentLeft = GetBinContent(binNum-1);
    double binContentRight = GetBinContent(binNum);
    return scale * (leftDiff*binContentLeft+rightDiff*binContentRight);
  }

  else if (x > curBinX && x < nextBinX){
    double leftDiff = x - curBinX;
    double rightDiff = nextBinX - x;

    leftDiff = 1.0 - leftDiff/(double)kevPerBin;
    rightDiff = 1.0 - rightDiff/(double)kevPerBin;
    double binContentLeft = GetBinContent(binNum);
    double binContentRight = GetBinContent(binNum+1);
    return scale * (leftDiff*binContentLeft+rightDiff*binContentRight);
  }
  //std::cout << "FAILED IN HISTVALUE!" << std::endl;
  return scale * GetBinContent(binNum);
}

double GH1D::FitEval2(double *dim,double *par) {

  double x = dim[0] - par[1];
  //printf("par = 0x%08x\n",par); fflush(stdout);
  double scale = par[0];

  double binNum = GetXaxis()->FindBin(x); //gHist->GetBin() does not respect rebinning.

  double nBins = GetNbinsX();
  double kevPerBin = (GetXaxis()->GetXmax()-GetXaxis()->GetXmin())/nBins;
  double curBinX = GetBinCenter(binNum);
  double nextBinX = GetBinCenter(binNum+1);
  double prevBinX = GetBinCenter(binNum-1);

  if (x > prevBinX && x <= curBinX){
    double leftDiff = (x - prevBinX);
    double rightDiff = (curBinX - x);

    leftDiff = 1.0 - leftDiff/(double)kevPerBin;   //These numbers are now less than 1
    rightDiff = 1.0 - rightDiff/(double)kevPerBin; //and a measure of how close it is to that bin
    double binContentLeft = GetBinContent(binNum-1);
    double binContentRight = GetBinContent(binNum);
    return scale * (leftDiff*binContentLeft+rightDiff*binContentRight);
  }

  else if (x > curBinX && x < nextBinX){
    double leftDiff = x - curBinX;
    double rightDiff = nextBinX - x;

    leftDiff = 1.0 - leftDiff/(double)kevPerBin;
    rightDiff = 1.0 - rightDiff/(double)kevPerBin;
    double binContentLeft = GetBinContent(binNum);
    double binContentRight = GetBinContent(binNum+1);
    return scale * (leftDiff*binContentLeft+rightDiff*binContentRight);
  }
  //std::cout << "FAILED IN HISTVALUE!" << std::endl;
  return scale * GetBinContent(binNum);
}

TF1  *GH1D::ConstructTF1() const {
  if(GetDimension()!=1)
    return 0;

  ROOT::Math::ParamFunctor *f = new  ROOT::Math::ParamFunctor((GH1D*)this,&GH1D::FitEval);
  double low  = GetXaxis()->GetBinLowEdge(1);
  double high = GetXaxis()->GetBinUpEdge(GetXaxis()->GetNbins());

  TF1 *tf1 = new TF1(Form("%s_tf1",GetName()),*f,low,high,1,1);
  tf1->SetParameter(0,1.0);
  tf1->SetNpx(GetXaxis()->GetNbins());
  return tf1;

}

TF1  *GH1D::ConstructTF1_shift() const {
  //if(GetDimension()!=1)
    //return 0;

  ROOT::Math::ParamFunctor *f = new  ROOT::Math::ParamFunctor((GH1D*)this,&GH1D::FitEval2);
  double low  = GetXaxis()->GetXmin();
  double high = GetXaxis()->GetXmax();

  TF1 *tf1 = new TF1(Form("%s_tf1",GetName()),*f,low,high,2,1);
  tf1->SetParameter(0,1.0);
  tf1->SetParameter(1,0.0);
  tf1->SetNpx(GetXaxis()->GetNbins());
  return tf1;

}

bool GH1D::WriteDatFile(const char *outFile){
  if(strlen(outFile)<1) return 0;

  std::ofstream out;
  out.open(outFile);

  if(!(out.is_open())) return 0;

  for(int i=1;i<=GetNbinsX();i++){
    out << GetXaxis()->GetBinCenter(i) << "\t" << GetBinContent(i) << std::endl;
  }
  out << std::endl;
  out.close();

  return 1;
}

bool GH1D::WriteDatFileErrors(const char *outFile){
  if(strlen(outFile)<1) return 0;

  std::ofstream out;
  out.open(outFile);

  if(!(out.is_open())) return 0;

  for(int i=1;i<=GetNbinsX();i++){
    out << GetXaxis()->GetBinCenter(i) << "\t" << GetBinContent(i) << "\t" << GetBinError(i) << std::endl;
  }
  out << std::endl;
  out.close();  

  return 1;
}
