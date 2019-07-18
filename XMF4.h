#pragma once

using namespace DirectX;
#include <sstream>
#include <string>

struct XMF4 : XMFLOAT4 {
public:
	XMF4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {

	};
	XMF4() {

	};

	std::string str() {
		std::stringstream ss;
		ss << "{" << x << ", " << y << ", " << z << ", " << w << "}";
		return ss.str();
	};

	float * asArr() {
		float arr[4] = { x, y, z, w };
		return arr;
	}

	// I'm still a little unclear on w so I'm just not touching it

	// scalar multiplication
	XMF4 operator * (float a)
	{
		return XMF4(x*a, y*a, z*a, w);
	}

	// scalar division
	XMF4 operator / (float a)
	{
		return XMF4(x / a, y / a, z / a, w);
	}

	float operator * (XMF4 a)
	{
		return x * a.x + y * a.y + z * a.z + w * a.w;
	}

	// unholy math
	XMF4 operator + (float a)
	{
		return XMF4(x + a, y + a, z + a, w);
	}
	XMF4 operator - (float a)
	{
		return XMF4(x - a, y - a, z - a, w);
	}

	// vector addition
	XMF4 operator + (XMF4 a)
	{
		return XMF4(x + a.x, y + a.y, z + a.z, w);
	}

	XMF4 operator - (XMF4 a)
	{
		return XMF4(x - a.x, y - a.y, z - a.z, w);
	}

	bool operator >= (float a)
	{
		return x >= a && y > a && z > a;
	}

	bool operator >= (XMF4 a)
	{
		return x >= a.x && y > a.y && z > a.z;
	}

	bool operator <= (float a)
	{
		return x < a && y < a && z < a;
	}

	bool operator <= (XMF4 a)
	{
		return x <= a.x && y < a.y && z < a.z;
	}
};

struct XMF4x4 :XMFLOAT4X4 {
	float *addresses[4][4];

	XMF4x4(float m00, float m01, float m02, float m03,
		   float m10, float m11, float m12, float m13,
		   float m20, float m21, float m22, float m23,
		   float m30, float m31, float m32, float m33) : XMFLOAT4X4( m00,  m01,  m02,  m03,
																	  m10,  m11,  m12,  m13,
																	  m20,  m21,  m22,  m23,
																	  m30,  m31,  m32,  m33) {
		// sorry in advance
		_11 = m00;
		_12 = m01;
		_13 = m02;
		_14 = m03;
		
		_21 = m10;
		_22 = m11;
		_23 = m12;
		_24 = m13;

		_31 = m20;
		_32 = m21;
		_33 = m22;
		_34 = m23;
		
		_41 = m30;
		_42 = m31;
		_43 = m32;
		_44 = m33;
		
		float * _addresses[4][4] = { {&_11, &_12, &_13, &_14}, { &_21, &_22, &_23, &_24 }, { &_31, &_32, &_33, &_34 }, { &_41, &_42, &_43, &_44 } };
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				addresses[i][j] = _addresses[i][j];
			}
		}
	};

	XMF4 operator * (XMF4 a)
	{
		return XMF4(row(1) * a, row(2) * a, row(3) * a, row(4) * a);
	}

	std::string str() {
		return row(1).str() + row(2).str() + row(3).str() + row(4).str();
	};

	XMF4 row(int index) {
		if (index < 5 && index > 0) {
			return  XMF4(*addresses[index - 1][0], *addresses[index - 1][1], *addresses[index - 1][2], *addresses[index - 1][3]);
		}
		return XMF4(NULL, NULL, NULL, NULL);
	}


	XMF4 col(int index) {
		if (index < 5 && index > 0) {
			return  XMF4(*addresses[0][index - 1], *addresses[1][index - 1], *addresses[2][index - 1], *addresses[3][index - 1]);
		}
		return XMF4(NULL, NULL, NULL, NULL);
	}

	// this is very ugly -> revise later
	void setCol(int index, XMF4 newCol) {
		float * arr = newCol.asArr();
		if (index < 5 && index > 0) {
			for (int i = 0; i < 4; i++) {
				*addresses[i][index - 1] = arr[i];
			}
		}
	}

	static XMF4x4 identity() {
		return XMF4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}
};
