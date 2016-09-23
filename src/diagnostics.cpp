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

#include "diagnostics.h"
#include <fstream>
#include <iostream>

/** \file
	* Contains algorithms to diagnose problems with the program. */

namespace fpdt {

void checkPointer(const void * const p) {
	assertGreater(p, reinterpret_cast<void*>(0x10000)) << "Pointer is invalid. Please debug.\n";
}

/// @cond
class diagnosticsTestCls {
	public:
		diagnosticsTestCls() {
			checkPointer(this);
			multiAssertClsTest();
			assertCls1Test();
			//assertCls2Test(); simDiag << "Tests passed.\n"; std::exit(0);
		}
		void assertCls1Test() {
			fpdtAssert(1 == 1) << "Error message." << '\n';
			constexpr int varZero{0};
			assertZero(varZero) << "Error message." << '\n';
			constexpr int varNonZero{1};
			assertNonZero(varNonZero) << "Error message." << '\n';
			const void* varNull{nullptr};
			assertNull(varNull) << "Error message." << '\n';
			const void* varNonNull{this};
			assertNonNull(varNonNull) << "Error message." << '\n';
			constexpr bool varFalse{false};
			assertFalse(varFalse) << "Error message." << '\n';
			const bool varTrue{true};
			assertTrue(varTrue) << "Error message." << '\n';
			constexpr int varEqual{1};
			assertEqual(varEqual, 1) << "Error message." << '\n';
			constexpr int varDifferent{2};
			assertDifferent(varDifferent, 1) << "Error message." << '\n';
			constexpr int varGreaterEqual{1};
			assertGreaterEqual(varGreaterEqual, 1) << "Error message." << '\n';
			constexpr int varGreater{2};
			assertGreater(varGreater, 1) << "Error message." << '\n';
			constexpr int varLessEqual{1};
			assertLessEqual(varLessEqual, 1) << "Error message." << '\n';
			constexpr int varLess{0};
			assertLess(varLess, 1) << "Error message." << '\n';
			std::ifstream varifstream("CMakeCache.txt");
			assertfstream(varifstream);
		}
		void assertCls2Test() { simDiag << "Diagnostics message.\n"; }
		void multiAssertClsTest() {
			multiAssertCls multiAssert;
			multiAssert(1 == 1) << "Diag 1.\n";
			multiAssert(2 == 2) << "Diag 2.\n";
			multiAssert(3 == 3) << "Diag 3.\n";
			multiAssert.evaluate();
		}
};

#ifdef DEBUG
diagnosticsTestCls diagnosticsTest;
#endif // DEBUG

/// @endcond
} // namespace std
