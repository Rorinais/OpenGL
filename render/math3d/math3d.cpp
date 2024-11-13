#include"math3d.h"

Quaternion::Quaternion(float _x, float _y, float _z, float _w): x(_x), y(_y), z(_z), w(_w) {}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    const float _w = w * q.w - x * q.x - y * q.y - z * q.z;
    const float _x = x * q.w + w * q.x + y * q.z - z * q.y;
    const float _y = y * q.w + w * q.y + z * q.x - x * q.z;
    const float _z = z * q.w + w * q.z + x * q.y - y * q.x;
    return Quaternion(_x, _y, _z, _w);
}
void Quaternion::Normalize(){
	const float Length = sqrt(x * x + y * y + z * z + w * w);
	x /= Length;
	y /= Length;
	z /= Length;
	w /= Length;
}

Quaternion Quaternion::Conjugate(){ return Quaternion (-x, -y, -z, w);}

void Vector3f::Rotate(float Angle, const Vector3f& Axe){

    const float SinHalfAngle = sinf(ToRadian(Angle / 2));
    const float CosHalfAngle = cosf(ToRadian(Angle / 2));

    const float Rx = Axe.x * SinHalfAngle;
    const float Ry = Axe.y * SinHalfAngle;
    const float Rz = Axe.z * SinHalfAngle;
    const float Rw = CosHalfAngle;

    Quaternion RotationQ(Rx, Ry, Rz, Rw);
    Quaternion ConjugateQ = RotationQ.Conjugate();
    ConjugateQ.Normalize();
    Quaternion W = RotationQ * Quaternion(x, y, z, 0) * ConjugateQ;
    x = W.x;
    y = W.y;
    z = W.z;
}



