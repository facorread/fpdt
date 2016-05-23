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

#ifndef FILECLS_H
#define FILECLS_H

#include "diagnostics.h"
#include <string>

namespace fpdt {

/// Abstraction of a student submission
class fileCls {
	public:
		/// Opens the file and extracts the text into memory.
		fileCls(const std::string& filename);
		/// Returns the name of the file.
		const std::string& fileName() const _wur_ { return mFileName; }
		/// Returns the full text that was extracted from the file.
		const std::string& contents() const _wur_ { return mContents; }
		/// Removes text from questions and guides.
		void removeQuestions(const fileCls& questionsDocument);
		/// Searches for plagiarized phrases.
		void searchPlagiarism(const fileCls& otherAssignment);
	private:
		/// Contains the name of the file.
		std::string mFileName;
		/// Contains the full text that was extracted from the file.
		std::string mContents{};
		/// Contains the position of the start of a string comparison.
		mutable std::string::size_type mComparisonStart{0};
		/// Position within mContents
		mutable std::string::size_type mPosition{0};
		/// Resets the file position pointers
		void reset() const { mComparisonStart = 0; mPosition = 0; }
		/// Moves the pointers in order to start the next comparison.
		bool nextComparisonInviable() const _wur_;
		/// Restarts the current comparison.
		void restartComparison() const { mPosition = mComparisonStart; }
		/// Returns the char at the current position, then advances mPosition. Returns 0 if it at the end of the document.
		char nextChar() const _wur_;
		/// Removes the portion of text in [mComparisonStart, mContents - 1). Used te remove the text of a question.
		void removeQuestion();
};

}

#endif // FILECLS_H
