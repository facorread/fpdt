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

/** \file
 * Implementation of the calculation of the Estimated Time to Arrival or completion of a program. */

#include <iostream>
#include <fstream>
#include "chrono.h"
#include "eta.h"
#include "main.h"

std::chrono::system_clock systemClock;

void etaCls::start(const int nScenarios) {
	mNScenarios = nScenarios;
	mStartingTime = systemClock.now();
	mScenario = 0;
}

void etaCls::step() {
	std::chrono::system_clock::duration elapsedTime{systemClock.now() - mStartingTime};
	// The last time I checked, duration is in nanoseconds. I will use long double to calculate total time here, to avoid overflows on long simulations.
	long double estimatedTotalTimeD{static_cast<long double>(elapsedTime.count())};
	estimatedTotalTimeD *= mNScenarios;
	estimatedTotalTimeD /= ++mScenario;
	std::chrono::system_clock::duration estimatedTotalTime{static_cast<std::chrono::system_clock::duration::rep>(estimatedTotalTimeD)};
	mEstimatedRemainingTime = estimatedTotalTime - elapsedTime;
	mCompletedPercent = 100 * mScenario / mNScenarios;
}

void etaCls::info() const {
	std::cerr << "              \rPlagiarism reports: " << fpdt::nPlagiarismReports() << ". Submission comparison " << mScenario << "/" << mNScenarios << " " << mCompletedPercent << "%";
	if(mScenario > 10) {
		std::cerr << " ETA ";
		std::chrono::system_clock::duration remainingTimeToShow{mEstimatedRemainingTime};
		using std::chrono::duration_cast;
		typedef std::chrono::duration<int64_t, std::ratio<60 * 60 * 24 * 30>> monthsCls;
		const monthsCls months{duration_cast<monthsCls>(remainingTimeToShow)};
		if(months.count()) {
			std::cerr << months.count() << " M ";
			remainingTimeToShow -= months;
		}
		typedef std::chrono::duration<int64_t, std::ratio<60 * 60 * 24>> daysCls;
		const daysCls days{duration_cast<daysCls>(remainingTimeToShow)};
		if(days.count()) {
			std::cerr << days.count() << " d ";
			remainingTimeToShow -= days;
		}
		typedef std::chrono::duration<int64_t, std::ratio<60 * 60>> hoursCls;
		const hoursCls hours{std::chrono::duration_cast<std::chrono::hours>(remainingTimeToShow)};
		if(hours.count()) {
			std::cerr << hours.count() << " h ";
			remainingTimeToShow -= hours;
		}
		typedef std::chrono::duration<int64_t, std::ratio<60> > minutesCls;
		const minutesCls minutes{duration_cast<minutesCls>(remainingTimeToShow)};
		if(hours.count() || minutes.count()) {
			std::cerr << minutes.count() << " m ";
			if(minutes.count()) {
				remainingTimeToShow -= minutes;
			}
		}
		typedef std::chrono::seconds secondsCls;
		const secondsCls seconds{duration_cast<secondsCls>(remainingTimeToShow)};
		std::cerr << seconds.count() << " s"; // Always print;
	}
}

void etaCls::printDescription() const {
	std::chrono::system_clock::time_point endTime{systemClock.now()};
	std::chrono::system_clock::duration diffTime{endTime - mStartingTime};
	std::cerr << "Execution time: ";
	print(std::cerr, diffTime, timeOutputFormatEnum::units);
	std::cerr << "\n";
	const time_t endt{systemClock.to_time_t(endTime)};
	std::cerr << "\tEnd time: " << ctime(&endt);
}
