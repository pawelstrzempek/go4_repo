#include <stdio.h>
// #include "TTree.h"
// #include "TH1.h"
//#include "TGo4WinCond.h"

#include "base/EventProc.h"
#include "base/Event.h"
#include "hadaq/TdcSubEvent.h"
#include "hadaq/TrbProcessor.h"
#include "hadaq/TdcProcessor.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraph.h"
//#include "TStraw.cpp"
//#include "TStrawMap.cpp"

class PavelProcessor : public base::EventProc {
   public:

      base::H1handle  hRef1;  //!< one dimensional hist with all channels
      base::H2handle  hRef2;  //!< two dimensional hist with time diff for different channels
      base::H2handle  hTot2;  //!< two dimensional hist with time diff for different channels
      base::C1handle  testCanvas;
      TCanvas *can;
      //TH1F *histpawel;
      TGraph *strawMap1;      
      int n = 50;
      double x[60];
      double y[60];
     // TStrawMap *SS2;
	TH2F *SM;

      PavelProcessor() :
         base::EventProc("Pavel")
      {
         hRef1 = MakeH1("Ref1D","Time diff to reference channel", 5000, 1000., 1100., "diff,ns");
         hRef2 = MakeH2("Ref2D","Time diff to reference channel", 16, 0, 15, 5000, 1000., 1100., "ch;diff,ns");
         hTot2 = MakeH2("Tot2D","Time over threshold", 49, 0, 49, 2000, 0., 1000., "ch;tot,ns");
//	 testCanvas = MakeC1("Canvas my name",600,600);
//	 histpawel = new TH1F("histpawel","histpawel",2000,0,2000);
	   for (Int_t i=0;i<n;i++) {
     		x[i] = 0.1;
	        y[i] = i*1;
   		}


	 strawMap1 = new TGraph(n,x,y);
	 strawMap1->SetMarkerSize(3);
	 strawMap1->SetMarkerStyle(8);
//	 n=4;
//	SS2 = new TStrawMap(16,2);

      SM = new TH2F("SM","SM",40,0,40,40,0,40);

      can = new TCanvas("canPawel","canPawel",600,600);
      can->SetGrid();
      }

      virtual bool Process(base::Event* ev)
      {

	
	RegisterObject(SM); 
	RegisterObject(can);
//	SM->Draw();
//	can->Update();
//	RegisterObject(strawMap1);
//	n=n^3;
//	strawMap1->SetPoint(3,n,n);

//	TGo4WinCond* cond = MakeWinCond("randowm pawel", 0, 0, "histo name");
         // we use only subevents for TDC 0xe000 and 0xe0002
//	TCanvas *can = new TCanvas("canPawel","canPawel",600,600);
         hadaq::TdcSubEvent* sub0 =
               dynamic_cast<hadaq::TdcSubEvent*> (ev->GetSubEvent("TDC_E000"));

         hadaq::TdcSubEvent* sub2 =
               dynamic_cast<hadaq::TdcSubEvent*> (ev->GetSubEvent("TDC_E000"));

         // printf("NUM %u sub0 = %p sub2 = %p\n", ev->NumSubEvents(), sub0, sub2);

         if ((sub0==0) || (sub2==0)) return false;

         // first search reference time in the 2 channel
         double ref = 0;
         double rt = 0.0;
	 double ft = 0.0;
         
         for (unsigned cnt=0;cnt<sub2->Size();cnt++) {
            const hadaq::TdcMessageExt& ext = sub2->msg(cnt);

            if ((ext.msg().getHitChannel() == 2) && ext.msg().isHitRisingEdge()) {
               ref = ext.GetGlobalTime();
               break;
            }
         }

         // printf("Ref time in channel 2 is %f\n", ref);

         // ignore event where no reference channel is found
         if (ref==0) return false;

//         bool found[16];
//         for (int ch=0;ch<16;ch++) found[ch] = false;

         for (unsigned cnt=0;cnt<sub0->Size();cnt++) {
            const hadaq::TdcMessageExt& ext = sub0->msg(cnt);

            unsigned chid = ext.msg().getHitChannel();
            // ignore all even channels
//            if ((chid<1) || (chid>15) || (chid % 2 == 0)) continue;
            // ignore channels which already found
//            if (found[chid]) continue;

//            found[chid] = true;
	   	
            double tm = ext.GetGlobalTime() /* - ev->GetTriggerTime() */;
            if (ext.msg().isHitRisingEdge())
	    rt = ext.GetGlobalTime() /* - ev->GetTriggerTime() */;
	    if (!(ext.msg().isHitRisingEdge()))
            ft = ext.GetGlobalTime() /* - ev->GetTriggerTime() */;

	    if(rt != 0 && ft != 0){
		FillH2(hTot2,chid, (ft-rt)*1e9);
		rt = 0.;
		ft = 0.;

	//	if(chid == 5)
		//histpawel->Fill((ft-rt)*1e9);
	    }
	    if (ext.msg().isHitRisingEdge()){	
                FillH1(hRef1, (tm - ref)*1e9);
                FillH2(hRef2, chid, (tm - ref)*1e9);
	    }
//chid += 1;	
	//	strawMap->SetPoint(2,n,n);
	    switch((chid)/33){
		case 0: //first layer
			if(chid%2 ==0){
				SM->Fill((chid/2)+1,0); 
			}
			else
			{
				SM->Fill((chid/2)+1,1); 
			}
		break;
		case 1: //second layer
			chid = chid - 32;
                        if(chid%2 ==0){
                                SM->Fill((chid/2)+1,3);
                        }
                        else
                        {
                                SM->Fill((chid/2)+1,4);
                        }
		break;
		case 2:
			chid = chid - 64;
		        if(chid%2 ==0){
                                SM->Fill((chid/2)+1,6);
                        }
                        else
                        {
                                SM->Fill((chid/2)+1,7);
                        }
		break;
//
		default:
		break;
	    }
//	    SM->Fill((chid)%16,(chid)/16);			

}
         return true;
      }

};


void second()
{
   new PavelProcessor();
}


