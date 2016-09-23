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

#ifndef STUDENTCLS_H
#define STUDENTCLS_H

#include "diagnostics.h"
#include "hash.h"
#include <list>
#include <string>
#include <vector>

namespace fpdt {

/// Abstraction of a student and his submissions
class studentCls {
	public:
		/// Sets the student name.
		void setStudentName(const std::string& studentname) { mStudentName = studentname; }
		/// Opens the file and extracts the text into memory.
		void add(const std::string& submissionFileName);
		/// Returns the name of the file.
		const std::string& studentName() const _wur_ { return mStudentName; }
		/// Organizes the hashes for fast comparisons.
		void organize();
		/// Removes text from questions and guides.
		void removeQuestionsAndOrganize(const studentCls& questionsDocument);
		/// Returns whether no text was found in `this` student's submissions.
		bool noTextFound() const _wur_ { return mHashes.empty(); }
		/// Searches for plagiarized phrases.
		void searchPlagiarism(const studentCls& otherStudent) const;
	private:
		/// Typedef for the hashes
		typedef stringHashCls::result_type phraseHashCls;
		/// Typedef for the unordered hashes of each and every sentence in the student's submissions.
		typedef std::list<phraseHashCls> unorderedHashesCls;
		/// Typedef for the ordered, unique hashes of each and every sentence in the student's submissions.
		typedef std::vector<phraseHashCls> orderedHashesCls;
		/// Contains the name of the student.
		std::string mStudentName;
		/// Unordered hashes of each and every sentence in the student's submissions.
		unorderedHashesCls mUnorderedHashes;
		/// Ordered, unique hashes of each and every sentence in the student's submissions.
		orderedHashesCls mHashes;
};

}

#endif // STUDENTCLS_H
