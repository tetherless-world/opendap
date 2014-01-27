// ProvReporter.cc

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

#include "ProvReporter.h"
#include <BESInternalError.h>
#include <TheBESKeys.h>
#include <BESContextManager.h>
#include <BESLog.h>

ProvReporter::ProvReporter()
    : BESReporter()
{
}

ProvReporter::~ProvReporter()
{
}

/** @brief reports any provenance information to an external provenance
 * ttl file
 *
 * @param dhi structure that contains all information pertaining to the data
 * request
 */
void
ProvReporter::report( BESDataHandlerInterface &dhi )
{
    // TODO: use BESKeys::get_value(const string& s, string &val, bool &found)
    // to access 
    // Prov.cr_base_uri=http://opendap.tw.rpi.edu
    // Prov.cr_data_root=/home/prizms/prizms/opendap/data/source
    // Prov.cr_source_id=us
    // Prov.cr_dataset_id=opendap-prov


    // TODO: generate versionID yyyyMMdd-s-uuid[4]
    // e.g. 20140123-1390489968-016e

    // TODO: write to file:
    // 
    // /home/prizms/prizms/opendap/data/source/us/opendap-prov/version/20140123-1390489968-016e/source/opendap-provenance.ttl
    // ^^^Prov.cr_data_root^^^^^^^^^^^^^^^^^^^
    //                                        /
    //                                           /
    //      Prov.cr_source_id (or 'us' if DNE) ^^
    //     Prov.cr_dataset_id (or 'opendap-prov') ^^^^^^^^^^^^
    //                                                        /version/
    //                                                                 ^^$VERSION_ID^^^^^^^^^^
    //                                                                                        /source/opendap-provenance.ttl
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance along with information about
 * the containers stored in this volatile list.
 *
 * @param strm C++ i/o stream to dump the information to
 */
void
ProvReporter::dump( ostream &strm ) const
{
    strm << BESIndent::LMarg << "ProvReporter::dump - ("
			     << (void *)this << ")" << endl ;
    BESIndent::Indent() ;
    BESIndent::UnIndent() ;
}

