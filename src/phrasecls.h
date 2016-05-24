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
#include <string>

namespace fpdt {
	// Forward declaration.
	class studentSubmissionsCls;
	/// Represents a phrase that is being compared between two documents.
	class phraseCls {
		public:
			/// Constructor.
			phraseCls(const std::string& phrase, const studentSubmissionsCls& file1, const studentSubmissionsCls& file2) : mPhrase{phrase}, mStudent1{file1.studentName()}, mStudent2{file2.studentName()} {}
			/// Comparison operator
			bool operator>(const phraseCls& rhs) const _wur_ { return mPhrase.length() > rhs.mPhrase.length(); }
			/// Prints the plagiarized phrase.
			void print() const;
		private:
			/// Phrase.
			std::string mPhrase;
			/// Student that wrote the phrase.
			std::string mStudent1;
			/// Student that wrote the phrase.
			std::string mStudent2;
	};

}

#endif // PHRASECLS_H
