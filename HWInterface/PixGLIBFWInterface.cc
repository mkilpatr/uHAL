/*

        FileName :                    PixGLIBFWInterface.cc
        Content :                     PixGLIBFWInterface base class
        Programmer :                  G.Auzinger
        Version :                     1.0
        Date of creation :            30/11/15
        Support :                     mail to : georg.auzinger@SPAMNOTcern.ch

 */



#include "PixGLIBFWInterface.h"
#include "FpgaConfig.h"
#include <iomanip>

#define DEV_FLAG         0

//Constructor, makes the board map
PixGLIBFWInterface::PixGLIBFWInterface ( const char* puHalConfigFileName, uint32_t pBoardId ) :
    RegManager ( puHalConfigFileName, pBoardId ),
    fNumAcq ( 0 )
    //fData( nullptr )
{
}
//Constructor, makes the board map
PixGLIBFWInterface::PixGLIBFWInterface ( const char* pId, const char* pUri, const char* pAddressTable ) :
    RegManager ( pId, pUri, pAddressTable ),
    fNumAcq ( 0 )
{
}
/////////////////////////////////////////////
// CONFIG / START / STOP METHODS
////////////////////////////////////////////

std::string PixGLIBFWInterface::getBoardType()
{
    // adapt me!
    std::string cBoardTypeString;

    uhal::ValWord<uint32_t> cBoardType = ReadReg ( "user_iphc_ascii_01" );

    char cChar = ( ( cBoardType & 0xFF000000 ) >> 24 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x00FF0000 ) >> 16 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x0000FF00 ) >> 8 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( cBoardType & 0x000000FF );
    cBoardTypeString.push_back ( cChar );

    cBoardType = ReadReg ( "user_iphc_ascii_02" );

    cChar = ( ( cBoardType & 0xFF000000 ) >> 24 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x00FF0000 ) >> 16 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x0000FF00 ) >> 8 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( cBoardType & 0x000000FF );
    cBoardTypeString.push_back ( cChar );

    return cBoardTypeString;

}

void PixGLIBFWInterface::getBoardInfo()
{
    std::cout << std::endl << "Board Type: " << getBoardType() << std::endl;
    std::string cBoardTypeString;

    uhal::ValWord<uint32_t> cBoardType = ReadReg ( "user_rice_ascii_01" );

    char cChar = ( ( cBoardType & 0xFF000000 ) >> 24 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x00FF0000 ) >> 16 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x0000FF00 ) >> 8 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( cBoardType & 0x000000FF );
    cBoardTypeString.push_back ( cChar );

    cBoardType = ReadReg ( "user_rice_ascii_02" );

    cChar = ( ( cBoardType & 0xFF000000 ) >> 24 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x00FF0000 ) >> 16 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x0000FF00 ) >> 8 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( cBoardType & 0x000000FF );
    cBoardTypeString.push_back ( cChar );

    cBoardType = ReadReg ( "user_rice_ascii_03" );

    cChar = ( ( cBoardType & 0xFF000000 ) >> 24 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x00FF0000 ) >> 16 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x0000FF00 ) >> 8 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( cBoardType & 0x000000FF );
    cBoardTypeString.push_back ( cChar );

    cBoardType = ReadReg ( "user_rice_ascii_04" );

    cChar = ( ( cBoardType & 0xFF000000 ) >> 24 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x00FF0000 ) >> 16 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( ( cBoardType & 0x0000FF00 ) >> 8 );
    cBoardTypeString.push_back ( cChar );

    cChar = ( cBoardType & 0x000000FF );
    cBoardTypeString.push_back ( cChar );

    std::cout << "Board Use: " << cBoardTypeString << std::endl;

    std::cout << "FW version RICE : " << ReadReg ( "user_rice_fw_ver_year.ver_major" ) << "." << ReadReg ( "user_rice_fw_ver_year.ver_minor" ) << "." << ReadReg ( "user_rice_fw_ver_year.ver_build" ) << "; Date: " << ReadReg ( "user_rice_fw_ver_year.firmware_dd" ) << "." << ReadReg ( "user_rice_fw_ver_year.firmware_mm" ) << "." << ReadReg ( "user_rice_fw_ver_year.firmware_yy" ) <<  std::endl;

}


void PixGLIBFWInterface::PKAM_Length( int PKAM_init, int PKAM_Constant, int Which_Chan ){

  std::string index[8] = {"0","1","2","3","4","5","6","7"};
  std::string fiber = "Fiber_";
  std::string Value_chA = "PKAM_Reset.chA";
  std::string Value_chB = "PKAM_Reset.chB";
  std::string constant_chA = "PKAM_Constant.chA";
  std::string constant_chB = "PKAM_Constant.chB";
  std::string PKAM_Value_chA;
  std::string PKAM_Value_chB;
  std::string PKAM_Constant_chA;
  std::string PKAM_Constant_chB;
  int Number_Channels = 0;

  if(Which_Chan == 0) Number_Channels = 0x55555555;
  else Number_Channels = Which_Chan;

  for (int i = 0; i < 8; i++) {

    PKAM_Value_chA = fiber + index[i] + "." + Value_chA;
    PKAM_Value_chB = fiber + index[i] + "." + Value_chB;
    PKAM_Constant_chA = fiber + index[i] + "." + constant_chA;
    PKAM_Constant_chB = fiber + index[i] + "." + constant_chB;

    if((Number_Channels & 0x1) == 1){
        WriteReg(PKAM_Value_chA, PKAM_init );
        WriteReg(PKAM_Constant_chA, PKAM_Constant);
    }
    Number_Channels = Number_Channels >> 2;
    if((Number_Channels & 0x1) == 1){
        WriteReg(PKAM_Value_chB, PKAM_init );
        WriteReg(PKAM_Constant_chB, PKAM_Constant);
    }
    Number_Channels = Number_Channels >> 2;
  }
}

void PixGLIBFWInterface::PKAM_Enable( uint32_t PKAM_Enable){
    
    std::string index[8] = {"0","1","2","3","4","5","6","7"};
    std::string fiber = "Fiber_";
    std::string Enable = "PKAM_Enable";
    std::string Enable_string;

    for (int i = 0; i < 8; i++) {
        Enable_string = fiber + index[i] + "." + Enable;
        WriteReg(Enable_string, PKAM_Enable );
    }
}

void PixGLIBFWInterface::Delay_Time( int Delay_init ){

  WriteReg("Delay_Reset", 1 );
  usleep(10);
  WriteReg("Delay_Signal", Delay_init );
  usleep(10);
  WriteReg("Delay_Reset", 0 );
}

void PixGLIBFWInterface::Marker_Error( int Which_Chan, int Marker_Type, int Marker_Value, int Marker_Rate){
    int Number_Channels = 0;
    std::string Chan[ 8 ] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    std::string fiber ("Fiber_");
    std::string Marker_Value_chA     ("Marker_Error.chA_Value"    );
    std::string Marker_Value_chB     ("Marker_Error.chB_Value"    );
    std::string Marker_Type_chA      ("Marker_Error.chA_Type"     );
    std::string Marker_Type_chB      ("Marker_Error.chB_Type"     );
    std::string Marker_Clock_chA     ("Marker_Error_chA_Clock"    );
    std::string Marker_Clock_chB     ("Marker_Error_chB_Clock"    );
    std::string Marker_ROC_Value_chA ("Marker_Error_ROC_Count_chA");
    std::string Marker_ROC_Value_chB ("Marker_Error_ROC_Count_chB");
    std::string MarkerErrorValue;
    std::string MarkerErrorType;
    std::string MarkerErrorClock;
    std::string MarkerErrorRoc;
    
    if(Which_Chan == 0)
            Number_Channels = 0x55555555;
    else
            Number_Channels = Which_Chan;
    for (int i = 0; i < 8; i++){
        int Which_ROC = rand() % 8;
        if((Number_Channels & 0x1) == 1){
            MarkerErrorValue = fiber + Chan[i] + "." + Marker_Value_chA;
            MarkerErrorType  = fiber + Chan[i] + "." + Marker_Type_chA;
            MarkerErrorClock = fiber + Chan[i] + "." + Marker_Clock_chA;
            MarkerErrorRoc   = fiber + Chan[i] + "." + Marker_ROC_Value_chA;
    
            WriteReg(MarkerErrorClock, Marker_Rate  );
            WriteReg(MarkerErrorValue, Marker_Value );
            WriteReg(MarkerErrorType , Marker_Type  );
            WriteReg(MarkerErrorRoc  , Which_ROC    );
        }
        Number_Channels = Number_Channels >> 2;
        if((Number_Channels & 0x1) == 1){
            MarkerErrorValue = fiber + Chan[i] + "." + Marker_Value_chB;
            MarkerErrorType  = fiber + Chan[i] + "." + Marker_Type_chB;
            MarkerErrorClock = fiber + Chan[i] + "." + Marker_Clock_chB;
            MarkerErrorRoc   = fiber + Chan[i] + "." + Marker_ROC_Value_chB;
    
            WriteReg(MarkerErrorClock, Marker_Rate  );
            WriteReg(MarkerErrorValue, Marker_Value );
            WriteReg(MarkerErrorType , Marker_Type  );
            WriteReg(MarkerErrorRoc  , Which_ROC    );
         }
         Number_Channels = Number_Channels >> 2;
     }
}

void PixGLIBFWInterface::writeTTSState( int State ){
    WriteReg("TTSState", State);
}

void PixGLIBFWInterface::Set_ROCS(int roc_input)
{
  uint32_t Number_ROCS = 0x88;
  uint32_t inter_num;
  uint32_t ROC_Clk = 0x9898;  //Puts a value of 160 clk cycles in channel A & B

  if (roc_input == 0) { }
  else if (roc_input <= 8) {
    inter_num = (roc_input << 4);
    Number_ROCS = inter_num | roc_input;
  }

  std::string index[8] = {"0","1","2","3","4","5","6","7"};
  std::string fiber ("Fiber_");

  for (int i = 0; i<8; i++) {
    std::string roc_num = "ROC_Number";
    roc_num = fiber + index[i] + "." + roc_num;

    std::string roc_clock = "ROC_CLK";
    roc_clock = fiber + index[i] + "." + roc_clock;

    WriteReg(roc_num  , Number_ROCS );
    WriteReg(roc_clock, ROC_Clk );
  }

  writeTTSState(8);
}

void PixGLIBFWInterface::Set_Num_Hits(int hit_input)
{
  uint32_t Hits1 = 0x00000000;
  uint32_t Hits2 = 0x00000000;
  uint32_t inter_hits[4];
  if (hit_input == 0) { }
  else {
    for (int i = 0; i < 4; i++) {
      inter_hits[i] = hit_input << (8*(3-i));
    }
    Hits1 = inter_hits[0] | inter_hits[1] | inter_hits[2] | inter_hits[3];
    Hits2 = Hits1;
  }
  std::string index[8] = {"0","1","2","3","4","5","6","7"};
  std::string fiber ("Fiber_");
  for (int i = 0; i < 8; i++) {
    std::string cha_hits1 = "CHA_Hits_1";
    std::string cha_hits2 = "CHA_Hits_2";
    std::string chb_hits1 = "CHB_Hits_1";
    std::string chb_hits2 = "CHB_Hits_2";
    cha_hits1 = fiber + index[i] + "." + cha_hits1;
    cha_hits2 = fiber + index[i] + "." + cha_hits2;
    chb_hits1 = fiber + index[i] + "." + chb_hits1;
    chb_hits2 = fiber + index[i] + "." + chb_hits2;
    
    WriteReg(cha_hits1, Hits1 );
    WriteReg(chb_hits1, Hits1 );
    WriteReg(cha_hits2, Hits2 );
    WriteReg(chb_hits2, Hits2 );
  }
}

void PixGLIBFWInterface::Write_Data_Header( int Header_Value ){
    std::string Chan[8] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    std::string fiber ("Fiber_");
    
    for (int i = 0; i < 8; i++){
        std::string Header_A ("Header_Flag_A");
        std::string Header_B ("Header_Flag_B");
        Header_A = fiber + Chan[i] + "." + Header_A;
        Header_B = fiber + Chan[i] + "." + Header_B;
        
        WriteReg(Header_A, Header_Value );
        WriteReg(Header_B, Header_Value );
    }
}

void PixGLIBFWInterface::ADCAsEvn( int Enable ){
    WriteReg("ADCAsEvn", Enable);
}

void PixGLIBFWInterface::SEU(int enable, int type, int mode, int Which_Chan){
    std::string Chan[ 16 ] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
    int Number_Channels = 0;

    if(Which_Chan == 0) Number_Channels = 0x55555555;
    else Number_Channels = Which_Chan;

    for(int chan = 0; chan < 16; chan++){
          std::string Enable("SEU.Enable_"), Type("SEU.Type_"), Mode("SEU.Mode_");
          if(Number_Channels & 0x1 == 1){
            Enable += Chan[chan];
            Type += Chan[chan];
            Mode += Chan[chan];
            WriteReg(Enable, enable);
            WriteReg(Type  , type);
            WriteReg(Mode  , mode);
          }
          Number_Channels = Number_Channels >> 2;
    }
}

void PixGLIBFWInterface::SRAM_Disable(){
    WriteReg("Wea", 0);
}

void PixGLIBFWInterface::SRAM_Mode(){
        WriteReg("Wea"        , 1);
        WriteReg("sys_regs"   , 0x10001);
        WriteReg("SRAM2_WData", 0x0);
        WriteReg("Wea2_cs"    , 1);
        WriteReg("Wea2_en"    , 0);
        usleep(10);
}

void PixGLIBFWInterface::Clear_SRAM_FIFO(){
    WriteReg("SRAM_FIFO_Clear", 1);
    usleep(10);
    WriteReg("SRAM_FIFO_Clear", 0);
    usleep(10);
}

bool PixGLIBFWInterface::ConfigureBoard ( const PixGLIB* pPixGLIB, bool pFakeData )
{
    std::vector< std::pair<std::string, uint32_t> > cVecReg;
    std::vector< uint32_t > cVecWrite;
    PixGLIBRegMap cPixGLIBRegMap = pPixGLIB->getPixGLIBRegMap();

    for ( auto const& it : cPixGLIBRegMap )
    {
        cVecWrite.push_back ( it.second );
        cVecReg.push_back ( {it.first, it.second} );
    }

    PKAM_Length( cVecWrite[0], cVecWrite[1], cVecWrite[2] );
    PKAM_Enable(cVecWrite[3] );
    Delay_Time( cVecWrite[4] );
    Marker_Error( cVecWrite[5], cVecWrite[6], cVecWrite[7], cVecWrite[8] );
    Set_ROCS( cVecWrite[9] );
    Set_Num_Hits( cVecWrite[10] );
    Write_Data_Header( cVecWrite[11] );
    ADCAsEvn( cVecWrite[12] );
    SEU( cVecWrite[13], cVecWrite[14], cVecWrite[15], cVecWrite[16] );
    if(cVecWrite[17] == 1) SRAM_Mode();
    else SRAM_Disable();
    Clear_SRAM_FIFO();

    //    WriteStackReg ( cVecReg );

    cVecReg.clear();

    return true;

}

std::vector<std::pair<std::string, uint32_t> > PixGLIBFWInterface::GLIBHeaderCountDump( PixGLIB* pPixGLIB, uint32_t pBlockSize ){
	std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	Counter_reg.push_back({"HeaderID_tbm_ch_1" , ReadReg("HeaderID_tbm_ch_1" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_2" , ReadReg("HeaderID_tbm_ch_2" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_3" , ReadReg("HeaderID_tbm_ch_3" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_4" , ReadReg("HeaderID_tbm_ch_4" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_5" , ReadReg("HeaderID_tbm_ch_5" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_6" , ReadReg("HeaderID_tbm_ch_6" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_7" , ReadReg("HeaderID_tbm_ch_7" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_8" , ReadReg("HeaderID_tbm_ch_8" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_9" , ReadReg("HeaderID_tbm_ch_9" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_10" , ReadReg("HeaderID_tbm_ch_10" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_11" , ReadReg("HeaderID_tbm_ch_11" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_12" , ReadReg("HeaderID_tbm_ch_12" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_13" , ReadReg("HeaderID_tbm_ch_13" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_14" , ReadReg("HeaderID_tbm_ch_14" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_15" , ReadReg("HeaderID_tbm_ch_15" ) });
	Counter_reg.push_back({"HeaderID_tbm_ch_16" , ReadReg("HeaderID_tbm_ch_16" ) });
	return Counter_reg;
}

std::vector<std::pair<std::string, uint32_t> > PixGLIBFWInterface::GLIBTrailerCountDump( PixGLIB* pPixGLIB, uint32_t pBlockSize ){
	std::vector<std::pair<std::string, uint32_t> > Counter_reg;
	Counter_reg.push_back({"TrailerID_tbm_ch_1" , ReadReg("TrailerID_tbm_ch_1" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_2" , ReadReg("TrailerID_tbm_ch_2" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_3" , ReadReg("TrailerID_tbm_ch_3" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_4" , ReadReg("TrailerID_tbm_ch_4" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_5" , ReadReg("TrailerID_tbm_ch_5" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_6" , ReadReg("TrailerID_tbm_ch_6" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_7" , ReadReg("TrailerID_tbm_ch_7" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_8" , ReadReg("TrailerID_tbm_ch_8" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_9" , ReadReg("TrailerID_tbm_ch_9" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_10" , ReadReg("TrailerID_tbm_ch_10" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_11" , ReadReg("TrailerID_tbm_ch_11" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_12" , ReadReg("TrailerID_tbm_ch_12" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_13" , ReadReg("TrailerID_tbm_ch_13" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_14" , ReadReg("TrailerID_tbm_ch_14" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_15" , ReadReg("TrailerID_tbm_ch_15" ) });
	Counter_reg.push_back({"TrailerID_tbm_ch_16" , ReadReg("TrailerID_tbm_ch_16" ) });
	return Counter_reg;
}



uint32_t PixGLIBFWInterface::computeBlockSize ( bool pFakeData )
{
    if (pFakeData)
    {
        // this is the number of bits to read from DDR
        fBlockSize = fNTBM * fNCh * fNPattern * fPacketSize;
    }

    // since the DDR data widt is 256 this is the number of 32 bit words I have to read
    fBlockSize32 = static_cast<uint32_t> ( ceil (fBlockSize / double (8 ) ) - 1);
    return fBlockSize;
}

std::vector<uint32_t> PixGLIBFWInterface::ReadBlockRegValue ( const std::string& pRegNode, const uint32_t& pBlocksize )
{
    uhal::ValVector<uint32_t> valBlock = ReadBlockReg ( pRegNode, pBlocksize );
    std::vector<uint32_t> vBlock = valBlock.value();
    return vBlock;
}

bool PixGLIBFWInterface::WriteBlockReg ( const std::string& pRegNode, const std::vector< uint32_t >& pValues )
{
    bool cWriteCorr = RegManager::WriteBlockReg ( pRegNode, pValues );
    return cWriteCorr;
}

