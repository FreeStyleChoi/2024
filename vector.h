#pragma once

#include <math.h>
#include <stdio.h>

struct Vector
{
	// The elements of Vector
	double x, y;

	// Reset Vector
	Vector& Reset();

	// Vector v1(this) = Vector v1(this) + Vector v2
	Vector& Add(const Vector& v2);

	// 
	Vector& Substract(Vector& v2);

	//
	Vector& Multiply(const int& scale);

	//
	Vector& Divide(const int& scale);

	//
	double GetLength();

	//
	void Print();

	//
	friend Vector& operator+(Vector& v1, const Vector& v2);
};
