void GLIB_Test ()
{

  using namespace std;
  using namespace uhal;

  const char* cHWFile;

  std::string filename ("settings/HWDescription_Pixel_Alive.xml");
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

  for (auto& cGLIB : cSystemController.fPixGLIBVector)
  {
    cSystemController.fGLIBInterface->getBoardInfo(cGLIB);
  }
}

