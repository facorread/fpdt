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

#include <fstream>
#include "hash.h"
#include <iostream>
#include "phrasecls.h"
#include "studentcls.h"

namespace fpdt {
	std::ofstream outFile("fpdt.txt");

	class longerStringCls {
		public:
			bool operator()(const std::string* a, const std::string* b) { return a->length() > b->length(); }
	};

	void fpdt::phraseCls::print() const {
		outFile << "Plagiarism detected: " << mStudent1 << '\t' << mStudent2 << '\n';
		std::list<const std::string*> phrases;
		for(const matchedHashesCls::value_type& hash : mMatchedHashes)
			phrases.emplace_back(&(phraseFromHash(hash)));
		phrases.sort(longerStringCls{});
		for(const std::string* s : phrases)
			outFile << *s << '\n';
		outFile << "\n\n";
	}

	void phraseClsTidyUp() {
		outFile.close();
	}

} // namespace fpdt
