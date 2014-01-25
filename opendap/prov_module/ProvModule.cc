// ProvModule.cc

// This file is part of the OPeNDAP Prov Module, providing provenance
// tracing for data requess

// Copyright (c) 2014 Tetherless World Constellation at Rensselaer
// Polytechnic Institute
// Author: Patrick West <westp@rpi.edu> and Tim Lebo <lebot@rpi.edu>
//
// This module is free software; you can redistribute it and/or
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
// License along with this module; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// You can contact Tetherless World Constellation at Rensselaer
// Polytechnic Institute at 110 8th Street, Troy, NY, 12180
 
// (c) COPYRIGHT Tetherless World Constellation at Rensselaer
// Polytechnic Institute 2014
// Please read the full copyright statement in the file COPYRIGHT_TWC.
//
// Authors:
//      westp       Patrick West <westp@rpi.edu>
//      lebot       Tim Lebo <lebot@rpi.edu>

#include "config_prov.h"

#include <iostream>

using std::endl ;

#include "ProvModule.h"
#include <BESDebug.h>
#include <BESRequestHandlerList.h>
#include "ProvRequestHandler.h"
#include <BESReporterList.h>
#include "ProvReporter.h"

void
ProvModule::initialize( const string &modname )
{
    BESDEBUG( modname, "Initializing Prov Module "
		       << modname << endl ) ;

    BESDEBUG( modname, "    adding Prov debug context" << endl ) ;
    BESDebug::Register( modname ) ;

    BESDEBUG( "prov", "    adding " << modname << " request handler" << endl ) ;
    BESRequestHandler *handler = new ProvRequestHandler( modname ) ;
    BESRequestHandlerList::TheList()->add_handler( modname, handler ) ;

    BESDEBUG( "prov", "    adding Prov reporter" << endl ) ;
    BESReporterList::TheList()->add_reporter( modname, new ProvReporter ) ;

    BESDEBUG( modname, "Done Initializing Prov Module "
		       << modname << endl ) ;
}

void
ProvModule::terminate( const string &modname )
{
    BESDEBUG( modname, "Cleaning Prov module " << modname << endl ) ;

    BESDEBUG( "prov", "    removing Prov reporter" << endl ) ;
    BESReporter *r = BESReporterList::TheList()->remove_reporter( modname ) ;
    if( r ) delete r ;

    BESDEBUG( "prov", "    removing Prov Handler" << modname << endl ) ;
    BESRequestHandler *rh = BESRequestHandlerList::TheList()->remove_handler( modname ) ;
    if( rh ) delete rh ;

    BESDEBUG( modname, "Done Cleaning Prov module " << modname << endl ) ;
}

extern "C"
{
    BESAbstractModule *maker()
    {
	return new ProvModule ;
    }
}

void
ProvModule::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "ProvModule::dump - ("
			     << (void *)this << ")" << endl ;
}

