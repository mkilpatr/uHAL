#include "FIFO1.c"

void Scan_Pixels();
void Scan_ADC( int increment );
int Set_Cal(uint32_t cal_input);
int Test_Hits_Resets(int test_input);
void Test_Hits_Full(int loops_input, int col_start, int col_input, int row_start, int row_input, int adc_start, int adc_input, int choice_input);
void Test_Phases(int fiber_input, int time_input);
void Stack_Test(int loops_input, int choice_input);

void Scan_Pixels() 
{
  using namespace std;
  using namespace uhal;
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
  HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );

  int j,k,i;
  int Max_Column = 0;
  int Max_Row = 0;
  int Hit_infoA_Start = 0;
  int Hit_infoB_Start = 0;
  //int start = 0;
  //int temp[10] = {0};
  //int* information = temp;
  int row_loops = 0;
  ValWord < uint32_t > mem, mem2, mem3;
  
  string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
  string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"}; 
	
  Max_Column = 51;
  Max_Row = 79;
  Hit_infoA_Start = 0;
  Hit_infoB_Start = 0;

  int Hit_infoA_dummy = Hit_infoA_Start;
  int Hit_infoB_dummy = Hit_infoB_Start;

  for( j = 0; j < Max_Row; j++){
    int Hit_infoA = Hit_infoA_dummy;
    int Hit_infoB = Hit_infoB_dummy;
  		
    for( k = 0; k < Max_Column; k++){

      for( i = 0; i < 8; i++){
	Write_ROCs( i, Hit_infoA, Hit_infoB);
      }	

      //information = Test_Hits(information,start);

      //start = 1;
      Hit_infoA = Hit_infoA + 0x00000100;
      Hit_infoB = Hit_infoB + 0x00000100;	

    }

    row_loops++;
    Hit_infoA = Hit_infoA + (0x00010000 * row_loops);
    Hit_infoB = Hit_infoB + (0x00010000 * row_loops);
  }
}

void Scan_ADC(int increment)
{
  using namespace uhal;
  using namespace std;
  ConnectionManager manager ("file://test/dummy_connections.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card
  HwInterface hw3=manager.getDevice ( "GLIB.crate.slot_11" );

  int i,j;
  int min_adc = 0;
  int max_adc = 0;
  ValWord < uint32_t > mem,mem2,mem3; 
  string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
  string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};

  min_adc = 0;
  if (min_adc < 0) {
    min_adc = 0;
  }
  else if (min_adc > 255) {
    min_adc = 255;
  }
  max_adc = 255;
  if (max_adc < min_adc && max_adc != 0) {
    max_adc = min_adc;
    std::cout << "Max is less than min." << std::endl;
  }
  else if (max_adc > 255 || max_adc == 0) {
    max_adc = 255;
  }
   
  int hit_info = min_adc;

  for (i=min_adc;i<max_adc;i=i+increment){
    for (j=0;j<8;j++){
      Write_ROCs( j, hit_info, hit_info);
    }   
    
    hit_info = hit_info + (0x00000001*increment);
    std::cout << "Right here" << std::endl;
  }

}

int Set_Cal(uint32_t cal_input) {

  using namespace std;
  using namespace uhal;

  ConnectionManager manager ("file://test/dummy_connections_mulit_chan.xml");
  HwInterface hw=manager.getDevice ( "GLIB.crate.slot_9" );   //this is to glib with the optical card
  HwInterface hw2=manager.getDevice ( "GLIB.crate.slot_10" );  //this is the glib with the debug card

  ValWord < uint32_t > mem,mem2;

  if (cal_input > 1) {
    std::cout << "Not a valid option. Exiting program." << std::endl;
    return 0;
  }
  else {
 
    hw.getNode("Cal_Trigger").write( cal_input );
    mem = hw.getNode ( "Cal_Trigger" ).read();
    hw.dispatch();
    std::cout << "Cal Trigger set to " << std::hex << (mem.value() & 0xF) << std::endl;

    hw2.getNode("Cal_Trigger").write( cal_input );
    mem2 = hw2.getNode( "Cal_Trigger" ).read();
    hw2.dispatch();
    std::cout << "Cal Trigger set to " << std::hex << (mem.value() & 0xF) << std::endl;
  }

  if (cal_input == 0) {
    return 0;
  }

  std::cout << "Calibration options:" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "  1. Calibrate Mode" << std::endl;
  std::cout << "  2. Sync Mode" << std::endl;
  std::cout << "  3. Reset Mode" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Which calibration mode would you like to test? ";
  int cal_choice;
  cin >> cal_choice;

    std::string filename ("settings/HWDescription_Resets.xml");
    const char* cHWFile = filename.c_str();
  
    uhal::setLogLevelTo(uhal::Debug());

    // instantiate System Controller
    SystemController cSystemController;
    Amc13Controller  cAmc13Controller;

    // initialize map of settings so I can know the proper number of acquisitions and TBMs
    cSystemController.InitializeSettings(cHWFile, std::cout);

    // initialize HWdescription from XML, beware, settings have to be read first
    cAmc13Controller.InitializeAmc13( cHWFile, std::cout );
    cSystemController.InitializeHw(cHWFile, std::cout);

    // configure the HW
    cAmc13Controller.ConfigureAmc13( std::cout );
    cSystemController.ConfigureHw(std::cout );

    //cAmc13Controller.fAmc13Interface->StartL1A();
    //for (fAmc13Controller->BGO vector)
    cAmc13Controller.fAmc13Interface->EnableBGO(0);

    auto cSetting = cSystemController.fSettingsMap.find("NAcq");
    int cNAcq = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 10;
    cSetting = cSystemController.fSettingsMap.find("BlockSize");
    int cBlockSize = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 2;

    cSetting = cSystemController.fSettingsMap.find("ChannelOfInterest");
    int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

    cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
    int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

    //get the board info of all boards and start the acquistion
    //for (auto& cFED : cSystemController.fPixFEDVector)
    //{
      auto& cFED = cSystemController.fPixFEDVector[0];
      cSystemController.fFEDInterface->getBoardInfo(cFED);
      cSystemController.fFEDInterface->findPhases(cFED);
      cSystemController.fFEDInterface->Start (cFED);
    //}

    auto& cFED2 = cSystemController.fPixFEDVector[1];
    auto& cFED3 = cSystemController.fPixFEDVector[2];
    auto& cFED4 = cSystemController.fPixFEDVector[3];

    std::vector<unsigned int> FIFO;
    std::vector<unsigned int> FIFO_CHB;
    std::vector<unsigned int> Event_Number;
    unsigned int Event_one = 0;
    unsigned int Event_two = 0;

    int ROC_Value = 0;
    int Compare_Event = 0;
    int Loops = 1;
    uint32_t Current_Event_Num = 0;
    int Error_Count = 0;
    int Do_Error = 0;
    int Pixel = 0;
    int Pixel_Hit = 0;
    std::fill(FIFO.begin(), FIFO.end(), 0);
    std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);

  if (cal_choice == 1) {
    //Run Calibration mode test
  }
  else if (cal_choice == 2) {
    //Run Sync mode test

    for (int l = 0; l < Loops; l++) {
      
      for (int i = 0; i < 8; i++) {
        //cSystemController.fFEDInterface->WriteBoardReg(cFED2, "sync_test" , 1);

	//cSystemController.fFEDInterface->WriteBoardReg(cFED3, "sync_test" , 1);
      }

      cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);   

      sleep(4);
      //cAmc13Controller.fAmc13Interface->BurstL1A();
      sleep(0.001);

      FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
      FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
 
      for( std::vector<unsigned int>::size_type i = 0; i != FIFO.size(); i++ )
      {

        if (FIFO[i] == 0x0) {
          break;
        }

        if (((FIFO[i] >> 4) & 0xF) == 0x9) {
          if ((FIFO[i] & 0xF) == 0) {
            std::cout << "Sync test succesfull!" << std::endl;
          }
          else {
            std::cout << "Sync test failed" << std::endl;
            std::cout << "Event Num = " << (FIFO[i] & 0xF) << std::endl;
          }
        }

      }

    }

  return 0;

  }
  else if (cal_choice == 3) {
    //Run Reset mode test
  

    for (int l = 0; l < Loops; l++) {
      
      if (l == 2) {
        //cSystemController.fFEDInterface->WriteBoardReg(cFED2, "reset_test", 1);
        //cSystemController.fFEDInterface->WriteBoardReg(cFED3, "reset_test", 1);
      }

      for (int i = 0; i < 8; i++) {
        //cSystemController.fFEDInterface->WriteBoardReg(cFED2, "sync_test" , 1);

	//cSystemController.fFEDInterface->WriteBoardReg(cFED3, "sync_test" , 1);
      }

      cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);   

      if (l == 0) {
        sleep(4);
        //cAmc13Controller.fAmc13Interface->BurstL1A();
        sleep(0.001);
      }
      else {
        cAmc13Controller.fAmc13Interface->BurstL1A();
        sleep(0.001);
      }

      FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
      FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
 
      for( std::vector<unsigned int>::size_type i = 0; i != FIFO.size(); i++ )
      {

        if (FIFO[i] == 0x0) {
          break;
        }

        if (((FIFO[i] >> 4) & 0xF) == 0x9) {
          
          std::cout << "Event Number = " << (FIFO[i] & 0xF) << std::endl;

        }

      }

    }

  return 0;
  
  }
  else {
    std::cout << "Not a valid option. Exiting program" << std::endl;
    return 0;
  }


}


void Test_Hits_Full(int loops_input, int col_start, int col_input, int row_start, int row_input, int adc_start, int adc_input, int choice_input)
{

  using namespace std;
  using namespace uhal;

  const char* cHWFile;

  std::string filename ("settings/HWDescription_Delay_single.xml");
  cHWFile = filename.c_str();

  string ROCSA[ 8 ] = {"CHA_ROC0", "CHA_ROC1", "CHA_ROC2", "CHA_ROC3", "CHA_ROC4", "CHA_ROC5", "CHA_ROC6", "CHA_ROC7"};
  string ROCSB[ 8 ] = {"CHB_ROC0", "CHB_ROC1", "CHB_ROC2", "CHB_ROC3", "CHB_ROC4", "CHB_ROC5", "CHB_ROC6", "CHB_ROC7"};

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

  // configure the HW
  cAmc13Controller.ConfigureAmc13( std::cout );
  cSystemController.ConfigureHw(std::cout );

  auto& cFED = cSystemController.fPixFEDVector[0];
  cSystemController.fFEDInterface->getBoardInfo(cFED);
  cSystemController.fFEDInterface->findPhases(cFED);
  cSystemController.fFEDInterface->Start (cFED);
 
  auto& cFED2 = cSystemController.fPixFEDVector[1];
  auto& cFED3 = cSystemController.fPixFEDVector[2];
  auto& cFED4 = cSystemController.fPixFEDVector[3];

  std::vector<unsigned int> FIFO;
  std::vector<unsigned int> FIFO_CHB;
  std::vector<std::vector<unsigned int>> FIFO1;
  std::vector<unsigned int> FIFO1_marker;
  std::vector<unsigned int> Event_Number;
  unsigned int Event_one = 0;
  unsigned int Event_two = 0;

  int Total_Events = 0;
  int Incorrect_Header = 0;
  int Incorrect_Trailer = 0;
  int Incorrect_Event_Num = 0;
  int Incorrect_ROC = 0;
  int Incorrect_adc = 0;
  int Incorrect_col = 0;
  int Incorrect_row = 0;
  int Incorrect_pix_info = 0;
  int ROC_Value = 0;
  int Compare_Event = 0;
  int Loops = loops_input;
  uint32_t Current_Event_Num = 0;
  uint32_t Current_col_num = 0;
  uint32_t Current_row_num = 0;
  uint32_t Current_adc_num = 0;
  int Error_Count = 0;
  int Do_Error = 0;
  int Pixel = 0;
  int Pixel_Hit = 0;
  int Incorrect_Pixel = 0;
  int new_row = 0;
  int Hit_infoA_buff = 0;
  int Hit_infoB_buff = 0;
  int Hit_infoA_new = 0;
  int Hit_infoB_new = 0;
  std::fill(FIFO.begin(), FIFO.end(), 0);
  std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);
  //std::fill(FIFO1.begin(), FIFO1.end(), 0);
  std::fill(FIFO1_marker.begin(), FIFO1_marker.end(), 0);

  cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
  cAmc13Controller.fAmc13Interface->SendEC0();
  cAmc13Controller.fAmc13Interface->EnableBGO(0);
  cAmc13Controller.fAmc13Interface->DisableBGO(0);

  int max_row = row_input; //atoi(argv[3]); //80
  if (max_row > 80 || max_row == 0) {
  	max_row = 80;
  }
  int max_col = col_input; //atoi(argv[4]); //52
  if (max_col > 52 || max_col == 0) {
  	max_col = 52;
  }
  int max_adc = adc_input; //atoi(argv[5]); //255
  if (max_adc > 255 || max_adc == 0) {
  	max_adc = 255;
  }

  int adc_loops = 0;
  int col_loops = 0;
  int row_loops = 0;

  int test_adc = 0;
  int test_col = 0;
  int test_row = 0;

  int num_ROC = -1;

  std::string index[8] = {"0","1","2","3","4","5","6","7"};
  std::string fiber ("Fiber_");

  for (int l = 0; l < Loops; l++) 
  {
    if(l%5000 == 0){
      std::cout << "Loop Number = " << std::dec << l << std::endl;
    }     

    int Hit_infoA = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);
    int Hit_infoB = (0x00000001 * adc_start) + (0x00000100 * col_start) + (0x00010000 * row_start);

    int row = 0;
    int col = 0;
    int adc = 0;
    int dcol = 0;

    for (row = row_start; row < max_row; row++) {

      for (col = col_start; col < max_col; col++) {

	if(col % 2 == 0)
		dcol = col / 2;

      	std::cout << "Starting row " << std::dec << row << ", column " << std::dec << col << std::endl;

        for (adc = adc_start; adc < 8/*max_adc*/; adc+=8) {

          for (int j = 0; j < 8; j++) {

            for (int i = 0; i < 8; i++) {
              //Write_ROCs( i, Hit_infoA, Hit_infoB); 

	      string ROCA = fiber + index[i] + "." + ROCSA[j];
	      string ROCB = fiber + index[i] + "." + ROCSB[j];
	
	      Hit_infoA_buff = Illegal_Col(dcol);
              Hit_infoB_buff = Illegal_Col(dcol);
              new_row = New_Row(row, col);

              Hit_infoA_new = (new_row << 16) | (Hit_infoA_buff << 8) | (Hit_infoA & 0xFF);
              Hit_infoB_new = (new_row << 16) | (Hit_infoB_buff << 8) | (Hit_infoB & 0xFF);

	      cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCA, Hit_infoA_new);
	      cSystemController.fFEDInterface->WriteBoardReg(cFED2, ROCB, Hit_infoB_new);

	      cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCA, Hit_infoA_new);
              cSystemController.fFEDInterface->WriteBoardReg(cFED3, ROCB, Hit_infoB_new);
          
	      cSystemController.fFEDInterface->WriteBoardReg(cFED4, ROCA, Hit_infoA_new);
              cSystemController.fFEDInterface->WriteBoardReg(cFED4, ROCB, Hit_infoB_new);

              ROCSA[j] = ROCSA[j].substr(0,9);
              ROCSB[j] = ROCSB[j].substr(0,9);

            }

            Hit_infoA = Hit_infoA + 0x00000001;
            Hit_infoB = Hit_infoB + 0x00000001;

            adc_loops++;
 
          }


            if(l == 0 && row == 0 && col == 0 && adc == 0){
  	    cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);   
	      sleep(4);
	      cAmc13Controller.fAmc13Interface->BurstL1A();
	      sleep(0.001);
            }
            else{
	      cAmc13Controller.fAmc13Interface->BurstL1A();
	      sleep(0.001);
            }

            FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
            FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
            FIFO1 = cSystemController.fFEDInterface->readFIFO1_vec(cFED);
   //         FIFO1_marker = cSystemController.fFEDInterface->readFIFO1Marker(cFED);

	  int k = 0;
	  int j = 0;
          int h = 0;
	  int Next_step = 0;
	  int Next_step_roc = 1;
	  int Num_ROC = 0;
	  int END_Event = 0;
	  int Error_Loop = 0;
	  int Start_Event = 0;
	  int Stack[2];
	  int Do_Stack = 0;

	  for( std::vector<unsigned int>::size_type i = 0; i != FIFO.size(); i++ )
	  {

	    if(FIFO[i] == 0x00){
	      break;
	    }

	    if(((FIFO[i] >> 4) & 0xF) == 0x8){
	      Error_Count = 0;
	      Error_Loop = 0;
	      h++;
	      if( h >= 2 ){
	        k = 1;
	      }
	      //std::cout << "****************** Start bitstream ****************" << std::endl;
	    }

	    // if (FIFO[i] >> 4 == 0x7){
	    //   std::cout << "0x" << FIFO[i] << " **********************" << std::endl;
	    // }
	    // else {
	    // 	 std::cout << "0x" << FIFO[i] << std::endl;
	    // }
	    				
	    if(((FIFO[i] >> 4) & 0xF) == 0x9)
	    {
	      j = i;
	      Total_Events++;
	      Current_Event_Num = ((FIFO[i - 1] & 0xF) << 4) | (FIFO[i] & 0xF);
	      if(Current_Event_Num - Compare_Event != 1 && Compare_Event - Current_Event_Num != 255){
	        Incorrect_Event_Num++;
                Error_Count++;
	      }
	      Compare_Event = Current_Event_Num;
	    }

            // if (FIFO[i] >> 4 == 0x7 && Next_step_roc == 0){
	    //   Next_step_roc = 1;
	    //   continue;
	    // }
	    // else if (FIFO[i] >> 4 == 0x7 && Next_step_roc != 0){
	    // 	 Incorrect_pix_info++;
	    //   Error_Count++;
	    // 	 continue;
	    // }

            //int test_fif = ((FIFO[i] >> 4) & 0xF);
            //std::cout << "FIFO[i] = " << std::hex << test_fif << std::endl;

	    if (((FIFO[i] >> 4) & 0xF) == 0x1 && Next_step_roc == 1) {
              Next_step_roc = 2;
              num_ROC++;
	      continue;
	    }
	    else if (((FIFO[i] >> 4) & 0xF) == 0x1 && Next_step_roc != 1){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 2;
	      continue;
	    }

            if (((FIFO[i] >> 4) & 0xF) == 0x2 && Next_step_roc == 2) {
	      Current_col_num = ((FIFO[i - 1] & 0xF) << 2) | ((FIFO[i] & 0xF) >> 2);
	      test_col = Hit_infoA_buff;
	      //std::cout << "col = " << std::dec << col << " Current_col_num = " << std::dec << Current_col_num << std::endl;
	      // std::cout << std::hex << FIFO[i-1] << " = " << std::dec << FIFO[i-1] << std::endl;
	      // std::cout << std::hex << FIFO[i] << " = " << std::dec << FIFO[i] << std::endl;
	      // std::cout << std::hex << FIFO[i] << " >> " << std::hex << (FIFO[i] & 0xF) << " >> " << std::hex << ((FIFO[i] & 0xF) >> 2) << std::endl;
	      // std::cout << std::hex << ((FIFO[i] & 0xF) >> 2) << " turns into " << std::hex << ((FIFO[i - 1] & 0xF) << 2) << " | " << std::hex << ((FIFO[i] & 0xF) >> 2) << " = " << std::dec << Current_col_num << " =? " << std::dec << col << std::endl;
	      // if (test_col = 36){
	      //   test_col = 16;
	      // }
	      if (Current_col_num != test_col){
	        Incorrect_col++;
		Error_Count++;
		std::cout << std::dec << Current_col_num << " not equal to " << test_col << std::endl;
	        std::cout << std::bitset<31> (Hit_infoA_new) << std::dec << std::endl;
	      }
	      Next_step_roc = 3;
	    }
	    else if (((FIFO[i] >> 4) & 0xF) == 0x2 && Next_step_roc != 2){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 3;
	      continue;
	    }

            if (((FIFO[i] >> 4) & 0xF) == 0x3 && Next_step_roc == 3){
	      Next_step_roc = 4;
	      continue;
	    }
	    else if (((FIFO[i] >> 4) & 0xF) == 0x3 && Next_step_roc == 3){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 4;
	      continue;
	    }

	    if (((FIFO[i] >> 4) & 0xF) == 0x4 && Next_step_roc == 4) {
              Current_row_num = ((FIFO[i-2] & 0x3) << 7) | ((FIFO[i-1] & 0xF) << 3) | ((FIFO[i] & 0xF) >> 1);
	      test_row = new_row;
	      //std::cout << "Current_row_num = " << std::dec << Current_row_num << " row = " << std::dec << row << std::endl;
	      if (Current_row_num != test_row){
	        Incorrect_row++;
	        Error_Count++;
	      	std::cout << Current_row_num << " not equal to " << test_row << std::endl;
	    	std::cout << std::bitset<31> (Hit_infoA_new) << std::dec << std::endl;
	      }
	      Next_step_roc = 5;
	    }
	    else if (((FIFO[i] >> 4) & 0xF) == 0x4 && Next_step_roc != 4){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 5;
	      continue;
	    }

	    if (((FIFO[i] >> 4) & 0xF) == 0x5 && Next_step_roc == 5){
	      Next_step_roc = 6;
	      continue;
	    }
	    else if (((FIFO[i] >> 4) & 0xF) == 0x5 && Next_step_roc != 5){
	      Incorrect_pix_info++;
	      Error_Count++;
	      Next_step_roc = 6;
	      continue;
	    }

	    if (((FIFO[i] >> 4) & 0xF) == 0x6 && Next_step_roc == 6) {
              Current_adc_num = (((FIFO[i-2] & 0xF) & 0x1) << 7) | (((FIFO[i-1] & 0xF) >> 1) << 4) | (FIFO[i] & 0xF);
              Current_adc_num = Current_adc_num;
              test_adc = adc_loops - (8 - num_ROC);
	      //std::cout << "Current_adc_num = " << std::dec << Current_adc_num << " adc = " << std::dec << adc << std::endl;
	      //if (Current_adc_num == 36){
	      //  test_adc = 16;
	      //}
	      if (Current_adc_num != test_adc){
	        Incorrect_adc++;
		Error_Count++;
	      }
              Next_step_roc = 1;						 
            }
	    else if (((FIFO[i] >> 4) & 0xF) == 6 && Next_step_roc != 6) {
	      Incorrect_pix_info++;
	      Error_Count++;
              Next_step_roc = 1;
	      continue;
            }

	    if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step == 0){ 
	      Next_step = 1;
	      Start_Event = i;
	      Do_Stack = 0;
	      continue;
            }
	    else if(((FIFO[i] >> 4) & 0xF) == 0x8 && Next_step != 0){
	      Incorrect_Header++;
	      Error_Count++;
	      continue;
	    }
						
	    if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step == 1){
	      Next_step = 2;
	      continue;
	    }
	    else if(((FIFO[i] >> 4) & 0xF) == 0x9 && Next_step != 1){
              Incorrect_Header++;
	      Error_Count++;
	      continue;
            }
						
	    if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step == 2){
              Next_step = 3;
	      continue;
	    }
	    else if(((FIFO[i] >> 4) & 0xF) == 0xA && Next_step != 2){
              Incorrect_Header++;
	      Error_Count++;
	      continue;
	    }
						
	    if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step == 3){
	      Next_step = 4;
	      continue;
	    }
	    else if(((FIFO[i] >> 4) & 0xF) == 0xB && Next_step != 3){
	      Incorrect_Header++;
	      Error_Count++;
	      continue;
            }
             
            if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step == 4){
	      Next_step = 5;
	      Num_ROC = 0;

	      if(FIFO[i] & 0x8){
	        std::cout << "There was a NTP" << std::endl;
	      }

	      if(FIFO[i] & 0x4){
                std::cout << "There was a TBM Reset" << std::endl;
              }

              if(FIFO[i] & 0x2){
                std::cout << "There was a ROC Reset" << std::endl;
              }

	      continue;
	    }
	    else if(((FIFO[i] >> 4) & 0xF) == 0xC && Next_step != 4){
	      Incorrect_Trailer++;
	      Error_Count++;
	      continue;
            }
						
	    if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step == 5){
	      Next_step = 6;
              if ((FIFO[i] & 0x1) == 0x1) {
                std::cout << "Stack is full!" << std::endl;
              }
	      continue;
	    }
	    else if(((FIFO[i] >> 4) & 0xF) == 0xD && Next_step != 5){
	      Incorrect_Trailer++;
	      Error_Count++;
	      continue;
	    }

	    if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step == 6){
	      Next_step = 7;
	      //std::cout << FIFO[i] << std::endl;
					
	      if( ((FIFO[i] & 0x4) >> 2) == 0x1 ){
	        std::cout << "There was a PKAM Reset at Event " << std::dec << Current_Event_Num << std::endl;
	      }
              num_ROC = -1;

	      Stack[0] = (FIFO[i] & 0x3);
	      continue;
            }
	    else if(((FIFO[i] >> 4) & 0xF) == 0xE && Next_step != 6){
	      Incorrect_Trailer++;
              Error_Count++;
              num_ROC = -1;
              continue;
	    }

            if(((FIFO[i] >> 4) & 0xF) == 0xF && Next_step == 7){
              Next_step = 0;
	      END_Event = i;
	      Error_Loop = 1;
              Do_Stack = 1;
	      Stack[1] = (Stack[0] << 4) | (FIFO[i] & 0xF);	
						
	      if(ROC_Value != 1){
	        Incorrect_ROC++;
	        Error_Count++;
	       }	
             }
             else if(((FIFO[i] >> 4) & 0xF) == 0xF && ((FIFO[i - 1] >> 4)) & 0xF != 0xF && Next_step != 7){
	       Incorrect_Trailer++;
	       Error_Count++;
	       END_Event = i;
	       Error_Loop = 1;
	     }
						
	     if(Stack[1] != 0x0 && Do_Stack == 1){
	       std::cout << "The Stack count is " << std::dec << Stack[1] << std::endl;
	     }

	     if(Error_Count != 0 && Error_Loop == 1 && choice_input == 0){
	       std::cout << "Event = " << std::dec << Current_Event_Num << " and Loop = " << std::dec << l << std::endl; 
	       std::cout << "Total Number of Errors = " <<std::dec << Error_Count << std::endl;
	       int Error = 0;

               //for(Error = Start_Event; Error <= END_Event; Error++){
	       //  std::cout << "FIFO[" << std::dec << Error << "] = " << std::hex << FIFO[Error] << std::endl;
	       //}

	       //for(Error = Start_Event; Error <= END_Event; Error++){
	       //  std::cout << "FIFO_CHB[" << std::dec << Error << "] = " << std::hex << FIFO_CHB[Error] << std::endl;
               //}
	       Error_Loop = 0;
	     }

	     if(((FIFO[i] >> 4) & 0xF) == 0x6){
	       Pixel++;
	     }
	     else if(((FIFO[i] >> 4) & 0xF) == 0x7){
	       Num_ROC++;
	       ROC_Value = 0;
	     }
					
	     if(Num_ROC == 8){
	       ROC_Value = 1;
	     }
             else{
               ROC_Value = 0;
	     }

  	  } // Ends error check loop
      			
      	  //} // Ends crate component loop
   
	  //Hit_infoA = Hit_infoA + 0x00000008;
          //Hit_infoB = Hit_infoB + 0x00000008;

          //adc_loops++;
             
        } // Ends adc loop

        Hit_infoA = Hit_infoA + 0x00000100 - (0x00000001 * adc_loops);
        Hit_infoB = Hit_infoB + 0x00000100 - (0x00000001 * adc_loops);

        col_loops++;
        adc_loops = 0;

      } // Ends column loop

      Hit_infoA = Hit_infoA + 0x00010000 - (0x00000100 * col_loops);
      Hit_infoB = Hit_infoB + 0x00010000 - (0x00000100 * col_loops);

      row_loops++;
      col_loops = 0;

    } // Ends row loop

    Hit_infoA = Hit_infoA - (0x00010000 * row_loops);
    Hit_infoB = Hit_infoB - (0x00010000 * row_loops);

    row_loops = 0;

  
  } // Ends Loops loop

  if (choice_input == 0) {
    std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_pix_info << " incorrect Number of pix hit info " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_adc << " incorrect Number of adc values " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_col << " incorrect Number of columns " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_row << " incorrect Number of rows " << std::endl;
  }
  else if (choice_input > 0) {
    std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Pixel << " incorrect Number of Pixels " << std::endl;
  }
  //exit(0);

}

int Test_Hits_Resets( int test_input, int Loops )
{

  using namespace std;

  const char* cHWFile;

  if ( test_input == 0) {
    //std::cout << "Testing normal event." << std::endl;
    std::string filename ("settings/HWDescription_PKAM.xml");
    cHWFile = filename.c_str();
  }
  else if ( test_input == 1) {
    //std::cout << "Testing TBM Reset." << std::endl;
    std::string filename ("settings/HWDescription_Resets.xml");
    cHWFile = filename.c_str();
  }
  else if ( test_input >= 3) {
    std::cout << "Not a valid option. Exiting program" << std::endl;
    return 0;
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

    // get the board info of all boards and start the acquistion
    for (auto& cFED : cSystemController.fPixFEDVector)
    {
	//auto& cFED = cSystemController.fPixFEDVector[0];
	cSystemController.fFEDInterface->getBoardInfo(cFED);
        cSystemController.fFEDInterface->findPhases(cFED);
	cSystemController.fFEDInterface->Start (cFED);
    }
	
    std::vector<unsigned int> FIFO;
    std::vector<unsigned int> FIFO_CHB;
    std::vector<std::vector<unsigned int>> Spy_Analyze(2);
    int Compare_Event_Spy = 0;
    int Total_Events = 0;
    int Incorrect_Header = 0;
    int Incorrect_Trailer = 0;
    int Incorrect_Event_Num = 0;
    int Incorrect_ROC = 0;
    int ROC_Value = 0;
   // int Loops = 1;
    uint32_t Current_Event_Num = 0;
    int Error_Count = 0;
    int Do_Error = 0;
    int Pixel = 0;
    int Pixel_Hit = 0;
    int Incorrect_Pixel = 0;
    int Compare_Event = 0;
    
    std::fill(FIFO.begin(), FIFO.end(), 0);
    std::fill(FIFO_CHB.begin(), FIFO_CHB.end(), 0);

    std::cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << std::endl;
    cAmc13Controller.fAmc13Interface->SendEC0();
    cAmc13Controller.fAmc13Interface->EnableBGO(0);
    sleep(0.001);
    cAmc13Controller.fAmc13Interface->DisableBGO(0);
	

    for (int l = 0; l < Loops; l++){
        for (auto& cFED : cSystemController.fPixFEDVector){
            if(l == 1){
                cAmc13Controller.fAmc13Interface->EnableBGO(1);
	    		sleep(0.001);
	    		cAmc13Controller.fAmc13Interface->DisableBGO(1);
            }
	
	        cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);
            sleep(4);
            cAmc13Controller.fAmc13Interface->BurstL1A();
            sleep(0.001);
	
	        FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
	        FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);
	        cSystemController.fFEDInterface->readFIFO1(cFED);
	
	      	Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
	
        }

        Compare_Event_Spy = Spy_Analyze[0][0];
        Total_Events += Spy_Analyze[0][1];
    	Incorrect_Header += Spy_Analyze[0][2];
    	Incorrect_Trailer += Spy_Analyze[0][3];
    	Incorrect_Event_Num += Spy_Analyze[0][4];
    	Incorrect_ROC += Spy_Analyze[0][5];

    }

    std::cout << "There have been " << std::dec << Total_Events << " Events" << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Event_Num << " Incorrect Event Numbers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Header << " Incorrect TBM Headers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_Trailer << " Incorrect TBM Trailers " << std::endl;
    std::cout << "There have been " << std::dec << Incorrect_ROC << " incorrect Number of ROCs " << std::endl;

    return 0;
}

void Test_Phases (int fiber_input, int time_input, int test_input)
{

  const char* cHWFile;
  std::string filename ("settings/HWDescription_Delay.xml");
  cHWFile = filename.c_str();

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
  int cChannelOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 3;

  cSetting = cSystemController.fSettingsMap.find("ROCOfInterest");
  int cROCOfInterest = (cSetting != std::end(cSystemController.fSettingsMap)) ? cSetting->second : 0;

  cAmc13Controller.ConfigureAmc13( std::cout );
  cSystemController.ConfigureHw(std::cout );

  //for (auto& cFED : cSystemController.fPixFEDVector){
  auto& cFED = cSystemController.fPixFEDVector[0];  
  if(fiber_input < 0){}
  else{
  	cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.fifo_config.channel_of_interest", fiber_input);	
  }
 
  cSystemController.fFEDInterface->getBoardInfo(cFED);
  cSystemController.fFEDInterface->getBoardInfo(cFED);
  cSystemController.fFEDInterface->findPhases2(cFED, cChannelOfInterest);

  std::cout << "Monitoring Phases for selected Channel of Interest for " << time_input << " seconds ... " << std::endl << std::endl;
  std::cout << BOLDGREEN << "FIBRE CTRL_RDY CNTVAL_Hi CNTVAL_Lo   pattern:                     S H1 L1 H0 L0   W R" << RESET << std::endl;
  if(test_input != 0){
  cChannelOfInterest = fiber_input; 
  for(int i = 0; i < time_input/3; i++)
  {
      cSystemController.fFEDInterface->monitorPhases(cFED, cChannelOfInterest);
  }
  }
  else{
    for(int j = 0; j < 24; j++){
      for(int i = 0; i < time_input/3; i++) cSystemController.fFEDInterface->monitorPhases(cFED, j);
    }
  }
}

void Stack_Test (int loops_input, int choice_input)
{

  using namespace std;
  using namespace uhal;

  const char* cHWFile;

  std::string filename ("settings/HWDescription_Stack.xml");
  cHWFile = filename.c_str();

  uhal::setLogLevelTo(uhal::Debug());

  SystemController cSystemController;
  Amc13Controller  cAmc13Controller;

  cSystemController.InitializeSettings(cHWFile, std::cout);

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

  auto& cFED = cSystemController.fPixFEDVector[0];
  cSystemController.fFEDInterface->getBoardInfo(cFED);
  cSystemController.fFEDInterface->findPhases(cFED);
  cSystemController.fFEDInterface->Start (cFED);
 
  std::vector<unsigned int> FIFO;
  std::vector<unsigned int> FIFO_CHB;
  std::vector<unsigned int> Event_Number;
  std::vector<std::vector<unsigned int>> Spy_Analyze(2);
  unsigned int Event_one = 0;
  unsigned int Event_two = 0;

  int Compare_Event_Spy = 0;
  int Total_Events = 0;
  int Incorrect_Header = 0;
  int Incorrect_Trailer = 0;
  int Incorrect_Event_Num = 0;
  int Incorrect_ROC = 0;
  int Incorrect_adc = 0;
  int Incorrect_col = 0;
  int Incorrect_row = 0;
  int Incorrect_pix_info = 0;
  int ROC_Value = 0;
  int Compare_Event = 0;
  int Loops = loops_input;
  uint32_t Current_Event_Num = 0;
  uint32_t Current_col_num = 0;
  uint32_t Current_row_num = 0;
  uint32_t Current_adc_num = 0;
  int Error_Count = 0;
  int Do_Error = 0;
  int Pixel = 0;
  int Pixel_Hit = 0;
  int Incorrect_Pixel = 0;

  int max_stack = 0;

  cout << "FED Configured, SLink Enabled, pressing Enter will send an EC0 & start periodic L1As" << endl;
  cAmc13Controller.fAmc13Interface->SendEC0();
  cAmc13Controller.fAmc13Interface->EnableBGO(0);
  sleep(0.001);
  cAmc13Controller.fAmc13Interface->DisableBGO(0);

  for (int l = 0; l < Loops; l++){
      if(l == 0){
        cSystemController.fFEDInterface->WriteBoardReg(cFED, "fe_ctrl_regs.decode_reg_reset", 1);   
        sleep(4);
        cAmc13Controller.fAmc13Interface->BurstL1A();
      }
      else{
        cAmc13Controller.fAmc13Interface->BurstL1A();
      }

      sleep(0.001);
      FIFO = cSystemController.fFEDInterface->readSpyFIFO(cFED);
      FIFO_CHB = cSystemController.fFEDInterface->readSpyFIFO_CHB(cFED);

      Spy_Analyze = Spy_FIFO_Event(FIFO, FIFO_CHB, Compare_Event_Spy);
      Compare_Event_Spy = Spy_Analyze[0][0];
      std::cout << "Max stack was " << std::dec << Spy_Analyze[0][7] << std::endl;
  }
}
