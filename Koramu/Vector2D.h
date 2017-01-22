#pragma once

#include <math.h>

/*	Einen Vektor benutzen wir um z.B. die Geschwindigkeit oder Beschleunigung eines Objektes zu beschreiben.
*	(Aber auch als Ortsvektor)
*/

class Vector2D
{
private:
	float m_xComponent;					//	Die beiden Komponenten (x | y)
	float m_yComponent;

public:
	Vector2D(float, float);				//	Konstruktor
	~Vector2D();						//	Destruktor

	Vector2D normalize();				//	Den Einheitsvektor erhalten

	//	Überladene Operatoren
	Vector2D operator+(const Vector2D&) const;				//	Arithmetischer Operator +
	Vector2D operator-(const Vector2D&) const;				//	Arithmetischer Operator -
	Vector2D operator*(const float) const;					//	Multiplikation mit einem Skalar (Variation in Länge)
	float operator*(const Vector2D&) const;					//	Skalarprodukt
	Vector2D& operator+=(const Vector2D&);					//	Zuweisungsoperator +=
	Vector2D& operator-=(const Vector2D&);					//	Zuweisungsoperator -=
	Vector2D& operator*=(const float);						//	Zuweisungsoperator *= (Skalarmultiplikation)
	Vector2D& operator=(const Vector2D&);					//	Zuweisungsoperator =

	//	getter-Funktionen
	float getX() const { return m_xComponent; }
	float getY() const { return m_yComponent; }
	float getLength() const;

	//	setter-Funnktionen
	void setX(float x) { m_xComponent = x; }
	void setY(float y) { m_yComponent = y; }
};