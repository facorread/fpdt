/* This file is part of swiddenFarming: multi-agent, social-network-based simulation of swidden farming
 * Copyright (C) 2015 - 2016 Fabio Correa <fabio5@umd.edu>, Sean Downey <sdowney2@umd.edu>
 *
 * You can use this file according to the license agreement contained in file license.md
 */

#include "chrono.h"
#include <iostream>

/** \file
	* Contains functions to work with the time. */

constexpr char space{' '};

template<class timeT, class timeToShowT> void print(std::ostream& out, bool& prependSpace, timeT& t, const std::string& units) {
	const timeToShowT timeToShow{std::chrono::duration_cast<timeToShowT>(t)};
	if(timeToShow.count()) {
		if(prependSpace)
			out << space;
		else
			prependSpace = true;
		out << timeToShow.count() << " " << units; // Prepend spaces if necessary;
		if(timeToShow.count() > 1)
			out << "s"; // Neat plural;
		t -= std::chrono::duration_cast<timeT>(timeToShow);
	}
}

void print(std::ostream& out, std::chrono::system_clock::duration time, const timeOutputFormatEnum format) {
	if(format == timeOutputFormatEnum::intervals) {
		out << time.count();
		return;
	}
	typedef std::chrono::system_clock::duration timeT;
	bool prependSpace{false};
	print<timeT, fpdt::years>(out, prependSpace, time, "year");
	print<timeT, fpdt::months>(out, prependSpace, time, "month");
	print<timeT, fpdt::days>(out, prependSpace, time, "day");
	print<timeT, fpdt::hours>(out, prependSpace, time, "hour");
	print<timeT, fpdt::minutes>(out, prependSpace, time, "minute");
	print<timeT, fpdt::seconds>(out, prependSpace, time, "second");
}
