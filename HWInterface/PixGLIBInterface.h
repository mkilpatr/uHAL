/*!

        \file                                            PixGLIBInterface.h
        \brief                                           User Interface to the Boards
        \author                                          G. Auzinger
        \version                                         1.0
        \date                        01/12/15
        Support :                    mail to :georg.auzinger@SPAMNOTcern.ch
*/
#ifndef PixGLIBInterface_h_
#define PixGLIBInterface_h_

#include "PixGLIBFWInterface.h"
#include "../Utils/ConsoleColor.h"
#include "../Utils/Utilities.h"
#include "../HWDescription/PixGLIB.h"

/*!
 * \namespace Ph2_HwInterface
 * \brief Namespace regrouping all the interfaces to the hardware
 */

typedef std::map<uint16_t, PixGLIBFWInterface*> PixGLIBFWMap;    /*!< Map of Board connected */

/*!
 * \class BeBoardInterface
 * \brief Class representing the User Interface to the different boards
 */
class PixGLIBInterface
{

  private:
    PixGLIBFWMap fFWMap;                     /*!< Map of Board connected */
    PixGLIBFWInterface* fGLIBFW;                     /*!< Board loaded */
    uint16_t prevBoardIdentifier;                     /*!< Id of the previous board */

  private:
    /*!
     * \brief Set the board to talk with
     * \param pBoardId
     */
    void setBoard ( uint16_t pBoardIdentifier );

  public:
    /*!
     * \brief Constructor of the BeBoardInterface class
     * \param pBoardMap Reference to the BoardFWInterface
     */
    PixGLIBInterface ( const PixGLIBFWMap& pFWMap );
    /*!
     * \brief Destructor of the BeBoardInterface class
     */
    ~PixGLIBInterface();

    //////////////////////////////////////
    // PixGLIB Register Methods
    /////////////////////////////////////

    /*!
     * \brief Update both Board register and Config File
     * \param pGLIB
     * \param pRegNode : Node of the register to update
     * \param pVal : Value to write
     */
    void WriteBoardReg ( PixGLIB* pGLIB, const std::string& pRegNode, const uint32_t& pVal );
    /*!
         * \brief Write a block of a given size into the board
        * \param pGLIB
        * \param pRegNode : Node of the register to write
        * pValVec Vector of values to write
         */
    void WriteBlockBoardReg ( PixGLIB* pGLIB, const std::string& pRegNode, const std::vector<uint32_t>& pValVec );
    /*!
     * \brief Write: Update both Board register and Config File
     * \param pGLIB
     * \param pRegVec : Vector of Register/Value pairs
     */
    void WriteBoardMultReg ( PixGLIB* pGLIB, const std::vector < std::pair< std::string , uint32_t > >& pRegVec );
    /*!
    * \brief Update Config File with the value in the Board register
    * \param pGLIB
    * \param pRegNode : Node of the register to update
    */
    uint32_t ReadBoardReg ( PixGLIB* pGLIB, const std::string& pRegNode );
    /*!
         * \brief Read a block of a given size from the board
        * \param pGLIB
        * \param pRegNode : Node of the register to read
        * \param pSize Number of 32-bit words in the block
         */
    std::vector<uint32_t> ReadBlockBoardReg ( PixGLIB* pGLIB, const std::string& pRegNode, uint32_t pSize );
    /*!
     * \brief Read a vector of Registers
     * \param pGLIB
     * \param pRegVec : Vector of Register/Value pairs
     */
    void ReadBoardMultReg ( PixGLIB* pGLIB, std::vector < std::pair< std::string , uint32_t > >& pRegVec );

    /////////////////////////////////////
    // PixGLIB System Methods
    /////////////////////////////////////

    /*!
     * \brief Get the board infos
     * \param pGLIB
     */
    void getBoardInfo ( const PixGLIB* pGLIB );
    /*!
     * \brief Configure the board with its Config File
     * \param pGLIB
     */
    void ConfigureGLIB ( const PixGLIB* pGLIB );
    /*
     * \brief: read the Error FIFO
     */
    std::vector<std::pair<std::string, uint32_t> > GLIBHeaderCountDump ( PixGLIB* pGLIB, uint32_t pBlockSize = 0 );
    std::vector<std::pair<std::string, uint32_t> > GLIBTrailerCountDump ( PixGLIB* pGLIB, uint32_t pBlockSize = 0 );

    /*!
     * \brief Read data from DAQ
     * \param pGLIB
     * \param pBreakTrigger : if true, enable the break trigger
     * \return cData: the data in 32 bit words
     */
    //std::vector<uint32_t> ReadNEvents( PixGLIB* pGLIB, uint32_t pNEvents = 1 );
    /*!
    * \brief: Initialize the Slink
    * \param: pointer to PixGLIB
    */
    /*! \brief Get a uHAL node object from its path in the uHAL XML address file
     * \param pGLIB pointer to a board description
     * \return Reference to the uhal::node object
     */
    const uhal::Node& getUhalNode ( const PixGLIB* pGLIB, const std::string& pStrPath );
    /*! \brief Access to the uHAL main interface for a given board
     * \param pGLIB pointer to a board description
     * \return pointer to the uhal::HwInterface object
     */
    uhal::HwInterface* getHardwareInterface ( const PixGLIB* pGLIB );
    /*! \brief Upload a configuration in a board FPGA
     * \param pGLIB pointer to a board description
     * \param numConfig FPGA configuration number to be uploaded
     * \param pstrFile path to MCS file containing the FPGA configuration
     */
  private:
};

#endif
