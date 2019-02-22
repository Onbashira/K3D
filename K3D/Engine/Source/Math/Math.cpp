#include "Math.h"
#include "Engine/Source/Utility/Utility.h"
#include <chrono>
#include <xmmintrin.h>
#include <immintrin.h>
#include <zmmintrin.h>
#include <DirectXMath.h>

float K3D::DegToRad(float deg)
{
	return deg * (F_PI / 180.f);
}

float K3D::RadToDeg(float rad)
{
	return rad * (180.f / F_PI);
}

bool K3D::IsZero(float value)
{
	return fabs(value) <= F_EPSILON;
}

bool K3D::IsZero(double value)
{
	return abs(value) <= D_EPSILON;
}

bool K3D::IsEqual(float a, float b)
{
	return fabs(a - b) <= F_EPSILON;
}

bool K3D::IsEqual(double a, double b)
{
	return abs(a - b) <= D_EPSILON;
}

bool K3D::IsNan(float value)
{
	return (value != value);
}

bool K3D::IsInf(float value)
{
	unsigned int f = *reinterpret_cast<unsigned int*>(&value);
	if (((f & 0x7e000000) == 0x7e000000) && (value == value))
	{
		return true;
	}
	return false;
}

unsigned int K3D::Fact(unsigned int value)
{
	unsigned int result = 1;
	for (unsigned int i = 1; i <= value; ++i)
	{
		result *= i;
	}
	return result;
}

unsigned int K3D::DoubleFact(unsigned int value)
{
	unsigned int result = 1;
	unsigned int start = ((value % 2) == 0) ? 2 : 1;
	for (unsigned int i = start; i <= value; i += 2)
	{
		result *= i;
	}
	return result;
}

float K3D::Bernstein(unsigned int n, unsigned int i, float t)
{
	return static_cast<float>(Binormal(n, i)*std::powf(t, static_cast<float>(i))*std::powf(1.0f - t, static_cast<float>(n - i)));
}

float K3D::Binormal(unsigned int n, unsigned int k)
{
	return static_cast<float>(Fact(n) / (Fact(k)*(Fact(n - k))));
}

unsigned int K3D::Perm(unsigned int n, unsigned int r)
{
	assert(n >= r);
	return Fact(n) / Fact(n - r);
}

unsigned int K3D::Comb(unsigned int n, unsigned int r)
{
	assert(n >= r);
	return Fact(n) / (Fact(n - r) * Fact(r));
}

float K3D::QuickSqrt(float value)
{

	union {
		float f;
		unsigned int i;
	}conv;

	float x2 = 0;
	float threehalf = 1.5f;

	x2 = value * 0.5f;
	conv.f = value;
	conv.i = 0x5f3759f - (conv.i >> 1);
	conv.f = conv.f*(threehalf - x2 * (conv.f * conv.f));

	return conv.f;
}

float K3D::Fresnel(float n1, float n2, float cosTheta)
{
	auto a = n1 + n2;
	auto b = n1 - n2;
	auto R = (a * a) / (b * b);
	return R + (1.0f - R) * powf(1.0f - cosTheta, 5.0f);
}

double K3D::Fresnel(double n1, double n2, double cosTheta)
{
	auto a = n1 + n2;
	auto b = n1 - n2;
	auto R = (a * a) / (b * b);
	return R + (1.0 - R) * pow(1.0f - cosTheta, 5.0);
}

float K3D::Lerp(float a, float b, float amount)
{
	return a - amount * (a - b);
}

double K3D::Lerp(double a, double b, double amount)
{
	return a - amount * (a - b);
}

///////////////////////////////////////////////
//
//
//
//
///////////////////////////////////////////////

const K3D::Vector2 K3D::Vector2::up(0.0f, 1.0f);
const K3D::Vector2 K3D::Vector2::left(-1.0f, 0.0f);
const K3D::Vector2 K3D::Vector2::down(0.0f, -1.0f);
const K3D::Vector2 K3D::Vector2::right(1.0f, 0.0f);
const K3D::Vector2 K3D::Vector2::zero(0.0f, 0.0f);
const K3D::Vector2 K3D::Vector2::one(1.0f, 1.0f);

K3D::Vector2::Vector2() :x(), y()
{
}

K3D::Vector2::Vector2(const float x, const float y) : x(x), y(y)
{
}

K3D::Vector2::Vector2(const K3D::Vector2 & other) : x(other.x), y(other.y)
{
}

K3D::Vector2::Vector2(K3D::Vector2 && other)
{
	*this = std::move(other);
}

K3D::Vector2::~Vector2()
{
}

K3D::Vector2 & K3D::Vector2::operator=(const K3D::Vector2 & other)
{
	x = other.x;
	y = other.y;
	return *this;
}

K3D::Vector2 &  K3D::Vector2::operator=(K3D::Vector2 && value)
{
	*this = value;
	value.x = 0.0f;
	value.y = 0.0f;

	return *this;
}

K3D::Vector2 & K3D::Vector2::operator+=(const K3D::Vector2 & value)
{
	x += value.x;
	y += value.y;
	return *this;
}

K3D::Vector2 & K3D::Vector2::operator-=(const K3D::Vector2 & value)
{
	x -= value.x;
	y -= value.y;
	return *this;
}

K3D::Vector2 & K3D::Vector2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

K3D::Vector2 & K3D::Vector2::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

K3D::Vector2 K3D::Vector2::operator+() const
{
	return *this;
}

K3D::Vector2 K3D::Vector2::operator-() const
{
	return K3D::Vector2(-x, -y);
}

K3D::Vector2 K3D::Vector2::operator+(const K3D::Vector2 & value) const
{
	return K3D::Vector2(this->x + value.x, this->y + value.y);
}

K3D::Vector2 K3D::Vector2::operator-(const K3D::Vector2 & value) const
{
	return K3D::Vector2(this->x - value.x, this->y - value.y);
}

K3D::Vector2 K3D::Vector2::operator*(float scalar)
{
	return K3D::Vector2(this->x * scalar, this->y * scalar);
}

K3D::Vector2  K3D::operator*(float scalar, const K3D::Vector2 & value)
{
	return K3D::Vector2(value.x * scalar, value.y * scalar);
}

K3D::Vector2 K3D::Vector2::operator/(float scalar)
{
	assert(fabs(scalar) == 0);
	return K3D::Vector2(this->x / scalar, this->y / scalar);
}

bool K3D::Vector2::operator==(const K3D::Vector2 & value) const
{
	return IsEqual(x, value.x) && IsEqual(y, value.y);
}

bool K3D::Vector2::operator!=(const K3D::Vector2 & value) const
{
	return !IsEqual(x, value.x) || !IsEqual(y, value.y);
}

float K3D::Vector2::Length() const
{
	return std::sqrtf(x*x + y * y);
}

float K3D::Vector2::LengthSqr() const
{
	return (x*x + y * y);
}

K3D::Vector2 & K3D::Vector2::Normalize()
{
	auto magnitude = Length();
	assert(magnitude > 0.0f);
	x /= magnitude;
	y /= magnitude;
	return (*this);
}

K3D::Vector2 & K3D::Vector2::SafeNormalize(const K3D::Vector2 & value)
{
	auto magnitude = Length();
	if (magnitude > 0.0f)
	{
		x /= magnitude;
		y /= magnitude;
		return (*this);
	}

	x = value.x;
	y = value.y;
	return (*this);
}

K3D::Vector2 K3D::Vector2::Abs(const K3D::Vector2 & value)
{
	return K3D::Vector2(fabs(value.x), fabs(value.y));
}

void K3D::Vector2::Abs(const K3D::Vector2 & value, K3D::Vector2 & result)
{
	result.x = fabs(value.x);
	result.y = fabs(value.y);
}

K3D::Vector2 K3D::Vector2::Clamp(const K3D::Vector2 & value, const K3D::Vector2 & min, const K3D::Vector2 & max)
{
	return K3D::Vector2(::Clamp(value.x, min.x, max.x), ::Clamp(value.y, min.y, max.y));
}

void K3D::Vector2::Clamp(const K3D::Vector2 & value, const K3D::Vector2 & min, const K3D::Vector2 & max, K3D::Vector2 & result)
{
	result.x = ::Clamp(value.x, min.x, max.x);
	result.y = ::Clamp(value.y, min.y, max.y);
}

K3D::Vector2 K3D::Vector2::Saturate(const K3D::Vector2 & value)
{
	return K3D::Vector2(::Saturate(value.x), ::Saturate(value.y));
}

void K3D::Vector2::Saturate(const K3D::Vector2 & value, K3D::Vector2 & result)
{
	result.x = ::Saturate(value.x);
	result.y = ::Saturate(value.y);
}

float K3D::Vector2::Distance(const K3D::Vector2 & a, const K3D::Vector2 & b)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	return std::sqrtf(X * X + Y * Y);
}

void K3D::Vector2::Distance(const K3D::Vector2 & a, const K3D::Vector2 & b, float & result)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	result = std::sqrtf(X * X + Y * Y);
}

float K3D::Vector2::DistanceSqr(const K3D::Vector2 & a, const K3D::Vector2 & b)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	return X * X + Y * Y;
}

void K3D::Vector2::DistanceSqr(const K3D::Vector2 & a, const K3D::Vector2 & b, float & result)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	result = X * X + Y * Y;
}

float K3D::Vector2::Dot(const K3D::Vector2 & a, const K3D::Vector2 & b)
{
	return (a.x * b.x + a.y * b.y);
}

void K3D::Vector2::Dot(const K3D::Vector2 & a, const K3D::Vector2 & b, float& result)
{
	result = (a.x * b.x + a.y * b.y);
}

K3D::Vector2 K3D::Vector2::Normalize(const K3D::Vector2 & value)
{
	auto mag = value.Length();
	assert(mag > 0.0f);
	return K3D::Vector2(
		value.x / mag,
		value.y / mag
	);
}

void K3D::Vector2::Normalize(const K3D::Vector2 & value, K3D::Vector2 & result)
{
	auto mag = value.Length();
	assert(mag > 0.0f);
	result.x = value.x / mag;
	result.y = value.y / mag;
}

K3D::Vector2 K3D::Vector2::SafeNormalize(const K3D::Vector2 & value, const K3D::Vector2 & set)
{
	auto mag = value.Length();
	if (mag > 0.0f)
	{
		return K3D::Vector2(
			value.x / mag,
			value.y / mag
		);
	}

	return set;
}

void K3D::Vector2::SafeNormalize(const K3D::Vector2 & value, const K3D::Vector2 & set, K3D::Vector2 & result)
{

	auto mag = value.Length();
	if (mag > 0.0f)
	{
		result.x = value.x / mag;
		result.y = value.y / mag;
	}
	else
	{
		result.x = set.x;
		result.y = set.y;
	}
}

float K3D::Vector2::ComputeCrossingAngle(const K3D::Vector2 & a, const K3D::Vector2 & b)
{
	auto d = a.Length() * b.Length();
	if (d <= 0.0f)
	{
		return 0.0f;
	}
	auto c = K3D::Vector2::Dot(a, b) / d;
	if (c >= 1.0f)
	{
		return 0.0f;
	}
	if (c <= -1.0f)
	{
		return F_PI;
	}
	return acosf(c);
}

void K3D::Vector2::ComputeCrossingAngle(const K3D::Vector2 & a, const K3D::Vector2 & b, float result)
{
	auto d = a.Length() * b.Length();
	if (d <= 0.0f)
	{
		result = 0.0f;
		return;
	}
	auto c = K3D::Vector2::Dot(a, b) / d;
	if (c >= 1.0f)
	{
		result = 0.0f;
		return;
	}
	if (c <= -1.0f)
	{
		result = F_PI;
		return;
	}
	result = acosf(c);
}

K3D::Vector2 K3D::Vector2::Min(const K3D::Vector2 & a, const K3D::Vector2 & b)
{
	return K3D::Vector2(
		::Min(a.x, b.x),
		::Min(a.y, b.y)
	);
}

void K3D::Vector2::Min(const K3D::Vector2 & a, const K3D::Vector2 & b, K3D::Vector2 & result)
{
	result.x = ::Min(a.x, b.x);
	result.y = ::Min(a.y, b.y);
}

K3D::Vector2 K3D::Vector2::Max(const K3D::Vector2 & a, const K3D::Vector2 & b)
{
	return K3D::Vector2(
		::Max(a.x, b.x),
		::Max(a.y, b.y)
	);
}

void K3D::Vector2::Max(const K3D::Vector2 & a, const K3D::Vector2 & b, K3D::Vector2 & result)
{
	result.x = ::Max(a.x, b.x);
	result.y = ::Max(a.y, b.y);
}

K3D::Vector2 K3D::Vector2::Reflect(const K3D::Vector2 & in, const K3D::Vector2 & norm)
{
	auto dot = norm.x * in.x + norm.y * in.y;
	return K3D::Vector2(
		in.x - (2.0f * norm.x) * dot,
		in.y - (2.0f * norm.y) * dot
	);
}

void K3D::Vector2::Reflect(const K3D::Vector2 & in, const K3D::Vector2 & norm, K3D::Vector2 & result)
{
	auto dot = norm.x * in.x + norm.y * in.y;
	result.x = in.x - (2.0f * norm.x) * dot;
	result.y = in.y - (2.0f * norm.y) * dot;
}

K3D::Vector2 K3D::Vector2::Reflect(const K3D::Vector2 & in, const K3D::Vector2 & norm, const float eta)
{
	auto cosi = (-in.x * norm.x) + (-in.y * norm.y);
	auto cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
	auto sign = Sign(cost2);
	auto sqrtC2 = std::sqrtf(fabs(cost2));
	auto coeff = eta * cosi - sqrtC2;

	return K3D::Vector2(
		sign * (eta * in.x + coeff * norm.x),
		sign * (eta * in.y + coeff * norm.y)
	);
}

void K3D::Vector2::Reflect(const K3D::Vector2 & in, const K3D::Vector2 & norm, const float eta, K3D::Vector2 & result)
{
	auto cosi = (-in.x * norm.x) + (-in.y * norm.y);
	auto cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
	auto sign = Sign(cost2);
	auto sqrtC2 = std::sqrtf(fabs(cost2));
	auto coeff = eta * cosi - sqrtC2;

	result.x = sign * (eta * in.x + coeff * norm.x);
	result.y = sign * (eta * in.y + coeff * norm.y);

}

K3D::Vector2 K3D::Vector2::Barycentric(const K3D::Vector2 & a, const K3D::Vector2 & b, const K3D::Vector2 & c, const float f, const float g)
{
	return K3D::Vector2(
		a.x + f * (b.x - a.x) + g * (c.x - a.x),
		a.y + f * (b.y - a.y) + g * (c.y - a.y)
	);
}

void K3D::Vector2::Barycentric(const K3D::Vector2 & a, const K3D::Vector2 & b, const K3D::Vector2 & c, const float f, const float g, K3D::Vector2 & result)
{
	result.x = a.x + f * (b.x - a.x) + g * (c.x - a.x);
	result.y = a.y + f * (b.y - a.y) + g * (c.y - a.y);
}

K3D::Vector2 K3D::Vector2::Hermite(const K3D::Vector2 & a, const K3D::Vector2 & t1, const K3D::Vector2 & b, const K3D::Vector2 & t2, const float amount)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	K3D::Vector2 result;
	if (amount <= 0.0f)
	{
		result.x = a.x;
		result.y = a.y;
	}
	else if (amount >= 1.0f)
	{
		result.x = b.x;
		result.y = b.y;
	}
	else
	{
		result.x = (2.0f * a.x - 2.0f * b.x + t2.x + t1.x) * c3 + (3.0f * b.x - 3.0f * a.x - 2.0f * t1.x - t2.x) * c2 + t1.x * amount + a.x;
		result.y = (2.0f * a.y - 2.0f * b.y + t2.y + t1.y) * c3 + (3.0f * b.y - 3.0f * a.y - 2.0f * t1.y - t2.y) * c2 + t1.y * amount + a.y;
	}
	return result;
}

void K3D::Vector2::Hermite(const K3D::Vector2 & a, const K3D::Vector2 & t1, const K3D::Vector2 & b, const K3D::Vector2 & t2, const float amount, K3D::Vector2 & result)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	if (amount <= 0.0f)
	{
		result.x = a.x;
		result.y = a.y;
	}
	else if (amount >= 1.0f)
	{
		result.x = b.x;
		result.y = b.y;
	}
	else
	{
		result.x = (2.0f * a.x - 2.0f * b.x + t2.x + t1.x) * c3 + (3.0f * b.x - 3.0f * a.x - 2.0f * t1.x - t2.x) * c2 + t1.x * amount + a.x;
		result.y = (2.0f * a.y - 2.0f * b.y + t2.y + t1.y) * c3 + (3.0f * b.y - 3.0f * a.y - 2.0f * t1.y - t2.y) * c2 + t1.y * amount + a.y;
	}
}

K3D::Vector2 K3D::Vector2::CatmullRom(const K3D::Vector2 & a, const K3D::Vector2 & b, const K3D::Vector2 & c, const K3D::Vector2 & d, const float amount)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	return K3D::Vector2(
		(0.5f * (2.0f * b.x + (c.x - a.x) * amount + (2.0f * a.x - 5.0f * b.x + 4.0f * c.x - d.x) * c2 + (3.0f * b.x - a.x - 3.0f * c.x + d.x) * c3)),
		(0.5f * (2.0f * b.y + (c.y - a.y) * amount + (2.0f * a.y - 5.0f * b.y + 4.0f * c.y - d.y) * c2 + (3.0f * b.y - a.y - 3.0f * c.y + d.y) * c3))
	);
}

void K3D::Vector2::CatmullRom(const K3D::Vector2 & a, const K3D::Vector2 & b, const K3D::Vector2 & c, const K3D::Vector2 & d, const float amount, K3D::Vector2 & result)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	result.x = (0.5f * (2.0f * b.x + (c.x - a.x) * amount + (2.0f * a.x - 5.0f * b.x + 4.0f * c.x - d.x) * c2 + (3.0f * b.x - a.x - 3.0f * c.x + d.x) * c3));
	result.y = (0.5f * (2.0f * b.y + (c.y - a.y) * amount + (2.0f * a.y - 5.0f * b.y + 4.0f * c.y - d.y) * c2 + (3.0f * b.y - a.y - 3.0f * c.y + d.y) * c3));

}

K3D::Vector2 K3D::Vector2::Lerp(const K3D::Vector2 & a, const K3D::Vector2 & b, const float amount)
{
	return K3D::Vector2(
		a.x - amount * (a.x - b.x),
		a.y - amount * (a.y - b.y)
	);
}

void K3D::Vector2::Lerp(const K3D::Vector2 & a, const K3D::Vector2 & b, const float amount, K3D::Vector2 & result)
{
	result.x = a.x - amount * (a.x - b.x);
	result.y = a.y - amount * (a.y - b.y);
}

K3D::Vector2 K3D::Vector2::SmoothStep(const K3D::Vector2 & a, const K3D::Vector2 & b, const float amount)
{
	auto s = ::Clamp(amount, 0.0f, 1.0f);
	auto u = (s * s) + (3.0f - (2.0f * s));
	return K3D::Vector2(
		a.x - u * (a.x - b.x),
		a.y - u * (a.y - b.y)
	);
}

void K3D::Vector2::SmoothStep(const K3D::Vector2 & a, const K3D::Vector2 & b, const float t, K3D::Vector2 & result)
{
	auto s = ::Clamp(t, 0.0f, 1.0f);
	auto u = (s * s) + (3.0f - (2.0f * s));
	result.x = a.x - u * (a.x - b.x);
	result.y = a.y - u * (a.y - b.y);
}

K3D::Vector2 K3D::Vector2::Transform(const K3D::Vector2 & position, const K3D::Matrix & matrix)
{
	return K3D::Vector2(
		((position.x * matrix._11) + (position.y * matrix._21)) + matrix._41,
		((position.x * matrix._12) + (position.y * matrix._22)) + matrix._42
	);
}

void K3D::Vector2::Transform(const K3D::Vector2 & position, const K3D::Matrix & matrix, K3D::Vector2 & result)
{
	result.x = ((position.x * matrix._11) + (position.y * matrix._21)) + matrix._41;
	result.y = ((position.x * matrix._12) + (position.y * matrix._22)) + matrix._42;
}

K3D::Vector2 K3D::Vector2::TransformNormal(const K3D::Vector2 & normal, const K3D::Matrix & matrix)
{
	return K3D::Vector2(
		(normal.x * matrix._11) + (normal.y * matrix._21),
		(normal.x * matrix._12) + (normal.y * matrix._22)
	);
}

void K3D::Vector2::TransformNormal(const K3D::Vector2 & normal, const K3D::Matrix & matrix, K3D::Vector2 & result)
{
	result.x = (normal.x * matrix._11) + (normal.y * matrix._21);
	result.y = (normal.x * matrix._12) + (normal.y * matrix._22);
}

K3D::Vector2 K3D::Vector2::TransformCoord(const K3D::Vector2 & coords, const K3D::Matrix & matrix)
{
	auto X = ((((coords.x * matrix._11) + (coords.y * matrix._21))) + matrix._41);
	auto Y = ((((coords.x * matrix._12) + (coords.y * matrix._22))) + matrix._42);
	auto W = ((((coords.x * matrix._14) + (coords.y * matrix._24))) + matrix._44);
	return K3D::Vector2(X / W, Y / W);
}

void K3D::Vector2::TransformCoord(const K3D::Vector2 & coords, const K3D::Matrix & matrix, K3D::Vector2 & result)
{
	auto X = ((((coords.x * matrix._11) + (coords.y * matrix._21))) + matrix._41);
	auto Y = ((((coords.x * matrix._12) + (coords.y * matrix._22))) + matrix._42);
	auto W = ((((coords.x * matrix._14) + (coords.y * matrix._24))) + matrix._44);

	result.x = X / W;
	result.y = Y / W;
}

/////////////////////////////////////////
//	K3D::Vector3
/////////////////////////////////////////

const  K3D::Vector3  K3D::Vector3::right(1.0f, 0.0f, 0.0f);
const  K3D::Vector3  K3D::Vector3::up(0.0f, 1.0f, 0.0f);
const  K3D::Vector3  K3D::Vector3::forward(0.0f, 0.0f, 1.0f);
const  K3D::Vector3  K3D::Vector3::left(-1.0f, 0.0f, 0.0f);
const  K3D::Vector3  K3D::Vector3::down(0.0f, -1.0f, 0.0f);
const  K3D::Vector3  K3D::Vector3::back(0.0f, 0.0f, -1.0f);
const  K3D::Vector3  K3D::Vector3::zero(0.0f, 0.0f, 0.0f);
const  K3D::Vector3  K3D::Vector3::one(1.0f, 1.0f, 1.0f);

K3D::Vector3::Vector3() :x(0.0f), y(0.0f), z(0.0f)
{

}

K3D::Vector3::Vector3(const float x, const float y, const float z) :
	x(x), y(y), z(z)
{
}

K3D::Vector3::Vector3(const K3D::Vector3 & other) :
	x(other.x), y(other.y), z(other.z)
{
}

K3D::Vector3::Vector3(K3D::Vector3 && other)
{
	*this = std::move(other);
}

K3D::Vector3::Vector3(const K3D::Vector4 & other) :
	x(other.x), y(other.y), z(other.z)
{

}

K3D::Vector3::Vector3(K3D::Vector4 && other)
{
	*this = std::move(other);
}

K3D::Vector3::Vector3(const DirectX::XMFLOAT3 & nVec) :
	x(nVec.x), y(nVec.y), z(nVec.z)
{
}

K3D::Vector3::Vector3(DirectX::XMFLOAT3 && other)
{
	*this = std::move(other);
}

K3D::Vector3::~Vector3()
{
}

K3D::Vector3 &  K3D::Vector3::operator=(const K3D::Vector3 & value)
{
	this->x = value.x;
	this->y = value.y;
	this->z = value.z;
	return (*this);
}

K3D::Vector3 &  K3D::Vector3::operator=(K3D::Vector3 && value)
{
	*this = value;
	value.x = 0.0f;
	value.y = 0.0f;
	value.z = 0.0f;
	return *this;
}

K3D::Vector3 & K3D::Vector3::operator=(const K3D::Vector4 & value)
{
	this->x = value.x;
	this->y = value.y;
	this->z = value.z;
	return (*this);
}

K3D::Vector3 & K3D::Vector3::operator=(K3D::Vector4 && value)
{
	*this = value;
	value.x = 0.0f;
	value.y = 0.0f;
	value.z = 0.0f;
	return *this;
}

K3D::Vector3 &  K3D::Vector3::operator=(const DirectX::XMFLOAT3 & value)
{
	this->x = value.x;
	this->y = value.y;
	this->z = value.z;
	return (*this);
}

K3D::Vector3 &  K3D::Vector3::operator=(DirectX::XMFLOAT3 && value)
{
	this->x = value.x;
	this->y = value.y;
	this->z = value.z;
	value.x = 0.0f;
	value.y = 0.0f;
	value.z = 0.0f;
	return (*this);
}

K3D::Vector3 &  K3D::Vector3::operator+=(const K3D::Vector3 & value)
{
	x += value.x;
	y += value.y;
	z += value.z;
	return (*this);
}

K3D::Vector3 &  K3D::Vector3::operator+=(const DirectX::XMFLOAT3 & value)
{
	x += value.x;
	y += value.y;
	z += value.z;
	return (*this);
}

K3D::Vector3 &  K3D::Vector3::operator-=(const K3D::Vector3 & value)
{
	x -= value.x;
	y -= value.y;
	z -= value.z;
	return (*this);
}

K3D::Vector3 &  K3D::Vector3::operator-=(const DirectX::XMFLOAT3 & value)
{
	x -= value.x;
	y -= value.y;
	z -= value.z;
	return (*this);
}

K3D::Vector3 &  K3D::Vector3::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return (*this);
}

K3D::Vector3 &  K3D::Vector3::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return (*this);
}

K3D::Vector3  K3D::Vector3::operator+() const
{
	return *this;
}

K3D::Vector3  K3D::Vector3::operator-() const
{
	return K3D::Vector3(-x, -y, -z);
}

K3D::Vector3  K3D::Vector3::operator+(const K3D::Vector3 & value) const
{
	return K3D::Vector3(x + value.x, y + value.y, z + value.z);
}

K3D::Vector3  K3D::Vector3::operator+(const DirectX::XMFLOAT3 & value) const
{
	return K3D::Vector3(x + value.x, y + value.y, z + value.z);
}

K3D::Vector3  K3D::Vector3::operator-(const K3D::Vector3 & value) const
{
	return K3D::Vector3(x - value.x, y - value.y, z - value.z);
}

K3D::Vector3  K3D::Vector3::operator-(const DirectX::XMFLOAT3 & value) const
{
	return K3D::Vector3(x - value.x, y - value.y, z - value.z);
}

K3D::Vector3  K3D::Vector3::operator*(float scalar)
{
	return K3D::Vector3(x * scalar, y * scalar, z * scalar);
}

K3D::Vector3  K3D::operator*(float scalar, const K3D::Vector3 & value)
{
	return K3D::Vector3(value.x * scalar, value.y * scalar, value.z * scalar);
}

DirectX::XMFLOAT3  K3D::operator+(DirectX::XMFLOAT3 & a, const K3D::Vector3 & b)
{
	return DirectX::XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

DirectX::XMFLOAT3  K3D::operator-(DirectX::XMFLOAT3 & a, const K3D::Vector3 & b)
{
	return DirectX::XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}

K3D::Vector3  K3D::Vector3::operator/(float scalar)
{
	return K3D::Vector3(x / scalar, y / scalar, z / scalar);
}

bool  K3D::Vector3::operator==(const K3D::Vector3 & value) const
{
	return IsEqual(x, value.x)
		&& IsEqual(y, value.y)
		&& IsEqual(z, value.z);
}

bool  K3D::Vector3::operator==(const DirectX::XMFLOAT3 & value) const
{
	return IsEqual(x, value.x)
		&& IsEqual(y, value.y)
		&& IsEqual(z, value.z);

}

bool  K3D::Vector3::operator!=(const K3D::Vector3 & value) const
{
	return !IsEqual(x, value.x)
		|| !IsEqual(y, value.y)
		|| !IsEqual(z, value.z);
}

bool  K3D::Vector3::operator!=(const DirectX::XMFLOAT3 & value) const
{
	return !IsEqual(x, value.x)
		|| !IsEqual(y, value.y)
		|| !IsEqual(z, value.z);
}

float  K3D::Vector3::Length() const
{
	return std::sqrtf(x * x + y * y + z * z);
}

float  K3D::Vector3::LengthSqr() const
{
	return (x * x + y * y + z * z);
}

K3D::Vector3 &  K3D::Vector3::Normalize()
{
	auto mag = this->Length();
	assert(mag > 0.0f);
	x /= mag;
	y /= mag;
	z /= mag;
	return (*this);
}

K3D::Vector3 &  K3D::Vector3::SafeNormalize(const K3D::Vector3 & set)
{
	auto mag = Length();
	if (mag > 0.0f)
	{
		x /= mag;
		y /= mag;
		z /= mag;
		return (*this);
	}

	x = set.x;
	y = set.y;
	z = set.z;
	return (*this);
}

K3D::Vector3  K3D::Vector3::Abs(const K3D::Vector3 & value)
{
	return K3D::Vector3(fabs(value.x), fabs(value.y), fabs(value.z));
}

void  K3D::Vector3::Abs(const K3D::Vector3 & value, K3D::Vector3 & result)
{
	result.x = fabs(value.x);
	result.y = fabs(value.y);
	result.z = fabs(value.z);
}

K3D::Vector3  K3D::Vector3::Clamp(const K3D::Vector3 & value, const K3D::Vector3 & min, const K3D::Vector3 & max)
{
	return K3D::Vector3(::Clamp(value.x, min.x, max.x), ::Clamp(value.y, min.y, max.y), ::Clamp(value.z, min.z, max.z));
}

void  K3D::Vector3::Clamp(const K3D::Vector3 & value, const K3D::Vector3 & min, const K3D::Vector3 & max, K3D::Vector3 & result)
{
	result.x = ::Clamp(value.x, min.x, max.x);
	result.y = ::Clamp(value.y, min.y, max.y);
	result.z = ::Clamp(value.z, min.z, max.z);
}

K3D::Vector3 K3D::Vector3::ClampLength(const K3D::Vector3 & value, float minLength, float maxLength)
{

	K3D::Vector3 ret = value;
	if (value.LengthSqr() > (maxLength*maxLength)) {
		ret.Normalize() * maxLength;
	}
	else if (value.LengthSqr() < (minLength*minLength)) {
		ret.Normalize() * minLength;

	}

	return ret;
}

K3D::Vector3 K3D::Vector3::ClampLength(const K3D::Vector3 & value, float minLength, float maxLength, K3D::Vector3 & result)
{
	result = value;
	if (value.LengthSqr() > (maxLength*maxLength)) {
		result = result.Normalize() * maxLength;
	}
	else if (value.LengthSqr() < (minLength*minLength)) {
		result = result.Normalize() * minLength;
	}
	return result;
}

K3D::Vector3  K3D::Vector3::Saturate(const K3D::Vector3 & value)
{
	return K3D::Vector3(::Saturate(value.x), ::Saturate(value.y), ::Saturate(value.z));
}

void  K3D::Vector3::Saturate(const K3D::Vector3 & value, K3D::Vector3 & result)
{
	result.x = ::Saturate(value.x);
	result.y = ::Saturate(value.y);
	result.z = ::Saturate(value.z);
}

float  K3D::Vector3::Distance(const K3D::Vector3 & a, const K3D::Vector3 & b)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	auto Z = b.z - a.z;
	return std::sqrtf(X * X + Y * Y + Z * Z);
}

void  K3D::Vector3::Distance(const K3D::Vector3 & a, const K3D::Vector3 & b, float & result)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	auto Z = b.z - a.z;
	result = std::sqrtf(X * X + Y * Y + Z * Z);
}

float  K3D::Vector3::DistanceSqr(const K3D::Vector3 & a, const K3D::Vector3 & b)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	auto Z = b.z - a.z;
	return X * X + Y * Y + Z * Z;
}

void  K3D::Vector3::DistanceSqr(const K3D::Vector3 & a, const K3D::Vector3 & b, float & result)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	auto Z = b.z - a.z;
	result = X * X + Y * Y + Z * Z;
}

float  K3D::Vector3::Dot(const K3D::Vector3 & a, const K3D::Vector3 & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

void  K3D::Vector3::Dot(const K3D::Vector3 & a, const K3D::Vector3 & b, float & result)
{
	result = a.x * b.x + a.y * b.y + a.z * b.z;
}

K3D::Vector3  K3D::Vector3::Cross(const K3D::Vector3 & a, const K3D::Vector3 & b)
{
	return K3D::Vector3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x));
}

void  K3D::Vector3::Cross(const K3D::Vector3 & a, const K3D::Vector3 & b, K3D::Vector3 & result)
{
	result.x = (a.y * b.z) - (a.z * b.y);
	result.y = (a.z * b.x) - (a.x * b.z);
	result.z = (a.x * b.y) - (a.y * b.x);
}

K3D::Vector3  K3D::Vector3::Normalize(const K3D::Vector3 & value)
{
	auto mag = value.Length();
	assert(mag > 0.0f);
	return K3D::Vector3(value.x / mag, value.y / mag, value.z / mag);
}

void  K3D::Vector3::Normalize(const K3D::Vector3 & value, K3D::Vector3 & result)
{
	auto mag = value.Length();
	assert(mag > 0.0f);
	result.x = value.x / mag;
	result.y = value.y / mag;
	result.z = value.z / mag;
}

K3D::Vector3  K3D::Vector3::SafeNormalize(const K3D::Vector3 & value, const K3D::Vector3 & set)
{
	auto mag = value.Length();
	if (mag > 0.0f)
	{
		return K3D::Vector3(value.x / mag, value.y / mag, value.z / mag);
	}
	return set;
}

void  K3D::Vector3::SafeNormalize(const K3D::Vector3 & value, const K3D::Vector3 & set, K3D::Vector3 & result)
{
	auto mag = value.Length();
	if (mag > 0.0f)
	{
		result.x = value.x / mag;
		result.y = value.y / mag;
		result.z = value.z / mag;
	}
	else
	{
		result.x = set.x;
		result.y = set.y;
		result.z = set.z;
	}
}

K3D::Vector3  K3D::Vector3::ComputeNormal(const K3D::Vector3 & p1, const K3D::Vector3 & p2, const K3D::Vector3 & p3)
{
	auto v1 = p2 - p1;
	auto v2 = p3 - p1;
	auto result = K3D::Vector3::Cross(v1, v2);
	return result.Normalize();
}

void  K3D::Vector3::ComputeNormal(const K3D::Vector3 & p1, const K3D::Vector3 & p2, const K3D::Vector3 & p3, K3D::Vector3 & result)
{

	auto v1 = p2 - p1;
	auto v2 = p3 - p1;
	K3D::Vector3::Cross(v1, v2, result);
	result.Normalize();
}

K3D::Vector3  K3D::Vector3::ComputeQuadNormal(const K3D::Vector3 & p1, const K3D::Vector3 & p2, const K3D::Vector3 & p3, const K3D::Vector3 & p4)
{
	K3D::Vector3 result;
	auto n1a = K3D::Vector3::ComputeNormal(p1, p2, p3);
	auto n1b = K3D::Vector3::ComputeNormal(p1, p3, p4);
	auto n2a = K3D::Vector3::ComputeNormal(p2, p3, p4);
	auto n2b = K3D::Vector3::ComputeNormal(p2, p4, p1);
	if (K3D::Vector3::Dot(n1a, n1b) > K3D::Vector3::Dot(n2a, n2b))
	{
		result = n1a + n1b;
		result.Normalize();
	}
	else
	{
		result = n2a + n2b;
		result.Normalize();
	}
	return result;
}

void  K3D::Vector3::ComputeQuadNormal(const K3D::Vector3 & p1, const K3D::Vector3 & p2, const K3D::Vector3 & p3, const K3D::Vector3 & p4, K3D::Vector3 & result)
{
	auto n1a = K3D::Vector3::ComputeNormal(p1, p2, p3);
	auto n1b = K3D::Vector3::ComputeNormal(p1, p3, p4);
	auto n2a = K3D::Vector3::ComputeNormal(p2, p3, p4);
	auto n2b = K3D::Vector3::ComputeNormal(p2, p4, p1);
	if (K3D::Vector3::Dot(n1a, n1b) > K3D::Vector3::Dot(n2a, n2b))
	{
		result = n1a + n1b;
		result.Normalize();
	}
	else
	{
		result = n2a + n2b;
		result.Normalize();
	}
}

float  K3D::Vector3::ComputeCrossingAngle(const K3D::Vector3 & a, const K3D::Vector3 & b)
{
	auto d = a.Length() * b.Length();
	if (d <= 0.0f)
	{
		return 0.0f;
	}

	auto c = K3D::Vector3::Dot(a, b) / d;
	if (c >= 1.0f)
	{
		return 0.0f;
	}

	if (c <= -1.0f)
	{
		return F_PI;
	}

	return acosf(c);
}

void  K3D::Vector3::ComputeCrossingAngle(const K3D::Vector3 & a, const K3D::Vector3 & b, float result)
{
	auto d = a.Length() * b.Length();
	if (d <= 0.0f)
	{
		result = 0.0f;
		return;
	}

	auto c = K3D::Vector3::Dot(a, b) / d;
	if (c >= 1.0f)
	{
		result = 0.0f;
		return;
	}

	if (c <= -1.0f)
	{
		result = F_PI;
		return;
	}

	result = acosf(c);
}

K3D::Vector3  K3D::Vector3::Min(const K3D::Vector3 & a, const K3D::Vector3 & b)
{
	return K3D::Vector3(::Min(a.x, b.x), ::Min(a.y, b.y), ::Min(a.z, b.z));
}

void K3D::Vector3::Min(const K3D::Vector3 & a, const K3D::Vector3 & b, K3D::Vector3 & result)
{
	result.x = ::Min(a.x, b.x);
	result.y = ::Min(a.y, b.y);
	result.z = ::Min(a.z, b.z);
}

K3D::Vector3  K3D::Vector3::Max(const K3D::Vector3 & a, const K3D::Vector3 & b)
{
	return K3D::Vector3(::Max(a.x, b.x), ::Max(a.y, b.y), ::Max(a.z, b.z));
}

void  K3D::Vector3::Max(const K3D::Vector3 & a, const K3D::Vector3 & b, K3D::Vector3 & result)
{
	result.x = ::Max(a.x, b.x);
	result.y = ::Max(a.y, b.y);
	result.z = ::Max(a.z, b.z);
}

K3D::Vector3  K3D::Vector3::Reflect(const K3D::Vector3 & in, const K3D::Vector3 & norm)
{
	auto dot = norm.x * in.x + norm.y * in.y + norm.z * in.z;
	return K3D::Vector3(in.x - (2.0f * norm.x) * dot, in.y - (2.0f * norm.y) * dot, in.z - (2.0f * norm.z) * dot);
}

void  K3D::Vector3::Reflect(const K3D::Vector3 & in, const K3D::Vector3 & norm, K3D::Vector3 & result)
{
	auto dot = norm.x * in.x + norm.y * in.y + norm.z * in.z;
	result.x = in.x - (2.0f * norm.x) * dot;
	result.y = in.y - (2.0f * norm.y) * dot;
	result.z = in.z - (2.0f * norm.z) * dot;
}

K3D::Vector3  K3D::Vector3::Reflect(const K3D::Vector3 & in, const K3D::Vector3 & norm, const float eta)
{
	auto cosi = (-in.x * norm.x) + (-in.y * norm.y) + (-in.z * norm.z);
	auto cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
	auto sign = Sign(cost2);
	auto sqrtC2 = sqrtf(fabs(cost2));
	auto coeff = eta * cosi - sqrtC2;

	return K3D::Vector3(
		sign * (eta * in.x + coeff * norm.x),
		sign * (eta * in.y + coeff * norm.y),
		sign * (eta * in.z + coeff * norm.z)
	);
}

void  K3D::Vector3::Reflect(const K3D::Vector3 & in, const K3D::Vector3 & norm, const float eta, K3D::Vector3 & result)
{
	auto cosi = (-in.x * norm.x) + (-in.y * norm.y) + (-in.z * norm.z);
	auto cost2 = 1.0f - eta * eta * (1.0f - cosi * cosi);
	auto sign = Sign(cost2);
	auto sqrtC2 = sqrtf(fabs(cost2));
	auto coeff = eta * cosi - sqrtC2;

	result.x = sign * (eta * in.x + coeff * norm.x);
	result.y = sign * (eta * in.y + coeff * norm.y);
	result.z = sign * (eta * in.z + coeff * norm.z);
}

K3D::Vector3  K3D::Vector3::Barycentric(const K3D::Vector3 & a, const K3D::Vector3 & b, const K3D::Vector3 & c, const float f, const float g)
{
	return K3D::Vector3(
		a.x + f * (b.x - a.x) + g * (c.x - a.x),
		a.y + f * (b.y - a.y) + g * (c.y - a.y),
		a.z + f * (b.z - a.z) + g * (c.z - a.z)
	);
}

void  K3D::Vector3::Barycentric(const K3D::Vector3 & a, const K3D::Vector3 & b, const K3D::Vector3 & c, const float f, const float g, K3D::Vector3 & result)
{
	result.x = a.x + f * (b.x - a.x) + g * (c.x - a.x);
	result.y = a.y + f * (b.y - a.y) + g * (c.y - a.y);
	result.z = a.z + f * (b.z - a.z) + g * (c.z - a.z);
}

K3D::Vector3  K3D::Vector3::Hermite(const K3D::Vector3 & a, const K3D::Vector3 & t1, const K3D::Vector3 & b, const K3D::Vector3 & t2, const float amount)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	K3D::Vector3 result;
	if (amount <= 0.0f)
	{
		result.x = a.x;
		result.y = a.y;
		result.z = a.z;
	}
	else if (amount >= 1.0f)
	{
		result.x = b.x;
		result.y = b.y;
		result.z = b.z;
	}
	else
	{
		result.x = (2.0f * a.x - 2.0f * b.x + t2.x + t1.x) * c3 + (3.0f * b.x - 3.0f * a.x - 2.0f * t1.x - t2.x) * c2 + t1.x * amount + a.x;
		result.y = (2.0f * a.y - 2.0f * b.y + t2.y + t1.y) * c3 + (3.0f * b.y - 3.0f * a.y - 2.0f * t1.y - t2.y) * c2 + t1.y * amount + a.y;
		result.z = (2.0f * a.z - 2.0f * b.z + t2.z + t1.z) * c3 + (3.0f * b.z - 3.0f * a.z - 2.0f * t1.z - t2.z) * c2 + t1.y * amount + a.z;
	}
	return result;
}

void  K3D::Vector3::Hermite(const K3D::Vector3 & a, const K3D::Vector3 & t1, const K3D::Vector3 & b, const K3D::Vector3 & t2, const float amount, K3D::Vector3 & result)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	if (amount <= 0.0f)
	{
		result.x = a.x;
		result.y = a.y;
		result.z = a.z;
	}
	else if (amount >= 1.0f)
	{
		result.x = b.x;
		result.y = b.y;
		result.z = b.z;
	}
	else
	{
		result.x = (2.0f * a.x - 2.0f * b.x + t2.x + t1.x) * c3 + (3.0f * b.x - 3.0f * a.x - 2.0f * t1.x - t2.x) * c2 + t1.x * amount + a.x;
		result.y = (2.0f * a.y - 2.0f * b.y + t2.y + t1.y) * c3 + (3.0f * b.y - 3.0f * a.y - 2.0f * t1.y - t2.y) * c2 + t1.y * amount + a.y;
		result.z = (2.0f * a.z - 2.0f * b.z + t2.z + t1.z) * c3 + (3.0f * b.z - 3.0f * a.z - 2.0f * t1.z - t2.z) * c2 + t1.z * amount + a.z;
	}
}

K3D::Vector3  K3D::Vector3::CatmullRom(const K3D::Vector3 & a, const K3D::Vector3 & b, const K3D::Vector3 & c, const K3D::Vector3 & d, const float amount)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	return K3D::Vector3(
		(0.5f * (2.0f * b.x + (c.x - a.x) * amount + (2.0f * a.x - 5.0f * b.x + 4.0f * c.x - d.x) * c2 + (3.0f * b.x - a.x - 3.0f * c.x + d.x) * c3)),
		(0.5f * (2.0f * b.y + (c.y - a.y) * amount + (2.0f * a.y - 5.0f * b.y + 4.0f * c.y - d.y) * c2 + (3.0f * b.y - a.y - 3.0f * c.y + d.y) * c3)),
		(0.5f * (2.0f * b.z + (c.z - a.z) * amount + (2.0f * a.z - 5.0f * b.z + 4.0f * c.z - d.z) * c2 + (3.0f * b.z - a.z - 3.0f * c.z + d.z) * c3))
	);
}

void  K3D::Vector3::CatmullRom(const K3D::Vector3 & a, const K3D::Vector3 & b, const K3D::Vector3 & c, const K3D::Vector3 & d, const float amount, K3D::Vector3 & result)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	result.x = (0.5f * (2.0f * b.x + (c.x - a.x) * amount + (2.0f * a.x - 5.0f * b.x + 4.0f * c.x - d.x) * c2 + (3.0f * b.x - a.x - 3.0f * c.x + d.x) * c3));
	result.y = (0.5f * (2.0f * b.y + (c.y - a.y) * amount + (2.0f * a.y - 5.0f * b.y + 4.0f * c.y - d.y) * c2 + (3.0f * b.y - a.y - 3.0f * c.y + d.y) * c3));
	result.z = (0.5f * (2.0f * b.z + (c.z - a.z) * amount + (2.0f * a.z - 5.0f * b.z + 4.0f * c.z - d.z) * c2 + (3.0f * b.z - a.z - 3.0f * c.z + d.z) * c3));

}

K3D::Vector3  K3D::Vector3::Lerp(const K3D::Vector3 & a, const K3D::Vector3 & b, const float amount)
{
	return K3D::Vector3(a.x - amount * (a.x - b.x), a.y - amount * (a.y - b.y), a.z - amount * (a.z - b.z));
}

void  K3D::Vector3::Lerp(const K3D::Vector3 & a, const K3D::Vector3 & b, const float amount, K3D::Vector3 & result)
{
	result.x = a.x - amount * (a.x - b.x);
	result.y = a.y - amount * (a.y - b.y);
	result.z = a.z - amount * (a.z - b.z);
}

K3D::Vector3  K3D::Vector3::SmoothStep(const K3D::Vector3 & a, const K3D::Vector3 & b, const float amount)
{
	auto s = ::Clamp(amount, 0.0f, 1.0f);
	auto u = (s * s) + (3.0f - (2.0f * s));
	return K3D::Vector3(
		a.x - u * (a.x - b.x),
		a.y - u * (a.y - b.y),
		a.z - u * (a.z - b.z)
	);
}

void  K3D::Vector3::SmoothStep(const K3D::Vector3 & a, const K3D::Vector3 & b, const float amount, K3D::Vector3 & result)
{
	auto s = ::Clamp(amount, 0.0f, 1.0f);
	auto u = (s * s) + (3.0f - (2.0f * s));
	result.x = a.x - u * (a.x - b.x);
	result.y = a.y - u * (a.y - b.y);
	result.z = a.z - u * (a.z - b.z);
}

K3D::Vector3  K3D::Vector3::Transform(const K3D::Vector3 & position, const K3D::Matrix & matrix)
{
	return K3D::Vector3(
		(((position.x * matrix._11) + (position.y * matrix._21)) + (position.z * matrix._31)) + matrix._41,
		(((position.x * matrix._12) + (position.y * matrix._22)) + (position.z * matrix._32)) + matrix._42,
		(((position.x * matrix._13) + (position.y * matrix._23)) + (position.z * matrix._33)) + matrix._43);
}

void  K3D::Vector3::Transform(const K3D::Vector3 & position, const K3D::Matrix & matrix, K3D::Vector3 & result)
{
	result.x = (((position.x * matrix._11) + (position.y * matrix._21)) + (position.z * matrix._31)) + matrix._41;
	result.y = (((position.x * matrix._12) + (position.y * matrix._22)) + (position.z * matrix._32)) + matrix._42;
	result.z = (((position.x * matrix._13) + (position.y * matrix._23)) + (position.z * matrix._33)) + matrix._43;
}

K3D::Vector3  K3D::Vector3::TransformNormal(const K3D::Vector3 & normal, const K3D::Matrix & matrix)
{
	return K3D::Vector3(
		((normal.x * matrix._11) + (normal.y * matrix._21)) + (normal.z * matrix._31),
		((normal.x * matrix._12) + (normal.y * matrix._22)) + (normal.z * matrix._32),
		((normal.x * matrix._13) + (normal.y * matrix._23)) + (normal.z * matrix._33));
}

void  K3D::Vector3::TransformNormal(const K3D::Vector3 & normal, const K3D::Matrix & matrix, K3D::Vector3 & result)
{
	result.x = ((normal.x * matrix._11) + (normal.y * matrix._21)) + (normal.z * matrix._31);
	result.y = ((normal.x * matrix._12) + (normal.y * matrix._22)) + (normal.z * matrix._32);
	result.z = ((normal.x * matrix._13) + (normal.y * matrix._23)) + (normal.z * matrix._33);
}

K3D::Vector3  K3D::Vector3::TransformCoord(const K3D::Vector3 & coords, const K3D::Matrix & matrix)
{
	auto X = ((((coords.x * matrix._11) + (coords.y * matrix._21)) + (coords.z * matrix._31)) + matrix._41);
	auto Y = ((((coords.x * matrix._12) + (coords.y * matrix._22)) + (coords.z * matrix._32)) + matrix._42);
	auto Z = ((((coords.x * matrix._13) + (coords.y * matrix._23)) + (coords.z * matrix._33)) + matrix._43);
	auto W = ((((coords.x * matrix._14) + (coords.y * matrix._24)) + (coords.z * matrix._34)) + matrix._44);
	return K3D::Vector3(X / W, Y / W, Z / W);
}

void  K3D::Vector3::TransformCoord(const K3D::Vector3 & coords, const K3D::Matrix & matrix, K3D::Vector3 & result)
{
	auto X = ((((coords.x * matrix._11) + (coords.y * matrix._21)) + (coords.z * matrix._31)) + matrix._41);
	auto Y = ((((coords.x * matrix._12) + (coords.y * matrix._22)) + (coords.z * matrix._32)) + matrix._42);
	auto Z = ((((coords.x * matrix._13) + (coords.y * matrix._23)) + (coords.z * matrix._33)) + matrix._43);
	auto W = ((((coords.x * matrix._14) + (coords.y * matrix._24)) + (coords.z * matrix._34)) + matrix._44);

	result.x = X / W;
	result.y = Y / W;
	result.z = Z / W;
}

float  K3D::Vector3::ScalarTriple(const K3D::Vector3 & a, const K3D::Vector3 & b, const K3D::Vector3 & c)
{
	auto crossX = (b.y * c.z) - (b.z * c.y);
	auto crossY = (b.z * c.x) - (b.x * c.z);
	auto crossZ = (b.x * c.y) - (b.y * c.x);

	return (a.x * crossX) + (a.y * crossY) + (a.z * crossZ);
}

void  K3D::Vector3::ScalarTriple(const K3D::Vector3 & a, const K3D::Vector3 & b, const K3D::Vector3 & c, float & result)
{
	auto crossX = (b.y * c.z) - (b.z * c.y);
	auto crossY = (b.z * c.x) - (b.x * c.z);
	auto crossZ = (b.x * c.y) - (b.y * c.x);

	result = (a.x * crossX) + (a.y * crossY) + (a.z * crossZ);
}

K3D::Vector3  K3D::Vector3::VectorTriple(const K3D::Vector3 & a, const K3D::Vector3 & b, const K3D::Vector3 & c)
{
	auto crossX = (b.y * c.z) - (b.z * c.y);
	auto crossY = (b.z * c.x) - (b.x * c.z);
	auto crossZ = (b.x * c.y) - (b.y * c.x);

	return K3D::Vector3(
		((a.y * crossZ) - (a.z * crossY)),
		((a.z * crossX) - (a.x * crossZ)),
		((a.x * crossY) - (a.y * crossX))
	);
}

void  K3D::Vector3::VectorTriple(const K3D::Vector3 & a, const K3D::Vector3 & b, const K3D::Vector3 & c, K3D::Vector3 & result)
{
	auto crossX = (b.y * c.z) - (b.z * c.y);
	auto crossY = (b.z * c.x) - (b.x * c.z);
	auto crossZ = (b.x * c.y) - (b.y * c.x);

	result.x = (a.y * crossZ) - (a.z * crossY);
	result.y = (a.z * crossX) - (a.x * crossZ);
	result.z = (a.x * crossY) - (a.y * crossX);
}

K3D::Vector3  K3D::Vector3::Rotate(const K3D::Vector3 & value, const K3D::Quaternion & rotation)
{
	K3D::Quaternion a = K3D::Quaternion(value.x, value.y, value.z, 0.0f);
	K3D::Quaternion q = K3D::Quaternion::Conjugate(rotation);
	K3D::Quaternion result = K3D::Quaternion::Multiply(q, a);
	result = K3D::Quaternion::Multiply(result, rotation);
	if (IsZero(result.x)) {
		result.x = .0f;
	}
	if (IsZero(result.y)) {
		result.y = .0f;
	}
	if (IsZero(result.z)) {
		result.z = .0f;
	}
	return K3D::Vector3(result.x, result.y, result.z);
}

void  K3D::Vector3::Rotate(const K3D::Vector3 & value, const K3D::Quaternion & rotation, K3D::Vector3 & result)
{
	K3D::Quaternion a = K3D::Quaternion(value.x, value.y, value.z, 0.0f);
	K3D::Quaternion q = K3D::Quaternion::Conjugate(rotation);
	K3D::Quaternion r = K3D::Quaternion::Multiply(q, a);
	r = K3D::Quaternion::Multiply(r, rotation);

	result.x = r.x;
	result.y = r.y;
	result.z = r.z;

	if (IsZero(result.x)) {
		result.x = .0f;
	}
	if (IsZero(result.y)) {
		result.y = .0f;
	}
	if (IsZero(result.z)) {
		result.z = .0f;
	}
}

K3D::Vector3  K3D::Vector3::InverseRotate(const K3D::Vector3 & value, const K3D::Quaternion & rotation)
{
	K3D::Quaternion a = K3D::Quaternion(value.x, value.y, value.z, 0.0f);
	K3D::Quaternion r = K3D::Quaternion::Multiply(rotation, a);
	K3D::Quaternion q = K3D::Quaternion::Conjugate(rotation);
	r = K3D::Quaternion::Multiply(r, q);
	return K3D::Vector3(r.x, r.y, r.z);
}

void  K3D::Vector3::InverseRotate(const K3D::Vector3 & value, const K3D::Quaternion & rotation, K3D::Vector3 & result)
{
	K3D::Quaternion a = K3D::Quaternion(value.x, value.y, value.z, 0.0f);
	K3D::Quaternion r = K3D::Quaternion::Multiply(rotation, a);
	K3D::Quaternion q = K3D::Quaternion::Conjugate(rotation);
	r = K3D::Quaternion::Multiply(r, q);
	result.x = r.x;
	result.y = r.y;
	result.z = r.z;
}

DirectX::XMFLOAT3  K3D::Vector3::Store(const K3D::Vector3 & value)
{
	return DirectX::XMFLOAT3(value.x, value.y, value.z);
}

void  K3D::Vector3::Store(const K3D::Vector3 & value, DirectX::XMFLOAT3 & result)
{
	result = K3D::Vector3::Store(value);
}

K3D::Vector4::Vector4() :x(), y(), z(), w()
{
}
			
K3D::Vector4::Vector4(const K3D::Vector4 & value) :
	x(value.x), y(value.y), z(value.z), w(value.w)
{
}
			
K3D::Vector4::Vector4(K3D::Vector4 && other)
{
	*this = std::move(other);
}
			
K3D::Vector4::Vector4(const DirectX::XMFLOAT4 & value) :
	x(value.x), y(value.y), z(value.z), w(value.w)
{

}
			
K3D::Vector4::Vector4(DirectX::XMFLOAT4 && other)
{
	*this = std::move(other);
}
			
K3D::Vector4::Vector4(const K3D::Quaternion & value) :
	x(value.x), y(value.y), z(value.z), w(value.w)
{

}

K3D::Vector4::Vector4(K3D::Quaternion && other)
{
	*this = std::move(other);
}

K3D::Vector4::Vector4(const float x, const float y, const float z, float w) :
	x(x), y(y), z(z), w(w)
{
}

K3D::Vector4::Vector4(const K3D::Vector2 & value, const float z, float w) :
	x(value.x), y(value.y), z(z), w(w)
{
}

K3D::Vector4::Vector4(const DirectX::XMFLOAT2 & value, const float z, float w) :
	x(value.x), y(value.y), z(z), w(w)
{
}

K3D::Vector4::Vector4(const K3D::Vector3 & value, float w) :
	x(value.x), y(value.y), z(value.z), w(w)
{
}

K3D::Vector4::Vector4(const DirectX::XMFLOAT3 & value, float w) :
	x(value.x), y(value.y), z(value.z), w(w)
{
}

K3D::Vector4::~Vector4()
{
}

K3D::Vector4 &  K3D::Vector4::operator=(const K3D::Vector4 & value)
{
	x = value.x;
	y = value.y;
	z = value.z;
	w = value.w;
	return (*this);
}

K3D::Vector4 &  K3D::Vector4::operator=(K3D::Vector4 && value)
{
	*this = value;
	value.x = 0.0f;
	value.y = 0.0f;
	value.z = 0.0f;
	value.w = 0.0f;
	return *this;
}

K3D::Vector4 & K3D::Vector4::operator=(const K3D::Vector3 & value)
{
	x = value.x;
	y = value.y;
	z = value.z;
	return (*this);
}

K3D::Vector4 &  K3D::Vector4::operator=(const DirectX::XMFLOAT4 & value)
{
	x = value.x;
	y = value.y;
	z = value.z;
	w = value.w;
	return (*this);
}

K3D::Vector4 &  K3D::Vector4::operator=(DirectX::XMFLOAT4 && value)
{
	x = value.x;
	y = value.y;
	z = value.z;
	w = value.w;
	value.x = 0.0f;
	value.y = 0.0f;
	value.z = 0.0f;
	value.w = 0.0f;
	return (*this);
}

K3D::Vector4 & K3D::Vector4::operator=(K3D::Quaternion && value)
{
	x = value.x;
	y = value.y;
	z = value.z;
	w = value.w;
	value.x = 0.0f;
	value.y = 0.0f;
	value.z = 0.0f;
	value.w = 0.0f;
	return (*this);
}

K3D::Vector4 &  K3D::Vector4::operator+=(const K3D::Vector4 & value)
{
	x += value.x;
	y += value.y;
	z += value.z;
	w += value.w;
	return (*this);
}

K3D::Vector4 &  K3D::Vector4::operator+=(DirectX::XMFLOAT4 & value)
{
	x += value.x;
	y += value.y;
	z += value.z;
	w += value.w;
	return (*this);
}

K3D::Vector4 &  K3D::Vector4::operator-=(const K3D::Vector4 & value)
{
	x -= value.x;
	y -= value.y;
	z -= value.z;
	w -= value.w;
	return (*this);
}


K3D::Vector4 &  K3D::Vector4::operator-=(DirectX::XMFLOAT4 & value)
{
	x -= value.x;
	y -= value.y;
	z -= value.z;
	w -= value.w;
	return (*this);
}

K3D::Vector4 &  K3D::Vector4::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return (*this);
}

K3D::Vector4 &  K3D::Vector4::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return (*this);
}

K3D::Vector4  K3D::Vector4::operator+() const
{
	return (*this);;
}

K3D::Vector4  K3D::Vector4::operator-() const
{
	return K3D::Vector4(-x, -y, -z, -w);
}

K3D::Vector4  K3D::Vector4::operator+(const K3D::Vector4 & value) const
{
	return K3D::Vector4(x + value.x, y + value.y, z + value.z, w + value.w);
}

K3D::Vector4  K3D::Vector4::operator+(const DirectX::XMFLOAT4 & value) const
{
	return K3D::Vector4(x + value.x, y + value.y, z + value.z, w + value.w);
}

K3D::Vector4  K3D::Vector4::operator-(const K3D::Vector4 & value) const
{
	return K3D::Vector4(x - value.x, y - value.y, z - value.z, w - value.w);
}

K3D::Vector4  K3D::Vector4::operator-(const DirectX::XMFLOAT4 & value) const
{
	return K3D::Vector4(x - value.x, y - value.y, z - value.z, w - value.w);
}

K3D::Vector4  K3D::Vector4::operator*(float scalar)
{
	return K3D::Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
}

K3D::Vector4  K3D::operator*(float scalar, const K3D::Vector4 & value)
{
	return K3D::Vector4(value.x * scalar, value.y * scalar, value.z * scalar, value.w * scalar);
}

K3D::Vector4  K3D::Vector4::operator/(float scalar)
{
	assert(!IsZero(scalar));
	return K3D::Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
}

bool  K3D::Vector4::operator==(const K3D::Vector4 & value) const
{
	return IsEqual(x, value.x)
		&& IsEqual(y, value.y)
		&& IsEqual(z, value.z)
		&& IsEqual(w, value.z);
}

bool  K3D::Vector4::operator==(const DirectX::XMFLOAT4 & value) const
{
	return IsEqual(x, value.x)
		&& IsEqual(y, value.y)
		&& IsEqual(z, value.z)
		&& IsEqual(w, value.z);
}

bool  K3D::Vector4::operator!=(const K3D::Vector4 & value) const
{
	return !IsEqual(x, value.x)
		|| !IsEqual(y, value.y)
		|| !IsEqual(z, value.z)
		|| !IsEqual(w, value.w);
}

bool  K3D::Vector4::operator!=(const DirectX::XMFLOAT4 & value) const
{
	return !IsEqual(x, value.x)
		|| !IsEqual(y, value.y)
		|| !IsEqual(z, value.z)
		|| !IsEqual(w, value.w);
}

float  K3D::Vector4::Length() const
{
	return std::sqrtf(x * x + y * y + z * z + w * w);
}

float  K3D::Vector4::LengthSqr() const
{
	return (x * x + y * y + z * z + w * w);
}

K3D::Vector4 &  K3D::Vector4::Normalize()
{
	auto mag = Length();
	assert(mag > 0.0f);
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
	return (*this);
}

K3D::Vector4 &  K3D::Vector4::SafeNormalize(const K3D::Vector4 & set)
{
	auto mag = Length();
	if (mag > 0.0f)
	{
		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}
	else
	{
		x = set.x;
		y = set.y;
		z = set.z;
		w = set.w;
	}

	return (*this);
}

K3D::Vector4  K3D::Vector4::Abs(const K3D::Vector4 & value)
{
	return K3D::Vector4(fabs(value.x), fabs(value.y), fabs(value.z), fabs(value.w));
}

void  K3D::Vector4::Abs(const K3D::Vector4 & value, K3D::Vector4 & result)
{
	result.x = fabs(value.x);
	result.y = fabs(value.y);
	result.z = fabs(value.z);
	result.w = fabs(value.w);
}

K3D::Vector4  K3D::Vector4::Clamp(const K3D::Vector4 & value, const K3D::Vector4 & min, const K3D::Vector4 & max)
{
	return K3D::Vector4(
		::Clamp(value.x, min.x, max.x), ::Clamp(value.y, min.y, max.y),
		::Clamp(value.z, min.z, max.z), ::Clamp(value.w, min.w, max.w)
	);
}

void  K3D::Vector4::Clamp(const K3D::Vector4 & value, const K3D::Vector4 & min, const K3D::Vector4 & max, K3D::Vector4 & result)
{
	result.x = ::Clamp(value.x, min.x, min.x);
	result.y = ::Clamp(value.y, min.y, min.y);
	result.z = ::Clamp(value.z, min.z, min.z);
	result.w = ::Clamp(value.w, min.w, min.w);
}

K3D::Vector4  K3D::Vector4::Saturate(const K3D::Vector4 & value)
{
	return K3D::Vector4(
		::Saturate(value.x), ::Saturate(value.y),
		::Saturate(value.z), ::Saturate(value.w)
	);
}

void  K3D::Vector4::Saturate(const K3D::Vector4 & value, K3D::Vector4 & result)
{
	result.x = ::Saturate(value.x);
	result.y = ::Saturate(value.y);
	result.z = ::Saturate(value.z);
	result.w = ::Saturate(value.w);
}

float  K3D::Vector4::Distance(const K3D::Vector4 & a, const K3D::Vector4 & b)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	auto Z = b.z - a.z;
	auto W = b.w - a.w;
	return sqrtf(X * X + Y * Y + Z * Z + W * W);
}

void  K3D::Vector4::Distance(const K3D::Vector4 & a, const K3D::Vector4 & b, float & result)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	auto Z = b.z - a.z;
	auto W = b.w - a.w;
	result = sqrtf(X * X + Y * Y + Z * Z + W * W);
}

float  K3D::Vector4::DistanceSqr(const K3D::Vector4 & a, const K3D::Vector4 & b)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	auto Z = b.z - a.z;
	auto W = b.w - a.w;
	return X * X + Y * Y + Z * Z + W * W;
}

void  K3D::Vector4::DistanceSqr(const K3D::Vector4 & a, const K3D::Vector4 & b, float & result)
{
	auto X = b.x - a.x;
	auto Y = b.y - a.y;
	auto Z = b.z - a.z;
	auto W = b.w - a.w;
	result = X * X + Y * Y + Z * Z + W * W;
}

float  K3D::Vector4::Dot(const K3D::Vector4 & a, const K3D::Vector4 & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);;
}

void  K3D::Vector4::Dot(const K3D::Vector4 & a, const K3D::Vector4 & b, float & result)
{
	result = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

K3D::Vector4  K3D::Vector4::Normalize(const K3D::Vector4 & value)
{
	auto mag = value.Length();
	assert(mag > 0.0f);
	return K3D::Vector4(value.x / mag, value.y / mag, value.z / mag, value.w / mag);
}

void  K3D::Vector4::Normalize(const K3D::Vector4 & value, K3D::Vector4 & result)
{
	auto mag = value.Length();
	assert(mag > 0.0f);
	result.x = value.x / mag;
	result.y = value.y / mag;
	result.z = value.z / mag;
	result.w = value.w / mag;
}

K3D::Vector4  K3D::Vector4::SafeNormalize(const K3D::Vector4 & value, const K3D::Vector4 & set)
{
	auto mag = value.Length();
	if (mag > 0.0f)
	{
		return K3D::Vector4(
			value.x / mag,
			value.y / mag,
			value.z / mag,
			value.w / mag
		);
	}

	return set;
}

void  K3D::Vector4::SafeNormalize(const K3D::Vector4 & value, const K3D::Vector4 & set, K3D::Vector4 & result)
{
	auto mag = value.Length();
	if (mag > 0.0f)
	{
		result.x = value.x / mag;
		result.y = value.y / mag;
		result.z = value.z / mag;
		result.w = value.w / mag;
	}
	else
	{
		result.x = set.x;
		result.y = set.y;
		result.z = set.z;
		result.w = set.w;
	}
}

float  K3D::Vector4::ComputeCrossingAngle(const K3D::Vector4 & a, const K3D::Vector4 & b)
{
	auto d = a.Length() * b.Length();
	if (d <= 0.0f)
	{
		return 0.0f;
	}

	auto c = K3D::Vector4::Dot(a, b) / d;
	if (c >= 1.0f)
	{
		return 0.0f;
	}

	if (c <= -1.0f)
	{
		return F_PI;
	}

	return acosf(c);
}

void  K3D::Vector4::ComputeCrossingAngle(const K3D::Vector4 & a, const K3D::Vector4 & b, float result)
{
	auto d = a.Length() * b.Length();
	if (d <= 0.0f)
	{
		result = 0.0f;
		return;
	}

	auto c = K3D::Vector4::Dot(a, b) / d;
	if (c >= 1.0f)
	{
		result = 0.0f;
		return;
	}

	if (c <= -1.0f)
	{
		result = F_PI;
		return;
	}

	result = acosf(c);
}

K3D::Vector4  K3D::Vector4::Min(const K3D::Vector4 & a, const K3D::Vector4 & b)
{
	return K3D::Vector4(
		::Min(a.x, b.x), ::Min(a.y, b.y),
		::Min(a.z, b.z), ::Min(a.w, b.w)
	);
}

void  K3D::Vector4::Min(const K3D::Vector4 & a, const K3D::Vector4 & b, K3D::Vector4 & result)
{
	result.x = ::Min(a.x, b.x);
	result.y = ::Min(a.y, b.y);
	result.z = ::Min(a.z, b.z);
	result.w = ::Min(a.w, b.w);
}

K3D::Vector4  K3D::Vector4::Max(const K3D::Vector4 & a, const K3D::Vector4 & b)
{
	return K3D::Vector4(
		::Max(a.x, b.x), ::Max(a.y, b.y),
		::Max(a.z, b.z), ::Max(a.w, b.w)
	);
}

void  K3D::Vector4::Max(const K3D::Vector4 & a, const K3D::Vector4 & b, K3D::Vector4 & result)
{
	result.x = ::Max(a.x, b.x);
	result.y = ::Max(a.y, b.y);
	result.z = ::Max(a.z, b.z);
	result.w = ::Max(a.w, b.w);
}

K3D::Vector4  K3D::Vector4::Barycentric(const K3D::Vector4 & a, const K3D::Vector4 & b, const K3D::Vector4 & c, const float f, const float g)
{
	return K3D::Vector4(
		a.x + f * (b.x - a.x) + g * (c.x - a.x),
		a.y + f * (b.y - a.y) + g * (c.y - a.y),
		a.z + f * (b.z - a.z) + g * (c.z - a.z),
		a.w + f * (b.w - a.w) + g * (c.w - a.w)
	);
}

void  K3D::Vector4::Barycentric(const K3D::Vector4 & a, const K3D::Vector4 & b, const K3D::Vector4 & c, const float f, const float g, K3D::Vector4 & result)
{
	result.x = a.x + f * (b.x - a.x) + g * (c.x - a.x);
	result.y = a.y + f * (b.y - a.y) + g * (c.y - a.y);
	result.z = a.z + f * (b.z - a.z) + g * (c.z - a.z);
	result.w = a.w + f * (b.w - a.w) + g * (c.w - a.w);
}

K3D::Vector4  K3D::Vector4::Hermite(const K3D::Vector4 & a, const K3D::Vector4 & t1, const K3D::Vector4 & b, const K3D::Vector4 & t2, const float amount)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	K3D::Vector4 result;
	if (amount <= 0.0f)
	{
		result.x = a.x;
		result.y = a.y;
		result.z = a.z;
		result.w = a.w;
	}
	else if (amount >= 1.0f)
	{
		result.x = b.x;
		result.y = b.y;
		result.z = b.z;
		result.w = b.w;
	}
	else
	{
		result.x = (2.0f * a.x - 2.0f * b.x + t2.x + t1.x) * c3 + (3.0f * b.x - 3.0f * a.x - 2.0f * t1.x - t2.x) * c2 + t1.x * amount + a.x;
		result.y = (2.0f * a.y - 2.0f * b.y + t2.y + t1.y) * c3 + (3.0f * b.y - 3.0f * a.y - 2.0f * t1.y - t2.y) * c2 + t1.y * amount + a.y;
		result.z = (2.0f * a.z - 2.0f * b.z + t2.z + t1.z) * c3 + (3.0f * b.z - 3.0f * a.z - 2.0f * t1.z - t2.z) * c2 + t1.y * amount + a.z;
		result.w = (2.0f * a.w - 2.0f * b.w + t2.w + t1.w) * c3 + (3.0f * b.w - 3.0f * a.w - 2.0f * t1.w - t2.w) * c2 + t1.w * amount + a.w;
	}
	return result;
}

void  K3D::Vector4::Hermite(const K3D::Vector4 & a, const K3D::Vector4 & t1, const K3D::Vector4 & b, const K3D::Vector4 & t2, const float amount, K3D::Vector4 & result)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	if (amount <= 0.0f)
	{
		result.x = a.x;
		result.y = a.y;
		result.z = a.z;
		result.w = a.w;
	}
	else if (amount >= 1.0f)
	{
		result.x = b.x;
		result.y = b.y;
		result.z = b.z;
		result.w = b.w;
	}
	else
	{
		result.x = (2.0f * a.x - 2.0f * b.x + t2.x + t1.x) * c3 + (3.0f * b.x - 3.0f * a.x - 2.0f * t1.x - t2.x) * c2 + t1.x * amount + a.x;
		result.y = (2.0f * a.y - 2.0f * b.y + t2.y + t1.y) * c3 + (3.0f * b.y - 3.0f * a.y - 2.0f * t1.y - t2.y) * c2 + t1.y * amount + a.y;
		result.z = (2.0f * a.z - 2.0f * b.z + t2.z + t1.z) * c3 + (3.0f * b.z - 3.0f * a.z - 2.0f * t1.z - t2.z) * c2 + t1.z * amount + a.z;
		result.w = (2.0f * a.w - 2.0f * b.w + t2.w + t1.w) * c3 + (3.0f * b.w - 3.0f * a.w - 2.0f * t1.w - t2.w) * c2 + t1.w * amount + a.w;
	}
}

K3D::Vector4  K3D::Vector4::CatmullRom(const K3D::Vector4 & a, const K3D::Vector4 & b, const K3D::Vector4 & c, const K3D::Vector4 & d, const float amount)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	return K3D::Vector4(
		(0.5f * (2.0f * b.x + (c.x - a.x) * amount + (2.0f * a.x - 5.0f * b.x + 4.0f * c.x - d.x) * c2 + (3.0f * b.x - a.x - 3.0f * c.x + d.x) * c3)),
		(0.5f * (2.0f * b.y + (c.y - a.y) * amount + (2.0f * a.y - 5.0f * b.y + 4.0f * c.y - d.y) * c2 + (3.0f * b.y - a.y - 3.0f * c.y + d.y) * c3)),
		(0.5f * (2.0f * b.z + (c.z - a.z) * amount + (2.0f * a.z - 5.0f * b.z + 4.0f * c.z - d.z) * c2 + (3.0f * b.z - a.z - 3.0f * c.z + d.z) * c3)),
		(0.5f * (2.0f * b.w + (c.w - a.w) * amount + (2.0f * a.w - 5.0f * b.w + 4.0f * c.w - d.w) * c2 + (3.0f * b.w - a.w - 3.0f * c.w + d.w) * c3))
	);
}

void  K3D::Vector4::CatmullRom(const K3D::Vector4 & a, const K3D::Vector4 & b, const K3D::Vector4 & c, const K3D::Vector4 & d, const float amount, K3D::Vector4 & result)
{
	auto c2 = amount * amount;
	auto c3 = c2 * amount;

	result.x = (0.5f * (2.0f * b.x + (c.x - a.x) * amount + (2.0f * a.x - 5.0f * b.x + 4.0f * c.x - d.x) * c2 + (3.0f * b.x - a.x - 3.0f * c.x + d.x) * c3));
	result.y = (0.5f * (2.0f * b.y + (c.y - a.y) * amount + (2.0f * a.y - 5.0f * b.y + 4.0f * c.y - d.y) * c2 + (3.0f * b.y - a.y - 3.0f * c.y + d.y) * c3));
	result.z = (0.5f * (2.0f * b.z + (c.z - a.z) * amount + (2.0f * a.z - 5.0f * b.z + 4.0f * c.z - d.z) * c2 + (3.0f * b.z - a.z - 3.0f * c.z + d.z) * c3));
	result.w = (0.5f * (2.0f * b.w + (c.w - a.w) * amount + (2.0f * a.w - 5.0f * b.w + 4.0f * c.w - d.w) * c2 + (3.0f * b.w - a.w - 3.0f * c.w + d.w) * c3));

}

K3D::Vector4  K3D::Vector4::Lerp(const K3D::Vector4 & a, const K3D::Vector4 & b, const float amount)
{
	return K3D::Vector4(
		a.x - amount * (a.x - b.x),
		a.y - amount * (a.y - b.y),
		a.z - amount * (a.z - b.z),
		a.w - amount * (a.w - b.w)
	);
}

void  K3D::Vector4::Lerp(const K3D::Vector4 & a, const K3D::Vector4 & b, const float amount, K3D::Vector4 & result)
{
	result.x = a.x - amount * (a.x - b.x);
	result.y = a.y - amount * (a.y - b.y);
	result.z = a.z - amount * (a.z - b.z);
	result.w = a.w - amount * (a.w - b.w);
}

K3D::Vector4  K3D::Vector4::SmoothStep(const K3D::Vector4 & a, const K3D::Vector4 & b, const float amount)
{
	float s = ::Clamp<float>(amount, 0.0f, 1.0f);
	float u = (s * s) + (3.0f - (2.0f * s));
	return K3D::Vector4(
		a.x - u * (a.x - b.x),
		a.y - u * (a.y - b.y),
		a.z - u * (a.z - b.z),
		a.w - u * (a.w - b.w)
	);
}

void  K3D::Vector4::SmoothStep(const K3D::Vector4 & a, const K3D::Vector4 & b, const float amount, K3D::Vector4 & result)
{
	float s = ::Clamp<float>(amount, 0.0f, 1.0f);
	float u = (s * s) + (3.0f - (2.0f * s));
	result.x = a.x - u * (a.x - b.x);
	result.y = a.y - u * (a.y - b.y);
	result.z = a.z - u * (a.z - b.z);
	result.w = a.w - u * (a.w - b.w);
}

K3D::Vector4  K3D::Vector4::Transform(const K3D::Vector4 & position, const K3D::Matrix & matrix)
{
	return K3D::Vector4(
		((((position.x * matrix._11) + (position.y * matrix._21)) + (position.z * matrix._31)) + (position.w * matrix._41)),
		((((position.x * matrix._12) + (position.y * matrix._22)) + (position.z * matrix._32)) + (position.w * matrix._42)),
		((((position.x * matrix._13) + (position.y * matrix._23)) + (position.z * matrix._33)) + (position.w * matrix._43)),
		((((position.x * matrix._14) + (position.y * matrix._24)) + (position.z * matrix._43)) + (position.w * matrix._44))
	);
}

void  K3D::Vector4::Transform(const K3D::Vector4 & position, const K3D::Matrix & matrix, K3D::Vector4 & result)
{
	result.x = ((((position.x * matrix._11) + (position.y * matrix._21)) + (position.z * matrix._31)) + (position.w * matrix._41));
	result.y = ((((position.x * matrix._12) + (position.y * matrix._22)) + (position.z * matrix._32)) + (position.w * matrix._42));
	result.z = ((((position.x * matrix._13) + (position.y * matrix._23)) + (position.z * matrix._33)) + (position.w * matrix._43));
	result.w = ((((position.x * matrix._14) + (position.y * matrix._24)) + (position.z * matrix._43)) + (position.w * matrix._44));
}


void K3D::Matrix::EpsilonCheck()
{
	for (unsigned int i = 0; i < 16; ++i) {
		if (IsZero(matrix[i / 4][i % 4])) {
			matrix[i / 4][i % 4] = 0.0f;
		}
	}
}

K3D::Matrix::Matrix()
{
	_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
	_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
	_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
	_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
}

K3D::Matrix::Matrix(const K3D::Matrix & mat) :
	axisX(mat.axisX), axisY(mat.axisY), axisZ(mat.axisZ), axisW(mat.axisW)
{

}

K3D::Matrix::Matrix(K3D::Matrix && mat)
{
	*this = std::move(mat);
}

K3D::Matrix::Matrix(const DirectX::XMMATRIX & mat)
{
	_11 = mat.r[0].m128_f32[0]; _12 = mat.r[0].m128_f32[1]; _13 = mat.r[0].m128_f32[2]; _14 = mat.r[0].m128_f32[3];
	_21 = mat.r[1].m128_f32[0]; _22 = mat.r[1].m128_f32[1]; _23 = mat.r[1].m128_f32[2]; _24 = mat.r[1].m128_f32[3];
	_31 = mat.r[2].m128_f32[0]; _32 = mat.r[2].m128_f32[1]; _33 = mat.r[2].m128_f32[2]; _34 = mat.r[2].m128_f32[3];
	_41 = mat.r[3].m128_f32[0]; _42 = mat.r[3].m128_f32[1]; _43 = mat.r[3].m128_f32[2]; _44 = mat.r[3].m128_f32[3];
}

K3D::Matrix::Matrix(DirectX::XMMATRIX && mat)
{
	*this = std::move(mat);
}

K3D::Matrix::Matrix(
	const float m11, const float m12, const float m13, const float m14,
	const float m21, const float m22, const float m23, const float m24,
	const float m31, const float m32, const float m33, const float m34,
	const float m41, const float m42, const float m43, const float m44)
{
	_11 = m11; _12 = m12; _13 = m13; _14 = m14;
	_21 = m21; _22 = m22; _23 = m23; _24 = m24;
	_31 = m31; _32 = m32; _33 = m33; _34 = m34;
	_41 = m41; _42 = m42; _43 = m43; _44 = m44;
}

K3D::Matrix::Matrix(const K3D::Vector4 & v1, const K3D::Vector4 & v2, const K3D::Vector4 & v3, const K3D::Vector4 & v4) :
	axisX(v1), axisY(v2), axisZ(v3), axisW(v4)
{
}

K3D::Matrix::Matrix(const DirectX::XMFLOAT4 & v1, const DirectX::XMFLOAT4 & v2, const DirectX::XMFLOAT4 & v3, const DirectX::XMFLOAT4 & v4) :
	axisX(v1), axisY(v2), axisZ(v3), axisW(v4)
{
}

K3D::Matrix::~Matrix()
{
}

float &  K3D::Matrix::operator()(unsigned int row, unsigned int col)
{
	return matrix[row][col];
}

const float &  K3D::Matrix::operator()(unsigned int row, unsigned int col) const
{
	return matrix[row][col];
}

K3D::Matrix &  K3D::Matrix::operator=(const K3D::Matrix & value)
{
	std::memcpy(&_11, &value._11, sizeof(K3D::Matrix));
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator=(K3D::Matrix && value)
{
	*this = value;
	value.axisX = std::move(K3D::Vector4(0.f, 0.f, 0.f, 0.f));
	value.axisY = std::move(K3D::Vector4(0.f, 0.f, 0.f, 0.f));
	value.axisZ = std::move(K3D::Vector4(0.f, 0.f, 0.f, 0.f));
	value.axisW = std::move(K3D::Vector4(0.f, 0.f, 0.f, 0.f));
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator=(const DirectX::XMMATRIX & value)
{
	std::memcpy(&_11, &value.r[0], sizeof(K3D::Matrix));
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator=(DirectX::XMMATRIX && value)
{
	*this = value;
	value.r[0].m128_f32[0] = 0.0f; value.r[0].m128_f32[1] = 0.0f; value.r[0].m128_f32[2] = 0.0f; value.r[0].m128_f32[3] = 0.0f;
	value.r[1].m128_f32[0] = 0.0f; value.r[1].m128_f32[1] = 0.0f; value.r[1].m128_f32[2] = 0.0f; value.r[1].m128_f32[3] = 0.0f;
	value.r[2].m128_f32[0] = 0.0f; value.r[2].m128_f32[1] = 0.0f; value.r[2].m128_f32[2] = 0.0f; value.r[2].m128_f32[3] = 0.0f;
	value.r[3].m128_f32[0] = 0.0f; value.r[3].m128_f32[1] = 0.0f; value.r[3].m128_f32[2] = 0.0f; value.r[3].m128_f32[3] = 0.0f;
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator*=(const K3D::Matrix & value)
{

	//DirectX::XMMATRIX;
	auto m11 = (_11 * value._11) + (_12 * value._21) + (_13 * value._31) + (_14 * value._41);
	auto m12 = (_11 * value._12) + (_12 * value._22) + (_13 * value._32) + (_14 * value._42);
	auto m13 = (_11 * value._13) + (_12 * value._23) + (_13 * value._33) + (_14 * value._43);
	auto m14 = (_11 * value._14) + (_12 * value._24) + (_13 * value._34) + (_14 * value._44);

	auto m21 = (_21 * value._11) + (_22 * value._21) + (_23 * value._31) + (_24 * value._41);
	auto m22 = (_21 * value._12) + (_22 * value._22) + (_23 * value._32) + (_24 * value._42);
	auto m23 = (_21 * value._13) + (_22 * value._23) + (_23 * value._33) + (_24 * value._43);
	auto m24 = (_21 * value._14) + (_22 * value._24) + (_23 * value._34) + (_24 * value._44);

	auto m31 = (_31 * value._11) + (_32 * value._21) + (_33 * value._31) + (_34 * value._41);
	auto m32 = (_31 * value._12) + (_32 * value._22) + (_33 * value._32) + (_34 * value._42);
	auto m33 = (_31 * value._13) + (_32 * value._23) + (_33 * value._33) + (_34 * value._43);
	auto m34 = (_31 * value._14) + (_32 * value._24) + (_33 * value._34) + (_34 * value._44);

	auto m41 = (_41 * value._11) + (_42 * value._21) + (_43 * value._31) + (_44 * value._41);
	auto m42 = (_41 * value._12) + (_42 * value._22) + (_43 * value._32) + (_44 * value._42);
	auto m43 = (_41 * value._13) + (_42 * value._23) + (_43 * value._33) + (_44 * value._43);
	auto m44 = (_41 * value._14) + (_42 * value._24) + (_43 * value._34) + (_44 * value._44);

	_11 = m11;  _12 = m12;  _13 = m13;  _14 = m14;
	_21 = m21;  _22 = m22;  _23 = m23;  _24 = m24;
	_31 = m31;  _32 = m32;  _33 = m33;  _34 = m34;
	_41 = m41;  _42 = m42;  _43 = m43;  _44 = m44;

	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator*=(const DirectX::XMMATRIX & value)
{
	auto matrix = K3D::Matrix(value);
	*this *= matrix;
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator+=(const K3D::Matrix & mat)
{
	_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
	_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
	_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
	_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator+=(const DirectX::XMMATRIX & value)
{
	auto matrix = K3D::Matrix(value);
	_11 += matrix._11;  _12 += matrix._12;  _13 += matrix._13;  _14 += matrix._14;
	_21 += matrix._21;  _22 += matrix._22;  _23 += matrix._23;  _24 += matrix._24;
	_31 += matrix._31;  _32 += matrix._32;  _33 += matrix._33;  _34 += matrix._34;
	_41 += matrix._41;  _42 += matrix._42;  _43 += matrix._43;  _44 += matrix._44;
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator-=(const K3D::Matrix & mat)
{
	_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
	_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
	_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
	_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator-=(const DirectX::XMMATRIX & value)
{
	auto matrix = K3D::Matrix(value);
	_11 -= matrix._11;  _12 -= matrix._12;  _13 -= matrix._13;  _14 -= matrix._14;
	_21 -= matrix._21;  _22 -= matrix._22;  _23 -= matrix._23;  _24 -= matrix._24;
	_31 -= matrix._31;  _32 -= matrix._32;  _33 -= matrix._33;  _34 -= matrix._34;
	_41 -= matrix._41;  _42 -= matrix._42;  _43 -= matrix._43;  _44 -= matrix._44;
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator*=(float scalar)
{
	_11 *= scalar; _12 *= scalar; _13 *= scalar; _14 *= scalar;
	_21 *= scalar; _22 *= scalar; _23 *= scalar; _24 *= scalar;
	_31 *= scalar; _32 *= scalar; _33 *= scalar; _34 *= scalar;
	_41 *= scalar; _42 *= scalar; _43 *= scalar; _44 *= scalar;
	return (*this);
}

K3D::Matrix &  K3D::Matrix::operator/=(float scalar)
{
	assert(!IsZero(scalar));
	_11 /= scalar; _12 /= scalar; _13 /= scalar; _14 /= scalar;
	_21 /= scalar; _22 /= scalar; _23 /= scalar; _24 /= scalar;
	_31 /= scalar; _32 /= scalar; _33 /= scalar; _34 /= scalar;
	_41 /= scalar; _42 /= scalar; _43 /= scalar; _44 /= scalar;
	return (*this);
}

K3D::Matrix  K3D::Matrix::operator+() const
{
	return (*this);
}

K3D::Matrix  K3D::Matrix::operator-() const
{
	return K3D::Matrix(
		-_11, -_12, -_13, -_14,
		-_21, -_22, -_23, -_24,
		-_31, -_32, -_33, -_34,
		-_41, -_42, -_43, -_44
	);
}

K3D::Matrix  K3D::Matrix::operator*(const K3D::Matrix & value) const
{
	return K3D::Matrix(
		(_11 * value._11) + (_12 * value._21) + (_13 * value._31) + (_14 * value._41),
		(_11 * value._12) + (_12 * value._22) + (_13 * value._32) + (_14 * value._42),
		(_11 * value._13) + (_12 * value._23) + (_13 * value._33) + (_14 * value._43),
		(_11 * value._14) + (_12 * value._24) + (_13 * value._34) + (_14 * value._44),

		(_21 * value._11) + (_22 * value._21) + (_23 * value._31) + (_24 * value._41),
		(_21 * value._12) + (_22 * value._22) + (_23 * value._32) + (_24 * value._42),
		(_21 * value._13) + (_22 * value._23) + (_23 * value._33) + (_24 * value._43),
		(_21 * value._14) + (_22 * value._24) + (_23 * value._34) + (_24 * value._44),

		(_31 * value._11) + (_32 * value._21) + (_33 * value._31) + (_34 * value._41),
		(_31 * value._12) + (_32 * value._22) + (_33 * value._32) + (_34 * value._42),
		(_31 * value._13) + (_32 * value._23) + (_33 * value._33) + (_34 * value._43),
		(_31 * value._14) + (_32 * value._24) + (_33 * value._34) + (_34 * value._44),

		(_41 * value._11) + (_42 * value._21) + (_43 * value._31) + (_44 * value._41),
		(_41 * value._12) + (_42 * value._22) + (_43 * value._32) + (_44 * value._42),
		(_41 * value._13) + (_42 * value._23) + (_43 * value._33) + (_44 * value._43),
		(_41 * value._14) + (_42 * value._24) + (_43 * value._34) + (_44 * value._44)
	);
}

K3D::Matrix  K3D::Matrix::operator*(const DirectX::XMMATRIX & value) const
{
	auto matrix = K3D::Matrix(value);
	return K3D::Matrix(
		(_11 * matrix._11) + (_12 * matrix._21) + (_13 * matrix._31) + (_14 * matrix._41),
		(_11 * matrix._12) + (_12 * matrix._22) + (_13 * matrix._32) + (_14 * matrix._42),
		(_11 * matrix._13) + (_12 * matrix._23) + (_13 * matrix._33) + (_14 * matrix._43),
		(_11 * matrix._14) + (_12 * matrix._24) + (_13 * matrix._34) + (_14 * matrix._44),

		(_21 * matrix._11) + (_22 * matrix._21) + (_23 * matrix._31) + (_24 * matrix._41),
		(_21 * matrix._12) + (_22 * matrix._22) + (_23 * matrix._32) + (_24 * matrix._42),
		(_21 * matrix._13) + (_22 * matrix._23) + (_23 * matrix._33) + (_24 * matrix._43),
		(_21 * matrix._14) + (_22 * matrix._24) + (_23 * matrix._34) + (_24 * matrix._44),

		(_31 * matrix._11) + (_32 * matrix._21) + (_33 * matrix._31) + (_34 * matrix._41),
		(_31 * matrix._12) + (_32 * matrix._22) + (_33 * matrix._32) + (_34 * matrix._42),
		(_31 * matrix._13) + (_32 * matrix._23) + (_33 * matrix._33) + (_34 * matrix._43),
		(_31 * matrix._14) + (_32 * matrix._24) + (_33 * matrix._34) + (_34 * matrix._44),

		(_41 * matrix._11) + (_42 * matrix._21) + (_43 * matrix._31) + (_44 * matrix._41),
		(_41 * matrix._12) + (_42 * matrix._22) + (_43 * matrix._32) + (_44 * matrix._42),
		(_41 * matrix._13) + (_42 * matrix._23) + (_43 * matrix._33) + (_44 * matrix._43),
		(_41 * matrix._14) + (_42 * matrix._24) + (_43 * matrix._34) + (_44 * matrix._44)
	);
}

K3D::Matrix  K3D::operator*(float scalar, const K3D::Matrix & mat)
{
	return K3D::Matrix(
		mat._11 * scalar, mat._12 * scalar, mat._13 * scalar, mat._14 * scalar,
		mat._21 * scalar, mat._22 * scalar, mat._23 * scalar, mat._24 * scalar,
		mat._31 * scalar, mat._32 * scalar, mat._33 * scalar, mat._34 * scalar,
		mat._41 * scalar, mat._42 * scalar, mat._43 * scalar, mat._44 * scalar
	);
}

K3D::Matrix  K3D::Matrix::operator+(const K3D::Matrix & mat) const
{
	return K3D::Matrix(
		_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
		_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
		_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
		_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44
	);
}

K3D::Matrix  K3D::Matrix::operator+(const DirectX::XMMATRIX & value) const
{
	auto mat = K3D::Matrix(value);
	return K3D::Matrix(
		_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
		_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
		_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
		_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44
	);
	return K3D::Matrix();
}

K3D::Matrix  K3D::Matrix::operator-(const K3D::Matrix & mat) const
{

	return K3D::Matrix(
		_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
		_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
		_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
		_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44
	);
}

K3D::Matrix  K3D::Matrix::operator-(const DirectX::XMMATRIX & value) const
{
	auto mat = K3D::Matrix(value);
	return K3D::Matrix(
		_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
		_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
		_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
		_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44
	);
}

K3D::Matrix  K3D::Matrix::operator*(float scalar)
{
	return K3D::Matrix(
		_11 * scalar, _12 * scalar, _13 * scalar, _14 * scalar,
		_21 * scalar, _22 * scalar, _23 * scalar, _24 * scalar,
		_31 * scalar, _32 * scalar, _33 * scalar, _34 * scalar,
		_41 * scalar, _42 * scalar, _43 * scalar, _44 * scalar
	);
}

K3D::Matrix  K3D::Matrix::operator/(float scalar)
{
	assert(!IsZero(scalar));
	return K3D::Matrix(
		_11 / scalar, _12 / scalar, _13 / scalar, _14 / scalar,
		_21 / scalar, _22 / scalar, _23 / scalar, _24 / scalar,
		_31 / scalar, _32 / scalar, _33 / scalar, _34 / scalar,
		_41 / scalar, _42 / scalar, _43 / scalar, _44 / scalar
	);
}

bool  K3D::Matrix::operator==(const K3D::Matrix & mat) const
{
	return (0 == memcmp(this, &mat, sizeof(K3D::Matrix)));
}

bool  K3D::Matrix::operator==(const DirectX::XMMATRIX & value) const
{
	return false;
}

bool  K3D::Matrix::operator!=(const K3D::Matrix & mat) const
{
	return (0 != memcmp(this, &mat, sizeof(K3D::Matrix)));
}

bool  K3D::Matrix::operator!=(const DirectX::XMMATRIX & value) const
{
	return false;
}

float  K3D::Matrix::Determinant() const
{
	float det =
		_11 * _22 * _33 * _44 + _11 * _23 * _34 * _42 +
		_11 * _24 * _32 * _43 + _12 * _21 * _34 * _43 +
		_12 * _23 * _31 * _44 + _12 * _24 * _33 * _41 +
		_13 * _21 * _32 * _44 + _13 * _22 * _34 * _41 +
		_13 * _24 * _31 * _42 + _14 * _21 * _33 * _42 +
		_14 * _22 * _31 * _43 + _14 * _23 * _32 * _41 -
		_11 * _22 * _34 * _43 - _11 * _23 * _32 * _44 -
		_11 * _24 * _33 * _42 - _12 * _21 * _33 * _44 -
		_12 * _23 * _34 * _41 - _12 * _24 * _31 * _43 -
		_13 * _21 * _34 * _42 - _13 * _22 * _31 * _44 -
		_13 * _24 * _32 * _41 - _14 * _21 * _32 * _43 -
		_14 * _22 * _33 * _41 - _14 * _23 * _31 * _42;

	return det;
}

K3D::Matrix &  K3D::Matrix::Identity()
{
	_11 = _22 = _33 = _44 = 1.0f;
	_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0.0f;
	return (*this);
}

K3D::Vector3 K3D::Matrix::Scale()
{
	return K3D::Vector3(
		axisX.x * axisX.x + axisX.y*axisX.y + axisX.z * axisX.z,
		axisY.x * axisY.x + axisY.y*axisY.y + axisY.z * axisY.z,
		axisZ.x * axisZ.x + axisZ.y*axisZ.y + axisZ.z * axisZ.z
	);
}

K3D::Vector3 K3D::Matrix::Movement()
{
	return K3D::Vector3(axisW.x, axisW.y, axisW.z);
}

K3D::Matrix  K3D::Matrix::CreateIdentity()
{
	return K3D::Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

bool  K3D::Matrix::IsIdentity(const K3D::Matrix &value)
{
	return (
		IsEqual(value.matrix[0][0], 1.0f) && IsZero(value.matrix[0][1]) && IsZero(value.matrix[0][2]) && IsZero(value.matrix[0][3]) &&
		IsZero(value.matrix[1][0]) && IsEqual(value.matrix[1][1], 1.0f) && IsZero(value.matrix[1][2]) && IsZero(value.matrix[1][3]) &&
		IsZero(value.matrix[2][0]) && IsZero(value.matrix[2][1]) && IsEqual(value.matrix[2][2], 1.0f) && IsZero(value.matrix[2][3]) &&
		IsZero(value.matrix[3][0]) && IsZero(value.matrix[3][1]) && IsZero(value.matrix[3][2]) && IsEqual(value.matrix[3][3], 1.0f)
		);
	return false;
}

K3D::Matrix  K3D::Matrix::Transpose(const K3D::Matrix & value)
{
	return K3D::Matrix(
		value._11, value._21, value._31, value._41,
		value._12, value._22, value._32, value._42,
		value._13, value._23, value._33, value._43,
		value._14, value._24, value._34, value._44
	);
}

void  K3D::Matrix::Transpose(const K3D::Matrix & value, K3D::Matrix & result)
{
	result._11 = value._11;
	result._12 = value._21;
	result._13 = value._31;
	result._14 = value._41;

	result._21 = value._12;
	result._22 = value._22;
	result._23 = value._32;
	result._24 = value._42;

	result._31 = value._13;
	result._32 = value._23;
	result._33 = value._33;
	result._34 = value._34;

	result._41 = value._14;
	result._42 = value._24;
	result._43 = value._34;
	result._44 = value._44;
}

K3D::Matrix  K3D::Matrix::Multiply(const K3D::Matrix & a, const K3D::Matrix & b)
{
	return K3D::Matrix(
		(a._11 * b._11) + (a._12 * b._21) + (a._13 * b._31) + (a._14 * b._41),
		(a._11 * b._12) + (a._12 * b._22) + (a._13 * b._32) + (a._14 * b._42),
		(a._11 * b._13) + (a._12 * b._23) + (a._13 * b._33) + (a._14 * b._43),
		(a._11 * b._14) + (a._12 * b._24) + (a._13 * b._34) + (a._14 * b._44),

		(a._21 * b._11) + (a._22 * b._21) + (a._23 * b._31) + (a._24 * b._41),
		(a._21 * b._12) + (a._22 * b._22) + (a._23 * b._32) + (a._24 * b._42),
		(a._21 * b._13) + (a._22 * b._23) + (a._23 * b._33) + (a._24 * b._43),
		(a._21 * b._14) + (a._22 * b._24) + (a._23 * b._34) + (a._24 * b._44),

		(a._31 * b._11) + (a._32 * b._21) + (a._33 * b._31) + (a._34 * b._41),
		(a._31 * b._12) + (a._32 * b._22) + (a._33 * b._32) + (a._34 * b._42),
		(a._31 * b._13) + (a._32 * b._23) + (a._33 * b._33) + (a._34 * b._43),
		(a._31 * b._14) + (a._32 * b._24) + (a._33 * b._34) + (a._34 * b._44),

		(a._41 * b._11) + (a._42 * b._21) + (a._43 * b._31) + (a._44 * b._41),
		(a._41 * b._12) + (a._42 * b._22) + (a._43 * b._32) + (a._44 * b._42),
		(a._41 * b._13) + (a._42 * b._23) + (a._43 * b._33) + (a._44 * b._43),
		(a._41 * b._14) + (a._42 * b._24) + (a._43 * b._34) + (a._44 * b._44)
	);
}

void  K3D::Matrix::Multiply(const K3D::Matrix & a, const K3D::Matrix & b, K3D::Matrix & result)
{
	result._11 = (a._11 * b._11) + (a._12 * b._21) + (a._13 * b._31) + (a._14 * b._41);
	result._12 = (a._11 * b._12) + (a._12 * b._22) + (a._13 * b._32) + (a._14 * b._42);
	result._13 = (a._11 * b._13) + (a._12 * b._23) + (a._13 * b._33) + (a._14 * b._43);
	result._14 = (a._11 * b._14) + (a._12 * b._24) + (a._13 * b._34) + (a._14 * b._44);

	result._21 = (a._21 * b._11) + (a._22 * b._21) + (a._23 * b._31) + (a._24 * b._41);
	result._22 = (a._21 * b._12) + (a._22 * b._22) + (a._23 * b._32) + (a._24 * b._42);
	result._23 = (a._21 * b._13) + (a._22 * b._23) + (a._23 * b._33) + (a._24 * b._43);
	result._24 = (a._21 * b._14) + (a._22 * b._24) + (a._23 * b._34) + (a._24 * b._44);

	result._31 = (a._31 * b._11) + (a._32 * b._21) + (a._33 * b._31) + (a._34 * b._41);
	result._32 = (a._31 * b._12) + (a._32 * b._22) + (a._33 * b._32) + (a._34 * b._42);
	result._33 = (a._31 * b._13) + (a._32 * b._23) + (a._33 * b._33) + (a._34 * b._43);
	result._34 = (a._31 * b._14) + (a._32 * b._24) + (a._33 * b._34) + (a._34 * b._44);

	result._41 = (a._41 * b._11) + (a._42 * b._21) + (a._43 * b._31) + (a._44 * b._41);
	result._42 = (a._41 * b._12) + (a._42 * b._22) + (a._43 * b._32) + (a._44 * b._42);
	result._43 = (a._41 * b._13) + (a._42 * b._23) + (a._43 * b._33) + (a._44 * b._43);
	result._44 = (a._41 * b._14) + (a._42 * b._24) + (a._43 * b._34) + (a._44 * b._44);
}

K3D::Matrix  K3D::Matrix::Multiply(const K3D::Matrix & value, const float scalar)
{
	return K3D::Matrix(
		value._11 * scalar, value._12 * scalar, value._13 * scalar, value._14 * scalar,
		value._21 * scalar, value._22 * scalar, value._23 * scalar, value._24 * scalar,
		value._31 * scalar, value._32 * scalar, value._33 * scalar, value._34 * scalar,
		value._41 * scalar, value._42 * scalar, value._43 * scalar, value._44 * scalar);
}

void  K3D::Matrix::Multiply(const K3D::Matrix & value, const float scalar, K3D::Matrix & result)
{
	result._11 = value._11 * scalar;
	result._12 = value._12 * scalar;
	result._13 = value._13 * scalar;
	result._14 = value._14 * scalar;

	result._21 = value._21 * scalar;
	result._22 = value._22 * scalar;
	result._23 = value._23 * scalar;
	result._24 = value._24 * scalar;

	result._31 = value._31 * scalar;
	result._32 = value._32 * scalar;
	result._33 = value._33 * scalar;
	result._34 = value._34 * scalar;

	result._41 = value._41 * scalar;
	result._42 = value._42 * scalar;
	result._43 = value._43 * scalar;
	result._44 = value._44 * scalar;
}

K3D::Matrix  K3D::Matrix::MultiplyTranspose(const K3D::Matrix & a, const K3D::Matrix & b)
{
	return K3D::Matrix(
		(a._11 * b._11) + (a._12 * b._21) + (a._13 * b._31) + (a._14 * b._41),
		(a._11 * b._12) + (a._12 * b._22) + (a._13 * b._32) + (a._14 * b._42),
		(a._11 * b._13) + (a._12 * b._23) + (a._13 * b._33) + (a._14 * b._43),
		(a._11 * b._14) + (a._12 * b._24) + (a._13 * b._34) + (a._14 * b._44),

		(a._21 * b._11) + (a._22 * b._21) + (a._23 * b._31) + (a._24 * b._41),
		(a._21 * b._12) + (a._22 * b._22) + (a._23 * b._32) + (a._24 * b._42),
		(a._21 * b._13) + (a._22 * b._23) + (a._23 * b._33) + (a._24 * b._43),
		(a._21 * b._14) + (a._22 * b._24) + (a._23 * b._34) + (a._24 * b._44),

		(a._31 * b._11) + (a._32 * b._21) + (a._33 * b._31) + (a._34 * b._41),
		(a._31 * b._12) + (a._32 * b._22) + (a._33 * b._32) + (a._34 * b._42),
		(a._31 * b._13) + (a._32 * b._23) + (a._33 * b._33) + (a._34 * b._43),
		(a._31 * b._14) + (a._32 * b._24) + (a._33 * b._34) + (a._34 * b._44),

		(a._41 * b._11) + (a._42 * b._21) + (a._43 * b._31) + (a._44 * b._41),
		(a._41 * b._12) + (a._42 * b._22) + (a._43 * b._32) + (a._44 * b._42),
		(a._41 * b._13) + (a._42 * b._23) + (a._43 * b._33) + (a._44 * b._43),
		(a._41 * b._14) + (a._42 * b._24) + (a._43 * b._34) + (a._44 * b._44)
	);
}

void  K3D::Matrix::MultiplyTranspose(const K3D::Matrix & a, const K3D::Matrix & b, K3D::Matrix & result)
{

	result._11 = (a._11 * b._11) + (a._12 * b._21) + (a._13 * b._31) + (a._14 * b._41);
	result._21 = (a._11 * b._12) + (a._12 * b._22) + (a._13 * b._32) + (a._14 * b._42);
	result._31 = (a._11 * b._13) + (a._12 * b._23) + (a._13 * b._33) + (a._14 * b._43);
	result._41 = (a._11 * b._14) + (a._12 * b._24) + (a._13 * b._34) + (a._14 * b._44);

	result._12 = (a._21 * b._11) + (a._22 * b._21) + (a._23 * b._31) + (a._24 * b._41);
	result._22 = (a._21 * b._12) + (a._22 * b._22) + (a._23 * b._32) + (a._24 * b._42);
	result._32 = (a._21 * b._13) + (a._22 * b._23) + (a._23 * b._33) + (a._24 * b._43);
	result._42 = (a._21 * b._14) + (a._22 * b._24) + (a._23 * b._34) + (a._24 * b._44);

	result._13 = (a._31 * b._11) + (a._32 * b._21) + (a._33 * b._31) + (a._34 * b._41);
	result._23 = (a._31 * b._12) + (a._32 * b._22) + (a._33 * b._32) + (a._34 * b._42);
	result._33 = (a._31 * b._13) + (a._32 * b._23) + (a._33 * b._33) + (a._34 * b._43);
	result._43 = (a._31 * b._14) + (a._32 * b._24) + (a._33 * b._34) + (a._34 * b._44);

	result._14 = (a._41 * b._11) + (a._42 * b._21) + (a._43 * b._31) + (a._44 * b._41);
	result._24 = (a._41 * b._12) + (a._42 * b._22) + (a._43 * b._32) + (a._44 * b._42);
	result._34 = (a._41 * b._13) + (a._42 * b._23) + (a._43 * b._33) + (a._44 * b._43);
	result._44 = (a._41 * b._14) + (a._42 * b._24) + (a._43 * b._34) + (a._44 * b._44);
}

K3D::Matrix  K3D::Matrix::Invert(const K3D::Matrix & value)
{
	auto det = value.Determinant();
	assert(!IsZero(det));

	auto m11 = value._22*value._33*value._44 + value._23*value._34*value._42 + value._24*value._32*value._43 - value._22*value._34*value._43 - value._23*value._32*value._44 - value._24*value._33*value._42;
	auto m12 = value._12*value._34*value._43 + value._13*value._32*value._44 + value._14*value._33*value._42 - value._12*value._33*value._44 - value._13*value._34*value._42 - value._14*value._32*value._43;
	auto m13 = value._12*value._23*value._44 + value._13*value._24*value._42 + value._14*value._22*value._43 - value._12*value._24*value._43 - value._13*value._22*value._44 - value._14*value._23*value._42;
	auto m14 = value._12*value._24*value._33 + value._13*value._22*value._34 + value._14*value._23*value._32 - value._12*value._23*value._34 - value._13*value._24*value._32 - value._14*value._22*value._33;

	auto m21 = value._21*value._34*value._43 + value._23*value._31*value._44 + value._24*value._33*value._41 - value._21*value._33*value._44 - value._23*value._34*value._41 - value._24*value._31*value._43;
	auto m22 = value._11*value._33*value._44 + value._13*value._34*value._41 + value._14*value._31*value._43 - value._11*value._34*value._43 - value._13*value._31*value._44 - value._14*value._33*value._41;
	auto m23 = value._11*value._24*value._43 + value._13*value._21*value._44 + value._14*value._23*value._41 - value._11*value._23*value._44 - value._13*value._24*value._41 - value._14*value._21*value._43;
	auto m24 = value._11*value._23*value._34 + value._13*value._24*value._31 + value._14*value._21*value._33 - value._11*value._24*value._33 - value._13*value._21*value._34 - value._14*value._23*value._31;

	auto m31 = value._21*value._32*value._44 + value._22*value._34*value._41 + value._24*value._31*value._42 - value._21*value._34*value._42 - value._22*value._31*value._44 - value._24*value._32*value._41;
	auto m32 = value._11*value._34*value._42 + value._12*value._31*value._44 + value._14*value._32*value._41 - value._11*value._32*value._44 - value._12*value._34*value._41 - value._14*value._31*value._42;
	auto m33 = value._11*value._22*value._44 + value._12*value._24*value._41 + value._14*value._21*value._42 - value._11*value._24*value._42 - value._12*value._21*value._44 - value._14*value._22*value._41;
	auto m34 = value._11*value._24*value._32 + value._12*value._21*value._34 + value._14*value._22*value._31 - value._11*value._22*value._34 - value._12*value._24*value._31 - value._14*value._21*value._32;

	auto m41 = value._21*value._33*value._42 + value._22*value._31*value._43 + value._23*value._32*value._41 - value._21*value._32*value._43 - value._22*value._33*value._41 - value._23*value._31*value._42;
	auto m42 = value._11*value._32*value._43 + value._12*value._33*value._41 + value._13*value._31*value._42 - value._11*value._33*value._42 - value._12*value._31*value._43 - value._13*value._32*value._41;
	auto m43 = value._11*value._23*value._42 + value._12*value._21*value._43 + value._13*value._22*value._41 - value._11*value._22*value._43 - value._12*value._23*value._41 - value._13*value._21*value._42;
	auto m44 = value._11*value._22*value._33 + value._12*value._23*value._31 + value._13*value._21*value._32 - value._11*value._23*value._32 - value._12*value._21*value._33 - value._13*value._22*value._31;

	return K3D::Matrix(
		m11 / det, m12 / det, m13 / det, m14 / det,
		m21 / det, m22 / det, m23 / det, m24 / det,
		m31 / det, m32 / det, m33 / det, m34 / det,
		m41 / det, m42 / det, m43 / det, m44 / det
	);
}

void  K3D::Matrix::Invert(const K3D::Matrix & value, K3D::Matrix & result)
{
	auto det = value.Determinant();
	assert(det != 0.0f);

	result._11 = value._22*value._33*value._44 + value._23*value._34*value._42 + value._24*value._32*value._43 - value._22*value._34*value._43 - value._23*value._32*value._44 - value._24*value._33*value._42;
	result._12 = value._12*value._34*value._43 + value._13*value._32*value._44 + value._14*value._33*value._42 - value._12*value._33*value._44 - value._13*value._34*value._42 - value._14*value._32*value._43;
	result._13 = value._12*value._23*value._44 + value._13*value._24*value._42 + value._14*value._22*value._43 - value._12*value._24*value._43 - value._13*value._22*value._44 - value._14*value._23*value._42;
	result._14 = value._12*value._24*value._33 + value._13*value._22*value._34 + value._14*value._23*value._32 - value._12*value._23*value._34 - value._13*value._24*value._32 - value._14*value._22*value._33;

	result._21 = value._21*value._34*value._43 + value._23*value._31*value._44 + value._24*value._33*value._41 - value._21*value._33*value._44 - value._23*value._34*value._41 - value._24*value._31*value._43;
	result._22 = value._11*value._33*value._44 + value._13*value._34*value._41 + value._14*value._31*value._43 - value._11*value._34*value._43 - value._13*value._31*value._44 - value._14*value._33*value._41;
	result._23 = value._11*value._24*value._43 + value._13*value._21*value._44 + value._14*value._23*value._41 - value._11*value._23*value._44 - value._13*value._24*value._41 - value._14*value._21*value._43;
	result._24 = value._11*value._23*value._34 + value._13*value._24*value._31 + value._14*value._21*value._33 - value._11*value._24*value._33 - value._13*value._21*value._34 - value._14*value._23*value._31;

	result._31 = value._21*value._32*value._44 + value._22*value._34*value._41 + value._24*value._31*value._42 - value._21*value._34*value._42 - value._22*value._31*value._44 - value._24*value._32*value._41;
	result._32 = value._11*value._34*value._42 + value._12*value._31*value._44 + value._14*value._32*value._41 - value._11*value._32*value._44 - value._12*value._34*value._41 - value._14*value._31*value._42;
	result._33 = value._11*value._22*value._44 + value._12*value._24*value._41 + value._14*value._21*value._42 - value._11*value._24*value._42 - value._12*value._21*value._44 - value._14*value._22*value._41;
	result._34 = value._11*value._24*value._32 + value._12*value._21*value._34 + value._14*value._22*value._31 - value._11*value._22*value._34 - value._12*value._24*value._31 - value._14*value._21*value._32;

	result._41 = value._21*value._33*value._42 + value._22*value._31*value._43 + value._23*value._32*value._41 - value._21*value._32*value._43 - value._22*value._33*value._41 - value._23*value._31*value._42;
	result._42 = value._11*value._32*value._43 + value._12*value._33*value._41 + value._13*value._31*value._42 - value._11*value._33*value._42 - value._12*value._31*value._43 - value._13*value._32*value._41;
	result._43 = value._11*value._23*value._42 + value._12*value._21*value._43 + value._13*value._22*value._41 - value._11*value._22*value._43 - value._12*value._23*value._41 - value._13*value._21*value._42;
	result._44 = value._11*value._22*value._33 + value._12*value._23*value._31 + value._13*value._21*value._32 - value._11*value._23*value._32 - value._12*value._21*value._33 - value._13*value._22*value._31;

	result._11 /= det;
	result._12 /= det;
	result._13 /= det;
	result._14 /= det;

	result._21 /= det;
	result._22 /= det;
	result._23 /= det;
	result._24 /= det;

	result._31 /= det;
	result._32 /= det;
	result._33 /= det;
	result._34 /= det;

	result._41 /= det;
	result._42 /= det;
	result._43 /= det;
	result._44 /= det;
}

K3D::Matrix  K3D::Matrix::CreateScaleMatrix(const float scale)
{

	return K3D::Matrix(
		scale, 0.0f, 0.0f, 0.0f,
		0.0f, scale, 0.0f, 0.0f,
		0.0f, 0.0f, scale, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void  K3D::Matrix::CreateScaleMatrix(const float scale, K3D::Matrix &result)
{
	result._11 = scale;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = scale;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = scale;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateScaleMatrix(const float sx, const float sy, const float sz)
{
	return K3D::Matrix(
		sx, 0.0f, 0.0f, 0.0f,
		0.0f, sy, 0.0f, 0.0f,
		0.0f, 0.0f, sz, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void  K3D::Matrix::CreateScaleMatrix(const float sx, const float sy, const float sz, K3D::Matrix & result)
{
	result._11 = sx;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = sy;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = sz;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateScaleMatrix(const K3D::Vector3 & scale)
{
	return K3D::Matrix(
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void  K3D::Matrix::CreateScaleMatrix(const K3D::Vector3 & scale, K3D::Matrix & result)
{
	result._11 = scale.x;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = scale.y;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = scale.z;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateTranslationMatrix(const float tx, const float ty, const float tz)
{

	return K3D::Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		tx, ty, tz, 1.0f);
}

void  K3D::Matrix::CreateTranslationMatrix(const float tx, const float ty, const float tz, K3D::Matrix & result)
{
	result._11 = 1.0f;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 1.0f;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = 1.0f;
	result._34 = 0.0f;

	result._41 = tx;
	result._42 = ty;
	result._43 = tz;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateTranslationMatrix(const K3D::Vector3 & pos)
{
	return K3D::Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f);
}

void  K3D::Matrix::CreateTranslationMatrix(const K3D::Vector3 & pos, K3D::Matrix & result)
{
	result._11 = 1.0f;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 1.0f;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = 1.0f;
	result._34 = 0.0f;

	result._41 = pos.x;
	result._42 = pos.y;
	result._43 = pos.z;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateRotationX(const float rad)
{
	auto cosRad = std::cosf(rad);
	auto sinRad = std::sinf(rad);
	return K3D::Matrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosRad, sinRad, 0.0f,
		0.0f, -sinRad, cosRad, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void  K3D::Matrix::CreateRotationX(const float rad, K3D::Matrix & result)
{
	auto cosRad = std::cosf(rad);
	auto sinRad = std::sinf(rad);

	result._11 = 1.0f;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = cosRad;
	result._23 = sinRad;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = -sinRad;
	result._33 = cosRad;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateRotationY(const float rad)
{
	auto cosRad = cosf(rad);
	auto sinRad = sinf(rad);

	return K3D::Matrix(
		cosRad, 0.0f, -sinRad, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinRad, 0.0f, cosRad, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void  K3D::Matrix::CreateRotationY(const float rad, K3D::Matrix & result)
{
	auto cosRad = cosf(rad);
	auto sinRad = sinf(rad);

	result._11 = cosRad;
	result._12 = 0.0f;
	result._13 = -sinRad;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 1.0f;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = sinRad;
	result._32 = 0.0f;
	result._33 = cosRad;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateRotationZ(const float rad)
{
	auto cosRad = cosf(rad);
	auto sinRad = sinf(rad);

	return K3D::Matrix(
		cosRad, sinRad, 0.0f, 0.0f,
		-sinRad, cosRad, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
}

void  K3D::Matrix::CreateRotationZ(const float rad, K3D::Matrix & result)
{
	auto cosRad = cosf(rad);
	auto sinRad = sinf(rad);

	result._11 = cosRad;
	result._12 = sinRad;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = -sinRad;
	result._22 = cosRad;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = 1.0f;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateFromQuaternion(const K3D::Quaternion & q)
{
	auto xx = q.x * q.x;
	auto yy = q.y * q.y;
	auto zz = q.z * q.z;

	auto xy = q.x * q.y;
	auto yw = q.y * q.w;
	auto yz = q.y * q.z;
	auto xw = q.x * q.w;
	auto zx = q.z * q.x;
	auto zw = q.z * q.w;

	auto ret = K3D::Matrix(
		1.0f - 2.0f * (yy + zz),
		2.0f * (xy + zw),
		2.0f * (zx - yw),
		0.0f,

		2.0f * (xy - zw),
		1.0f - 2.0f * (xx + zz),
		2.0f * (yz + xw),
		0.0f,

		2.0f * (zx + yw),
		2.0f * (yz - xw),
		1.0f - 2.0f * (xx + yy),
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f);
	//ret.EpsilonCheck();
	return ret;
}

void  K3D::Matrix::CreateFromQuaternion(const K3D::Quaternion & q, K3D::Matrix & result)
{
	auto xx = q.x * q.x;
	auto yy = q.y * q.y;
	auto zz = q.z * q.z;
	auto xy = q.x * q.y;
	auto yw = q.y * q.w;
	auto yz = q.y * q.z;
	auto xw = q.x * q.w;
	auto zx = q.z * q.x;
	auto zw = q.z * q.w;

	result._11 = 1.0f - (2.0f * (yy - zz));
	result._12 = 2.0f * (xy + zw);
	result._13 = 2.0f * (zx - yw);
	result._14 = 0.0f;

	result._21 = 2.0f * (xy - zw);
	result._22 = 1.0f - (2.0f * (xx - zz));
	result._23 = 2.0f * (yz + xw);
	result._24 = 0.0f;

	result._31 = 2.0f * (zx + yw);
	result._32 = 2.0f * (yz - xw);
	result._33 = 1.0f - (2.0f * (xx - yy));
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;

	//result.EpsilonCheck();
}

K3D::Matrix  K3D::Matrix::CreateFromAxisAngles(const K3D::Vector3 & axis, float rad)
{
	float sinRad = sinf(rad);
	float cosRad = cosf(rad);
	//����0�̋ߎ��l�Ȃ��

	if (IsZero(sinRad)) {
		sinRad = 0.0f;
	}
	if (IsZero(cosRad)) {
		cosRad = 0.0f;
	}

	float xy = axis.x * axis.y;
	float yz = axis.y * axis.z;
	float zx = axis.z * axis.x;

	float xx = axis.x * axis.x;
	float yy = axis.y * axis.y;
	float zz = axis.z * axis.z;

	float a = (1.0f - cosRad);

	return K3D::Matrix(
		xx * a + cosRad,
		xy * a + axis.z * sinRad,
		zx * a - axis.y * sinRad,
		0.0f,

		xy * a - axis.z * sinRad,
		yy * a + cosRad,
		yz * a + axis.x * sinRad,
		0.0f,

		zx * a + axis.y * sinRad,
		yz * a - axis.x * sinRad,
		zz * a + cosRad,
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		1.0f);

}

void  K3D::Matrix::CreateFromAxisAngles(const K3D::Vector3 & axis, float rad, K3D::Matrix & result)
{
	auto sinRad = sinf(rad);
	auto cosRad = cosf(rad);
	//����0�̋ߎ��l�Ȃ��
	if (IsZero(sinRad)) {
		sinRad = 0.0f;
	}
	if (IsZero(cosRad)) {
		cosRad = 0.0f;
	}

	float xy = axis.x * axis.y;
	float yz = axis.y * axis.z;
	float zx = axis.z * axis.x;
	float tx = axis.x * axis.x;
	float ty = axis.y * axis.y;
	float tz = axis.z * axis.z;

	result._11 = tx * (1.0f - cosRad) + cosRad;
	result._12 = xy * (1.0f - cosRad) + axis.z * sinRad;
	result._13 = zx * (1.0f - cosRad) - axis.y * sinRad;
	result._14 = 0.0f;

	result._21 = xy * (1.0f - cosRad) - axis.z * sinRad;
	result._22 = ty * (1.0f - cosRad) + cosRad;
	result._23 = yz * (1.0f - cosRad) + axis.x * sinRad;
	result._24 = 0.0f;

	result._31 = zx * (1.0f - cosRad) + axis.y * sinRad;
	result._32 = yz * (1.0f - cosRad) - axis.x * sinRad;
	result._33 = tz * (1.0f - cosRad) + cosRad;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = 0.0f;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll)
{
	auto value = K3D::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	return K3D::Matrix::CreateFromQuaternion(value);
}

void  K3D::Matrix::CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll, K3D::Matrix & result)
{
	auto value = K3D::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
	K3D::Matrix::CreateFromQuaternion(value, result);
}

K3D::Matrix  K3D::Matrix::CreateLookAt(const K3D::Vector3 & position, const K3D::Vector3 & target, const K3D::Vector3 & upward)
{
	auto zaxis = target - position;
	zaxis.Normalize();

	auto xaxis = K3D::Vector3::Cross(upward, zaxis);
	xaxis.Normalize();

	auto yaxis = K3D::Vector3::Cross(zaxis, xaxis);
	yaxis.Normalize();

	return K3D::Matrix(
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		-K3D::Vector3::Dot(xaxis, position),
		-K3D::Vector3::Dot(yaxis, position),
		-K3D::Vector3::Dot(zaxis, position),
		1.0f);
}

void  K3D::Matrix::CreateLookAt(const K3D::Vector3 & position, const K3D::Vector3 & target, const K3D::Vector3 & upward, K3D::Matrix & result)
{
	auto zaxis = target - position;
	zaxis.Normalize();

	auto xaxis = K3D::Vector3::Cross(upward, zaxis);
	xaxis.Normalize();

	auto yaxis = K3D::Vector3::Cross(zaxis, xaxis);
	yaxis.Normalize();

	result._11 = xaxis.x;
	result._12 = yaxis.x;
	result._13 = zaxis.x;
	result._14 = 0.0f;

	result._21 = xaxis.y;
	result._22 = yaxis.y;
	result._23 = zaxis.y;
	result._24 = 0.0f;

	result._31 = xaxis.z;
	result._32 = yaxis.z;
	result._33 = zaxis.z;
	result._34 = 0.0f;

	result._41 = -K3D::Vector3::Dot(xaxis, position);
	result._42 = -K3D::Vector3::Dot(yaxis, position);
	result._43 = -K3D::Vector3::Dot(zaxis, position);
	result._44 = 1.0f;
}

K3D::Matrix K3D::Matrix::CreateLookAtObject(const K3D::Vector3 & position, const K3D::Vector3 & target, const K3D::Vector3 & upward, const K3D::Vector3 & rightward)
{
	K3D::Vector3 fixAxia = {};
	fixAxia = upward;

	auto zaxis = target - position;
	zaxis.Normalize();
	auto xaxis = K3D::Vector3::Cross(fixAxia, zaxis);
	xaxis.Normalize();
	auto yaxis = K3D::Vector3::Cross(zaxis, xaxis);
	yaxis.Normalize();
	//�������ڐ��UpVectro���ꏏ�Ȃ�Ί�{�Œ莲��ύX,����ю��̍Đ���
	if (zaxis == upward) {
		fixAxia = rightward;
		auto yaxis = K3D::Vector3::Cross(zaxis, fixAxia);
		yaxis.Normalize();
		auto xaxis = K3D::Vector3::Cross(yaxis, zaxis);
		xaxis.Normalize();
	}


	return ExtractRotationMatrix(K3D::Matrix(
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		-K3D::Vector3::Dot(xaxis, position),
		-K3D::Vector3::Dot(yaxis, position),
		-K3D::Vector3::Dot(zaxis, position),
		1.0f));
}

void K3D::Matrix::CreateLookAtObject(const K3D::Vector3 & position, const K3D::Vector3 & target, const K3D::Vector3 & upward, const K3D::Vector3 & rightward, K3D::Matrix & result)
{
	K3D::Vector3 fixAxia = {};
	fixAxia = upward;

	auto zaxis = target - position;
	zaxis.Normalize();
	auto xaxis = K3D::Vector3::Cross(fixAxia, zaxis);
	xaxis.Normalize();
	auto yaxis = K3D::Vector3::Cross(zaxis, xaxis);
	yaxis.Normalize();
	//�������ڐ��UpVectro���ꏏ�Ȃ�Ί�{�Œ莲��ύX,����ю��̍Đ���
	if (zaxis == upward) {
		fixAxia = rightward;
		auto yaxis = K3D::Vector3::Cross(zaxis, fixAxia);
		yaxis.Normalize();
		auto xaxis = K3D::Vector3::Cross(yaxis, zaxis);
		xaxis.Normalize();
	}

	result._11 = xaxis.x;
	result._12 = yaxis.x;
	result._13 = zaxis.x;
	result._14 = 0.0f;

	result._21 = xaxis.y;
	result._22 = yaxis.y;
	result._23 = zaxis.y;
	result._24 = 0.0f;

	result._31 = xaxis.z;
	result._32 = yaxis.z;
	result._33 = zaxis.z;
	result._34 = 0.0f;

	result._41 = -K3D::Vector3::Dot(xaxis, position);
	result._42 = -K3D::Vector3::Dot(yaxis, position);
	result._43 = -K3D::Vector3::Dot(zaxis, position);
	result._44 = 1.0f;

	result = ExtractRotationMatrix(result);
}

K3D::Matrix  K3D::Matrix::CreateLookTo(const K3D::Vector3 & position, const K3D::Vector3 & viewDir, const K3D::Vector3 & upward)
{
	auto zaxis = viewDir;
	zaxis.Normalize();

	auto xaxis = K3D::Vector3::Cross(upward, zaxis);
	xaxis.Normalize();

	auto yaxis = K3D::Vector3::Cross(zaxis, xaxis);
	yaxis.Normalize();

	return K3D::Matrix(
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		-K3D::Vector3::Dot(xaxis, position),
		-K3D::Vector3::Dot(yaxis, position),
		-K3D::Vector3::Dot(zaxis, position),
		1.0f);
}

void  K3D::Matrix::CreateLookTo(const K3D::Vector3 & position, const K3D::Vector3 & viewDir, const K3D::Vector3 & upward, K3D::Matrix & result)
{
	auto zaxis = viewDir;
	zaxis.Normalize();

	auto xaxis = K3D::Vector3::Cross(upward, zaxis);
	xaxis.Normalize();

	auto yaxis = K3D::Vector3::Cross(zaxis, xaxis);
	yaxis.Normalize();

	result._11 = xaxis.x;
	result._12 = yaxis.x;
	result._13 = zaxis.x;
	result._14 = 0.0f;

	result._21 = xaxis.y;
	result._22 = yaxis.y;
	result._23 = zaxis.y;
	result._24 = 0.0f;

	result._31 = xaxis.z;
	result._32 = yaxis.z;
	result._33 = zaxis.z;
	result._34 = 0.0f;

	result._41 = -K3D::Vector3::Dot(xaxis, position);
	result._42 = -K3D::Vector3::Dot(yaxis, position);
	result._43 = -K3D::Vector3::Dot(zaxis, position);
	result._44 = 1.0f;

	//result = ExtractRotationMatrix(result);
}

K3D::Matrix K3D::Matrix::ExtractRotationMatrix(K3D::Matrix view)
{
	view.axisW = K3D::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	//�]�u
	return Transpose(view);
}

void K3D::Matrix::ExtractRotationMatrix(K3D::Matrix view, K3D::Matrix & result)
{
	view.axisW = K3D::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	//�]�u
	result = Transpose(view);
}

K3D::Matrix  K3D::Matrix::CreatePerspective(const float width, const float height, const float nearClip, const float farClip)
{
	assert(!IsZero(width));
	assert(!IsZero(height));
	assert(!IsZero(farClip - nearClip));
	auto range = farClip / farClip - nearClip;

	return K3D::Matrix(
		2.0f * nearClip / width,
		0.0f,
		0.0f,
		0.0f,

		0.0f,
		2.0f * nearClip / height,
		0.0f,
		0.0f,

		0.0f,
		0.0f,
		range,
		1.0f,

		0.0f,
		0.0f,
		range * -nearClip,
		0.0f);
}

void  K3D::Matrix::CreatePerspective(const float width, const float height, const float nearClip, const float farClip, K3D::Matrix & result)
{
	assert(!IsZero(width));
	assert(!IsZero(height));
	assert(!IsZero(farClip - nearClip));
	auto range = farClip / (farClip - nearClip);

	result._11 = 2.0f * nearClip / width;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 2.0f * nearClip / height;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = range;
	result._34 = 1.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = range * nearClip;
	result._44 = 0.0f;
}

K3D::Matrix  K3D::Matrix::CreatePerspectiveFOV(const float fieldOfView, const float aspectRatio, const float nearClip, const float farClip)
{
	assert(!IsZero(aspectRatio));
	assert(!IsZero(farClip - nearClip));
	auto sinFov = std::sin(0.5f * fieldOfView);
	auto cosFov = std::cos(0.5f * fieldOfView);
	auto height = cosFov / sinFov;
	auto width = height / aspectRatio;
	auto range = farClip / (farClip - nearClip);

	return K3D::Matrix(
		width,
		0.0f,
		0.0f,
		0.0f,

		0.0f,
		height,
		0.0f,
		0.0f,

		0.0f,
		0.0f,
		range,
		1.0f,

		0.0f,
		0.0f,
		range * -nearClip,
		0.0f);
}

void  K3D::Matrix::CreatePerspectiveFOV(const float fieldOfView, const float aspectRatio, const float nearClip, const float farClip, K3D::Matrix & result)
{
	assert(!IsZero(aspectRatio));
	assert(!IsZero(farClip - nearClip));
	auto sinFov = std::sin(0.5f * fieldOfView);
	auto cosFov = std::cos(0.5f * fieldOfView);
	auto height = cosFov / sinFov;
	auto width = height / aspectRatio;
	auto range = farClip / (farClip - nearClip);

	result._11 = width;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = height;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = range;
	result._34 = 1.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = range * -nearClip;
	result._44 = 0.0f;
}

K3D::Matrix  K3D::Matrix::CreatePerspectiveOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip)
{
	auto width = right - left;
	auto height = top - bottom;
	auto depth = farClip - nearClip;
	assert(!IsZero(width));
	assert(!IsZero(height));
	assert(!IsZero(depth));

	return K3D::Matrix(
		2.0f * nearClip / width,
		0.0f,
		0.0f,
		0.0f,

		0.0f,
		2.0f * nearClip / height,
		0.0f,
		0.0f,

		(left + right) / width,
		(top + bottom) / height,
		farClip / depth,
		1.0f,

		0.0f,
		0.0f,
		-nearClip * farClip / depth,
		0.0f);
}

void  K3D::Matrix::CreatePerspectiveOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip, K3D::Matrix & result)
{
	auto width = right - left;
	auto height = top - bottom;
	auto depth = farClip - nearClip;
	assert(!IsZero(width));
	assert(!IsZero(height));
	assert(!IsZero(depth));

	result._11 = 2.0f * nearClip / width;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 2.0f * nearClip / height;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = (left + right) / width;
	result._32 = (top + bottom) / height;
	result._33 = farClip / depth;
	result._34 = 1.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = nearClip * farClip / depth;
	result._44 = 0.0f;
}

K3D::Matrix  K3D::Matrix::CreateOrthographic(const float width, const float height, const float nearClip, const float farClip)
{
	assert(!IsZero(width));
	assert(!IsZero(height));
	assert(!IsZero(farClip - nearClip));
	auto range = 1.0f / (farClip - nearClip);

	return K3D::Matrix(
		2.0f / width,
		0.0f,
		0.0f,
		0.0f,

		0.0f,
		2.0f / height,
		0.0f,
		0.0f,

		0.0f,
		0.0f,
		range,
		0.0f,

		0.0f,
		0.0f,
		range * -nearClip,
		1.0f);
}

void  K3D::Matrix::CreateOrthographic(const float width, const float height, const float nearClip, const float farClip, K3D::Matrix & result)
{
	assert(!IsZero(width));
	assert(!IsZero(height));
	assert(!IsZero(farClip - nearClip));
	auto range = 1.0f / (farClip - nearClip);

	result._11 = 2.0f / width;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 2.0f / height;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = range;
	result._34 = 0.0f;

	result._41 = 0.0f;
	result._42 = 0.0f;
	result._43 = range * -nearClip;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::CreateOrthographicOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip)
{
	auto width = right - left;
	auto height = bottom - top;
	auto depth = farClip - nearClip;
	assert(!IsZero(width));
	assert(!IsZero(height));
	assert(!IsZero(depth));

	return K3D::Matrix(
		2.0f / width,
		0.0f,
		0.0f,
		0.0f,

		0.0f,
		2.0f / height,
		0.0f,
		0.0f,

		0.0f,
		0.0f,
		1.0f / depth,
		0.0f,

		(left + right) / width,
		(top + bottom) / height,
		nearClip / depth,
		1.0f
	);
}

void  K3D::Matrix::CreateOrthographicOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip, K3D::Matrix & result)
{
	auto width = right - left;
	auto height = bottom - top;
	auto depth = farClip - nearClip;
	assert(!IsZero(width));
	assert(!IsZero(height));
	assert(!IsZero(depth));

	result._11 = 2.0f / width;
	result._12 = 0.0f;
	result._13 = 0.0f;
	result._14 = 0.0f;

	result._21 = 0.0f;
	result._22 = 2.0f / height;
	result._23 = 0.0f;
	result._24 = 0.0f;

	result._31 = 0.0f;
	result._32 = 0.0f;
	result._33 = 1.0f / depth;
	result._34 = 0.0f;

	result._41 = (left + right) / width;
	result._42 = (top + bottom) / height;
	result._43 = nearClip / depth;
	result._44 = 1.0f;
}

K3D::Matrix  K3D::Matrix::Lerp(const K3D::Matrix & a, const K3D::Matrix & b, const float amount)
{
	return K3D::Matrix(
		a._11 - amount * (a._11 - b._11),
		a._12 - amount * (a._12 - b._12),
		a._13 - amount * (a._13 - b._13),
		a._14 - amount * (a._14 - b._14),

		a._21 - amount * (a._21 - b._21),
		a._22 - amount * (a._22 - b._22),
		a._23 - amount * (a._23 - b._23),
		a._24 - amount * (a._24 - b._24),

		a._31 - amount * (a._31 - b._31),
		a._32 - amount * (a._32 - b._32),
		a._33 - amount * (a._33 - b._33),
		a._34 - amount * (a._34 - b._34),

		a._41 - amount * (a._41 - b._41),
		a._42 - amount * (a._42 - b._42),
		a._43 - amount * (a._43 - b._43),
		a._44 - amount * (a._44 - b._44)
	);
}

void  K3D::Matrix::Lerp(const K3D::Matrix & a, const K3D::Matrix & b, const float amount, K3D::Matrix & result)
{
	result._11 = a._11 - amount * (a._11 - b._11);
	result._12 = a._12 - amount * (a._12 - b._12);
	result._13 = a._13 - amount * (a._13 - b._13);
	result._14 = a._14 - amount * (a._14 - b._14);

	result._21 = a._21 - amount * (a._21 - b._21);
	result._22 = a._22 - amount * (a._22 - b._22);
	result._23 = a._23 - amount * (a._23 - b._23);
	result._24 = a._24 - amount * (a._24 - b._24);

	result._31 = a._31 - amount * (a._31 - b._31);
	result._32 = a._32 - amount * (a._32 - b._32);
	result._33 = a._33 - amount * (a._33 - b._33);
	result._34 = a._34 - amount * (a._34 - b._34);

	result._41 = a._41 - amount * (a._41 - b._41);
	result._42 = a._42 - amount * (a._42 - b._42);
	result._43 = a._43 - amount * (a._43 - b._43);
	result._44 = a._44 - amount * (a._44 - b._44);
}

DirectX::XMMATRIX  K3D::Matrix::Store(const K3D::Matrix & mat)
{
	DirectX::XMMATRIX matrix;
	matrix.r[0].m128_f32[0] = mat._11;  matrix.r[0].m128_f32[1] = mat._12;  matrix.r[0].m128_f32[2] = mat._13;  matrix.r[0].m128_f32[3] = mat._14;
	matrix.r[1].m128_f32[0] = mat._21;  matrix.r[1].m128_f32[1] = mat._22;  matrix.r[1].m128_f32[2] = mat._23;  matrix.r[1].m128_f32[3] = mat._24;
	matrix.r[2].m128_f32[0] = mat._31;  matrix.r[2].m128_f32[1] = mat._32;  matrix.r[2].m128_f32[2] = mat._33;  matrix.r[2].m128_f32[3] = mat._34;
	matrix.r[3].m128_f32[0] = mat._41;  matrix.r[3].m128_f32[1] = mat._42;  matrix.r[3].m128_f32[2] = mat._43;  matrix.r[3].m128_f32[3] = mat._44;

	return matrix;
}

void  K3D::Matrix::Store(const K3D::Matrix & mat, DirectX::XMMATRIX & result)
{
	result.r[0].m128_f32[0] = mat._11;  result.r[0].m128_f32[1] = mat._12;  result.r[0].m128_f32[2] = mat._13;  result.r[0].m128_f32[3] = mat._14;
	result.r[1].m128_f32[0] = mat._21;  result.r[1].m128_f32[1] = mat._22;  result.r[1].m128_f32[2] = mat._23;  result.r[1].m128_f32[3] = mat._24;
	result.r[2].m128_f32[0] = mat._31;  result.r[2].m128_f32[1] = mat._32;  result.r[2].m128_f32[2] = mat._33;  result.r[2].m128_f32[3] = mat._34;
	result.r[3].m128_f32[0] = mat._41;  result.r[3].m128_f32[1] = mat._42;  result.r[3].m128_f32[2] = mat._43;  result.r[3].m128_f32[3] = mat._44;
}

K3D::Quaternion::Quaternion() :
	x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{

}

K3D::Quaternion::Quaternion(const K3D::Quaternion & other) :
	x(other.x), y(other.y), z(other.z), w(other.w)
{
}

K3D::Quaternion::Quaternion(K3D::Quaternion && other)
{
	*this = std::move(other);
}

K3D::Quaternion::Quaternion(const K3D::Vector4 & other) :
	x(other.x), y(other.y), z(other.z), w(other.w)
{
}

K3D::Quaternion::Quaternion(K3D::Vector4 && other)
{
}

K3D::Quaternion::Quaternion(const float x, const float y, const float z, const float w) :
	x(x), y(y), z(z), w(w)
{}

K3D::Quaternion &  K3D::Quaternion::operator=(const K3D::Quaternion & value)
{
	x = value.x;
	y = value.y;
	z = value.z;
	w = value.w;
	return (*this);
}

K3D::Quaternion &  K3D::Quaternion::operator=(K3D::Quaternion && value)
{
	*this = value;

	value.x = 0.0f;
	value.y = 0.0f;
	value.z = 0.0f;
	value.w = 0.0f;

	return (*this);
}

K3D::Quaternion & K3D::Quaternion::operator=(const K3D::Vector4 & value)
{
	x = value.x;
	y = value.y;
	z = value.z;
	w = value.w;
	return (*this);
}

K3D::Quaternion & K3D::Quaternion::operator=(K3D::Vector4 && value)
{
	*this = value;

	value.x = 0.0f;
	value.y = 0.0f;
	value.z = 0.0f;
	value.w = 0.0f;

	return (*this);
}

K3D::Quaternion &  K3D::Quaternion::operator+=(const K3D::Quaternion & value)
{
	x += value.x;
	y += value.y;
	z += value.z;
	w += value.w;
	return (*this);
}

K3D::Quaternion &  K3D::Quaternion::operator-=(const K3D::Quaternion & value)
{

	x -= value.x;
	y -= value.y;
	z -= value.z;
	w -= value.w;
	return (*this);
}

K3D::Quaternion &  K3D::Quaternion::operator*=(const K3D::Quaternion & value)
{
	auto W = (value.w * w) - (value.x * x) - (value.y * y) - (value.z * z);
	auto X = (value.x * w) + (x * value.w) + (value.y * z) - (value.z * y);
	auto Y = (value.y * w) + (y * value.w) + (value.z * x) - (value.x * z);
	auto Z = (value.z * w) + (z * value.w) + (value.x * y) - (value.y * x);
	x = X;
	y = Y;
	z = Z;
	w = W;
	return (*this);
}

K3D::Quaternion &  K3D::Quaternion::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return (*this);
}

K3D::Quaternion &  K3D::Quaternion::operator/=(float scalar)
{
	assert(!IsZero(scalar));
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return (*this);
}

K3D::Quaternion  K3D::Quaternion::operator+() const
{
	return (*this);
}

K3D::Quaternion  K3D::Quaternion::operator-() const
{
	return K3D::Quaternion(-x, -y, -z, -w);
}

K3D::Quaternion  K3D::Quaternion::operator*(const K3D::Quaternion & value) const
{
	return K3D::Quaternion(
		(value.x * w) + (x * value.w) + (value.y * z) - (value.z * y),
		(value.y * w) + (y * value.w) + (value.z * x) - (value.x * z),
		(value.z * w) + (z * value.w) + (value.x * y) - (value.y * x),
		(value.w * w) - (value.x * x) - (value.y * y) - (value.z * z)
	);
}

K3D::Quaternion  K3D::Quaternion::operator+(const K3D::Quaternion & value) const
{
	return K3D::Quaternion(x + value.x, y + value.y, z + value.z, w + value.z);
}

K3D::Quaternion  K3D::Quaternion::operator-(const K3D::Quaternion & value) const
{
	return K3D::Quaternion(x - value.x, y - value.y, z - value.z, w - value.z);
}

K3D::Quaternion  K3D::Quaternion::operator*(float scalar) const
{
	return K3D::Quaternion(x * scalar, y * scalar, z * scalar, w *scalar);
}

K3D::Quaternion  K3D::Quaternion::operator/(float scalar) const
{
	assert(!IsZero(scalar));
	return K3D::Quaternion(x / scalar, y / scalar, z / scalar, w / scalar);
}

bool  K3D::Quaternion::operator==(const K3D::Quaternion & value) const
{
	return IsEqual(x, value.x)
		&& IsEqual(y, value.y)
		&& IsEqual(z, value.z)
		&& IsEqual(w, value.w);
}

bool  K3D::Quaternion::operator!=(const K3D::Quaternion & value) const
{
	return !IsEqual(x, value.x)
		|| !IsEqual(y, value.y)
		|| !IsEqual(z, value.z)
		|| !IsEqual(w, value.w);
}

float  K3D::Quaternion::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

float  K3D::Quaternion::LengthSqr() const
{
	return x * x + y * y + z * z + w * w;
}

K3D::Vector3  K3D::Quaternion::EulerAngles()
{
	float p;
	float h;
	float b;
	float sp = -2.0f * (y*z + w * x);

	if (fabs(sp) > 0.9999f) {
		p = F_PIDIV2 * sp;
		h = atan2f(-x * z + w * y, 0.5f - y * y - z * z);
		b = 0.0f;

	}
	else {
		p = asinf(sp);
		h = atan2f(x*z + w * y, 0.5f - x * x - y * y);
		b = atan2f(x*y + w * z, 0.5f - x * x - z * z);
	}
	if (IsZero(RadToDeg(p))) {
		p = 0.0f;
	}
	if (IsZero(RadToDeg(h))) {
		h = 0.0f;

	}
	if (IsZero(RadToDeg(b))) {
		b = 0.0f;
	}

	return K3D::Vector3(RadToDeg(p), RadToDeg(h), RadToDeg(b));



	//float sqrX = std::powf(this->x, 2.0f);
	//float sqrY = std::powf(this->y, 2.0f);
	//float sqrZ = std::powf(this->z, 2.0f);

	//float xy = this->x * this->y;
	//float xz = this->x * this->z;
	//float yz = this->y * this->z;
	//float wx = this->w * this->x;
	//float wy = this->w * this->y;
	//float wz = this->w * this->z;

	//float m00 = 1.0f - (2.0f * sqrY) - (2.0f * sqrZ);
	//float m01 = (2.0f * xy) + (2.0f * wz);
	//float m10 = (2.0f * xy) - (2.0f * wz);
	//float m11 = 1.0f - (2.0f * sqrX) - (2.0f * sqrZ);
	//float m20 = (2.0f * xz) + (2.f * wy);
	//float m21 = (2.0f * yz) + (2.f * wx);
	//float m22 = 1.0f - (2.0f * sqrX) - (2.0f * sqrY);

	//float x, y, z;


	//if (IsEqual(1.0f, m21)) {
	//	x = F_PIDIV2;
	//	y = 0;
	//	z = RadToDeg(atan2f(m10, m00));

	//}
	//else if (IsEqual(1.0f, -m21)) {
	//	x = -F_PIDIV2;
	//	y = 0;
	//	z = RadToDeg(atan2f(m10, m00));
	//}
	//else {
	//	x = RadToDeg(asinf(m21));
	//	y = RadToDeg(atan2f(m20, m22));
	//	z = RadToDeg(atan2f(m01, m11));
	//}
	return K3D::Vector3(x, y, z);
}

K3D::Quaternion &  K3D::Quaternion::Normalize()
{
	auto mag = Length();
	assert(mag > 0.0f);
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
	return (*this);
}

K3D::Quaternion &  K3D::Quaternion::SafeNormalize(const K3D::Quaternion & set)
{
	auto mag = Length();
	if (mag > 0.0f)
	{
		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}
	else
	{
		x = set.x;
		y = set.y;
		z = set.z;
		w = set.w;
	}
	return (*this);
}

K3D::Quaternion &  K3D::Quaternion::Identity()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
	return (*this);
}

K3D::Quaternion	  K3D::Quaternion::CreateIdentity()
{
	return K3D::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

bool  K3D::Quaternion::IsIdentity(const K3D::Quaternion & value)
{
	return IsEqual(value.x, 0.0f)
		&& IsEqual(value.y, 0.0f)
		&& IsEqual(value.z, 0.0f)
		&& IsEqual(value.w, 1.0f);
}

bool  K3D::Quaternion::IsUnit(const K3D::Quaternion & value)
{
	return IsZero(1.0f - value.Length());
}

K3D::Quaternion  K3D::Quaternion::Multiply(const K3D::Quaternion & a, const K3D::Quaternion & b)
{
	return K3D::Quaternion(
		(b.x * a.w) + (a.x * b.w) + (b.y * a.z) - (b.z * a.y),
		(b.y * a.w) + (a.y * b.w) + (b.z * a.x) - (b.x * a.z),
		(b.z * a.w) + (a.z * b.w) + (b.x * a.y) - (b.y * a.x),
		(b.w * a.w) - (b.x * a.x) - (b.y * a.y) - (b.z * a.z)
	);
}

void  K3D::Quaternion::Multiply(const K3D::Quaternion & a, const K3D::Quaternion & b, K3D::Quaternion & result)
{
	result.x = (a.w * b.x) + (a.x * b.w) + (a.z * b.y) - (a.y * b.z);
	result.y = (a.w * b.y) + (a.y * b.w) + (a.x * b.z) - (a.z * b.x);
	result.z = (a.w * b.z) + (a.z * b.w) + (a.y * b.x) - (a.x * b.y);
	result.w = (a.w * b.w) - (a.x * b.x) - (a.y * b.y) - (a.z * b.z);

}

float  K3D::Quaternion::Dot(const K3D::Quaternion & a, const K3D::Quaternion & b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

void  K3D::Quaternion::Dot(const K3D::Quaternion & a, const K3D::Quaternion & b, float & result)
{
	result = (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

K3D::Quaternion  K3D::Quaternion::Conjugate(const K3D::Quaternion & value)
{
	return K3D::Quaternion(-value.x, -value.y, -value.z, value.w);
}

void  K3D::Quaternion::Conjugate(const K3D::Quaternion & value, K3D::Quaternion & result)
{
	result.x = -value.x;
	result.y = -value.y;
	result.z = -value.z;
	result.w = value.w;
}

K3D::Quaternion  K3D::Quaternion::Normalize(const K3D::Quaternion & value)
{
	auto mag = value.Length();
	assert(mag > 0.0f);
	return K3D::Quaternion(
		value.x / mag,
		value.y / mag,
		value.z / mag,
		value.w / mag
	);
}

void  K3D::Quaternion::Normalize(const K3D::Quaternion & value, K3D::Quaternion & result)
{
	auto mag = value.Length();
	assert(mag > 0.0f);
	result.x /= mag;
	result.y /= mag;
	result.z /= mag;
	result.w /= mag;
}

K3D::Quaternion  K3D::Quaternion::SafeNormalize(const K3D::Quaternion & value, const K3D::Quaternion & set)
{
	auto mag = value.Length();
	if (mag > 0.0f)
	{
		return K3D::Quaternion(
			value.x / mag,
			value.y / mag,
			value.z / mag,
			value.w / mag
		);
	}

	return set;
}

void  K3D::Quaternion::SafeNormalize(const K3D::Quaternion & value, const K3D::Quaternion & set, K3D::Quaternion & result)
{
	auto mag = value.Length();
	if (mag > 0.0f)
	{
		result.x = value.x / mag;
		result.y = value.y / mag;
		result.z = value.z / mag;
		result.w = value.w / mag;
	}
	else
	{
		result.x = set.x;
		result.y = set.y;
		result.z = set.z;
		result.w = set.w;
	}
}

K3D::Quaternion  K3D::Quaternion::CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll)
{
	return CreateFromEulerAngles(pitch, yaw, roll);
}

void  K3D::Quaternion::CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll, K3D::Quaternion & result)
{
	result = std::move(CreateFromEulerAngles(pitch, yaw, roll));
}

K3D::Quaternion  K3D::Quaternion::CreateFromEulerAngles(const float x, const float y, const float z)
{
	float h = DegToRad(y);
	float p = DegToRad(x);
	float b = DegToRad(z);

	auto hc = std::cosf(h * 0.5f);
	auto pc = std::cosf(p * 0.5f);
	auto bc = std::cosf(b * 0.5f);

	auto hs = std::sinf(h * 0.5f);
	auto ps = std::sinf(p * 0.5f);
	auto bs = std::sinf(b * 0.5f);
	K3D::Quaternion result;

	result.w = (hc * pc * bc) + (hs * ps * bs);
	result.x = (hc * ps * bc) + (hs * pc * bs);
	result.y = (hs * pc * bc) - (hc * ps * bs);
	result.z = (hc * pc * bs) - (hs * ps * bc);


	return result;

}

K3D::Quaternion  K3D::Quaternion::CreateFromEulerAngles(const K3D::Vector3 & eulerAngles)
{
	float h = DegToRad(eulerAngles.y);
	float p = DegToRad(eulerAngles.x);
	float b = DegToRad(eulerAngles.z);

	auto hc = std::cosf(h * 0.5f);
	auto pc = std::cosf(p * 0.5f);
	auto bc = std::cosf(b * 0.5f);

	auto hs = std::sinf(h * 0.5f);
	auto ps = std::sinf(p * 0.5f);
	auto bs = std::sinf(b * 0.5f);
	K3D::Quaternion result;
	result.w = (hc * pc * bc) + (hs * ps * bs);
	result.x = (hc * ps * bc) + (hs * pc * bs);
	result.y = (hs * pc * bc) - (hc * ps * bs);
	result.z = (hc * pc * bs) - (hs * ps * bc);

	return result;
}

void  K3D::Quaternion::CreateFromEulerAngles(const float x, const float y, const float z, K3D::Quaternion & result)
{
	float h = DegToRad(y);
	float p = DegToRad(x);
	float b = DegToRad(z);

	auto hc = std::cosf(h * 0.5f);
	auto pc = std::cosf(p * 0.5f);
	auto bc = std::cosf(b * 0.5f);

	auto hs = std::sinf(h * 0.5f);
	auto ps = std::sinf(p * 0.5f);
	auto bs = std::sinf(b * 0.5f);

	result.w = (hc * pc * bc) + (hs * ps * bs);
	result.x = (hc * ps * bc) + (hs * pc * bs);
	result.y = (hs * pc * bc) - (hc * ps * bs);
	result.z = (hc * pc * bs) - (hs * ps * bc);

}

void  K3D::Quaternion::CreateFromEulerAngles(const K3D::Vector3 & eulerAngles, K3D::Quaternion & result)
{
	float h = DegToRad(eulerAngles.y);
	float p = DegToRad(eulerAngles.x);
	float b = DegToRad(eulerAngles.z);

	auto hc = std::cosf(h * 0.5f);
	auto pc = std::cosf(p * 0.5f);
	auto bc = std::cosf(b * 0.5f);

	auto hs = std::sinf(h * 0.5f);
	auto ps = std::sinf(p * 0.5f);
	auto bs = std::sinf(b * 0.5f);

	result.w = (hc * pc * bc) + (hs * ps * bs);
	result.x = (hc * ps * bc) + (hs * pc * bs);
	result.y = (hs * pc * bc) - (hc * ps * bs);
	result.z = (hc * pc * bs) - (hs * ps * bc);
}

K3D::Quaternion  K3D::Quaternion::CreateFromAxisAngles(const K3D::Vector3 & axis, float radian)
{
	assert(!IsZero(axis.Length()));
	auto halfRad = radian * 0.5f;
	auto sinX = sinf(halfRad);
	return K3D::Quaternion(
		axis.x * sinX,
		axis.y * sinX,
		axis.z * sinX,
		cosf(halfRad)
	);
}

void  K3D::Quaternion::CreateFromAxisAngles(const K3D::Vector3 & axis, float radian, K3D::Quaternion & result)
{
	assert(!IsZero(axis.Length()));
	auto halfRad = radian * 0.5f;
	auto sinX = sinf(halfRad);
	result.x = axis.x * sinX;
	result.y = axis.y * sinX;
	result.z = axis.z * sinX;
	result.w = cosf(halfRad);
}

K3D::Quaternion  K3D::Quaternion::CreateFromRotationMatrix(const K3D::Matrix & value)
{
	float fourWSquaredMinus1 = value._11 + value._22 + value._33;
	float fourXSquaredMinus1 = value._11 - value._22 - value._33;
	float fourYSquaredMinus1 = value._22 - value._11 - value._33;
	float fourZSquaredMinus1 = value._33 - value._11 - value._22;

	int biggestIndex = 0;
	float fourBiggestSquaredMinus1 = fourWSquaredMinus1;

	if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if (fourZSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}

	//�������Ə��Z�����s

	float biggestValue = sqrtf(fourBiggestSquaredMinus1 + 1.0f) *0.5f;
	float mult = 0.25f / biggestValue;
	K3D::Quaternion ret;
	switch (biggestIndex)
	{
	case 0:
		ret.w = biggestValue;
		ret.x = (value._23 - value._32)*mult;
		ret.y = (value._31 - value._13)*mult;
		ret.z = (value._12 - value._21)*mult;
		break;
	case 1:
		ret.w = (value._23 - value._32)*mult;
		ret.x = biggestValue;
		ret.y = (value._12 + value._21)*mult;
		ret.z = (value._31 + value._13)*mult;
		break;
	case 2:
		ret.w = (value._31 - value._13)*mult;
		ret.x = (value._12 + value._21)*mult;
		ret.y = biggestValue;
		ret.z = (value._23 + value._32)*mult;
		break;
	case 3:
		ret.w = (value._12 - value._21)*mult;
		ret.x = (value._31 + value._13)*mult;
		ret.y = (value._23 + value._32)*mult;
		ret.z = biggestValue;
		break;
	default:
		break;
	}
	if (IsZero(ret.w)) {
		ret.w = 0.0f;
	}
	if (IsZero(ret.x)) {
		ret.x = 0.0f;
	}
	if (IsZero(ret.y)) {
		ret.y = 0.0f;
	}
	if (IsZero(ret.z)) {
		ret.z = 0.0f;
	}

	return ret;

}

void  K3D::Quaternion::CreateFromRotationMatrix(const K3D::Matrix & value, K3D::Quaternion & result)
{

	float fourWSquaredMinus1 = value._11 + value._22 + value._33;
	float fourXSquaredMinus1 = value._11 - value._22 - value._33;
	float fourYSquaredMinus1 = value._22 - value._11 - value._33;
	float fourZSquaredMinus1 = value._33 - value._11 - value._22;

	int biggestIndex = 0;
	float fourBiggestSquaredMinus1 = fourWSquaredMinus1;

	if (fourXSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if (fourWSquaredMinus1 > fourBiggestSquaredMinus1) {
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}

	//�������Ə��Z�����s

	float biggestValue = sqrtf(fourBiggestSquaredMinus1 + 1.0f) *0.5f;
	float mult = 0.25f / biggestValue;
	switch (biggestIndex)
	{
	case 0:
		result.w = biggestValue;
		result.x = (value._23 - value._32)*mult;
		result.y = (value._31 - value._13)*mult;
		result.z = (value._12 - value._21)*mult;
		break;
	case 1:
		result.w = (value._23 - value._32)*mult;
		result.x = biggestValue;
		result.y = (value._12 + value._21)*mult;
		result.z = (value._31 + value._13)*mult;
		break;
	case 2:
		result.w = (value._31 - value._13)*mult;
		result.x = (value._12 + value._21)*mult;
		result.y = biggestValue;
		result.z = (value._23 + value._32)*mult;
		break;
	case 3:
		result.w = (value._12 - value._21)*mult;
		result.x = (value._31 + value._13)*mult;
		result.y = (value._23 + value._32)*mult;
		result.z = biggestValue;
		break;
	default:
		break;
	}
	if (IsZero(result.w)) {
		result.w = 0.0f;
	}
	if (IsZero(result.x)) {
		result.x = 0.0f;
	}
	if (IsZero(result.y)) {
		result.y = 0.0f;
	}
	if (IsZero(result.z)) {
		result.z = 0.0f;
	}
}

K3D::Quaternion  K3D::Quaternion::Slerp(const K3D::Quaternion & a, const K3D::Quaternion & b, float amount)
{

	//�͈͊O�p�����[�^�̃`�F�b�N
	//���̃R�[�h���̂܂��Z�������ċߎ��l���o����\��������

	if (amount <= 0.0f) {
		return a;
	}
	if (amount >= 1.0f) {
		return b;
	}

	float cosOmega = Dot(a, b);


	float bW = b.w;
	float bX = b.x;
	float bY = b.y;
	float bZ = b.z;

	bool flag = false;

	if (cosOmega < 0.0f)
	{
		flag = true;
		bW = -b.w;
		bX = -b.x;
		bY = -b.y;
		bZ = -b.z;
		cosOmega = -cosOmega;
	}
	assert(cosOmega < 1.1f);

	float k0 = 0.0f;
	float k1 = 0.0f;

	//���ɋ߂��@0Div����̂��ߐ��`���
	if (cosOmega > 0.9999f)
	{
		k0 = 1.0f - amount;
		k1 = amount;
	}
	else {
		float sinOmega = std::sqrtf(1.0f - cosOmega * cosOmega);
		float omega = atan2f(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		k0 = sinf((1.0f - amount)*omega) * oneOverSinOmega;
		k1 = sinf(amount * omega) * oneOverSinOmega;

	}
	return
		K3D::Quaternion(
		(k0 * a.x) + (k1 * bX),
			(k0 * a.y) + (k1 * bY),
			(k0 * a.z) + (k1 * bZ),
			(k0 * a.w) + (k1 * bW)
		);

}

void  K3D::Quaternion::Slerp(const K3D::Quaternion & a, const K3D::Quaternion & b, float amount, K3D::Quaternion & result)
{
	if (amount <= 0.0f) {
		result = a;
	}
	if (amount >= 1.0f) {
		result = b;
	}

	float cosOmega = Dot(a, b);


	float bW = b.w;
	float bX = b.x;
	float bY = b.y;
	float bZ = b.z;

	bool flag = false;

	if (cosOmega < 0.0f)
	{
		flag = true;
		bW = -b.w;
		bX = -b.x;
		bY = -b.y;
		bZ = -b.z;
		cosOmega = -cosOmega;
	}
	assert(cosOmega < 1.1f);

	float k0 = 0.0f;
	float k1 = 0.0f;

	//���ɋ߂��@0Div����̂��ߐ��`���
	if (cosOmega > 0.9999f)
	{
		k0 = 1.0f - amount;
		k1 = amount;
	}
	else {
		float sinOmega = std::sqrtf(1.0f - cosOmega * cosOmega);
		float omega = atan2f(sinOmega, cosOmega);
		float oneOverSinOmega = 1.0f / sinOmega;
		k0 = sinf((1.0f - amount)*omega) * oneOverSinOmega;
		k1 = sinf(amount * omega) * oneOverSinOmega;

	}

	result.x = (k0 * a.x) + (k1 * bX);
	result.y = (k0 * a.y) + (k1 * bY);
	result.z = (k0 * a.z) + (k1 * bZ);
	result.w = (k0 * a.w) + (k1 * bW);
}

K3D::Quaternion K3D::Quaternion::Squad(const K3D::Quaternion & value, const K3D::Quaternion & a, const K3D::Quaternion & b, const K3D::Quaternion & c, float amount)
{
	auto d = K3D::Quaternion::Slerp(value, c, amount);
	auto e = K3D::Quaternion::Slerp(a, b, amount);
	return K3D::Quaternion::Slerp(d, e, 2.0f * amount * (1.0f - amount));
}

void K3D::Quaternion::Squad(const K3D::Quaternion & value, const K3D::Quaternion & a, const K3D::Quaternion & b, const K3D::Quaternion & c, float amount, K3D::Quaternion & result)
{
	auto d = K3D::Quaternion::Slerp(value, c, amount);
	auto e = K3D::Quaternion::Slerp(a, b, amount);
	K3D::Quaternion::Slerp(d, e, 2.0f * amount * (1.0f - amount), result);
}

K3D::OrthonormalBasis::OrthonormalBasis() :
	u(), v(), w()
{

}

K3D::OrthonormalBasis::OrthonormalBasis(const K3D::OrthonormalBasis & value) :
	u(value.u), v(value.v), w(value.w)
{
}

K3D::OrthonormalBasis::OrthonormalBasis(K3D::OrthonormalBasis && value)
{
	*this = std::move(value);
}

K3D::OrthonormalBasis::OrthonormalBasis(const K3D::Vector3 & nU, const K3D::Vector3 & nV, const K3D::Vector3 & nW) :
	u(nU), v(nV), w(nW)
{
}

void K3D::OrthonormalBasis::InitFromU(const K3D::Vector3 & nU)
{
	K3D::Vector3 n(K3D::Vector3::forward);
	K3D::Vector3 m(K3D::Vector3::right);

	u = K3D::Vector3::Normalize(nU);
	v = K3D::Vector3::Cross(n, u);
	if (v.Length() < ONB_EPSILON)
	{
		v = K3D::Vector3::Cross(u, m);
	}
	w = K3D::Vector3::Cross(v, u);
}

void K3D::OrthonormalBasis::InitFromV(const K3D::Vector3 & nV)
{
	K3D::Vector3 n(K3D::Vector3::forward);
	K3D::Vector3 m(K3D::Vector3::up);

	v = K3D::Vector3::Normalize(nV);
	u = K3D::Vector3::Cross(v, n);
	if (u.LengthSqr() < ONB_EPSILON)
	{
		u = K3D::Vector3::Cross(v, m);
	}
	w = K3D::Vector3::Cross(v, u);
}

void K3D::OrthonormalBasis::InitFromW(const K3D::Vector3 & nW)
{
	K3D::Vector3 n(K3D::Vector3::up);
	K3D::Vector3 m(K3D::Vector3::forward);

	w = K3D::Vector3::Normalize(nW);
	u = K3D::Vector3::Cross(n, w);
	if (u.Length() < ONB_EPSILON)
	{
		u = K3D::Vector3::Cross(w, m);
	}
	u.Normalize();

	v = K3D::Vector3::Cross(u, w);
	v.Normalize();
}

void K3D::OrthonormalBasis::InitFromUV(const K3D::Vector3 & nU, const K3D::Vector3 & nV)
{
	u = K3D::Vector3::Normalize(nU);
	w = K3D::Vector3::Normalize(K3D::Vector3::Cross(nU, nV));
	v = K3D::Vector3::Cross(w, u);
}

//void  K3D::OrthonormalBasis::InitFromVU(const K3D::Vector3 & nV, const K3D::Vector3 & nU)
//{
//	v = K3D::Vector3::Normalize(nV);
//	w = K3D::Vector3::Normalize(K3D::Vector3::Cross(nV, nU));
//	u = K3D::Vector3::Cross(v, w);
//}

void  K3D::OrthonormalBasis::InitFromUW(const K3D::Vector3 & nU, const K3D::Vector3 & nW)
{
	u = K3D::Vector3::Normalize(nU);
	v = K3D::Vector3::Normalize(K3D::Vector3::Cross(nW, nU));
	w = K3D::Vector3::Cross(u, v);
}

//void  K3D::OrthonormalBasis::InitFromWU(const K3D::Vector3 & nW, const K3D::Vector3 & nU)
//{
//	w = K3D::Vector3::Normalize(nW);
//	v = K3D::Vector3::Normalize(K3D::Vector3::Cross(nU, nW));
//	u = K3D::Vector3::Cross(v, w);
//}

void  K3D::OrthonormalBasis::InitFromVW(const K3D::Vector3 & nV, const K3D::Vector3 & nW)
{
	v = K3D::Vector3::Normalize(nV);
	u = K3D::Vector3::Normalize(K3D::Vector3::Cross(nV, nW));
	w = K3D::Vector3::Cross(u, v);
}

//void  K3D::OrthonormalBasis::InitFromWV(const K3D::Vector3 & nW, const K3D::Vector3 & nV)
//{
//	w = K3D::Vector3::Normalize(nW);
//	u = K3D::Vector3::Normalize(K3D::Vector3::Cross(nV, nW));
//	v = K3D::Vector3::Cross(w, u);
//}

K3D::OrthonormalBasis &  K3D::OrthonormalBasis::operator=(const K3D::OrthonormalBasis & value)
{
	u = value.u;
	v = value.v;
	w = value.w;
	return (*this);
}

K3D::OrthonormalBasis &  K3D::OrthonormalBasis::operator=(K3D::OrthonormalBasis && value)
{
	*this = value;
	value.u = std::move(K3D::Vector3());
	value.v = std::move(K3D::Vector3());
	value.w = std::move(K3D::Vector3());
	return (*this);
}

bool K3D::OrthonormalBasis::operator==(const K3D::OrthonormalBasis & value) const
{
	return (u == value.u)
		&& (v == value.v)
		&& (w == value.w);
}

bool K3D::OrthonormalBasis::operator!=(const K3D::OrthonormalBasis & value) const
{
	return (u != value.u)
		|| (v != value.v)
		|| (w != value.w);
}

float K3D::Easing::Linear::In(float startValue, float changeValue, float time, float duration)
{
	return changeValue * time / duration + startValue;
}

float K3D::Easing::Quadratic::In(float startValue, float changeValue, float time, float duration)
{
	time /= duration;

	return changeValue * time*time + startValue;
}

float K3D::Easing::Quadratic::Out(float startValue, float changeValue, float time, float duration)
{
	time /= duration;

	return -changeValue * time*(time - 2) + startValue;
}

float K3D::Easing::Quadratic::InOut(float startValue, float changeValue, float time, float duration)
{
	time /= duration / 2.0f;
	if (time < 1.0f) {
		return changeValue / 2.0f * time*time*startValue;
	}
	--time;

	return -changeValue / 2.0f*(time*(time - 2.0f) - 1.0f) + startValue;
}

float K3D::Easing::Cubic::In(float startValue, float changeValue, float time, float duration)
{
	time /= duration;

	return changeValue * time*time*time + startValue;
}

float K3D::Easing::Cubic::Out(float startValue, float changeValue, float time, float duration)
{
	time /= duration;
	time--;
	return changeValue * (time*time*time + 1) + startValue;
}

float K3D::Easing::Cubic::InOut(float startValue, float changeValue, float time, float duration)
{
	time /= duration / 2.0f;
	if (time < 1.0f) return changeValue / 2.0f * time*time*time + startValue;
	time -= 2.0f;
	return changeValue / 2.0f* (time*time*time + 2.0f) + startValue;
}

float K3D::Easing::Quartic::In(float startValue, float changeValue, float time, float duration)
{
	time /= duration;
	return changeValue * time*time*time*time + startValue;
}

float K3D::Easing::Quartic::Out(float startValue, float changeValue, float time, float duration)
{
	time /= duration;
	time--;
	return -changeValue * (time*time*time*time - 1.0f) + startValue;
}

float K3D::Easing::Quartic::InOut(float startValue, float changeValue, float time, float duration)
{
	time /= duration / 2.0f;
	if (time < 1.0f) return changeValue / 2.0f * time*time*time*time + startValue;
	time -= 2.0f;
	return -changeValue / 2.0f * (time*time*time*time - 2.0f) + startValue;
}

float K3D::Easing::Quintic::In(float startValue, float changeValue, float time, float duration)
{
	time /= duration;
	return changeValue * time*time*time*time*time + startValue;
}

float K3D::Easing::Quintic::Out(float startValue, float changeValue, float time, float duration)
{
	time /= duration;
	time--;
	return changeValue * (time*time*time*time*time + 1) + startValue;
}

float K3D::Easing::Quintic::InOut(float startValue, float changeValue, float time, float duration)
{
	time /= duration / 2.0f;
	if (time < 1.0f) return changeValue / 2.0f * time*time*time*time*time + startValue;
	time -= 2.0f;
	return changeValue / 2.0f * (time*time*time*time*time + 2.0f) + startValue;
}

float K3D::Easing::Shinusoidal::In(float startValue, float changeValue, float time, float duration)
{
	return -changeValue * std::cosf(time / duration * (F_PI / 2.0f)) + changeValue + startValue;
}

float K3D::Easing::Shinusoidal::Out(float startValue, float changeValue, float time, float duration)
{
	return changeValue * std::sinf(time / duration * (F_PI / 2.0f)) + startValue;
}

float K3D::Easing::Shinusoidal::InOut(float startValue, float changeValue, float time, float duration)
{
	return -changeValue / 2.0f * (std::cosf(F_PI*time / duration) - 1.0f) + startValue;
}

float K3D::Easing::Exponential::In(float startValue, float changeValue, float time, float duration)
{
	return changeValue * std::powf(2.0f, 10.0f * (time / duration - 1.0f)) + startValue;
}

float K3D::Easing::Exponential::Out(float startValue, float changeValue, float time, float duration)
{
	return changeValue * (-std::powf(2.0f, -10.0f * time / duration) + 1) + startValue;
}

float K3D::Easing::Exponential::InOut(float startValue, float changeValue, float time, float duration)
{
	time /= duration / 2.0f;
	if (time < 1.0f) return changeValue / 2.0f * std::powf(2.0f, 10.0f * (time - 1.0f)) + startValue;
	time--;
	return changeValue / 2 * (-std::powf(2.0f, -10.0f * time) + 2.0f) + startValue;
}

float K3D::Easing::Circuler::In(float startValue, float changeValue, float time, float duration)
{
	time /= duration;
	return -changeValue * (std::sqrtf(1.0f - time * time) - 1.0f) + startValue;
}

float K3D::Easing::Circuler::Out(float startValue, float changeValue, float time, float duration)
{
	time /= duration;
	time--;
	return changeValue * std::sqrtf(1.0f - time * time) + startValue;
}

float K3D::Easing::Circuler::InOut(float startValue, float changeValue, float time, float duration)
{
	time /= duration / 2.0f;
	if (time < 1.0f) return -changeValue / 2.0f * (std::sqrtf(1.0f - time * time) - 1.0f) + startValue;
	time -= 2.0f;
	return changeValue / 2.0f * (std::sqrtf(1.0f - time * time) + 1.0f) + startValue;
}