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
	class fileCls;
	/// Represents a phrase that is being compared between two documents.
	class phraseCls {
		public:
			/// Constructor.
			phraseCls(const std::string& phrase, const fileCls& file1, const fileCls& file2) : mPhrase{phrase}, pFile1{&file1}, pFile2{&file2} {}
			/// Comparison operator
			bool operator>(const phraseCls& rhs) const _wur_ { return mPhrase.length() > rhs.mPhrase.length(); }
			/// Prints the plagiarized phrase.
			void print() const;
		private:
			/// Phrase.
			std::string mPhrase;
			/// File containing the phrase.
			const fileCls* pFile1;
			/// File containing the phrase.
			const fileCls* pFile2;
	};

}

#endif // PHRASECLS_H
