/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id: RangeList.h,v 1.3 2006/01/12 16:39:19 zr Exp $

#include <vector>

typedef struct _RangeEntry {
	float min, max;
} RangeEntry;

class RangeList {
public:
	RangeEntry *ranges;
	int numRanges, rangesSize;

public:
	RangeList(float min, float max);
	~RangeList();

	void reset(float min, float max);

	void print();

	void subtract(float min, float max);

private:
	void deleteRange(int pos);
	void insertRange(int pos, float min, float max);
};
