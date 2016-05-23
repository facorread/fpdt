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
#include "filecls.h"

namespace fpdt {

	static const std::string wordXMLfilename{"word/document.xml"};
	static const std::string excelXMLfilename{"xl/workbook.xml"};

	/// Extracts the Word document or Excel workbook XML from a docx/xlsx file.
	const std::string& extractXML(const std::string& filename) {
		std::system(std::string("rm -f " + wordXMLfilename + " " + excelXMLfilename).c_str());
		// unzip returns 0 on successful extraction
		if(std::system(std::string("{ unzip -l '" + filename + "' | grep -q " + wordXMLfilename + "; } && unzip -qq '" + filename + "' " + wordXMLfilename).c_str())) {
			if(std::system(std::string("{ unzip -l '" + filename + "' | grep -q " + excelXMLfilename + "; } && unzip -qq '" + filename + "' " + excelXMLfilename).c_str())) {
				errorMsg << "Error extracting information from " << filename.c_str() << ": not a word/excel file.\n";
				std::abort();
			}
			return excelXMLfilename;
		}
		return wordXMLfilename;
	}

	/// Returns whether a character should be considered as valid content
	bool isContent(const char inputChar) {
		static const std::locale loc;
		return std::isalnum(inputChar, loc) || std::ispunct(inputChar, loc);
	}

	fileCls::fileCls(const std::string& filename) {
		std::ifstream file(extractXML(filename));
		// Used to transform all nonPrinting characters into only one whitespace
		bool skippingWhitespace{false};
		// Skipping an XML tag
		bool skippingTag{false};
		while(true) {
			char inputChar;
			file.get(inputChar);
			if(!file.good())
				break;
			if(skippingTag) {
				if(inputChar == '>') {
					skippingTag = false;
					if(!skippingWhitespace) {
						mContents += ' ';
						skippingWhitespace = true;
					}
				}
				continue;
			}
			if(inputChar == '<') {
				skippingTag = true;
				continue;
			}
			if(isContent(inputChar)) {
				if(skippingWhitespace)
					skippingWhitespace = false; // continue below;
				mContents += inputChar;
			} else if(!skippingWhitespace) {
				mContents += ' ';
				skippingWhitespace = true;
			}
		}
	}

	char fileCls::nextChar() {
		// mContents.length() is optimized;
		if(mPosition < mContents.length())
			return mContents[mPosition++];
		else
			return 0;
	}

	void fileCls::removeQuestion() {
		if(mPosition > mComparisonStart + 1)
			mContents.erase(mComparisonStart, mPosition - 1 - mComparisonStart);
		else {
			errorMsg << "Attempt to erase an invalid portion of a string, at positions [" << mComparisonStart << ", " << mPosition - 1 << "). Please debug.\n";
			std::abort();
		}
	}

#ifdef DEBUG
	// Debug class
	class debugCls {
		public:
			debugCls() {
				const std::string testOutputFileName{"fileClsTestOutput.txt"};
				std::ofstream testOutput(testOutputFileName);
				testOutput << questionsDocument.contents() << '\n' << questionsSpreadsheet.contents() << '\n';
				errorMsg << "Please check the output of " << testOutputFileName.c_str() << ".\n";
			}
		private:
			fileCls questionsDocument{"../test/Samplequestionsdocument.docx"};
			fileCls questionsSpreadsheet{"../test/Sample questions spreadsheet.xlsx"};
	};

	debugCls debug;
#endif // DEBUG

}
