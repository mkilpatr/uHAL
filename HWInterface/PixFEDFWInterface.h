/*!
        \file                PixFEDFWInterface.h
        \brief                           PixFEDFWInterface base class of all type of boards
        \author                          Lorenzo BIDEGAIN, Nicolas PIERRE
        \version             1.0
        \date                            28/07/14
        Support :                        mail to : lorenzo.bidegain@gmail.com, nico.pierre@icloud.com

 */

#ifndef _PixFEDFWInterface_h__
#define _PixFEDFWInterface_h__

#include <boost/thread.hpp>
#include <uhal/uhal.hpp>
#include "RegManager.h"
#include "../Utils/Utilities.h"
#include "../Utils/Exception.h"
#include "../Utils/ConsoleColor.h"
#include "../Utils/Utilities.h"
#include "../Utils/Timer.h"
#include "../HWDescription/PixFED.h"
#include "CtaFpgaConfig.h"

#include <iostream>
#include <fstream>

#define MAX_NB_LOOP 50

class CtaFpgaConfig;

/*!
 * \class PixFEDFWInterface
 * \brief Class separating board system FW interface from uHal wrapper
 */
class PixFEDFWInterface : public RegManager
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
    * \brief Constructor of the PixFEDFWInterface class
    * \param puHalConfigFileName : path of the uHal Config File
    * \param pFileHandler : pointer to file handler for saving Raw Data*/
    PixFEDFWInterface ( const char* puHalConfigFileName, uint32_t pBoardId );
    /*!
    * \brief Constructor of the PixFEDFWInterface class
    * \param pId : identifier string
    * \param pUri : URI string
    * \param pAddressTable: address table string*/
    PixFEDFWInterface ( const char* pId, const char* pUri, const char* pAddressTable );
    /*!
    * \brief Destructor of the PixFEDFWInterface class
    */
    ~PixFEDFWInterface()
    {
        //if ( fData ) delete fData;
    }
    /*!
     * \brief set the number of TBMs
     * \param pNTBM : number of TBMs connected
     */
    void setNTBM (uint32_t pNTBM)
    {
        fNTBM = pNTBM;
    };
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
    * \param pPixFED
    */
    bool ConfigureBoard ( const PixFED* pPixFED, bool pFakeData = false );
    /*!
     * \brief: Halt Board and put it back to safe state with internal Clock and golden Image FW
     */
    void HaltBoard();
    // Methods for management of FMCs
    /*!
     * \brief Disable FMC power
     */
    void disableFMCs();
    /*!
     * \brief enable FMC power
     */
    void enableFMCs();
    /*!
     * \brief: find correct phases for incoming data stream
     */
    void findPhases ();
    /*!
     * \brief: find correct phases for incoming data stream
     */
    void monitorPhases (uint32_t pScopeFIFOCh = 0);
    /*!
     * \brief: initialize Slink to send data
     */
    void getSFPStatus (uint8_t pFMCId);
    /*!
     * \brief: read the bistream FIFO of the PixFED
     */
    std::vector<uint32_t> readTransparentFIFO();
    /*!
     * \brief: read the contents of the SpyFifo for TBM cores A & B
     */
    std::vector<uint32_t> readSpyFIFO();
    std::vector<uint32_t> readSpyFIFO_CHB();
    /*!
     * \brief: read the contents of FIFO 1 for TBM cores A & B
     */
    std::string readFIFO1();
    /*
     * \brief: read the OSD readback word from ROCs
     * \param: ROC Number
     * \return: the joined 16bit OSD words for both TBM cores A is bits [0:15], B is bits [16:31]
     */
    uint32_t readOSDWord ( uint32_t pROCId, uint32_t pScopeFIFOCh );
    /*!
     * \brief: Read the TTS State
     */
    uint8_t readTTSState();
    /*!
     * \brief: read the Error FIFO
     */
    void readErrorFIFO (bool pForce);
    /*!
     * \brief Start a DAQ
     */
    void Start();
    /*!
     * \brief Stop a DAQ
     * \param pNthAcq : actual number of acquisitions
     */
    void Stop();
    /*!
     * \brief Pause a DAQ
     */
    void Pause();
    /*!
     * \brief Resume a DAQ
     */
    void Resume();
    /*!
     * \brief Read data from DAQ
     * \param pPixFED
     * \param pBreakTrigger : if true, enable the break trigger
     * \return vector<uint32_t> cData
     */
    std::vector<uint32_t> ReadData ( PixFED* pPixFED, uint32_t pBlockSize = 0 );
    /*!
     * \brief Read data from Slink FIFO
     * \param pPixFED
     * \return vector<uint32_t> cData
     */
    //std::vector<uint32_t> ReadNEvents( PixFED* pPixFED, uint32_t pNEvents = 1 );

    // EVENT HANDLING
    //const Event* GetNextEvent( const PixFED* pPixFED ) const;
    //const Event* GetEvent( const PixFED* pPixFED, int i ) const;
    //const std::vector<Event*>& GetEvents( const PixFED* pPixFED ) const;

    std::vector<uint32_t> ReadBlockRegValue ( const std::string& pRegNode, const uint32_t& pBlocksize );
    bool WriteBlockReg ( const std::string& pRegNode, const std::vector< uint32_t >& pValues );

    //FITEL METHODS

    //Encode/Decode FITEL FMC values
    /*!
    * \brief Encode a/several word(s) readable for a Fitel
    * \param pRegItem : RegItem containing infos (name, adress, value...) about the register to write
    * \param pFitelId : Id of the Fitel to work with
    * \param pVecReq : Vector to stack the encoded words
    */
    void EncodeReg ( const FitelRegItem& pRegItem, uint8_t pFMCId, uint8_t pFitelId, std::vector<uint32_t>& pVecReq );
    /*!
    * \brief Decode a word from a read of a register of the Fitel
    * \param pRegItem : RegItem containing infos (name, adress, value...) about the register to read
    * \param pFitelId : Id of the Fitel to work with
    * \param pWord : variable to put the decoded word
    */
    void DecodeReg ( FitelRegItem& pRegItem, uint8_t pFMCId, uint8_t pCFitelId, uint32_t pWord );

    //pure methods which are defined in the proper BoardFWInterface class
    //r/w the Fitel registers
    /*!
    * \brief Write register blocks of a Fitel
    * \param pVecReq : Block of words to write
    */
    bool WriteFitelBlockReg ( std::vector<uint32_t>& pVecReq );
    /*!
    * \brief Read register blocks of a Fitel
    * \param pVecReq : Vector to stack the read words
    */
    bool ReadFitelBlockReg ( std::vector<uint32_t>& pVecReq );

    /*!
     * \brief Read the ADC values for a given FITEL Receiver
     * \return Vector of 5 ADC values
     */
    std::vector<double> ReadADC ( const uint8_t pFMCId, const uint8_t pFitelId, bool pPrintAll);

    /*!
     * \brief Read and print the status of the SLINK
     *
     */
    void PrintSlinkStatus();

  private:
    void getFEDNetworkParameters();
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

    void prettyPrintTransparentFIFO (const std::vector<uint32_t>& pFifoVec, const std::vector<uint8_t>& p5bSymbol, const std::vector<uint8_t>& p5bNRZI, const std::vector<uint8_t>& p4bNRZI);
    void uglyprintSpyFIFO (const std::vector<uint32_t>& pVec);
    void prettyprintSpyFIFO (const std::vector<uint32_t>& pVec);
    void prettyprintFIFO1 ( const std::vector<uint32_t>& pFifoVec, const std::vector<uint32_t>& pMarkerVec, std::ostream& os = std::cout);
    void prettyprintTBMFIFO (const std::vector<uint32_t>& pData);
    void prettyprintSlink (const std::vector<uint64_t>& pData);
    void prettyprintPhase ( const std::vector<uint32_t>& pData, int pChannel );
    void decode_symbols (const std::vector<uint32_t>& pInData, std::vector<uint8_t>& p5bSymbol, std::vector<uint8_t>& p5bNRZI, std::vector<uint8_t>& p4bNRZI);
    void prettypPrintErrors (const uint32_t& cWord);

    // FPGA CONFIG METHODS
  public:
    void checkIfUploading();
    /*! \brief Upload a firmware (FPGA configuration) from a file in MCS format into a given configuration
     * \param strConfig FPGA configuration name
     * \param pstrFile path to MCS file
     */
    void FlashProm ( const std::string& strConfig, const char* pstrFile );
    /*! \brief Jump to an FPGA configuration */
    void JumpToFpgaConfig ( const std::string& strConfig );
    /*! \brief: download FPGA config file from CTA SD card */
    void DownloadFpgaConfig ( const std::string& strConfig, const std::string& strDest );
    /*! \brief Is the FPGA being configured ?
     * \return FPGA configuring process or NULL if configuration occurs */
    const FpgaConfig* getConfiguringFpga()
    {
        return ( const FpgaConfig* ) fpgaConfig;
    }
    /*! \brief Get the list of available FPGA configuration (or firmware images)*/
    std::vector<std::string> getFpgaConfigList( );
    /*! \brief Delete one Fpga configuration (or firmware image)*/
    void DeleteFpgaConfig ( const std::string& strId );
};

#endif
