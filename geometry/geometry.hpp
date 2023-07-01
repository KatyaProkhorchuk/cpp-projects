#pragma once
#include <cstdint>
#include <iostream>
class Vector {
 public:
  struct Point {
    int64_t x{0};
    int64_t y{0};
    Point(int64_t coord_x, int64_t coord_y) : x(coord_x), y(coord_y){};
    Point(const Point& point) : x(point.x), y(point.y){};
  };
  explicit Vector();
  explicit Vector(const int64_t& coord_x, const int64_t& coord_y);
  explicit Vector(const Vector::Point& first, const Point& second);
  Vector(const Point& point);
  Vector& operator+=(const Vector& other);
  Vector& operator-=(const Vector& other);
  Vector& operator*=(const int64_t& other);
  int64_t GetX() const;
  int64_t GetY() const;
  double GetLen() const;
  Vector operator-() const;
  void SetCoordinate(int64_t coord_x, int64_t coord_y);
  ~Vector() = default;

 private:
  Point coordinate_{0, 0};
};

class Point;
class Segment;

class IShape {
 public:
  virtual IShape& Move(const Vector& vector) = 0;
  virtual bool ContainsPoint(const Point& point) const = 0;
  virtual bool CrossSegment(const Segment& segment) const = 0;
  virtual IShape* Clone() const = 0;
  virtual ~IShape() = default;
};

class Point : public IShape {
 public:
  explicit Point();
  explicit Point(int64_t coord_x, int64_t coord_y);
  IShape& Move(const Vector& vector) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  int64_t GetX() const;
  int64_t GetY() const;
  Point operator+=(const Vector& vector);
  Point operator-();
  ~Point() override = default;

 private:
  int64_t x_ = 0;
  int64_t y_ = 0;
};

Vector operator-(const Point& point_first, const Point& point_second);

class Segment : public IShape {
 public:
  explicit Segment();
  explicit Segment(const Point& first, const Point& second);
  IShape& Move(const Vector& vector) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  double DistToSegment(const Point& point) const;
  Point GetA() const;
  Point GetB() const;
  static int64_t Area(const Point& a_point, const Point& b_point,
                      const Point& c_point);
  static bool Intersect(int64_t a_point, int64_t b_point, int64_t c_point,
                        int64_t d_point);
  ~Segment() override = default;

 private:
  Point start_{0, 0};
  Point end_{0, 0};
};
class Line : public IShape {
 public:
  explicit Line();
  explicit Line(const Point& first, const Point& second);
  explicit Line(int64_t arg_a, int64_t arg_b, int64_t arg_c);
  IShape& Move(const Vector& vector) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossSegment(const Segment& segment) const override;
  double DistToLine(const Point& point) const;
  IShape* Clone() const override;
  int64_t GetA() const;
  int64_t GetB() const;
  int64_t GetC() const;
  ~Line() = default;

 private:
  int64_t A_{0};
  int64_t B_{0};
  int64_t C_{0};
  Point start_{0, 0};
  Point end_{0, 0};
};
class Ray : public IShape {
 public:
  explicit Ray() = default;
  explicit Ray(const Point& first, const Point& second);
  explicit Ray(const Point& first, const Vector& vector);
  IShape& Move(const Vector& vector) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  Point GetA() const;
  Vector GetVector() const;
  ~Ray() = default;

 private:
  Point start_{0, 0};
  Vector vector_{0, 0};
};
class Circle : public IShape {
 public:
  explicit Circle() = default;
  explicit Circle(const Point& center, size_t radius);
  IShape& Move(const Vector& vector) override;
  bool ContainsPoint(const Point& point) const override;
  bool CrossSegment(const Segment& segment) const override;
  IShape* Clone() const override;
  Point GetCentre() const;
  size_t GetRadius() const;
  ~Circle() = default;

 private:
  Point center_{0, 0};
  int64_t radius_{0};
};
