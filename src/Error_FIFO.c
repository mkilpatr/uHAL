#include "DDR.c"

//Test resets on the DDR and Delay studies varying delays or incorrect markers
void Test_Timeouts_Error( int Delay1, int Delay2, int Loops, int Choice_Input){


  using namespace std;

  const char* cHWFile;

  if(Choice_Input == 0){
  	std::string filename ("settings/HWDescription_Delay.xml");
  	cHWFile = filename.c_str();
  }
  else if(Choice_Input == 1){
  	std::string filename ("settings/HWDescription_Resets.xml");
  	cHWFile = filename.c_str();
  }
  else if(Choice_Input == 3){
  	std::string filename ("settings/HWDescription_PKAM.xml");
  	cHWFile = filename.c_str();
  }

    uhal::setLogLevelTo(uhal::Debug());

    // instantiate System Controller
    SystemController cSystemController;
    Amc13Controller  cAmc13Controller;
    
    // initialize map of settings so I can know the proper number of acquisitions and TBMs
    cSystemController.InitializeSettings(cHWFile, std::cout);
    
    // initialize HWdescription from XML, beware, settings have to be read first
    cAmc13Controller.InitializeAmc13( cHWFile, std::cout );
    cSystemController.InitializeHw(cHWFile, std::cout);
    
    auto cSetting = cSystemController.fSettingsMap.find("NAcq");
    int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
    cSetting = cSystemController.fSettingsMap.find("BlockSize");
    int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;
    
    cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
    int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
    
    cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
    int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;
    
    cAmc13Controller.ConfigureAmc13( std::cout );
    cSystemController.ConfigureHw(std::cout );
    
    std::cout << "Configure OK" << std::endl;

  // Can loop over all FEDs since we aren't writing to GLIB registers within the program
  // get the board info of all boards and start the acquistion
  //for (auto& cFED : cSystemController.fPixFEDVector)
  //{
    auto& cFED = cSystemController.fPixFEDVector[0];
    cSystemController.fFEDInterface->getBoardInfo(cFED);
    cSystemController.fFEDInterface->findPhases(cFED);
    cSystemController.fFEDInterface->Start (cFED);
  //}

  std::vector<uint32_t> Error_FIFO;
  std::vector<uint32_t> Error_Analyze;

  int Incorrect_Event_Num = 0;
  int Timeout_Error = 0;
  int pixel_hit = 0;
  uint64_t TBM_Mask_1 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_1");
  uint64_t TBM_Mask_2 = cSystemController.fFEDInterface->ReadBoardReg(cFED, "pixfed_ctrl_regs.TBM_MASK_2");
  uint64_t TBM_Mask = (TBM_Mask_2 << 32) | TBM_Mask_1;
  int MarkerCount = Marker_Read();

  std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
  cAmc13Controller.fAmc13Interface->SendEC0();
  cAmc13Controller.fAmc13Interface->EnableBGO(0);
  sleep(0.001);
  cAmc13Controller.fAmc13Interface->DisableBGO(0);

  for (int l = 0; l < Loops; l++){

    if( l == 1 && Choice_Input == 1){
	cAmc13Controller.fAmc13Interface->EnableBGO(1);
  	sleep(0.001);
  	cAmc13Controller.fAmc13Interface->DisableBGO(1);

    }

    //for (auto& cFED : cSystemController.fPixFEDVector){

      if(l == 0){
  	cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
        if(Choice_Input == 0)
		Delay_Time(Delay1);
        sleep(4);
        cAmc13Controller.fAmc13Interface->BurstL1A();
      }
      else if(l == 1){
        if(Choice_Input == 0)
		Delay_Time(Delay2);
        sleep(0.04);
        cAmc13Controller.fAmc13Interface->BurstL1A();
      }

      else{
	if(Choice_Input == 0)
        	Delay_Time(Delay1);
        sleep(0.04);
        cAmc13Controller.fAmc13Interface->BurstL1A();
      }

      sleep(0.001);

      cSystemController.fFEDInterface->readSpyFIFO(cFED);
      //cSystemController.fFEDInterface->readErrorFIFO(cFED, true);
      Error_FIFO = cSystemController.fFEDInterface->readErrorFIFO_vec(cFED, true);
      cSystemController.fFEDInterface->readTTSState(cFED);
      cSystemController.fFEDInterface->ReadData(cFED, 0 );

    //}

    Error_Analyze = Error_Event(Error_FIFO, Timeout_Error, TBM_Mask, MarkerCount);
    Incorrect_Event_Num += Error_Analyze[0];
    Timeout_Error += Error_Analyze[1];
    pixel_hit += Error_Analyze[2];

  }
	std::cout << "There were " << Incorrect_Event_Num << " Incorrect Event numbers." << std::endl;
	std::cout << "There were " << Timeout_Error << " timeout errors." << std::endl;
	std::cout << "There were " << pixel_hit << " pixel hits." << std::endl;
 
}
