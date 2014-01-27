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
#include <BESDebug.h>
#include <BESContextManager.h>
#include <BESLog.h>

#define PROV_BASE_URI "Prov.cr_base_uri"
#define PROV_DATA_ROOT "Prov.cr_data_root"
#define PROV_SOURCE_ID "Prov.cr_source_id"
#define PROV_DATASET_ID "Prov.cr_dataset_id"

/** @brief ProvReporter constructur loads configuration options
 *
 * Required parameters from configuration. If not present or not set
 * then an exception is thrown
 * - Prov.cr_base_uri
 * - Prov.cr_data_root
 *
 * Additional parameters with defaults
 * - Prov.cr_source_id = us
 * - Prov.cr_dataset_id = opendap-prov
 */
ProvReporter::ProvReporter()
    : BESReporter()
{
    // use BESKeys::get_value(const string& s, string &val, bool &found)
    // to access 
    // Prov.cr_base_uri=http://opendap.tw.rpi.edu
    // Prov.cr_data_root=/home/prizms/prizms/opendap/data/source
    // Prov.cr_source_id=us
    // Prov.cr_dataset_id=opendap-prov

    bool found = false ;
    TheBESKeys::TheKeys()->get_value( PROV_BASE_URI, _base_uri, found ) ;
    if( _base_uri == "" )
    {
        string err = (string)"Could not determine Prov base URI, "
                     + PROV_BASE_URI
                     + " parameter not found or set in configuration file" ;
        throw BESInternalError( err, __FILE__, __LINE__ ) ;
    }

    found = false ;
    TheBESKeys::TheKeys()->get_value( PROV_DATA_ROOT, _data_root, found ) ;
    if( _data_root == "" )
    {
        string err = (string)"Could not determine Prov base URI, "
                     + PROV_DATA_ROOT
                     + " parameter not found or set in configuration file" ;
        throw BESInternalError( err, __FILE__, __LINE__ ) ;
    }

    found = false ;
    string _source_id ;
    TheBESKeys::TheKeys()->get_value( PROV_SOURCE_ID, _source_id, found ) ;
    if( _source_id == "" )
    {
        _source_id = "us" ;
    }

    found = false ;
    string _dataset_id ;
    TheBESKeys::TheKeys()->get_value( PROV_DATASET_ID, _dataset_id, found ) ;
    if( _dataset_id == "" )
    {
        _dataset_id = "opendap-prov" ;
    }
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
    BESDEBUG( "prov", "ProvReporter::report" << endl ) ;
    // TODO: generate versionID yyyyMMdd-s-uuid[4]
    // e.g. 20140123-1390489968-016e
    string filename = "/tmp/prov" ;

    ofstream strm( filename.c_str() ) ;
    if( !strm )
    {
        // Need to do something here, but not throw an exception
    }
    else
    {
        strm << "Write some stuff" << endl ;
        strm.close() ;
    }

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
    BESDEBUG( "prov", "ProvReporter::report - leaving" << endl ) ;
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
    strm << BESIndent::LMarg << "base URI = " << _base_uri << endl ;
    strm << BESIndent::LMarg << "data root = " << _data_root << endl ;
    strm << BESIndent::LMarg << "source id = " << _source_id << endl ;
    strm << BESIndent::LMarg << "dataset id = " << _dataset_id << endl ;
    BESIndent::UnIndent() ;
}

