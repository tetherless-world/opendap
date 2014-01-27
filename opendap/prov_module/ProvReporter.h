// ProvReporter.h

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

#ifndef A_ProvReporter_h
#define A_ProvReporter_h 1

#include <fstream>
#include <ctime>       // For versionID timestamp
#include <sys/stat.h>  // To make directories.
#include <sys/types.h> // To make directories.

using std::ofstream ;
using std::ios ;
using std::endl ;

#include <BESReporter.h>
#include <BESDataHandlerInterface.h>

class ProvDB ;

class ProvReporter : public BESReporter
{
private:
    string          _base_uri ;
    string          _data_root ;
    string          _source_id ;
    string          _dataset_id ;
public:
			        ProvReporter() ;
    virtual		    ~ProvReporter() ;

    virtual void	report( BESDataHandlerInterface &dhi ) ;

    virtual void	dump( ostream &strm ) const ;
} ;

#endif // A_ProvReporter_h

