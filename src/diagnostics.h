/*

This file is part of fpdt: Fabio's plagiarism detection tool
Copyright (C) 2016 - 2017 Fabio Correa <facorread@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

/** \file
	* Contains algorithms to diagnose problems with the program. */

#include <cstddef>

/// Wrapper for std::cerr. Saves compile time by removing <iostream> from diagnostics.h.
class errorMsgWrapperCls {
	public:
		/// Outputs an error message to std::cerr. Returns a reference to \c this. \param [in] var Error message or value.
		template<typename varT> errorMsgWrapperCls& operator<<(const varT var);
};

// Explicit template instantiations to save on compilation time.
extern template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const int var);
extern template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const unsigned int var);
extern template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const bool var);
extern template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const double var);
extern template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const size_t var);
extern template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const char* var);
/// Wrapper for std::cerr. Saves compile time by removing <iostream> from diagnostics.h.
extern errorMsgWrapperCls errorMsgWrapper;

#define errorMsg errorMsgWrapper << __FILE__ << " " << __LINE__ << " " << __PRETTY_FUNCTION__ << "() "

/// \def _wur_
/// \brief Warns about unused values that are returned by functions.
/// \details _wur_ applies to GCC only.
#ifdef __GNUG__
#define _wur_ __attribute__ (( warn_unused_result ))
#else
#define _wur_
#endif

/// Checks that a pointer has a valid memory address.
void checkPointer(void * const p);

#endif
