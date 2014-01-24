// NCModule.cc

// This file is part of bes, A C++ back-end server implementation framework
// for the OPeNDAP Data Access Protocol.

// Copyright (c) 2004,2005 University Corporation for Atmospheric Research
// Author: Patrick West <pwest@ucar.org>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// You can contact University Corporation for Atmospheric Research at
// 3080 Center Green Drive, Boulder, CO 80301

// (c) COPYRIGHT University Corporation for Atmospheric Research 2004-2005
// Please read the full copyright statement in the file COPYRIGHT_UCAR.
//
// Authors:
//      pwest       Patrick West <pwest@ucar.edu>

#include <iostream>

using std::endl;

#include "NCModule.h"
#include <BESRequestHandlerList.h>
#include "NCRequestHandler.h"
#include <BESDapService.h>
#include <BESContainerStorageList.h>
#include <BESContainerStorageCatalog.h>
#include <BESCatalogDirectory.h>
#include <BESCatalogList.h>
#include <BESDebug.h>

#define NC_CATALOG "catalog"

void NCModule::initialize( const string & modname )
{
    BESDEBUG( "nc", "Initializing NC module " << modname << endl ) ;

    BESDEBUG( "nc", "    adding " << modname << " request handler" << endl ) ;
    BESRequestHandler *handler = new NCRequestHandler( modname ) ;
    BESRequestHandlerList::TheList()->add_handler( modname, handler ) ;

    BESDEBUG( "nc", modname << " handles dap services" << endl ) ;
    BESDapService::handle_dap_service( modname ) ;

    BESDEBUG( "nc", "    adding " << NC_CATALOG << " catalog" << endl ) ;
    if( !BESCatalogList::TheCatalogList()->ref_catalog( NC_CATALOG ) )
    {
	BESCatalogList::TheCatalogList()->
	    add_catalog( new BESCatalogDirectory( NC_CATALOG ) ) ;
    }
    else
    {
	BESDEBUG( "nc", "    catalog already exists, skipping" << endl ) ;
    }

    BESDEBUG( "nc", "    adding catalog container storage " << NC_CATALOG
		    << endl ) ;
    if( !BESContainerStorageList::TheList()->ref_persistence( NC_CATALOG ) )
    {
	BESContainerStorageCatalog *csc =
	    new BESContainerStorageCatalog( NC_CATALOG ) ;
	BESContainerStorageList::TheList()->add_persistence( csc ) ;
    }
    else
    {
	BESDEBUG( "nc", "    storage already exists, skipping" << endl ) ;
    }

    BESDEBUG( "nc", "    adding nc debug context" << endl ) ;
    BESDebug::Register( "nc" ) ;

    BESDEBUG( "nc", "Done Initializing NC module " << modname << endl ) ;
}

void NCModule::terminate( const string & modname )
{
    BESDEBUG( "nc", "Cleaning NC module " << modname << endl ) ;

    BESDEBUG( "nc", "    removing NC Handler" << modname << endl ) ;
    BESRequestHandler *rh = BESRequestHandlerList::TheList()->remove_handler( modname ) ;
    if( rh ) delete rh ;

    BESDEBUG( "nc", "    removing catalog container storage"
                    << NC_CATALOG << endl ) ;
    BESContainerStorageList::TheList()->deref_persistence( NC_CATALOG ) ;

    BESDEBUG( "nc", "    removing " << NC_CATALOG << " catalog" << endl ) ;
    BESCatalogList::TheCatalogList()->deref_catalog( NC_CATALOG ) ;

    BESDEBUG( "nc", "Done Cleaning NC module " << modname << endl ) ;
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance
 *
 * @param strm C++ i/o stream to dump the information to
 */
void NCModule::dump( ostream & strm ) const
{
    strm << BESIndent::LMarg << "NCModule::dump - (" << (void *) this << ")" 
	 << endl;
}

extern "C" BESAbstractModule * maker()
{
    return new NCModule ;
}

