#pragma once

#include <string>
#include "Maths/Vec3.hpp"
#include "Maths/Mat4.hpp"


class Quaternion
{
public:

	float w;
	float x;
	float y;
	float z;

	Quaternion();
	Quaternion(const Vec3& eulerAngles);
	Quaternion(float _w, float _x, float _y, float _z);

	void Mult(const Quaternion& q2);
	Quaternion Mult(const Quaternion& q2) const;

	static float DotProduct(const Quaternion& q1, const Quaternion& q2);

	void Add(const Quaternion& q2);
	Quaternion Add(const Quaternion& q2) const;

	void Neg(const Quaternion& q2);
	Quaternion Neg(const Quaternion& q2) const;

	void Scale(float);
	const Quaternion Scale(float) const;

	void Normalise();
	Quaternion Normalised() const;

	void Conjugate();
	const Quaternion Conjugate() const;

	float GetAngle() const;

	Vec3 GetAxis() const;

	Vec3 RotateVector(const Vec3& vec) const;

	Mat4 GetRotationMatrix() const;

	float Norm() const;

	float Length() const;
	float SquaredLength() const;

	void Inverse();
	Quaternion Inversed() const;

    static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
    static Quaternion SLerp(const Quaternion& q1, const Quaternion& q2, float t);
    static Quaternion NLerp(const Quaternion& q1, const Quaternion& q2, float t);

	std::string ToString() const;

	Quaternion operator*(const Quaternion q2) const
	{
		return Mult(q2);
	}
	Quaternion operator+(const Quaternion& q2) const
	{
		return Add(q2);
	}
	Quaternion operator*(const float f) const
	{
		return Scale(f);
	}
	Quaternion operator/(const float f) const
	{
		return Quaternion(w / f, x / f, y / f, z / f);
	}
	Quaternion operator-() const
	{
		return Quaternion(-w, -x, -y, -z);
	}
};

