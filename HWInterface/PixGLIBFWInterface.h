/*!
        \file                PixGLIBFWInterface.h
        \brief                           PixGLIBFWInterface base class of all type of boards
        \author                          Lorenzo BIDEGAIN, Nicolas PIERRE
        \version             1.0
        \date                            28/07/14
        Support :                        mail to : lorenzo.bidegain@gmail.com, nico.pierre@icloud.com

 */

#ifndef _PixGLIBFWInterface_h__
#define _PixGLIBFWInterface_h__

#include <boost/thread.hpp>
#include <uhal/uhal.hpp>
#include "RegManager.h"
#include "../Utils/Utilities.h"
#include "../Utils/Exception.h"
#include "../Utils/ConsoleColor.h"
#include "../Utils/Utilities.h"
#include "../Utils/Timer.h"
#include "../HWDescription/PixGLIB.h"
#include "CtaFpgaConfig.h"

#include <iostream>
#include <fstream>

#define MAX_NB_LOOP 50

class CtaFpgaConfig;

/*!
 * \class PixGLIBFWInterface
 * \brief Class separating board system FW interface from uHal wrapper
 */
class PixGLIBFWInterface : public RegManager
{
    //MEMBER VARIABLES
  private:
    //Data* fData; [>!< Data read storage<]

    // Size Variables for Data to read
    uint32_t fNumAcq;       // the number of acquisitions
    uint32_t fNTBM;         // the number of TBMs
    uint32_t fNCh = 2;          // the number of Channels per TBM = 2
    uint32_t fNPattern = 8;     // the number of Patterns to send per channel = 8 (8 ROCs per TBM channel)
    uint32_t fPacketSize = 2;   // the size of a packet for each acquisition = 2;

    // to be computed internally
    uint32_t fBlockSize;    // the number of bits to read from the DDR memory
    uint32_t fBlockSize32; // the number of 32-bit words to read from DDR memory
    uint32_t fNthAcq;       // the index of the current Acquistion - to be used to select the correct DDR bank
    uint32_t fAcq_mode;
    uint32_t fNEvents_calmode;
    uint32_t fCalibMode;
    //struct timeval fStartVeto;
    CtaFpgaConfig* fpgaConfig;
    // strings fro DDR control
    std::string fStrDDR, fStrDDRControl, fStrFull, fStrReadout;

  public:
    /*!
    * \brief Constructor of the PixGLIBFWInterface class
    * \param puHalConfigFileName : path of the uHal Config File
    * \param pFileHandler : pointer to file handler for saving Raw Data*/
    PixGLIBFWInterface ( const char* puHalConfigFileName, uint32_t pBoardId );
    /*!
    * \brief Constructor of the PixGLIBFWInterface class
    * \param pId : identifier string
    * \param pUri : URI string
    * \param pAddressTable: address table string*/
    PixGLIBFWInterface ( const char* pId, const char* pUri, const char* pAddressTable );
    /*!
    * \brief Destructor of the PixGLIBFWInterface class
    */
    ~PixGLIBFWInterface()
    {
        //if ( fData ) delete fData;
    }
    /*!
     * \brief set the blocksize for the FIFO (incl. TBM FIFO reads)
     * \param pBlockSize : number of 32 bit words to read from global TBM FIFO, all other FIFO depths are adapted correspondingly
     */
    void setBlockSize (uint32_t pBlockSize)
    {
        fBlockSize = pBlockSize;
        //fBlockSize32 = ceil(pBlockSize / double(8) ) - 1;
    };

    //DAQ METHODS
    /*!
    * \brief Get the board type
    */
    std::string getBoardType();
    /*!
    * \brief Get the board infos
    */
    void getBoardInfo();
    /*!
    * \brief Configure the board with its Config File
    * \param pPixGLIB
    */
    void PKAM_Length( int PKAM_init, int PKAM_Constant, int Which_Chan );
    void PKAM_Enable( uint32_t PKAM_Enable );
    void Delay_Time( int Delay_init );
    void Marker_Error( int Which_Chan, int Marker_Type, int Marker_Value, int Marker_Rate);
    void writeTTSState( int State );
    void Set_ROCS(int roc_input);
    void Set_Num_Hits(int hit_input);
    void Write_Data_Header( int Header_Value );
    void ADCAsEvn( int Enable );
    void SEU(int enable, int type, int mode, int Which_Chan);
    void SRAM_Disable();
    void SRAM_Mode();
    void Clear_SRAM_FIFO();
    bool ConfigureBoard ( const PixGLIB* pPixGLIB, bool pFakeData = false );
    std::vector<std::pair<std::string, uint32_t> > GLIBHeaderCountDump( PixGLIB* pPixGLIB, uint32_t pBlockSize = 0);
    std::vector<std::pair<std::string, uint32_t> > GLIBTrailerCountDump( PixGLIB* pPixGLIB, uint32_t pBlockSize = 0);

    std::vector<uint32_t> ReadBlockRegValue ( const std::string& pRegNode, const uint32_t& pBlocksize );
    bool WriteBlockReg ( const std::string& pRegNode, const std::vector< uint32_t >& pValues );


  private:

    void getGLIBNetworkParameters();
    /*!
     * \brief DDR selection for DAQ
     * \param pNthAcq : actual number of acquisitions
     */
    void SelectDaqDDR ( uint32_t pNthAcq );

    //I2C Methods
    void i2cRelease (uint32_t pTries);
    bool polli2cAcknowledge (uint32_t pTries);

    /*! Compute the size of an acquisition data block
     * \param pFakeData: boolean flag to determine whether deterministic fake data or not
     * \return Number of 32-bit words to be read at each iteration */
    uint32_t computeBlockSize (bool pFakeData = false);

    // FPGA CONFIG METHODS
  public:
};

#endif
