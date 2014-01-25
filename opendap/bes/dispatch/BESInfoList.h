// BESInfoList.h

// This file is part of bes, A C++ back-end server implementation framework
// for the OPeNDAP Data Access Protocol.

// Copyright (c) 2004-2009 University Corporation for Atmospheric Research
// Author: Patrick West <pwest@ucar.edu> and Jose Garcia <jgarcia@ucar.edu>
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
//      jgarcia     Jose Garcia <jgarcia@ucar.edu>

#ifndef I_BESInfoList_h
#define I_BESInfoList_h 1

#include <map>
#include <string>

using std::map ;
using std::string ;

#include "BESObj.h"

class BESInfo ;

typedef BESInfo * (*p_info_builder)( const string &info_type ) ;

class BESInfoList : public BESObj
{
private:
    static BESInfoList *	_instance ;
    map< string, p_info_builder >_info_list ;

    typedef map< string, p_info_builder >::const_iterator Info_citer ;
    typedef map< string, p_info_builder >::iterator Info_iter ;
protected:
				BESInfoList(void) ;
public:
    virtual			~BESInfoList(void) ;

    virtual bool		add_info_builder( const string &info_type,
					   p_info_builder info_builder ) ;
    virtual bool		rem_info_builder( const string &info_type ) ;
    virtual BESInfo *		build_info( ) ;

    virtual void		dump( ostream &strm ) const ;

    static BESInfoList *	TheList() ;
};

#endif // I_BESInfoList_h
