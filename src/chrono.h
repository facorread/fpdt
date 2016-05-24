/* This file is part of swiddenFarming: multi-agent, social-network-based simulation of swidden farming
 * Copyright (C) 2015 - 2016 Fabio Correa <fabio5@umd.edu>, Sean Downey <sdowney2@umd.edu>
 *
 * You can use this file according to the license agreement contained in file license.md
 */

#ifndef CHRONO_H
#define CHRONO_H

/** \file
	* Contains functions to work with both simulation time and computer time. */

#include <array>
#include <chrono>
#include "diagnostics.h"

/// Extensions to std::chrono
namespace fpdt {
	// We will use the type unsigned int for our years; this allows us to express up to 136.193 years in seconds, which shouldn't be necessary.
	typedef unsigned int timeRepresentation;
	// We use duration classes instead of time_point because the simulation doesn't need an epoch. Besides, duration has more useful operations like %
	typedef std::chrono::duration<timeRepresentation> seconds;
	typedef std::chrono::duration<timeRepresentation, std::ratio<60>> minutes;
	typedef std::chrono::duration<timeRepresentation, std::ratio<3600>> hours;
	typedef std::chrono::duration<timeRepresentation, std::ratio_multiply<hours::period, std::ratio<24>>> days;
	typedef std::chrono::duration<timeRepresentation, std::ratio_multiply<days::period, std::ratio<7>>> weeks;
	typedef std::chrono::duration<timeRepresentation, std::ratio_multiply<days::period, std::ratio<30>>> months;
	typedef std::chrono::duration<timeRepresentation, std::ratio_multiply<days::period, std::ratio<365>>> years;
}

/// Specifies the format of a time output.
enum class timeOutputFormatEnum {
	// Time output in simulation intervals, no units.
	intervals,
	// Time output in units: seconds, minutes, etc.
	units
};

#include <iosfwd>

/// Writes a time to a standard output stream. \param [out] out Output stream. \param [in] time Time to be printed out. \param [in] format Output time format.
void print(std::ostream& out, std::chrono::system_clock::duration time, const timeOutputFormatEnum format);

#endif // CHRONO_H
