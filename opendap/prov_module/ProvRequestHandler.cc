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

#include <string>
#include <sstream>
#include <exception>

#include <BESResponseHandler.h>
#include <BESResponseNames.h>
#include <BESDataNames.h>
#include <TheBESKeys.h>
#include <BESServiceRegistry.h>
#include <BESUtil.h>
#include <BESDebug.h>
#include <BESContextManager.h>
#include <BESVersionInfo.h>

#include "ProvRequestHandler.h"

#define Prov_NAME "prov"

ProvRequestHandler::ProvRequestHandler(const string &name) :
    BESRequestHandler(name)
{
    BESDEBUG("prov", "In ProvRequestHandler::ProvRequestHandler" << endl);

    add_handler(HELP_RESPONSE, ProvRequestHandler::prov_build_help);
    add_handler(VERS_RESPONSE, ProvRequestHandler::prov_build_version);

    BESDEBUG("prov", "Exiting ProvRequestHandler::ProvRequestHandler" << endl);
}

ProvRequestHandler::~ProvRequestHandler()
{
}

bool ProvRequestHandler::prov_build_help(BESDataHandlerInterface & dhi)
{
    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESInfo *info = dynamic_cast<BESInfo *> (response);
    if (!info)
        throw BESInternalError("cast error", __FILE__, __LINE__);

    map < string, string > attrs;
    attrs["name"] = PACKAGE_NAME;
    attrs["version"] = PACKAGE_VERSION;
    list < string > services;
    BESServiceRegistry::TheRegistry()->services_handled(Prov_NAME, services);
    if (services.size() > 0) {
        string handles = BESUtil::implode(services, ',');
        attrs["handles"] = handles;
    }
    info->begin_tag("module", &attrs);
    info->end_tag("module");

    return true;
}

bool ProvRequestHandler::prov_build_version(BESDataHandlerInterface & dhi)
{
    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESVersionInfo *info = dynamic_cast<BESVersionInfo *> (response);
    if (!info)
        throw BESInternalError("cast error", __FILE__, __LINE__);

    info->add_module(PACKAGE_NAME, PACKAGE_VERSION);

    return true;
}

