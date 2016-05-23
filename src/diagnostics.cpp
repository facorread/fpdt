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
#include <iostream>
#include <cstdlib>

/** \file
	* Contains algorithms to diagnose problems with the program. */

template<typename varT> errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const varT var) {
	std::cerr << var;
	return *this;
}

void checkPointer(void* const p) {
	if(p < reinterpret_cast<void*>(0x10000))
		std::abort();
	if(p > reinterpret_cast<void*>(0xfffffff))
		std::abort();
}

template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const int var);
template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const bool var);
template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const double var);
template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const size_t var);
template errorMsgWrapperCls& errorMsgWrapperCls::operator<<(const char* var);
errorMsgWrapperCls errorMsgWrapper;

