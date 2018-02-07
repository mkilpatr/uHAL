/*!

        Filename :                              PixGLIB.cc
        Content :                               PixGLIB Description class, configs of the PixGLIB
        Programmer :                    Lorenzo BIDEGAIN
        Version :               1.0
        Date of Creation :              14/07/14
        Support :                               mail to : lorenzo.bidegain@gmail.com

 */

#include "PixGLIB.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

// Constructors

PixGLIB::PixGLIB() :
    fBeId( 0 ) {}

PixGLIB::PixGLIB( uint8_t pBeId ) :
    fBeId( pBeId )
{
}

PixGLIB::PixGLIB( uint8_t pBeId, const std::string& filename ) :
    fBeId( pBeId )
{
    loadConfigFile( filename );
}

// Public Members:

uint32_t PixGLIB::getReg( const std::string& pReg ) const
{
    PixGLIBRegMap::const_iterator i = fRegMap.find( pReg );
    if ( i == fRegMap.end() )
    {
        std::cout << "The Board object: " << +fBeId << " doesn't have " << pReg << std::endl;
        return 0;
    }
    else return i->second;
}

void PixGLIB::setReg( const std::string& pReg, uint32_t psetValue )
{
    PixGLIBRegMap::iterator i = fRegMap.find( pReg );
    if ( i == fRegMap.end() )
        fRegMap.insert( {pReg, psetValue} );
    else i->second = psetValue;
}


// Private Members:

void PixGLIB::loadConfigFile( const std::string& filename )

{

    std::ifstream cFile( filename.c_str(), std::ios::in );
    if ( !cFile ) std::cerr << "The PixGLIB Settings File " << filename << " could not be opened!" << std::endl;
    else
    {

        fRegMap.clear();
        std::string cLine, cName, cValue, cFound;

        while ( !( getline( cFile, cLine ).eof() ) )
        {

            if ( cLine.find_first_not_of( " \t" ) == std::string::npos ) continue;
            if ( cLine.at( 0 ) == '#' || cLine.at( 0 ) == '*' ) continue;
            if ( cLine.find( ":" ) == std::string::npos ) continue;

            std::istringstream input( cLine );
            input >> cName >> cFound >> cValue;


            // Here the Reg name sits in cName and the Reg value sits in cValue
            if ( cValue.find( "0x" ) != std::string::npos )
                fRegMap[cName] = strtol( cValue.c_str(), 0, 16 );
            else
                fRegMap[cName] = strtol( cValue.c_str(), 0, 10 );
        }

        cFile.close();

    }

}

