/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
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

// $Id: WeightedDiscretePDF.h,v 1.4 2006/07/07 05:54:31 zr Exp $

template <class T>
class WDPDF_Node
{
private:
	bool m_mark;

public:
	WDPDF_Node<T> *parent, *left, *right;
	T key;
	float weight, sumWeights;

public:
	WDPDF_Node(T key_, float weight_, WDPDF_Node<T> *parent_);
	~WDPDF_Node();

	WDPDF_Node<T> *sibling() { return this==parent->left?parent->right:parent->left; }

	void markRed() { m_mark = true; }
	void markBlack() { m_mark = false; }
	bool isRed() { return m_mark; }
	bool isBlack() { return !m_mark; }
	bool leftIsBlack() { return !left || left->isBlack(); }
	bool rightIsBlack() { return !right || right->isBlack(); }
	bool leftIsRed() { return !leftIsBlack(); }
	bool rightIsRed() { return !rightIsBlack(); }
	void setSum() { sumWeights = weight + (left?left->sumWeights:0) + (right?right->sumWeights:0); }
};

template <class T>
class WeightedDiscretePDF
{
private:
	WDPDF_Node<T> *m_root;

public:
	WeightedDiscretePDF();
	~WeightedDiscretePDF();

	void insert(T item, float weight);
	void update(T item, float newWeight);
	void remove(T item);
	bool inTree(T item);

		/* pick a tree element according to its
		 * weight. p should be in [0,1).
		 */
	T choose(float p);

private:
	WDPDF_Node<T> **lookup(T item, WDPDF_Node<T> **parent_out);
	void split(WDPDF_Node<T> *node);
	void rotate(WDPDF_Node<T> *node);
	void lengthen(WDPDF_Node<T> *node);
	void propogateSumsUp(WDPDF_Node<T> *n);
};

#include "WeightedDiscretePDF.hpp"
