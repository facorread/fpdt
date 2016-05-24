# This file is part of fpdt: Fabio's plagiarism detection tool
# Copyright (C) 2016 - 2017 Fabio Correa <facorread@gmail.com>

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

project(fptd CXX)

add_executable(
fpdt
chrono.cpp
diagnostics.cpp
eta.cpp
filecls.cpp
listoffiles.cpp
main.cpp
phrasecls.cpp
)

set(CMAKE_CXX_FLAGS "--pedantic-errors -Wall -Wextra -Werror -ftabstop=4 -march=native -pipe -std=gnu++14 -fabi-version=8 -Wfloat-equal")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -ggdb3 -DDEBUG")

# Debug statements
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
