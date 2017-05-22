#include "Vector2D.h"
#include "stdafx.h"

Vector2D::Vector2D() { 
	x = 0;
	y = 0;
}

Vector2D::Vector2D( float xInit, float yInit ) { 
	x = xInit;
	y = yInit;
}

void Vector2D::set( Vector2D& v ) {
	x = v.x;
	y = v.y;
}

void Vector2D::set( float xs, float ys ) {
	x = xs;
	y = ys;
}

Vector2D Vector2D::clone() {
	return Vector2D( x, y );
}

void Vector2D::zero() {
	x = 0;
	y = 0;	
}

void Vector2D::add( Vector2D& v ) {
	x += v.x;
	y += v.y;
}

void Vector2D::subtract( Vector2D& v ) {
	x -= v.x;
	y -= v.y;
}

void Vector2D::multiply( float s ) {
	x *= s;
	y *= s;
}

void Vector2D::divide( float d ) {
	x /= d;
	y /= d;	
}

void Vector2D::normalize() {
	float length = sqrt(x*x+y*y);
	if (length == 0) {
		y = 0; x = 0;
		length = 1;
	}
	x /= length;
	y /= length;
}

float Vector2D::getLength() {	
	float temp = sqrt(x*x + y*y);
	return temp;
}

void Vector2D::setLength( float l ) {
	float length = sqrt( x*x+y*y );
    if( length == 0 ) y = 1;
	x = (x / length) * l;
	y = (y / length) * l;
}

float Vector2D::getAngle() {
	return atan2( y, x );
}

float Vector2D::getAngleDeg() {
	return atan2(y,x) * 57.2957;
}

void Vector2D::setAngle( float a ) {
	float length = sqrt(x*x+y*y);
	x = cos(a)*length;
	y = sin(a)*length;
}

void Vector2D::setAngleDeg( float a ) {
	float length = sqrt(x*x+y*y);
	a *= 0.0174532925;
	x = cos(a)*length;
	y = sin(a)*length;
}

void Vector2D::rotateBy( float l ) {
	float angle = getAngle();
	float length = sqrt(x*x+y*y);
	x = cos(l+angle)*length;
	y = sin(l+angle)*length;
}

float Vector2D::dot( Vector2D& v ) {
	return x * v.x + y * v.y;
}

float Vector2D::cross( Vector2D& v ) {
	return (x * v.y) - (y * v.x);
}

float Vector2D::distanceTo( Vector2D v ) {
	Vector2D distanceVector = Vector2D(v.x - x, v.y - y );
	return distanceVector.getLength();
}	

Vector2D Vector2D::vectorTo( Vector2D v ) {
	return Vector2D(v.x - x, v.y - y );
}	

void Vector2D::lookAt( Vector2D& v ) {
	Vector2D *vectorToTarget = new Vector2D( v.x - x, v.y - y  );
	setAngle( vectorToTarget->getAngle() );
}


void Vector2D::reflect( Vector2D v ) {
    Vector2D unitNormal = v.unit();
    *this = *this - ( unitNormal * ( (unitNormal.dot( *this ) ) * 2 ) );
}

Vector2D Vector2D::unit() {
    Vector2D unit = this->clone();
    unit.normalize();
    return unit;
}

void Vector2D::invert() {
    x = -x;
    y = -y; 
}


//OPERATORS

Vector2D Vector2D::operator+ ( const Vector2D& v ) const {
	return Vector2D( x + v.x, y + v.y );	
}

Vector2D Vector2D::operator- ( const Vector2D& v ) const {
	return Vector2D( x - v.x, y - v.y );	
}

Vector2D Vector2D::operator* ( const float s ) const {
	return Vector2D( x * s, y * s );
}

//dot product
float Vector2D::operator* ( const Vector2D& v ) const {
    return (x * v.x + y * v.y);
}

//cross product	
float Vector2D::operator^ ( const Vector2D& v ) const {
	return (x * v.y) - (y * v.x);
}

Vector2D Vector2D::operator/ ( const float s ) const {
	return Vector2D( x / s, y / s );
}

Vector2D Vector2D::operator/ ( const Vector2D& v ) const {
	return Vector2D( x / v.x, y / v.y );
}

bool Vector2D::operator== ( const Vector2D& v ) const {
	return ( x == v.x && y == v.y );
}

void Vector2D::operator= ( const Vector2D& v ) {
	x = v.x;
	y = v.y;
}

void Vector2D::operator+= ( const Vector2D& v ) {
	x += v.x;
	y += v.y;	
}

void Vector2D::operator-= ( const Vector2D& v ) {
	x -= v.x;
	y -= v.y;	
}

void Vector2D::operator*= ( const float s ) {
	x *= s;
	y *= s;
}

void Vector2D::operator/= ( const float s ) {
	x /= s;
	y /= s;
}

float Vector2D::operator[] ( int i ) {
	if ( i == 0 ) return x;
    else return y;
}

void Vector2D::print( bool includeEndline ) {
	if( includeEndline ) printf( "%s%f%s%f%s", "[" , x , ",", y , "]\n" );
    else printf( "%s%f%s%f%s", "[" , x , ",", y , "]" );
}

//convenience///////////////

Vector2D lgv(float x, float y) {
    return Vector2D(x,y);    
}
