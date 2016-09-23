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

#include <iostream>
#include <cstddef>

namespace fpdt {
/// Strongly-typed version of the DEBUG instruction
#ifdef DEBUG
constexpr bool debugProgram{true};
#else
constexpr bool debugProgram{false};
#endif // DEBUG

/// Handling assertions and debugging code with pretty function printing and std::abort.
/** This enhances the funcionality of C/C++ `assert`. */
class assertCls {
	public:
		/// Destructor aborts.
		~assertCls() { std::abort(); }
		/// Outputs an error message to std::cerr. Returns a reference to `this`. @param [in] var Error message or value.
		template<typename varT> assertCls const& operator<<(const varT& var) const {
			std::cerr << var;
			return *this;
		}
};

/// Handling multiple related assertions before aborting a program.
/** This enhances the funcionality of C/C++ `assert`. */
class multiAssertCls {
	public:
		/// Aborts depending on the assertions.
		void evaluate() const {
			if(!mSuccess)
				std::abort();
		}
		/// Destructor aborts depending on the assertions.
		~multiAssertCls() {
			evaluate();
		}
		/// Outputs an error message to std::cerr. Returns a reference to `this`. @param [in] var Error message or value.
		template<typename varT> multiAssertCls& operator<<(const varT& var) {
			if(mPrint)
				std::cerr << var;
			return *this;
		}
		/// Makes a new assertion.
		multiAssertCls& operator()(const bool assertion) {
			mPrint = !assertion;
			if(mSuccess && mPrint)
				mSuccess = false;
			return *this;
		}
	private:
		/// Whether to print error messages
		bool mPrint{false};
		/// Whether any of the assertions has failed
		bool mSuccess{true};
};

/// Handling assertions and debugging code with pretty function printing and std::abort.
#define fpdtAssert(expr) if(!(expr)) assertCls() << __FILE__ << " " << __LINE__ << " " << __PRETTY_FUNCTION__ << " "

#define fpdtAbort fpdtAssert(false)

#define assertZero(expr) fpdtAssert(!(expr)) << "assertZero(" << #expr << ") : " << expr << ". "

#define assertNonZero(expr) fpdtAssert((expr)) << "assertNonZero(" << #expr << ") : " << expr << ". "

#define assertNull(expr) fpdtAssert(!(expr)) << "assertNull(" << #expr << ") : " << expr << ". "

#define assertNonNull(expr) fpdtAssert((expr)) << "assertNonNull(" << #expr << ") : " << expr << ". "

#define assertFalse(expr) fpdtAssert(!(expr)) << "assertFalse(" << #expr << ") : " << expr << ". "

#define assertTrue(expr) fpdtAssert((expr)) << "assertTrue(" << #expr << ") : " << expr << ". "

#define assertEqual(expr, expected) fpdtAssert((expr) == (expected)) << "assert(" << #expr << " == " << #expected << ") : " << expr << " == " << expected << ". "

#define assertDifferent(expr, expected) fpdtAssert((expr) != (expected)) << "assert(" << #expr << " != " << #expected << ") : " << expr << " != " << expected << ". "

#define assertGreaterEqual(expr, expected) fpdtAssert((expr) >= (expected)) << "assert(" << #expr << " >= " << #expected << ") : " << expr << " >= " << expected << ". "

#define assertGreater(expr, expected) fpdtAssert((expr) > (expected)) << "assert(" << #expr << " > " << #expected << ") : " << expr << " > " << expected << ". "

#define assertLessEqual(expr, expected) fpdtAssert((expr) <= (expected)) << "assert(" << #expr << " <= " << #expected << ") : " << expr << " <= " << expected << ". "

#define assertLess(expr, expected) fpdtAssert((expr) < (expected)) << "assert(" << #expr << " < " << #expected << ") : " << expr << " < " << expected << ". "

#define simDiag std::cerr << __FILE__ << " " << __LINE__ << " " << __PRETTY_FUNCTION__ << " "

#define assertfstream(ifile) fpdtAssert(ifile.is_open() && ifile.good()) << #ifile << ": is_open " << ifile.is_open() << ", bad " << ifile.bad() << ", eof " << ifile.eof() << ", fail " << ifile.fail() << ". "

/// \def _wur_
/// \brief Warns about unused values that are returned by functions.
/// \details _wur_ applies to GCC only.
#ifdef __GNUG__
#define _wur_ __attribute__ (( warn_unused_result ))
#else
#define _wur_
#endif

/// Checks that a pointer has a valid memory address.
void checkPointer(const void * const p);
} // namespace std
#endif
