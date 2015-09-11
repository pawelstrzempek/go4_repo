
void first()
{

   base::ProcMgr::instance()->SetTriggeredAnalysis();
   
   // base::ProcMgr::instance()->SetRawAnalysis(true);

   // this limits used for liner calibrations when nothing else is available
   hadaq::TdcMessage::SetFineLimits(31, 470);

   // default channel numbers and edges mask
   hadaq::TrbProcessor::SetDefaults(49, 0x3);

   hadaq::HldProcessor* hld = new hadaq::HldProcessor();

   // About time calibration - there are two possibilities
   // 1) automatic calibration after N hits in every enabled channel.
   //     Just use SetAutoCalibrations method for this
   // 2) generate calibration on base of provided data and than use it later statically for analysis
   //     Than one makes special run with SetWriteCalibrations() enabled.
   //     Later one reuse such calibrations enabling only LoadCalibrations() call

   hadaq::TrbProcessor* trb3 = new hadaq::TrbProcessor(0x8000, hld);
   trb3->SetHistFilling(4);
   trb3->SetCrossProcess(true);
   trb3->CreateTDC(0xE000, 0xE002, 0xE003);
   // enable automatic calibration, specify required number of hits in each channel
   trb3->SetAutoCalibrations(50000);
   // calculate and write static calibration at the end of the run
   //trb3->SetWriteCalibrations("run1");
   // load static calibration at the beginning of the run
   trb3->LoadCalibrations("run1");

   hadaq::TdcProcessor* tdc = hld->FindTDC(0xE000);
   for (int ch=1;ch<=48;ch+=1)
      tdc->SetRefChannel(ch, 1, 0xE000, 20000,  -100, 100., true);

   // When enabled, time of last hit will be used for reference calculations
  // tdc->SetUseLastHit(true);
}



