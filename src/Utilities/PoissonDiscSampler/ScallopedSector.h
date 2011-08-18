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

// $Id: ScallopedSector.h,v 1.3 2006/07/06 23:30:24 zr Exp $

#include <vector>

typedef struct {
	Vec2 P;
	float r, sign, d, theta, integralAtStart;
	float rSqrd, dSqrd;
} ArcData;

class ScallopedSector
{
public:
	Vec2 P;
	float a1, a2, area;
	
	ArcData arcs[2];

public:
	ScallopedSector(Vec2 &_Pt, float _a1, float _a2, Vec2 &P1, float r1, float sign1, Vec2 &P2, float r2, float sign2);

	float calcAreaToAngle(float angle);
	float calcAngleForArea(float area, RNG &rng);
	Vec2 sample(RNG &rng);

	float distToCurve(float angle, int index);

	void subtractDisk(Vec2 &C, float r, std::vector<ScallopedSector> *regions);

private:
	float canonizeAngle(float angle);

	void distToCircle(float angle, Vec2 &C, float r, float *d1_out, float *d2_out);
};

class ScallopedRegion
{
public:
	std::vector<ScallopedSector> *regions;
	float minArea;
	float area;

public:
	ScallopedRegion(Vec2 &P, float r1, float r2, float minArea=.00000001);
	~ScallopedRegion();

	bool isEmpty() { return regions->size()==0; }
	void subtractDisk(Vec2 C, float r);

	Vec2 sample(RNG &rng);
};
