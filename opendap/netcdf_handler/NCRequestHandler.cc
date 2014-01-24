// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of nc_handler, a data handler for the OPeNDAP data
// server.

// Copyright (c) 2002,2003 OPeNDAP, Inc.
// Author: James Gallagher <jgallagher@opendap.org>
//
// This is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License as published by the Free
// Software Foundation; either version 2.1 of the License, or (at your
// option) any later version.
//
// This software is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.

// NCRequestHandler.cc

#include "config_nc.h"

#include <string>
#include <sstream>
#include <exception>

#include <BESResponseHandler.h>
#include <BESResponseNames.h>
#include <BESDapNames.h>
#include <BESDASResponse.h>
#include <BESDDSResponse.h>
#include <BESDataDDSResponse.h>
#include <BESVersionInfo.h>

#include <BESDapError.h>
#include <BESInternalFatalError.h>
#include <BESDataNames.h>
#include <TheBESKeys.h>
#include <BESServiceRegistry.h>
#include <BESUtil.h>
#include <BESDebug.h>
#include <BESContextManager.h>

#include <InternalErr.h>
#include <Ancillary.h>

#include "NCRequestHandler.h"
#if 0
#include "gridfields_functions.h"
#endif

#define NC_NAME "nc"

using namespace libdap;

bool NCRequestHandler::_show_shared_dims = true;
bool NCRequestHandler::_show_shared_dims_set = false;

bool NCRequestHandler::_ignore_unknown_types = false;
bool NCRequestHandler::_ignore_unknown_types_set = false;

bool NCRequestHandler::_promote_byte_to_short = false;
bool NCRequestHandler::_promote_byte_to_short_set = false;

extern void nc_read_dataset_attributes(DAS & das, const string & filename);
extern void nc_read_dataset_variables(DDS & dds, const string & filename);

/** Is the version number string greater than or equal to the value.
 * @note Works only for versions with zero or one dot. If the conversion of
 * the string to a float fails for any reason, this returns false.
 * @param version The string value (e.g., 3.2)
 * @param value A floating point value.
 */
static bool version_ge(const string &version, float value)
{
    try {
        float v;
        istringstream iss(version);
        iss >> v;
        //cerr << "version: " << v << ", value: " << value << endl;
        return (v >= value);
    }
    catch (...) {
        return false;
    }

    return false; // quiet warnings...
}

NCRequestHandler::NCRequestHandler(const string &name) :
    BESRequestHandler(name)
{
    BESDEBUG("nc", "In NCRequestHandler::NCRequestHandler" << endl);

    add_handler(DAS_RESPONSE, NCRequestHandler::nc_build_das);
    add_handler(DDS_RESPONSE, NCRequestHandler::nc_build_dds);
    add_handler(DATA_RESPONSE, NCRequestHandler::nc_build_data);
    add_handler(HELP_RESPONSE, NCRequestHandler::nc_build_help);
    add_handler(VERS_RESPONSE, NCRequestHandler::nc_build_version);

    // Look for the SHowSharedDims property, if it has not been set
    if (NCRequestHandler::_show_shared_dims_set == false) {
        bool key_found = false;
        string doset;
        TheBESKeys::TheKeys()->get_value("NC.ShowSharedDimensions", doset, key_found);
        if (key_found) {
            // It was set in the conf file
            NCRequestHandler::_show_shared_dims_set = true;

            doset = BESUtil::lowercase(doset);
            if (doset == "true" || doset == "yes") {
                NCRequestHandler::_show_shared_dims = true;
            }
            else
                NCRequestHandler::_show_shared_dims = false;
        }
    }

    if (NCRequestHandler::_ignore_unknown_types_set == false) {
        bool key_found = false;
        string doset;
        TheBESKeys::TheKeys()->get_value("NC.IgnoreUnknownTypes", doset, key_found);
        if (key_found) {
            doset = BESUtil::lowercase(doset);
            if (doset == "true" || doset == "yes")
                NCRequestHandler::_ignore_unknown_types = true;
            else
                NCRequestHandler::_ignore_unknown_types = false;

            NCRequestHandler::_ignore_unknown_types_set = true;
        }
    }

    if (NCRequestHandler::_promote_byte_to_short_set == false) {
        bool key_found = false;
        string doset;
        TheBESKeys::TheKeys()->get_value("NC.PromoteByteToShort", doset, key_found);
        if (key_found) {
            doset = BESUtil::lowercase(doset);
            if (doset == "true" || doset == "yes")
                NCRequestHandler::_promote_byte_to_short = true;
            else
                NCRequestHandler::_promote_byte_to_short = false;

            NCRequestHandler::_promote_byte_to_short_set = true;
        }
    }

    BESDEBUG("nc", "Exiting NCRequestHandler::NCRequestHandler" << endl);
}

NCRequestHandler::~NCRequestHandler()
{
}

bool NCRequestHandler::nc_build_das(BESDataHandlerInterface & dhi)
{
    BESDEBUG("nc", "In NCRequestHandler::nc_build_das" << endl);

    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESDASResponse *bdas = dynamic_cast<BESDASResponse *> (response);
    if (!bdas)
        throw BESInternalError("cast error", __FILE__, __LINE__);
    try {
        bdas->set_container(dhi.container->get_symbolic_name());
        DAS *das = bdas->get_das();
        string accessed = dhi.container->access();
        nc_read_dataset_attributes(*das, accessed);
        Ancillary::read_ancillary_das(*das, accessed);
        bdas->clear_container();
    }
    catch (BESError &e) {
        throw;
    }
    catch (InternalErr & e) {
        BESDapError ex(e.get_error_message(), true, e.get_error_code(), __FILE__, __LINE__);
        throw ex;
    }
    catch (Error & e) {
        BESDapError ex(e.get_error_message(), false, e.get_error_code(), __FILE__, __LINE__);
        throw ex;
    }
    catch (std::exception &e) {
        string s = string("C++ Exception: ") + e.what();
        BESInternalFatalError ex(s, __FILE__, __LINE__);
        throw ex;
    }
    catch (...) {
        string s = "unknown exception caught building DAS";
        BESInternalFatalError ex(s, __FILE__, __LINE__);
        throw ex;
    }

    BESDEBUG("nc", "Exiting NCRequestHandler::nc_build_das" << endl);
    return true;
}

bool NCRequestHandler::nc_build_dds(BESDataHandlerInterface & dhi)
{
    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESDDSResponse *bdds = dynamic_cast<BESDDSResponse *> (response);
    if (!bdds)
        throw BESInternalError("cast error", __FILE__, __LINE__);

    try {
        // If there's no value for this set in the conf file, look at the context
        // and set the default behavior based on the protocol version clients say
        // they will accept.
        if (NCRequestHandler::_show_shared_dims_set == false) {
            bool context_found = false;
            string context_value = BESContextManager::TheManager()->get_context("xdap_accept", context_found);
            if (context_found) {
                BESDEBUG("nc", "xdap_accept: " << context_value << endl);
                if (version_ge(context_value, 3.2))
                    NCRequestHandler::_show_shared_dims = false;
                else
                    NCRequestHandler::_show_shared_dims = true;
            }
        }

        BESDEBUG("nc", "Fiddled with xdap_accept" << endl);

        bdds->set_container(dhi.container->get_symbolic_name());
        DDS *dds = bdds->get_dds();
        string accessed = dhi.container->access();
        dds->filename(accessed);

        BESDEBUG("nc", "Prior to nc_read_dataset_variables" << endl);

        nc_read_dataset_variables(*dds, accessed);

        BESDEBUG("nc", "Prior to Ancillary::read_ancillary_dds, accessed: " << accessed << endl);

        Ancillary::read_ancillary_dds(*dds, accessed);

        DAS *das = new DAS;
        BESDASResponse bdas(das);
        bdas.set_container(dhi.container->get_symbolic_name());
        nc_read_dataset_attributes(*das, accessed);
        Ancillary::read_ancillary_das(*das, accessed);

        BESDEBUG("nc", "Prior to dds->transfer_attributes" << endl);

        dds->transfer_attributes(das);

#if 0
        ConstraintEvaluator & ce = bdds->get_ce();
        ce.add_function("ugrid_restrict", function_ugrid_restrict);
#endif

        bdds->set_constraint(dhi);

        bdds->clear_container();
    }
    catch (BESError &e) {
        throw e;
    }
    catch (InternalErr & e) {
        BESDapError ex(e.get_error_message(), true, e.get_error_code(), __FILE__, __LINE__);
        throw ex;
    }
    catch (Error & e) {
        BESDapError ex(e.get_error_message(), false, e.get_error_code(), __FILE__, __LINE__);
        throw ex;
    }
    catch (std::exception &e) {
        string s = string("C++ Exception: ") + e.what();
        BESInternalFatalError ex(s, __FILE__, __LINE__);
        throw ex;
    }
    catch (...) {
        string s = "unknown exception caught building DDS";
        BESInternalFatalError ex(s, __FILE__, __LINE__);
        throw ex;
    }

    return true;
}

bool NCRequestHandler::nc_build_data(BESDataHandlerInterface & dhi)
{
    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESDataDDSResponse *bdds = dynamic_cast<BESDataDDSResponse *> (response);
    if (!bdds)
        throw BESInternalError("cast error", __FILE__, __LINE__);

    try {
        if (NCRequestHandler::_show_shared_dims_set == false) {
            bool context_found = false;
            string context_value = BESContextManager::TheManager()->get_context("xdap_accept", context_found);
            if (context_found) {
                BESDEBUG("nc", "xdap_accept: " << context_value << endl);
                if (version_ge(context_value, 3.2))
                    NCRequestHandler::_show_shared_dims = false;
                else
                    NCRequestHandler::_show_shared_dims = true;
            }
        }

        bdds->set_container(dhi.container->get_symbolic_name());
        DataDDS *dds = bdds->get_dds();
        string accessed = dhi.container->access();
        dds->filename(accessed);

        nc_read_dataset_variables(*dds, accessed);

        Ancillary::read_ancillary_dds(*dds, accessed);

        DAS *das = new DAS;
        BESDASResponse bdas(das);
        bdas.set_container(dhi.container->get_symbolic_name());
        nc_read_dataset_attributes(*das, accessed);
        Ancillary::read_ancillary_das(*das, accessed);

        dds->transfer_attributes(das);

#if 0
        ConstraintEvaluator & ce = bdds->get_ce();
        ce.add_function("ugrid_restrict", function_ugrid_restrict);
#endif

        bdds->set_constraint(dhi);

        bdds->clear_container();
    }
    catch (BESError &e) {
        throw;
    }
    catch (InternalErr & e) {
        BESDapError ex(e.get_error_message(), true, e.get_error_code(), __FILE__, __LINE__);
        throw ex;
    }
    catch (Error & e) {
        BESDapError ex(e.get_error_message(), false, e.get_error_code(), __FILE__, __LINE__);
        throw ex;
    }
    catch (std::exception &e) {
        string s = string("C++ Exception: ") + e.what();
        BESInternalFatalError ex(s, __FILE__, __LINE__);
        throw ex;
    }
    catch (...) {
        string s = "unknown exception caught building DAS";
        BESInternalFatalError ex(s, __FILE__, __LINE__);
        throw ex;
    }

    return true;
}

bool NCRequestHandler::nc_build_help(BESDataHandlerInterface & dhi)
{
    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESInfo *info = dynamic_cast<BESInfo *> (response);
    if (!info)
        throw BESInternalError("cast error", __FILE__, __LINE__);

    map < string, string > attrs;
    attrs["name"] = PACKAGE_NAME;
    attrs["version"] = PACKAGE_VERSION;
    list < string > services;
    BESServiceRegistry::TheRegistry()->services_handled(NC_NAME, services);
    if (services.size() > 0) {
        string handles = BESUtil::implode(services, ',');
        attrs["handles"] = handles;
    }
    info->begin_tag("module", &attrs);
    info->end_tag("module");

    return true;
}

bool NCRequestHandler::nc_build_version(BESDataHandlerInterface & dhi)
{
    BESResponseObject *response = dhi.response_handler->get_response_object();
    BESVersionInfo *info = dynamic_cast<BESVersionInfo *> (response);
    if (!info)
        throw BESInternalError("cast error", __FILE__, __LINE__);

    info->add_module(PACKAGE_NAME, PACKAGE_VERSION);

    return true;
}
