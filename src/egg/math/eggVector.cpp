#include <egg/math/eggVector.h>

namespace EGG {

const Vector2f Vector2f::zero(0.0f, 0.0f);
const Vector2f Vector2f::ex(1.0f, 0.0f);
const Vector2f Vector2f::ey(0.0f, 1.0f);

const Vector3f Vector3f::zero(0.0f, 0.0f, 0.0f);
const Vector3f Vector3f::ex(1.0f, 0.0f, 0.0f);
const Vector3f Vector3f::ey(0.0f, 1.0f, 0.0f);
const Vector3f Vector3f::ez(0.0f, 0.0f, 1.0f);

const Vector3s Vector3s::zero(0, 0, 0);
const Vector3s Vector3s::ex(1, 0, 0);
const Vector3s Vector3s::ey(0, 1, 0);
const Vector3s Vector3s::ez(0, 0, 1);

f32 Vector3f::normalise() {
    f32 len = length();
    if (len > 0.0f) {
        x *= 1.0f / len;
        y *= 1.0f / len;
        z *= 1.0f / len;
    }
    return len;
}

f32 Vector3f::setLength(const Vector3f &src, f32 len) {
    const f32 mag = src.length();

    if (mag <= FLT_EPSILON) {
        z = 0.0f;
        y = 0.0f;
        x = 0.0f;
        return 0.0f;
    }

    x = src.x * (len / mag);
    y = src.y * (len / mag);
    z = src.z * (len / mag);

    return mag;
}

f32 Vector3f::setLength(f32 len) {
    const f32 mag = length();

    if (mag <= FLT_EPSILON) {
        return 0.0f;
    }

    x *= len / mag;
    y *= len / mag;
    z *= len / mag;

    return mag;
}

} // namespace EGG