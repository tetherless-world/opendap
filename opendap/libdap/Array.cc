
// -*- mode: c++; c-basic-offset:4 -*-

// This file is part of libdap, A C++ implementation of the OPeNDAP Data
// Access Protocol.

// Copyright (c) 2002,2003 OPeNDAP, Inc.
// Author: James Gallagher <jgallagher@opendap.org>
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
// You can contact OPeNDAP, Inc. at PO Box 112, Saunderstown, RI. 02874-0112.

// (c) COPYRIGHT URI/MIT 1994-1999
// Please read the full copyright statement in the file COPYRIGHT_URI.
//
// Authors:
//      jhrg,jimg       James Gallagher <jgallagher@gso.uri.edu>

// Implementation for Array.
//
// jhrg 9/13/94


#include "config.h"

#include <algorithm>
#include <functional>
#include <sstream>

#include "Array.h"
#include "util.h"
#include "debug.h"
#include "InternalErr.h"
#include "escaping.h"

using namespace std;

namespace libdap {

void
Array::_duplicate(const Array &a)
{
    _shape = a._shape;
}

// The first method of calculating length works when only one dimension is
// constrained and you want the others to appear in total. This is important
// when selecting from grids since users may not select from all dimensions
// in which case that means they want the whole thing. Array projection
// should probably work this way too, but it doesn't. 9/21/2001 jhrg

/** @deprecated Calling this method should never be necessary. It is used
    internally called whenever the size of the Array is changed, e.g., by a
    constraint.

    Changes the length property of the array.
*/
void
Array::update_length(int)
{
    int length = 1;
    for (Dim_citer i = _shape.begin(); i != _shape.end(); i++) {
        length *= (*i).c_size > 0 ? (*i).c_size : 1;
    }

    set_length(length);
}

// Construct an instance of Array. The (BaseType *) is assumed to be
// allocated using new - The dtor for Vector will delete this object.

/** Build an array with a name and an element type. The name may be omitted,
    which will create a nameless variable. The template (element type) pointer
    may also be omitted, but if it is omitted when the Array is created, it
    \e must be added (with \c add_var()) before \c read() or \c deserialize()
    is called.

    @todo Force the Array::add_var() method to be used to add \e v.
    This version of add_var() calls Vector::add_var().

    @param n A string containing the name of the variable to be
    created.
    @param v A pointer to a variable of the type to be included
    in the Array. May be null and set later using add_var() or add_var_nocopy()
    @brief Array constructor
*/
Array::Array(const string &n, BaseType *v) : Vector(n, 0, dods_array_c)
{
    add_var(v); // Vector::add_var() stores null if v is null
}

/** Build an array on the server-side with a name, a dataset name from which
    this Array is being created, and an element type.

    @todo Force the Array::add_var() method to be used to add \e v.
    This version of add_var() calls Vector::add_var().

    @param n A string containing the name of the variable to be created.
    @param d A string containing the name of the dataset from which this
    variable is being created.
    @param v A pointer to a variable of the type to be included
    in the Array.
    @brief Array constructor
*/
Array::Array(const string &n, const string &d, BaseType *v)
    : Vector(n, d, 0, dods_array_c)
{
    add_var(v); // Vector::add_var() stores null is v is null
}

/** @brief The Array copy constructor. */
Array::Array(const Array &rhs) : Vector(rhs)
{
    _duplicate(rhs);
}

/** @brief The Array destructor. */
Array::~Array()
{
    DBG(cerr << "Entering ~Array (" << this << ")" << endl);
    DBG(cerr << "Exiting ~Array" << endl);
}

BaseType *
Array::ptr_duplicate()
{
    return new Array(*this);
}

Array &
Array::operator=(const Array &rhs)
{
    if (this == &rhs)
        return *this;

    dynamic_cast<Vector &>(*this) = rhs;

    _duplicate(rhs);

    return *this;
}

/** @brief Add the BaseType pointer to this constructor type
    instance.

    Propagate the name of the BaseType instance to this instance. This
    ensures that variables at any given level of the DDS table have
    unique names (i.e., that Arrays do not have their default name ""). If
    <tt>v</tt>'s name is null, then assume that the array \e is named and
    don't overwrite it with <tt>v</tt>'s null name.

    @note It is possible for the BaseType pointer to be null when this
    method is called, a behavior that differs considerably from that of
    the other 'add_var()' methods.

    @note This version checks to see if \e v is an array. If so, it calls
    Vector::add_var() using the template variable of \e v and then appends
    the dimensions of \e v to this array. This somewhat obscure behavior
    simplifies 'translating' Sequences to arrays when the actual variable
    being translated is not a regular Sequence but an array of Sequences.
    This is of very debatable usefulness, but it's here all the same.

    @param v The template variable for the array
    @param p The Part parameter defaults to nil and is ignored by this method.
*/

void
Array::add_var(BaseType *v, Part)
{
#if 0
    if (v && v->is_dap4_only_type())
        throw InternalErr(__FILE__, __LINE__, "Attempt to add a DAP4 type to a DAP2 Array.");
#endif
    // If 'v' is an Array, add the template instance to this object and
    // then copy the dimension information. Odd semantics; I wonder if this
    //is ever used. jhrg 6/13/12
    if (v && v->type() == dods_array_c) {
        Array *a = static_cast<Array*>(v);
        Vector::add_var(a->var());

        Dim_iter i = a->dim_begin();
        Dim_iter i_end = a->dim_end();
        while (i != i_end) {
            append_dim(a->dimension_size(i), a->dimension_name(i));
            ++i;
        }
    }
    else {
        Vector::add_var(v);
    }
}

void
Array::add_var_nocopy(BaseType *v, Part)
{
#if 0
    if (v && v->is_dap4_only_type())
        throw InternalErr(__FILE__, __LINE__, "Attempt to add a DAP4 type to a DAP2 Array.");
#endif
    // If 'v' is an Array, add the template instance to this object and
    // then copy the dimension information. Odd semantics; I wonder if this
    //is ever used. jhrg 6/13/12
    if (v && v->type() == dods_array_c) {
        Array &a = dynamic_cast<Array&>(*v);
        Vector::add_var_nocopy(a.var());
        Dim_iter i = a.dim_begin();
        Dim_iter i_end = a.dim_end();
        while (i != i_end) {
            append_dim(a.dimension_size(i), a.dimension_name(i));
            ++i;
        }
    }
    else {
        Vector::add_var_nocopy(v);
    }
}

/** Given a size and a name, this function adds a dimension to the
    array.  For example, if the Array is already 10 elements long,
    calling <tt>append_dim</tt> with a size of 5 will transform the array
    into a 10x5 matrix.  Calling it again with a size of 2 will
    create a 10x5x2 array, and so on.  This sets Vector's length
    member as a side effect.

    @param size The size of the desired new row.
    @param name The name of the new dimension.  This defaults to
    an empty string.
    @brief Add a dimension of a given size. */
void
Array::append_dim(int size, string name)
{
    dimension d;

    // This is invariant
    d.size = size;
    d.name = www2id(name);

    // this information changes with each constraint expression
    d.start = 0;
    d.stop = size - 1;
    d.stride = 1;
    d.c_size = size;

    _shape.push_back(d);

    update_length(size);
}

/** Creates a new OUTER dimension (slowest varying in rowmajor)
 * for the array by prepending rather than appending it.
 * @param size cardinality of the new dimension
 * @param name  optional name for the new dimension
 */
void
Array::prepend_dim(int size, const string& name/* = "" */)
{
  dimension d;

  // This is invariant
  d.size = size;
  d.name = www2id(name);

  // this information changes with each constraint expression
  d.start = 0;
  d.stop = size - 1;
  d.stride = 1;
  d.c_size = size;

  // Shifts the whole array, but it's tiny in general
  _shape.insert(_shape.begin(), d);

  update_length(size); // the number is ignored...
}

/** Remove all the dimensions currently set for the Array. This also
 * removes all constraint information.
 */
void
Array::clear_all_dims()
{
	_shape.clear();
}
/** Resets the dimension constraint information so that the entire
    array is selected.

    @brief Reset constraint to select entire array.
*/

void
Array::reset_constraint()
{
    set_length(-1);

    for (Dim_iter i = _shape.begin(); i != _shape.end(); i++) {
        (*i).start = 0;
        (*i).stop = (*i).size - 1;
        (*i).stride = 1;
        (*i).c_size = (*i).size;

        update_length((*i).size);
    }
}


/** Tell the Array object to clear the constraint information about
    dimensions. Do this <i>once</i> before calling <tt>add_constraint()</tt>
    for each new constraint expression. Only the dimensions explicitly
    selected using <tt>add_constraint()</tt> will be sent.

    @deprecated This should never be used.
    @brief Clears the projection; add each projected dimension explicitly using
    <tt>add_constraint</tt>.
*/
void
Array::clear_constraint()
{
    reset_constraint();
}

// Note: MS VC++ won't tolerate embedded newlines in strings, hence the \n
// is explicit.
static const char *array_sss = \
"Invalid constraint parameters: At least one of the start, stride or stop \n\
specified do not match the array variable.";

/** Once a dimension has been created (see append_dim()), it can
    be constrained.  This will make the array appear to the rest
    of the world to be smaller than it is.  This functions sets the
    projection for a dimension, and marks that dimension as part of the
    current projection.

    @note A stride value <= 0 or > the array size is an error and causes
    add_constraint to throw an Error. Similarly, start or stop values >
    size also cause an Error exception to be thrown.

    @brief Adds a constraint to an Array dimension.

    @param i An iterator pointing to the dimension in the list of
    dimensions.
    @param start The start index of the constraint.
    @param stride The stride value of the constraint.
    @param stop The stop index of the constraint.
    @exception Error Thrown if the any of values of start, stop or stride
    cannot be applied to this array. */
void
Array::add_constraint(Dim_iter i, int start, int stride, int stop)
{
    dimension &d = *i ;

    // if stop is -1, set it to the array's max element index
    // jhrg 12/20/12
    if (stop == -1)
        stop = d.size - 1;

    // Check for bad constraints.
    // Jose Garcia
    // Usually invalid data for a constraint is the user's mistake
    // because they build a wrong URL in the client side.
    if (start >= d.size || stop >= d.size || stride > d.size || stride <= 0)
        throw Error(malformed_expr, array_sss);

    if (((stop - start) / stride + 1) > d.size)
        throw Error(malformed_expr, array_sss);

    d.start = start;
    d.stop = stop;
    d.stride = stride;

    d.c_size = (stop - start) / stride + 1;

    DBG(cerr << "add_constraint: c_size = " << d.c_size << endl);

    update_length(d.c_size);
}

/** Returns an iterator to the first dimension of the Array. */
Array::Dim_iter
Array::dim_begin()
{
    return _shape.begin() ;
}

/** Returns an iterator past the last dimension of the Array. */
Array::Dim_iter
Array::dim_end()
{
    return _shape.end() ;
}

/** Return the total number of dimensions contained in the array.
    When <i>constrained</i> is TRUE, return the number of dimensions
    given the most recently evaluated constraint expression.

    @brief Return the total number of dimensions in the array.
    @param constrained A boolean flag to indicate whether the array is
    constrained or not.  Ignored.
*/

unsigned int
Array::dimensions(bool /*constrained*/)
{
#if 0
	// TODO This could be _shape.end() - _shape.begin()
    unsigned int dim = 0;
    for (Dim_citer i = _shape.begin(); i != _shape.end(); i++) {
        dim++;
    }

    return dim;
#endif

    return _shape.end() - _shape.begin();
}

/** Return the size of the array dimension referred to by <i>i</i>.
    If the dimension is constrained the constrained size is returned if
    <i>constrained</i> is \c true.

    @brief Returns the size of the dimension.

    @param i The dimension.

    @param constrained If this parameter is TRUE, the method returns the
    constrained size of the array so long as a constraint has been applied to
    this dimension. If TRUE and no constraint has been applied, this method
    returns zero. If it is FALSE, the method ignores any constraint that
    has been applied to this dimension and returns the full size of the
    dimension. The default value is FALSE.

    @return An integer containing the size of the specified dimension.
*/
int
Array::dimension_size(Dim_iter i, bool constrained)
{
    int size = 0;

    if (!_shape.empty()) {
        if (constrained)
            size = (*i).c_size;
        else
            size = (*i).size;
    }

    return size;
}

/** Use this function to return the start index of an array
    dimension.  If the array is constrained (indicated with the
    <i>constrained</i> argument), the start index of the constrained
    array is returned (or zero if the dimension in question is not
    selected at all).  See also <tt>dimension_stop()</tt> and
    <tt>dimension_stride()</tt>.

    @brief Return the start index of a dimension.

    @param i The dimension.
    @param constrained If this parameter is TRUE, the function
    returns the start index only if the dimension is constrained
    (subject to a start, stop, or stride constraint).  If
    the dimension is not constrained, the function returns zero.  If it
    is FALSE, the function returns the start index whether or not
    the dimension is constrained.
    @return The desired start index.
*/
int
Array::dimension_start(Dim_iter i, bool /*constrained*/)
{
    return (!_shape.empty()) ? (*i).start : 0;
}

/** Use this function to return the stop index of an array
    dimension.  If the array is constrained (indicated with the
    <i>constrained</i> argument), the stop index of the constrained
    array is returned (or zero if the dimension in question is not
    selected at all).  See also <tt>dimension_start()</tt> and
    <tt>dimension_stride()</tt>.

    @brief Return the stop index of the constraint.

    @param i The dimension.
    @param constrained If this parameter is TRUE, the function
    returns the stop index only if the dimension is  constrained
    (subject to a start, stop, or stride constraint).  If
    the dimension is not constrained, the function returns zero.  If it
    is FALSE, the function returns the stop index whether or not
    the dimension is constrained.
    @return The desired stop index.
*/
int
Array::dimension_stop(Dim_iter i, bool /*constrained*/)
{
    return (!_shape.empty()) ? (*i).stop : 0;
}

/** Use this function to return the stride value of an array
    dimension.  If the array is constrained (indicated with the
    <i>constrained</i> argument), the stride value of the constrained
    array is returned (or zero if the dimension in question is not
    selected at all).  See also <tt>dimension_stop()</tt> and
    <tt>dimension_start()</tt>.

    @brief Returns the stride value of the constraint.

    @param i The dimension.
    @param constrained If this parameter is TRUE, the function
    returns the stride value only if the dimension is constrained
    (subject to a start, stop, or stride constraint).  If
    the dimension is not constrained, the function returns zero.  If it
    is FALSE, the function returns the stride value whether or not
    the dimension is constrained.
    @return The stride value requested, or zero, if <i>constrained</i>
    is TRUE and the dimension is not selected.
*/
int
Array::dimension_stride(Dim_iter i, bool /*constrained*/)
{
    return (!_shape.empty()) ? (*i).stride : 0;
}

/** This function returns the name of the dimension indicated with
    <i>p</i>.  Since this method is public, it is possible to call it
    before the Array object has been properly initialized.  This will
    cause an exception.  So don't do that.

    @brief Returns the name of the specified dimension.

    @param i The dimension.
    @return A pointer to a string containing the dimension name.
*/
string
Array::dimension_name(Dim_iter i)
{
    // Jose Garcia
    // Since this method is public, it is possible for a user
    // to call it before the Array object has been properly set
    // this will cause an exception which is the user's fault.
    // (User in this context is the developer of the surrogate library.)
    if (_shape.empty())
        throw  InternalErr(__FILE__, __LINE__,
                           "*This* array has no dimensions.");
    return (*i).name;
}

/**
 * @brief Returns the width of the data, in bytes.
 * @param constrained if true, return the size of the array in bytes taking into
 * account the current constraints on various dimensions. False by default.
 * @return The number of bytes needed to store the array values.
 */
unsigned int Array::width(bool constrained)
{

	if (constrained) {
		// This preserves the original method's semantics when we ask for the
		// size of the constrained array but no constraint has been applied.
		// In this case, length will be -1. Wrong, I know...
		return length() * var()->width(constrained);
	}
	else {
		int length = 1;
		for (Dim_iter i = _shape.begin(); i != _shape.end(); i++) {
			length *= dimension_size(i, false);
		}
		return length * var()->width(false);
	}
}

/** Prints a declaration for the Array.  This is what appears in a
    DDS.  If the Array is constrained, the declaration will reflect
    the size of the Array once the constraint is applied.

    @brief Prints a DDS entry for the Array.

    @param out Write the output to this FILE *.
    @param space A string containing spaces to precede the
    declaration.
    @param print_semi A boolean indicating whether to print a
    semi-colon after the declaration.  (TRUE means ``print a
    semi-colon.'')
    @param constraint_info A boolean value.  See
    <tt>BaseType::print_decl()</tt>.
    @param constrained This argument should be TRUE if the Array is
    constrained, and FALSE otherwise.
*/
void
Array::print_decl(FILE *out, string space, bool print_semi,
                  bool constraint_info, bool constrained)
{
    ostringstream oss;
    print_decl(oss, space, print_semi, constraint_info, constrained);
    fwrite(oss.str().data(), sizeof(char), oss.str().length(), out);
}

/** Prints a declaration for the Array.  This is what appears in a
    DDS.  If the Array is constrained, the declaration will reflect
    the size of the Array once the constraint is applied.

    @brief Prints a DDS entry for the Array.

    @param out Write the output to this ostream.
    @param space A string containing spaces to precede the
    declaration.
    @param print_semi A boolean indicating whether to print a
    semi-colon after the declaration.  (TRUE means ``print a
    semi-colon.'')
    @param constraint_info A boolean value.  See
    <tt>BaseType::print_decl()</tt>.
    @param constrained This argument should be TRUE if the Array is
    constrained, and FALSE otherwise.
*/
void
Array::print_decl(ostream &out, string space, bool print_semi,
                  bool constraint_info, bool constrained)
{
    if (constrained && !send_p())
        return;

    // print it, but w/o semicolon
    var()->print_decl(out, space, false, constraint_info, constrained);

    for (Dim_citer i = _shape.begin(); i != _shape.end(); i++) {
	out << "[" ;
        if ((*i).name != "") {
	    out << id2www((*i).name) << " = " ;
        }
        if (constrained) {
	    out << (*i).c_size << "]" ;
        }
        else {
	    out << (*i).size << "]" ;
        }
    }

    if (print_semi) {
	out << ";\n" ;
    }
}

/**
 * @deprecated
 */
void
Array::print_xml(FILE *out, string space, bool constrained)
{
    XMLWriter xml(space);
    print_xml_writer_core(xml, constrained, "Array");
    fwrite(xml.get_doc(), sizeof(char), xml.get_doc_size(), out);
}

/**
 * @deprecated
 */
void
Array::print_xml(ostream &out, string space, bool constrained)
{
    XMLWriter xml(space);
    print_xml_writer_core(xml, constrained, "Array");
    out << xml.get_doc();
}

/**
 * @deprecated
 */
void
Array::print_as_map_xml(FILE *out, string space, bool constrained)
{
    XMLWriter xml(space);
    print_xml_writer_core(xml, constrained, "Map");
    fwrite(xml.get_doc(), sizeof(char), xml.get_doc_size(), out);
}

/**
 * @deprecated
 */
void
Array::print_as_map_xml(ostream &out, string space, bool constrained)
{
    XMLWriter xml(space);
    print_xml_writer_core(xml, constrained, "Map");
    out << xml.get_doc();
}

/**
 * @deprecated
 */
void
Array::print_xml_core(FILE *out, string space, bool constrained, string tag)
{
    XMLWriter xml(space);
    print_xml_writer_core(xml, constrained, tag);
    fwrite(xml.get_doc(), sizeof(char), xml.get_doc_size(), out);
}

/**
 * @deprecated
 */
void
Array::print_xml_core(ostream &out, string space, bool constrained, string tag)
{
    XMLWriter xml(space);
    print_xml_writer_core(xml, constrained, tag);
    out << xml.get_doc();
}

void
Array::print_xml_writer(XMLWriter &xml, bool constrained)
{
    print_xml_writer_core(xml, constrained, "Array");
}

void
Array::print_as_map_xml_writer(XMLWriter &xml, bool constrained)
{
    print_xml_writer_core(xml, constrained, "Map");
}

class PrintArrayDimXMLWriter : public unary_function<Array::dimension&, void>
{
    XMLWriter &xml;
    bool d_constrained;
public:
    PrintArrayDimXMLWriter(XMLWriter &xml, bool c) : xml(xml), d_constrained(c) {}

    void operator()(Array::dimension &d)
    {
        if (xmlTextWriterStartElement(xml.get_writer(), (const xmlChar*)"dimension") < 0)
            throw InternalErr(__FILE__, __LINE__, "Could not write dimension element");

        if (!d.name.empty())
            if (xmlTextWriterWriteAttribute(xml.get_writer(), (const xmlChar*) "name", (const xmlChar*)d.name.c_str()) < 0)
                throw InternalErr(__FILE__, __LINE__, "Could not write attribute for name");

        ostringstream size;
        size << (d_constrained ? d.c_size : d.size);
        if (xmlTextWriterWriteAttribute(xml.get_writer(), (const xmlChar*) "size", (const xmlChar*)size.str().c_str()) < 0)
            throw InternalErr(__FILE__, __LINE__, "Could not write attribute for name");

        if (xmlTextWriterEndElement(xml.get_writer()) < 0)
            throw InternalErr(__FILE__, __LINE__, "Could not end dimension element");
    }
};

void
Array::print_xml_writer_core(XMLWriter &xml, bool constrained, string tag)
{
    if (constrained && !send_p())
        return;

    if (xmlTextWriterStartElement(xml.get_writer(), (const xmlChar*)tag.c_str()) < 0)
        throw InternalErr(__FILE__, __LINE__, "Could not write " + tag + " element");

    if (!name().empty())
        if (xmlTextWriterWriteAttribute(xml.get_writer(), (const xmlChar*) "name", (const xmlChar*)name().c_str()) < 0)
            throw InternalErr(__FILE__, __LINE__, "Could not write attribute for name");

    get_attr_table().print_xml_writer(xml);

    BaseType *btp = var();
    string tmp_name = btp->name();
    btp->set_name("");
    btp->print_xml_writer(xml, constrained);
    btp->set_name(tmp_name);

    for_each(dim_begin(), dim_end(), PrintArrayDimXMLWriter(xml, constrained));

    if (xmlTextWriterEndElement(xml.get_writer()) < 0)
        throw InternalErr(__FILE__, __LINE__, "Could not end " + tag + " element");
}

/** Prints the values in ASCII of the entire (constrained) array. This method
    Attempts to make an aesthetically pleasing display. However, it is
    primarily intended for debugging purposes.

    @param out Write the output to this FILE *.
    @param index
    @param dims
    @param shape

    @brief Print the value given the current constraint.
*/
unsigned int
Array::print_array(FILE *out, unsigned int index, unsigned int dims,
                   unsigned int shape[])
{
    ostringstream oss;
    unsigned int i = print_array(oss, index, dims, shape);
    fwrite(oss.str().data(), sizeof(char), oss.str().length(), out);

    return i;
}

/** Prints the values in ASCII of the entire (constrained) array. This method
    Attempts to make an anesthetically pleasing display. However, it is
    primarily intended for debugging purposes.

    @param out Write the output to this ostream
    @param index
    @param dims
    @param shape

    @brief Print the value given the current constraint.
*/
unsigned int
Array::print_array(ostream &out, unsigned int index, unsigned int dims,
                   unsigned int shape[])
{
    if (dims == 1) {
	out << "{" ;
        for (unsigned i = 0; i < shape[0] - 1; ++i) {
            var(index++)->print_val(out, "", false);
	    out << ", " ;
        }
        var(index++)->print_val(out, "", false);
	out << "}" ;

        return index;
    }
    else {
	out << "{" ;
        // Fixed an off-by-one error in the following loop. Since the array
        // length is shape[dims-1]-1 *and* since we want one less dimension
        // than that, the correct limit on this loop is shape[dims-2]-1. From
        // Todd Karakasian.
        // The saga continues; the loop test should be `i < shape[0]-1'. jhrg
        // 9/12/96.
        for (unsigned i = 0; i < shape[0] - 1; ++i) {
            index = print_array(out, index, dims - 1, shape + 1);
	    out << "," ;
        }
        index = print_array(out, index, dims - 1, shape + 1);
	out << "}" ;

        return index;
    }
}

void
Array::print_val(FILE *out, string space, bool print_decl_p)
{
    ostringstream oss;
    print_val(oss, space, print_decl_p);
    fwrite(oss.str().data(), sizeof(char), oss.str().length(), out);
}

void
Array::print_val(ostream &out, string space, bool print_decl_p)
{
    // print the declaration if print decl is true.
    // for each dimension,
    //   for each element,
    //     print the array given its shape, number of dimensions.
    // Add the `;'

    if (print_decl_p) {
        print_decl(out, space, false, false, false);
	    out << " = " ;
    }

    unsigned int *shape = new unsigned int[dimensions(true)];
    unsigned int index = 0;
    for (Dim_iter i = _shape.begin(); i != _shape.end() && index < dimensions(true); ++i)
        shape[index++] = dimension_size(i, true);

    print_array(out, 0, dimensions(true), shape);

    delete [] shape; shape = 0;

    if (print_decl_p) {
	    out << ";\n" ;
    }
}

/** This function checks semantic features of the Array.  Currently,
    the only check specific to the Array is that there must be
    dimensions.  The rest is inherited from
    <tt>BaseType::check_semantics()</tt>.

    @brief Check semantic features of the Array.
    @return A boolean value.  FALSE means there was a problem.
*/

bool
Array::check_semantics(string &msg, bool)
{
    bool sem = BaseType::check_semantics(msg) && !_shape.empty();

    if (!sem)
        msg = "An array variable must have dimensions";

    return sem;
}

/** @brief dumps information about this object
 *
 * Displays the pointer value of this instance and information about this
 * instance.
 *
 * @param strm C++ i/o stream to dump the information to
 * @return void
 */
void
Array::dump(ostream &strm) const
{
    strm << DapIndent::LMarg << "Array::dump - ("
    << (void *)this << ")" << endl ;
    DapIndent::Indent() ;
    Vector::dump(strm) ;
    strm << DapIndent::LMarg << "shape:" << endl ;
    DapIndent::Indent() ;
    Dim_citer i = _shape.begin() ;
    Dim_citer ie = _shape.end() ;
    unsigned int dim_num = 0 ;
    for (; i != ie; i++) {
        strm << DapIndent::LMarg << "dimension " << dim_num++ << ":"
	     << endl ;
        DapIndent::Indent() ;
        strm << DapIndent::LMarg << "name: " << (*i).name << endl ;
        strm << DapIndent::LMarg << "size: " << (*i).size << endl ;
        strm << DapIndent::LMarg << "start: " << (*i).start << endl ;
        strm << DapIndent::LMarg << "stop: " << (*i).stop << endl ;
        strm << DapIndent::LMarg << "stride: " << (*i).stride << endl ;
        strm << DapIndent::LMarg << "constrained size: " << (*i).c_size
             << endl ;
        DapIndent::UnIndent() ;
    }
    DapIndent::UnIndent() ;
    DapIndent::UnIndent() ;
}

} // namespace libdap
