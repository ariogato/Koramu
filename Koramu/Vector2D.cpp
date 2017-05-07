#include "Vector2D.h"

Vector2D::Vector2D(float x, float y)		//	Konstruktor
	: m_xComponent(x), m_yComponent(y)		//	Liste von Elementinitialisierungen
{
}

Vector2D::Vector2D(const Vector2D& v)		//	Kopierkonstruktor
{
	this->m_xComponent = v.getX();
	this->m_yComponent = v.getY();
}

Vector2D::~Vector2D()						//	Destruktor
{}

Vector2D Vector2D::normalize()
{
	/*	Die Formel für den Einheitsvektor e von v ist:
	*		e = v / |v|
	*/

	return Vector2D(	this->getX() / this->getLength(),
						this->getY() / this->getLength());
}

//	CONSIDER: Kommentare für Rechnungen

Vector2D Vector2D::operator+(const Vector2D& summand) const
{
	return Vector2D(	this->getX() + summand.getX(),
						this->getY() + summand.getY());
}

Vector2D Vector2D::operator-(const Vector2D& subtrahend) const
{
	return Vector2D(	this->getX() - subtrahend.getX(),
						this->getY() - subtrahend.getY());
}

Vector2D Vector2D::operator*(const float scalar) const
{
	return Vector2D(	this->getX() * scalar,
						this->getY() * scalar);
}

float Vector2D::operator*(const Vector2D& vector2) const
{
	return	(this->getX() * vector2.getX())
			+ (this->getY() * vector2.getY());
}

Vector2D& Vector2D::operator+=(const Vector2D& summand)
{
	this->m_xComponent += summand.getX();
	this->m_yComponent += summand.getY();

	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& subtrahend)
{
	this->m_xComponent -= subtrahend.getX();
	this->m_yComponent -= subtrahend.getY();

	return *this;
}

Vector2D& Vector2D::operator*=(const float scalar)
{
	this->m_xComponent *= scalar;
	this->m_yComponent *= scalar;

	return *this;
}

Vector2D& Vector2D::operator=(const Vector2D& assign)
{
	this->m_xComponent = assign.getX();
	this->m_yComponent = assign.getY();

	return *this;
}

float Vector2D::getLength() const
{
	return sqrtf(m_xComponent * m_xComponent + m_yComponent * m_yComponent);
}
