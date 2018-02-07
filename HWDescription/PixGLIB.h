/*!

        \file                PiXGLIB.h
        \brief                           PiXGLIB Description class, configs of the PiXGLIB
        \author                          Lorenzo BIDEGAIN
        \date                            14/07/14
        \version                         1.0
        Support :                        mail to : lorenzo.bidegain@gmail.com

 */

#ifndef _PixGLIB_h__
#define _PixGLIB_h__

// #include "Definition.h"
#include <vector>
#include <map>
#include <stdint.h>
#include <string>

typedef std::map< std::string, uint32_t > PixGLIBRegMap;     /*!< Map containing the registers of a board */

/*!
 * \class PiXGLIB
 * \brief Read/Write PiXGLIB's registers on a file, handles a register map and handles a vector of Fitel which are connected to the PiXGLIB
 */
class PixGLIB
{

public:

    // C'tors: the PiXGLIB only needs to know about it's shelf and which BE it is
    /*!
     * \brief Default C'tor
     */
    PixGLIB();

    /*!
     * \brief Standard C'tor
     * \param pBeId
     */
    PixGLIB( uint8_t pBeId );

    /*!
    * \brief C'tor for a standard PiXGLIB reading a config file
    * \param pBeId
    * \param filename of the configuration file
    */
    PixGLIB( uint8_t pBeId, const std::string& filename );

    /*!
    * \brief Destructor
    */
    ~PixGLIB()
    {
    }

    // Public Methods

    /*!
    * \brief Get any register from the Map
    * \param pReg
    * \return The value of the register
    */
    uint32_t getReg( const std::string& pReg ) const;
    /*!
    * \brief Set any register of the Map, if the register is not on the map, it adds it.
    * \param pReg
    * \param psetValue
    */
    void setReg( const std::string& pReg, uint32_t psetValue );


    PixGLIBRegMap getPixGLIBRegMap() const
    {
        return fRegMap;
    }

    /*!
    * \brief Get the BeBoardId of the PiXGLIB
    * \return the PiXGLIB Id
    */
    uint8_t getBeId() const
    {
        return fBeId;
    }

    /*!
    * \brief Set the Be Id of the PiXGLIB
    * \param pBeId
    */
    void setBeId( uint8_t pBeId )
    {
        fBeId = pBeId;
    };

protected:
    //Connection Members
    uint8_t fBeId;


    PixGLIBRegMap fRegMap;             /*!< Map of PiXGLIB Register Names vs. Register Values */

private:

    /*!
    * \brief Load RegMap from a file
    * \param filename
    */
    void loadConfigFile( const std::string& filename );
};

#endif
