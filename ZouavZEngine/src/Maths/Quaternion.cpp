#include "Maths/Quaternion.hpp"

using namespace std;

Quaternion::Quaternion()
{
	w = x = y = z = 0;
}

Quaternion::Quaternion(const Vec3& eulerAngles)
{
	float cosx = cosf(eulerAngles.x / 2),
	      cosy = cosf(eulerAngles.y / 2),
	      cosz = cosf(eulerAngles.z / 2),
	      sinx = sinf(eulerAngles.x / 2),
	      siny = sinf(eulerAngles.y / 2),
	      sinz = sinf(eulerAngles.z / 2);

	w = cosx * cosy * cosz - sinx * siny * sinz;
	x = sinx * cosy * cosz + cosx * siny * sinz;
	y = cosx * siny * cosz + sinx * cosy * sinz;
	z = cosx * cosy * sinz - sinx * siny * cosz;
}

Quaternion::Quaternion(float _w, float _x, float _y, float _z)
{
	w = _w;
	x = _x;
	y = _y;
	z = _z;
}

void Quaternion::Mult(const Quaternion& q2)
{
	float tempw = w;
	float tempx = x;
	float tempy = y;
	float tempz = z;

	w = -tempx * q2.x - tempy * q2.y - tempz * q2.z + tempw * q2.w;
	x = tempx * q2.w + tempy * q2.z - tempz * q2.y + tempw * q2.x;
	y = -tempx * q2.z + tempy * q2.w + tempz * q2.x + tempw * q2.y;
	z = tempx * q2.y - tempy * q2.x + tempz * q2.w + tempw * q2.z;
}

Quaternion Quaternion::Mult(const Quaternion& q2) const
{
	return Quaternion(	-x * q2.x - y * q2.y - z * q2.z + w * q2.w,
						x * q2.w + y * q2.z - z * q2.y + w * q2.x,
						-x * q2.z + y * q2.w + z * q2.x + w * q2.y,
						x * q2.y - y * q2.x + z * q2.w + w * q2.z);
}

void Quaternion::Add(const Quaternion& q2)
{
	x = x + q2.x;
	y = y + q2.y;
	z = z + q2.z;
	w = w + q2.w;
}

Quaternion Quaternion::Add(const Quaternion& q2) const
{
	return Quaternion(w + q2.w, x + q2.x, y + q2.y, z + q2.z);
}

float Quaternion::DotProduct(const Quaternion& q1, const Quaternion& q2)
{
    return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

void Quaternion::Neg(const Quaternion& q2)
{
	x = x - q2.x;
	y = y - q2.y;
	z = z - q2.z;
	w = w - q2.w;
}

Quaternion Quaternion::Neg(const Quaternion& q2) const
{
	return Quaternion(w - q2.w, x - q2.x, y - q2.y, z - q2.z);
}

void Quaternion::Scale(float s)
{
	w *= s;
	x *= s;
	y *= s;
	z *= s;
}

const Quaternion Quaternion::Scale(float s) const
{
	return Quaternion(w * s, x * s, y * s, z * s);
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

Quaternion Quaternion::Inverse() const
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

Vec3 Quaternion::RotateVector(const Vec3& vec) const
{
    const float angle = GetAngle();
    const Vec3 unitAxis = GetAxis();

    const float cosAngle = cosf(angle);
    return cosAngle * vec + (1.f - cosAngle) * vec.Dot(unitAxis) * unitAxis + sin(angle) * unitAxis.Cross(vec);
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

Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    return q1 * (1.0f - t) + q2 * t;
}

Quaternion Quaternion::SLerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float dot = DotProduct(q1, q2);

    Quaternion tempQ1 = q1;

    if (dot < 0.f)
        tempQ1 = -tempQ1;

	if (abs(dot) >= 1.f)
		return q2;

    float omega = acosf(abs(dot));

	if (abs(sinf(omega)) < 0.00001f)
		return q2;
	
	Quaternion toreturn = (tempQ1 * sinf((1.f - t) * omega) + q2 * sinf(t * omega)) / sinf(omega);

	//std::cout << dot << "	" << abs(dot) << " " << omega << " " << sinf(omega) << " " << toreturn.ToString() << std::endl;

    return toreturn;
}

Quaternion Quaternion::NLerp(const Quaternion& q1, const Quaternion& q2, float t)
{
    return Quaternion::Lerp(q1, q2, t).Normalised();
}

string Quaternion::ToString() const
{
	return "[ " + to_string(w) + " ; " + to_string(x) + " ; " + to_string(y) + " ; " + to_string(z) + " ]";
}