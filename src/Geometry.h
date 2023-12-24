#pragma once

namespace Geometry {

/// Represents a vector in 3D space.
struct Vec {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    [[nodiscard]] Vec operator-(const Vec &other) const { return {x - other.x, y - other.y, z - other.z}; }
    [[nodiscard]] Vec operator+(const Vec &other) const { return {x + other.x, y + other.y, z + other.z}; }

    [[nodiscard]] Vec operator+(double v) const { return {x + v, y + v, z + v}; }
    [[nodiscard]] Vec operator-(double v) const { return {x - v, y - v, z - v}; }
    [[nodiscard]] Vec operator*(double v) const { return {x * v, y * v, z * v}; }
    [[nodiscard]] Vec operator/(double v) const { return {x / v, y / v, z / v}; }

    [[nodiscard]]
    double length() const {
        return sqrt((x * x) + (y * y) + (z * z));
    }

    [[nodiscard]]
    double dot(const Vec &other) const {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    [[nodiscard]]
    Vec cross(const Vec &other) const {
        return {
            (y * other.z) - (other.y * z),
            ((x * other.z) - (other.x * z)) * -1,
            (x * other.y) - (other.x * y),
        };
    }

    [[nodiscard]] Vec normalized() const {
        return *this / length();
    }
};

/// Represents a point in 3D space.
struct Pt {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    // TODO: proper semantics for point operations...
    [[nodiscard]] Pt operator-(const Pt &other) const { return {x - other.x, y - other.y, z - other.z}; }
    [[nodiscard]] Pt operator+(const Pt &other) const { return {x + other.x, y + other.y, z + other.z}; }
    [[nodiscard]] Pt operator-(const Vec &other) const { return {x - other.x, y - other.y, z - other.z}; }
    [[nodiscard]] Pt operator+(const Vec &other) const { return {x + other.x, y + other.y, z + other.z}; }

    [[nodiscard]] Pt operator+(double v) const { return {x + v, y + v, z + v}; }
    [[nodiscard]] Pt operator-(double v) const { return {x - v, y - v, z - v}; }
    [[nodiscard]] Pt operator*(double v) const { return {x * v, y * v, z * v}; }
    [[nodiscard]] Pt operator/(double v) const { return {x / v, y / v, z / v}; }

    [[nodiscard]] Vec toVec() const { return Vec{x, y, z}; }
};

/// Represents triangle in 3D space.
struct Tri {
    std::array<Pt, 3> pts;

    [[nodiscard]] Vec normal() const {
        return (pts[0] - pts[1]).toVec().cross((pts[0] - pts[2]).toVec());
    }
};

/// Represents 3D rectangle given by 4 points.
struct Rectangle {
    std::array<Pt, 4> pts;

    [[nodiscard]] Pt center() const { return (pts[0] + pts[2]) / 2.0; }

    [[nodiscard]] Vec normal() const { return t0().normal(); }

    [[nodiscard]] Rectangle translated(Vec v) const {
        return Rectangle{
            {
                pts[0] + v,
                pts[1] + v,
                pts[2] + v,
                pts[3] + v
            }
        };
    }

    [[nodiscard]] Rectangle flipped() const { return Rectangle{{pts[0], pts[3], pts[2], pts[1]}}; }

    [[nodiscard]] std::array<Tri, 2> triangles() const { return {t0(), t1()}; }

private:
    [[nodiscard]] Tri t0() const { return Tri{pts[0], pts[1], pts[2]}; }
    [[nodiscard]] Tri t1() const { return Tri{pts[2], pts[3], pts[0]}; }
};

/// Represents a rectangular prism given by 2 opposing faces (a, b).
struct RectangularPrism {
    Rectangle a;
    Rectangle b;

    // This will only make sense if you see the diagram I have written in my notebook.
    [[nodiscard]]
    Rectangle face(std::uint8_t i) const {
        if (i == 0) { return a; }
        if (i == 1) { return Rectangle{ a.pts[0], a.pts[3], b.pts[1], b.pts[0] }; }
        if (i == 2) { return Rectangle{ a.pts[3], a.pts[2], b.pts[2], b.pts[1] }; }
        if (i == 3) { return Rectangle{ a.pts[2], a.pts[1], b.pts[3], b.pts[2] }; }
        if (i == 4) { return Rectangle{ a.pts[1], a.pts[0], b.pts[0], b.pts[3] }; }
        if (i == 5) { return b; }
        assert(false);
    }

    [[nodiscard]]
    std::array<Tri, 12> triangles() const {
        std::array<Tri, 12> out;
        for (std::uint8_t i = 0; i < 6; ++i) {
            auto ts = face(i).triangles();
            out[(i * 2)] = ts[0];
            out[(i * 2) + 1] = ts[1];
        }
        return out;
    }
};

} // namespace Geometry
