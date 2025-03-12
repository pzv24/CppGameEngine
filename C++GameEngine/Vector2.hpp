#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>
#include<iostream>

template <typename T>
struct Vector2
{
	T x = 0;
	T y = 0;


	Vector2() = default;

	Vector2(T xin, T yin)
		: x(xin), y(yin)
	{}

	//constructor from sf::Vector2 to local Vector2
	Vector2(const sf::Vector2<T>& vec)
		: x(vec.x), y(vec.y)
	{}

	 //conversion from sf::Vector2 to Vector2
	operator sf::Vector2<T>()
	{
		return sf::Vector2<T>(x, y);
	}

	//vector simple operations
	Vector2 operator + (const Vector2& other) const
	{
		return Vector2(other.x + x, other.y + y);
	}
	Vector2 operator - (const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}
	Vector2 operator / (const T val) const
	{
		return Vector2(x / val, y / val);
	}
	Vector2 operator * (const T val) const
	{
		return Vector2(x * val, y * val);
	}

	//vector operations
	float magnitude() const
	{
		return (float)sqrt((x * x) + (y * y));
	}
	float dot(const Vector2& other) const
	{
		return (x * other.x + y * other.y);
	}

	//scaling
	void operator += (const Vector2& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator *= (const T val)
	{
		x *= val;
		y *= val;
	}
	void operator /= (const T val)
	{
		x /= val;
		y /= val;
	}

	// returns a copy of the current vector with a magnitude of one
	Vector2 normalized() const
	{
		Vector2 output(x,y);
		float mag = magnitude();
		if (mag > 0.0f)
		{
			output /= mag;
		}
		else
		{
			output = Vector2(0, 0);
		}
		return output;
	}

	//distance
	float distance(const Vector2& other) const
	{
		float xDif = other.x - x;
		float yDif = other.y - y;
		return sqrt((xDif * xDif) + (yDif * yDif));
	}
	float sqrdDistance(const Vector2& other) const
	{
		float xDif = other.x - x;
		float yDif = other.y - y;
		return (xDif * xDif) + (yDif * yDif);
	}

	//utility
	void print() const
	{
		std::cout << "(" << x << ", " << y << ")" << std::endl;
	}
	friend std::ostream& operator<<(std::ostream& out, const Vector2<T>& vec)
	{
		return out << "(" << vec.x << ", " << vec.y << ")";
	}

};
