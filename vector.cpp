#include "vector.h"

Vector& Vector::Reset()
{
	this->x = 0;
	this->y = 0;

	return *this;
}

Vector& Vector::Add(const Vector& v2)
{
	this->x += v2.x;
	this->y += v2.y;

	return *this;
}

Vector& Vector::Substract(Vector& v2)
{
	this->x -= v2.x;
	this->y -= v2.y;

	return *this;
}

Vector& Vector::Multiply(const int& scale)
{
	this->x *= scale;
	this->y *= scale;

	return *this;
}

Vector& Vector::Divide(const int& scale)
{
	if (scale == 0)
	{
		printf("ERROR: You Tried To Devide By \"ZERO\"\n");
		return *this;
	}
	this->x /= scale;
	this->y /= scale;

	return *this;
}

double Vector::GetLength()
{
	return sqrt((this->x * this->x) + (this->y * this->y));
}

void Vector::Print()
{
	printf("(x : %.4lf) (y: %.4lf)\n", this->x, this->y);
}