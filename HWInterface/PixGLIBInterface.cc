/*

        FileName :                    PixGLIBInterface.cc
        Content :                     User Interface to the Boards
        Programmer :                  Lorenzo BIDEGAIN, Nicolas PIERRE
        Version :                     1.0
        Date of creation :            31/07/14
        Support :                     mail to : lorenzo.bidegain@gmail.com nico.pierre@icloud.com

 */

#include "PixGLIBInterface.h"


PixGLIBInterface::PixGLIBInterface ( const PixGLIBFWMap& pFWMap ) :
    fFWMap ( pFWMap ),
    fGLIBFW ( nullptr ),
    prevBoardIdentifier ( 65535 )

{
}



PixGLIBInterface::~PixGLIBInterface()
{
    //std::string cImageName = "GoldenImage.bin";
    //std::vector<std::string> cImageList = fGLIBFW->getFpgaConfigList();
    //verifyImageName(cImageName, cImageList);
    //fGLIBFW->JumpToFpgaConfig(cImageName);
    //std::cout << "Successfully loaded golden FW on GLIB " << +pGLIB->getBeId() << std::endl;
}

void PixGLIBInterface::setBoard ( uint16_t pBoardIdentifier )
{
    if ( prevBoardIdentifier != pBoardIdentifier )
    {
        PixGLIBFWMap::iterator i = fFWMap.find ( pBoardIdentifier );

        if ( i == fFWMap.end() )
            std::cout << "The GLIB: " << +pBoardIdentifier  <<  "  doesn't exist" << std::endl;
        else
        {
            fGLIBFW = i->second;
            prevBoardIdentifier = pBoardIdentifier;
        }
    }
}

void PixGLIBInterface::WriteBoardReg ( PixGLIB* pGLIB, const std::string& pRegNode, const uint32_t& pVal )
{
    setBoard ( pGLIB->getBeId() );

    fGLIBFW->WriteReg ( pRegNode, pVal );
    pGLIB->setReg ( pRegNode, pVal );
}

void PixGLIBInterface::WriteBlockBoardReg ( PixGLIB* pGLIB, const std::string& pRegNode, const std::vector<uint32_t>& pValVec )
{
    setBoard ( pGLIB->getBeId() );
    fGLIBFW->WriteBlockReg ( pRegNode, pValVec );
}


void PixGLIBInterface::WriteBoardMultReg ( PixGLIB* pGLIB, const std::vector < std::pair< std::string , uint32_t > >& pRegVec )
{
    setBoard ( pGLIB->getBeId() );

    fGLIBFW->WriteStackReg ( pRegVec );

    for ( const auto& cReg : pRegVec )
    {
        // fGLIBFW->WriteReg( cReg.first, cReg.second );
        pGLIB->setReg ( cReg.first, cReg.second );
    }
}


uint32_t PixGLIBInterface::ReadBoardReg ( PixGLIB* pGLIB, const std::string& pRegNode )
{
    setBoard ( pGLIB->getBeId() );
    uint32_t cRegValue = static_cast<uint32_t> ( fGLIBFW->ReadReg ( pRegNode ) );
    pGLIB->setReg ( pRegNode,  cRegValue );
    return cRegValue;
}

void PixGLIBInterface::ReadBoardMultReg ( PixGLIB* pGLIB, std::vector < std::pair< std::string , uint32_t > >& pRegVec )
{
    setBoard ( pGLIB->getBeId() );

    for ( auto& cReg : pRegVec )
    {
        cReg.second = static_cast<uint32_t> ( fGLIBFW->ReadReg ( cReg.first ) );
        pGLIB->setReg ( cReg.first, cReg.second );
    }
}

std::vector<uint32_t> PixGLIBInterface::ReadBlockBoardReg ( PixGLIB* pGLIB, const std::string& pRegNode, uint32_t pSize )
{
    setBoard ( pGLIB->getBeId() );
    return fGLIBFW->ReadBlockRegValue ( pRegNode, pSize );
}

///////////////
// Startup  Methods
//////////////

void PixGLIBInterface::getBoardInfo ( const PixGLIB* pGLIB )
{
    setBoard ( pGLIB->getBeId() );
    fGLIBFW->getBoardInfo();
}

void PixGLIBInterface::ConfigureGLIB ( const PixGLIB* pGLIB )
{
    //before I can configure the GLIB FW, I need to load it to the CTA which runs the golden Image as default!
    setBoard ( pGLIB->getBeId() );
    std::string cImageName = "PixGLIBFeImage.bin";
    //std::vector<std::string> cImageList = fGLIBFW->getFpgaConfigList();
    //verifyImageName(cImageName, cImageList);
    //fGLIBFW->JumpToFpgaConfig(cImageName);
    //std::cout << "Successfully loaded FW on GLIB " << +pGLIB->getBeId() << std::endl;
    fGLIBFW->ConfigureBoard ( pGLIB );
}

std::vector<std::pair<std::string, uint32_t> > PixGLIBInterface::GLIBHeaderCountDump ( PixGLIB* pGLIB, uint32_t pBlockSize )
{
    setBoard ( pGLIB->getBeId() );
    return fGLIBFW->GLIBHeaderCountDump ( pGLIB, pBlockSize );
}

std::vector<std::pair<std::string, uint32_t> > PixGLIBInterface::GLIBTrailerCountDump ( PixGLIB* pGLIB, uint32_t pBlockSize )
{
    setBoard ( pGLIB->getBeId() );
    return fGLIBFW->GLIBTrailerCountDump ( pGLIB, pBlockSize );
}

///////////////
// Auxillary  Methods
//////////////

const uhal::Node& PixGLIBInterface::getUhalNode ( const PixGLIB* pGLIB, const std::string& pStrPath )
{
    setBoard ( pGLIB->getBeId() );
    return fGLIBFW->getUhalNode ( pStrPath );
}

uhal::HwInterface* PixGLIBInterface::getHardwareInterface ( const PixGLIB* pGLIB )
{
    setBoard ( pGLIB->getBeId() );
    return fGLIBFW->getHardwareInterface();
}


