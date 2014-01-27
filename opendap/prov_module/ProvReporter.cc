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

#include <cstring>
#include <cerrno>

#include "ProvReporter.h"
#include <BESInternalError.h>
#include <TheBESKeys.h>
#include <BESDebug.h>
#include <BESContextManager.h>
#include <BESLog.h>
#include <ctime>
#include <sys/stat.h>  // To make directories.
#include <sys/types.h> // To make directories.


// See https://github.com/tetherless-world/opendap/wiki/OPeNDAP-PROV-Module#wiki-configuration
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

    // TODO: Move the key getting into the constructor -- but it doesn't work from there :-/

    // Use BESKeys::get_value(const string& s, string &val, bool &found)
    // to access:
    //   Prov.cr_base_uri=http://opendap.tw.rpi.edu
    //   Prov.cr_data_root=/home/prizms/prizms/opendap/data/source
    //   Prov.cr_source_id=us
    //   Prov.cr_dataset_id=opendap-prov
    // See https://github.com/tetherless-world/opendap/wiki/OPeNDAP-PROV-Module#wiki-configuration

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
        string err = (string)"Could not determine Prov data root, "
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
    // ^^^^^^ TODO: Move the key getting into the constructor -- but it doesn't work from there :-/

    string abstractDataset = _base_uri + "/source/" + _source_id + "/dataset/" + _dataset_id ;

    // Get timestamp for versionID, e.g. 20140123-1390489968
    // http://stackoverflow.com/questions/16357999/current-date-and-time-as-string
    time_t rawtime;                                                             //
    struct tm * timeinfo;                                                       //
    char buffer[80];                                                            //
    //                                                                          //
    time (&rawtime);                                                            //
    timeinfo = localtime(&rawtime);                                             //
    //                                                                          //
    strftime(buffer,80,"%Y%m%d-%s",timeinfo);                                   //
    std::string dateStamp(buffer);                                              //
    // http://stackoverflow.com/questions/16357999/current-date-and-time-as-string

    // TODO: 4 character UUID at end of versionID.
    // http://stackoverflow.com/questions/3247861/example-of-uuid-generation-using-boost-in-c
    string uuid4 = "ab12" ;

    // e.g. 20140123-1390489968-016e
    string versionID = dateStamp + "-" + uuid4 ;

    string versionedDataset = abstractDataset + "/version/" + versionID ;

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
    string provenanceRecordDirPath  = _data_root + "/" + _source_id + "/" + _dataset_id + "/version/" + versionID + "/source" ;
    string provenanceRecordFilePath = provenanceRecordDirPath + "/opendap-provenance.ttl" ;

    // TODO: assume that _data_root exists and make directories:
    //    _data_root + "/" + _source_id
    //    _data_root + "/" + _source_id + "/" + _dataset_id
    //    _data_root + "/" + _source_id + "/" + _dataset_id + "/version/" + versionID + "/source"
    
    // http://pubs.opengroup.org/onlinepubs/009695399/functions/mkdir.html
    string dir = "/tmp/prov/"+versionID;
    int status;
    status = mkdir( dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) ;
    int myerrno = errno ;
    if( status != 0 )
    {
        BESDEBUG( "prov", "ProvReporter::report - mkdir FAILED, returned "
                          << status << endl ) ;
        char *serr = strerror( myerrno ) ;
        string err = "Unable to create the directory " + dir + ": " ;
        if( serr )
        {
            err.append( serr ) ;
        }
        else
        {
            err.append( "unknown error occurred" ) ;
        }
    }

    string filename = "/tmp/prov.ttl" ;

    ofstream strm( filename.c_str() ) ;
    if( !strm )
    {
        // Need to do something here, but not throw an exception
    }
    else
    {
        strm << "# " << provenanceRecordFilePath << endl ;
        strm << "@prefix dcterms: <http://purl.org/dc/terms/>."  << endl ;
        strm << "@prefix prov:    <http://www.w3.org/ns/prov#>." << endl ;
        strm << endl ;
        strm << "@base <" << versionedDataset << "/>." << endl ;
        strm << endl ;
        strm << "<response> a prov:Entity;" << endl ;
        strm << "   dcterms:format <http://provenanceweb.org/formats/pronom/fmt/286>;" << endl ;
        strm << "." << endl ;
        strm.close() ;
    }

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

