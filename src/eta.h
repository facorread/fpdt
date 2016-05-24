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

#ifndef ETA_H
#define ETA_H

/** \file
 * Facility for the calculation of the Estimated Time to Arrival or completion of a program. */

/// \addtogroup facilities Facilities

#include "diagnostics.h"
#include <chrono>

class etaCls;

/// \brief Class to calculate the Estimated Time to Arrival or completion of the program. \ingroup facilities
/// \details This class receives information of each big step in the run of a program, namely the evaluation of a simulation scenario, and based on the runtime it calculates the ETA of the program.
class etaCls {
	private:
		/// Total number of scenarios reported by the program.
		int mNScenarios;
		/// Current scenario number.
		int mScenario;
		/// Starting time of the program.
		std::chrono::system_clock::time_point mStartingTime;
		/// Time duration from current scenario until program completion.
		std::chrono::system_clock::duration mEstimatedRemainingTime;
		/// Currently completed scenario percentage.
		int mCompletedPercent;
	public:
		/// Initializes the object with the current time of program start. \param [in] nScenarios Number of scenarios that the program intends to run.
		void start(const int nScenarios);
		/// Returns the starting time.
		const std::chrono::system_clock::time_point& startingTime() const _wur_ { return mStartingTime; }
		/// Reports a simulation step in the program, to calculate the ETA.
		void step();
		/// Prints the ETA info to \c std::cerr.
		void info() const;
		/// Returns the scenario number that the program is running.
		int scenario() const _wur_ { return mScenario; }
		/// Returns the number of scenarios that the program reported.
		int nScenarios() const _wur_ { return mNScenarios; }
		/// Prints the execution times to the outfile.
		void printDescription() const;
};

#endif // ETA_H
