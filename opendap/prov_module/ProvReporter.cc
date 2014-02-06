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
#include <ctime>
#include <sys/stat.h>  // To make directories.
#include <sys/types.h> // To make directories.

#include "ProvReporter.h"
#include <BESInternalError.h>
#include <TheBESKeys.h>
#include <BESDebug.h>
#include <BESContextManager.h>
#include <BESLog.h>


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
    TheBESKeys::TheKeys()->get_value( PROV_SOURCE_ID, _source_id, found ) ;
    if( _source_id == "" )
    {
        _source_id = "us" ;
    }

    found = false ;
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
    BESDEBUG( "prov", "ProvReporter::report - this = " << *this << endl ) ;
    BESDEBUG( "prov", "ProvReporter::report - dhi = " << dhi << endl ) ;

    if( dhi.action.compare( 0, 3, "get" ) != 0 )
    {
        BESDEBUG( "prov", "ProvReporter::report - not a get, so leaving"
                          << endl ) ;
        return ;
    }

    string abstractDataset = _base_uri + "/source/" + _source_id + "/dataset/" + _dataset_id ;

    // Get timestamp for versionID, e.g. 20140123-1390489968
    // http://stackoverflow.com/questions/16357999/current-date-and-time-as-string
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    memset( buffer, 0, sizeof( buffer ) ) ;

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%Y%m%d-%s",timeinfo);
    string dateStamp(buffer);


    // TODO: 4 character UUID at end of versionID.
    // http://stackoverflow.com/questions/3247861/example-of-uuid-generation-using-boost-in-c
    string uuid4 = "ab12" ;

    // e.g. 20140123-1390489968-016e
    string versionID = dateStamp + "-" + uuid4 ;

    string versionedDataset = abstractDataset + "/version/" + versionID ;

    // Write to file:
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

    // Assume that _data_root exists and make directories:
    //    _data_root + "/" + _source_id
    //    _data_root + "/" + _source_id + "/" + _dataset_id
    //    _data_root + "/" + _source_id + "/" + _dataset_id + "/version/" + versionID + "/source"
    string sourceDir   = _data_root + "/" + _source_id ;
    string abstractDir = _data_root + "/" + _source_id + "/" + _dataset_id ;
    string versionDir1 = _data_root + "/" + _source_id + "/" + _dataset_id + "/version/" ;
    string versionDir2 = _data_root + "/" + _source_id + "/" + _dataset_id + "/version/" + versionID ;
    string versionDir3 = _data_root + "/" + _source_id + "/" + _dataset_id + "/version/" + versionID + "/source" ;
    
    // http://pubs.opengroup.org/onlinepubs/009695399/functions/mkdir.html
    //string dir = "/tmp/prov/"+versionID;

    string filename = provenanceRecordFilePath; //"/tmp/prov.ttl" ; // Default; gets updated if we can make directories.

    int status;
    status = mkdir( sourceDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) ;
    int myerrno = errno ;
    // TODO: Make a function to wrap this, since we call it 6 times.
    if( status != 0 )
    {
        char *serr = strerror( myerrno ) ;
        string err = "Unable to create the directory " + sourceDir + ": " ;
        if( serr )
        {
            err.append( serr ) ;
        }
        else
        {
            err.append( "unknown error occurred" ) ;
        }
        BESDEBUG( "prov", "ProvReporter::report - " << err << endl ) ;
    }
    // TODO: Make a function to wrap this, since we call it 6 times.
    status = mkdir( sourceDir.c_str(),   S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) ;
    // TODO: Make a function to wrap this, since we call it 6 times.
    status = mkdir( abstractDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) ;
    // TODO: Make a function to wrap this, since we call it 6 times.
    status = mkdir( versionDir1.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) ;
    // TODO: Make a function to wrap this, since we call it 6 times.
    status = mkdir( versionDir2.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) ;
    // TODO: Make a function to wrap this, since we call it 6 times.
    status = mkdir( versionDir3.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) ;
    if( status != 0 )
    {
        filename = "/tmp/opendap-prov.ttl" ;

        char *serr = strerror( myerrno ) ;
        string err = "Unable to create the directory " + versionDir3 + ": " ;
        if( serr )
        {
            err.append( serr ) ;
        }
        else
        {
            err.append( "unknown error occurred" ) ;
        }
        err.append( ". Using " + filename ) ;
        BESDEBUG( "prov", "ProvReporter::report - " << err << endl ) ;
    }

    ofstream strm( filename.c_str() ) ;
    if( !strm )
    {
        // Need to do something here, but not throw an exception
    }
    else
    {
        strm << "@prefix rdfs:    <http://www.w3.org/2000/01/rdf-schema#>." << endl ;
        strm << "@prefix dcterms: <http://purl.org/dc/terms/>."  << endl ;
        strm << "@prefix prov:    <http://www.w3.org/ns/prov#>." << endl ;
        strm << "@prefix pml:     <http://provenanceweb.org/ns/pml#>." << endl ;
        strm << endl ;
        strm << "@base <" << versionedDataset << "/>." << endl ;
        strm << endl ;

        // We need to know what files were loaded and what loaded them.
        // We have the list of containers, and those containers have the
        // full path to the file and the type. The type is the request
        // handler that would be used to read in the data. So get the
        // request handler and ask for the version information
        int counter = 1;
        list<BESContainer *>::const_iterator i = dhi.containers.begin();
        list<BESContainer *>::const_iterator ie = dhi.containers.end();
        for (; i != ie; i++)
        {
            strm << "<used/" << counter << "> a prov:Entity;" << endl ;
            strm << "   rdfs:label \"container name: " << (*i)->get_real_name() << "\";"
                 << endl ;
            strm << "   rdfs:comment \" container type: " << (*i)->get_container_type() << "\";" << endl ;
            //if ( string.compare("nc") == 0 ) {
            strm << "   dcterms:format <https://github.com/tetherless-world/opendap/wiki/OPeNDAP-Vocabulary#wiki-abstract-netcdf>;" << endl ;
            //}
            strm << "." << endl ;

            strm << endl ;
            strm << "<used/" << counter << "/data-dds> a prov:Entity;" << endl ;
            strm << "   dcterms:format <https://github.com/tetherless-world/opendap/issues/45#datadds>;" << endl ;
            strm << "   prov:wasDerivedFrom <used/" << counter << ">;" << endl;
            strm << "." << endl ;

            strm << endl ;
            strm << "<used/" << counter << "/constraint> a pml:DeclarativePlan, prov:Plan, prov:Entity;" << endl ;
            strm << "   a <https://github.com/tetherless-world/opendap/wiki/OPeNDAP-Vocabulary#Constraint>;" << endl;
            strm << "   prov:value \"" << (*i)->get_constraint() << "\";" << endl;
            strm << "." << endl ;
            // If there is a constraint then we know we need to include
            // information about the dap module

            // What we don't know unless we parse the constrant is
            // whether or not there are server-side functions being
            // called. And if they are, where do I get information about
            // the module that loaded the server-side function
            counter++;
        }

        // What's the response object here. We know it's get.x, so
        // what is x? From that we should be able to grab the request
        // handler for that and ask for the version information
        string actualaction = dhi.action.substr( 4 ) ;
        string ascii_val = "ascii";

        strm << endl ;
        strm << "<response/data-dds> a prov:Entity;" << endl ;
        strm << "   dcterms:format <https://github.com/tetherless-world/opendap/issues/45#datadds>;" << endl ;
        strm << "." << endl ;

        strm << endl ;
        strm << "<response> a prov:Entity;" << endl ;
        strm << "   rdfs:comment \"" << "Action: " << actualaction << "\";" << endl ;
        for( int c = 1; c < counter; c++ ) {
            strm << "   prov:wasDerivedFrom <used/" << c << ">;" << endl;
        }
        strm << "   prov:specializationOf <the-requested-url>;" << endl ;
        // TODO: fix compile error
        //if ( string.compare(ascii_val) == 0 ) {
            strm << "   dcterms:format <http://provenanceweb.org/format/mime/text/plain>;" << endl ;
        //}
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

