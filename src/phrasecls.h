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

#ifndef PHRASECLS_H
#define PHRASECLS_H

#include "diagnostics.h"
#include "hash.h"
#include <list>
#include <string>
#include "studentcls.h"
#include <vector>

namespace fpdt {
	// Forward declaration.
	class studentCls;
	/// Typedef for a list of hashes
	typedef std::list<stringHashCls::result_type> matchedHashesCls;
	/// Represents a phrase that is being compared between two documents.
	class phraseCls {
		public:
			/// Constructor.
			phraseCls(matchedHashesCls&& matchedHashes, const studentCls& file1, const studentCls& file2) : mMatchedHashes{std::move(matchedHashes)}, mStudent1{file1.studentName()}, mStudent2{file2.studentName()} {}
			/// Comparison operator
			bool operator>(const phraseCls& rhs) const _wur_ { return mMatchedHashes.size() > rhs.mMatchedHashes.size(); }
			/// Prints the plagiarized phrase.
			void print() const;
		private:
			/// Number of phrases matched between the students.
			matchedHashesCls mMatchedHashes;
			/// Student that wrote the phrase.
			std::string mStudent1;
			/// Student that wrote the phrase.
			std::string mStudent2;
	};

}

#endif // PHRASECLS_H
