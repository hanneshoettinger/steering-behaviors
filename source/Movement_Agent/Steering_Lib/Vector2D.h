#pragma once

#ifndef _Vector2D_
#define _Vector2D_

class Vector2D {
    
	public :
    
	float x, y;
	Vector2D();
	Vector2D( float xInit, float yInit );
    
	void zero();
	void set( Vector2D& v );
	void set( float xs, float ys );
	Vector2D clone();
    
	void setAngle( float a );
	void setAngleDeg( float a );	
	float getAngle();
	float getAngleDeg();
	void rotateBy( float l );	
    
	void add( Vector2D& v );
	void subtract( Vector2D& v );
	void multiply( float s );
	void divide( float s );
    
	void normalize();
	void setLength( float l );
	float getLength();
    
	float dot( Vector2D& v );
	float cross( Vector2D& v );
	float distanceTo( Vector2D v );
	Vector2D vectorTo( Vector2D v );	
	void lookAt( Vector2D& v );
    void reflect( Vector2D v );
    Vector2D unit();
    void invert();
    
	void print( bool includeEndline = true );
    
	Vector2D operator+ ( const Vector2D& v ) const;
	Vector2D operator- ( const Vector2D& v ) const;
	Vector2D operator* ( const float s ) const;	
	float operator* ( const Vector2D& v ) const; //dot product
	float operator^ ( const Vector2D& v ) const; //cross product	
	Vector2D operator/ ( const float s ) const;
	Vector2D operator/ ( const Vector2D& v ) const;
	bool operator== ( const Vector2D& v ) const;
	void operator= ( const Vector2D& v );
	void operator+= ( const Vector2D& v );
	void operator-= ( const Vector2D& v );
	void operator*= ( const float s );	
	void operator/= ( const float s );
	float operator[] ( int i );
    
};

Vector2D lgv(float x, float y);

#endif