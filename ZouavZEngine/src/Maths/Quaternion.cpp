#include "Maths/Quaternion.hpp"
#define _USE_MATH_DEFINES
#include <cmath>
using namespace std;

Quaternion::Quaternion()
{
	w = x = y = z = 0;
}

Quaternion::Quaternion(const Vec3& _eulerAngles)
{
	float cosx = cosf(_eulerAngles.x / 2),
	      cosy = cosf(_eulerAngles.y / 2),
	      cosz = cosf(_eulerAngles.z / 2),
	      sinx = sinf(_eulerAngles.x / 2),
	      siny = sinf(_eulerAngles.y / 2),
	      sinz = sinf(_eulerAngles.z / 2);

	//w = cosx * cosy * cosz - sinx * siny * sinz;
	//x = sinx * cosy * cosz + cosx * siny * sinz;
	//y = cosx * siny * cosz + sinx * cosy * sinz;
	//z = cosx * cosy * sinz - sinx * siny * cosz;

	w = cosx * cosy * cosz + sinx * siny * sinz;
	x = sinx * cosy * cosz - cosx * siny * sinz;
	y = cosx * siny * cosz + sinx * cosy * sinz;
	z = cosx * cosy * sinz - sinx * siny * cosz;

	Normalise();
}

Quaternion::Quaternion(const Vec4& _v)
{
	w = _v.w;
	x = _v.x;
	y = _v.y;
	z = _v.z;
}

Quaternion::Quaternion(const Quaternion& _q)
{
	w = _q.w;
	x = _q.x;
	y = _q.y;
	z = _q.z;
}

Quaternion::Quaternion(const Vec3& _v, float _f)
{
	w = _f;
	x = _v.x;
	y = _v.y;
	z = _v.z;
}

Quaternion::Quaternion(float _w, float _x, float _y, float _z)
{
	w = _w;
	x = _x;
	y = _y;
	z = _z;
}

void Quaternion::Mult(const Quaternion& _q2)
{
	float tempw = w;
	float tempx = x;
	float tempy = y;
	float tempz = z;

	w = -tempx * _q2.x - tempy * _q2.y - tempz * _q2.z + tempw * _q2.w;
	x = tempx * _q2.w + tempy * _q2.z - tempz * _q2.y + tempw * _q2.x;
	y = -tempx * _q2.z + tempy * _q2.w + tempz * _q2.x + tempw * _q2.y;
	z = tempx * _q2.y - tempy * _q2.x + tempz * _q2.w + tempw * _q2.z;
}

Quaternion Quaternion::Mult(const Quaternion& _q2) const
{
	return Quaternion(	-x * _q2.x - y * _q2.y - z * _q2.z + w * _q2.w,
						x * _q2.w + y * _q2.z - z * _q2.y + w * _q2.x,
						-x * _q2.z + y * _q2.w + z * _q2.x + w * _q2.y,
						x * _q2.y - y * _q2.x + z * _q2.w + w * _q2.z);
}

void Quaternion::Add(const Quaternion& _q2)
{
	x = x + _q2.x;
	y = y + _q2.y;
	z = z + _q2.z;
	w = w + _q2.w;
}

Quaternion Quaternion::Add(const Quaternion& _q2) const
{
	return Quaternion(w + _q2.w, x + _q2.x, y + _q2.y, z + _q2.z);
}

float Quaternion::DotProduct(const Quaternion& _q1, const Quaternion& _q2)
{
    return _q1.w * _q2.w + _q1.x * _q2.x + _q1.y * _q2.y + _q1.z * _q2.z;
}

void Quaternion::Neg(const Quaternion& _q2)
{
	x = x - _q2.x;
	y = y - _q2.y;
	z = z - _q2.z;
	w = w - _q2.w;
}

Quaternion Quaternion::Neg(const Quaternion& _q2) const
{
	return Quaternion(w - _q2.w, x - _q2.x, y - _q2.y, z - _q2.z);
}

void Quaternion::Scale(float s)
{
	w *= s;
	x *= s;
	y *= s;
	z *= s;
}

const Quaternion Quaternion::Scale(float _s) const
{
	return Quaternion(w * _s, x * _s, y * _s, z * _s);
}

void Quaternion::Normalise()
{
	float u = sqrtf(x * x + y * y + z * z + w * w);
	x /= u;
	y /= u;
	z /= u;
	w /= u;
}

Quaternion Quaternion::Normalised() const
{
	float u = sqrtf(x * x + y * y + z * z + w * w);
	return Quaternion(w / u, x / u, y / u,	z / u);
}

void Quaternion::Conjugate()
{
	x = -x;
	y = -y;
	z = -z;
}

const Quaternion Quaternion::Conjugate() const
{
	return Quaternion(w, -x, -y, -z);
}

float Quaternion::Norm() const
{
	return w * w + x * x + y * y + z * z;
}

float Quaternion::Length() const
{
	return sqrtf(Norm());
}

float Quaternion::SquaredLength() const
{
	return Norm();
}

void Quaternion::Inverse()
{
	Conjugate();
	Scale(1 / Norm());
}

Quaternion Quaternion::Inversed() const
{
	return Conjugate().Scale(1 / Norm());
}

float Quaternion::GetAngle() const
{
    return acosf(w) * 2.f;
}

Vec3 Quaternion::GetAxis() const
{
    const Vec3 xyz(x, y, z);
    return  xyz / sinf(GetAngle() / 2.f);
}

Vec3 Quaternion::ToEuler() const
{
	Vec3 angles;

	float sqw = w * w;
	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = x * y + z * w;
	if (test > 0.499f * unit) { // singularity at north pole
		angles.x = 1.57079632f;
		angles.y = 2.0f * atan2(x, w);
		angles.z = 0.0f;
		return angles * (180.0f / 3.14159265f);
	}
	if (test < -0.499f * unit) { // singularity at south pole
		angles.x = -1.57079632f;
		angles.y = -2.0f * atan2(x, w);
		angles.z = 0.0f;
		return angles * (180.0f / 3.14159265f);
	}
	angles.x = asinf(2.0f * test / unit);
	angles.y = atan2f(2.0f * y * w - 2.0f * x * z, sqx - sqy - sqz + sqw);
	angles.z = atan2f(2.0f * x * w - 2.0f * y * z, -sqx + sqy - sqz + sqw);

	return angles * (180.0f / 3.14159265f);
}

Vec3 Quaternion::RotateVector(const Vec3& _vec) const
{
    const float angle = GetAngle();
    const Vec3 unitAxis = GetAxis();

    const float cosAngle = cosf(angle);
    return cosAngle * _vec + (1.f - cosAngle) * _vec.Dot(unitAxis) * unitAxis + sin(angle) * unitAxis.Cross(_vec);
}

Mat4 Quaternion::GetRotationMatrix() const
{
    const float twoXX = 2.f * x * x;
    const float twoXY = 2.f * x * y;
    const float twoXZ = 2.f * x * z;
    const float twoXW = 2.f * x * w;
    const float twoYY = 2.f * y * y;
    const float twoYZ = 2.f * y * z;
    const float twoYW = 2.f * y * w;
    const float twoZZ = 2.f * z * z;
    const float twoZW = 2.f * z * w;

    const Vec3 vec1(1.f - twoYY - twoZZ, twoXY + twoZW, twoXZ - twoYW);
    const Vec3 vec2(twoXY - twoZW, 1.f - twoXX - twoZZ, twoYZ + twoXW);
    const Vec3 vec3(twoXZ + twoYW, twoYZ - twoXW, 1.f - twoXX - twoYY);

    Mat4 rst({	vec1.x, vec2.x, vec3.x, 0.0f,
				vec1.y, vec2.y, vec3.y, 0.0f,
				vec1.z, vec2.z, vec3.z, 0.0f,
				0.0f,	0.0f,	0.0f,	1.0f});

    return rst / SquaredLength();
}

Quaternion Quaternion::Lerp(const Quaternion& _q1, const Quaternion& _q2, float _t)
{
    return _q1 * (1.0f - _t) + _q2 * _t;
}

Quaternion Quaternion::SLerp(const Quaternion& _q1, const Quaternion& _q2, float _t)
{
	float dot = DotProduct(_q1, _q2);

    Quaternion tempQ1 = _q1;

    if (dot < 0.f)
        tempQ1 = -tempQ1;

	if (abs(dot) >= 1.f)
		return _q2;

    float omega = acosf(abs(dot));

	if (abs(sinf(omega)) < 0.00001f)
		return _q2;
	
	Quaternion toreturn = (tempQ1 * sinf((1.f - _t) * omega) + _q2 * sinf(_t * omega)) / sinf(omega);

	//std::cout << dot << "	" << abs(dot) << " " << omega << " " << sinf(omega) << " " << toreturn.ToString() << std::endl;

    return toreturn;
}

Quaternion Quaternion::NLerp(const Quaternion& _q1, const Quaternion& _q2, float _t)
{
    return Quaternion::Lerp(_q1, _q2, _t).Normalised();
}

string Quaternion::ToString() const
{
	return "[ " + to_string(w) + " ; " + to_string(x) + " ; " + to_string(y) + " ; " + to_string(z) + " ]";
}