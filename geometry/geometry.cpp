#include "geometry.hpp"

#include <cmath>
/* -----------Vector------------ */
Vector::Vector() = default;
Vector::Vector(const int64_t& coord_x, const int64_t& coord_y)
    : coordinate_(coord_x, coord_y) {}
Vector::Vector(const Vector::Point& first, const Vector::Point& second) {
  coordinate_.x = second.x - first.x;
  coordinate_.y = second.y - first.y;
}
Vector::Vector(const Point& point) : coordinate_(point) {}
Vector& Vector::operator+=(const Vector& other) {
  coordinate_.x += other.coordinate_.x;
  coordinate_.y += other.coordinate_.y;
  return *this;
}
Vector& Vector::operator*=(const int64_t& other) {
  coordinate_.x *= other;
  coordinate_.y *= other;
  return *this;
}
Vector& Vector::operator-=(const Vector& other) {
  coordinate_.x -= other.coordinate_.x;
  coordinate_.y -= other.coordinate_.y;
  return *this;
}
Vector operator+(const Vector& vector_first, const Vector& vector_second) {
  Vector copy = vector_first;
  copy += vector_second;
  return copy;
}
Vector operator-(const Vector& vector_first, const Vector& vector_second) {
  Vector copy = vector_first;
  copy -= vector_second;
  return copy;
}
int64_t operator^(const Vector& vector_first, const Vector& vector_second) {
  return (vector_first.GetX() * vector_second.GetY() -
          vector_first.GetY() * vector_second.GetX());
}
int64_t operator*(const Vector& vector_first, const Vector& vector_second) {
  return (vector_first.GetX() * vector_second.GetX() +
          vector_first.GetY() * vector_second.GetY());
}
Vector operator*(const int64_t& number, const Vector& vector) {
  Vector copy = vector;
  copy *= number;
  return copy;
}
Vector operator*(const Vector& vector, const int64_t& number) {
  Vector copy = vector;
  copy *= number;
  return copy;
}
Vector Vector::operator-() const {
  Point result(-coordinate_.x, -coordinate_.y);
  return Vector(result);
}
void Vector::SetCoordinate(int64_t coord_x, int64_t coord_y) {
  coordinate_.x = coord_x;
  coordinate_.y = coord_y;
}
int64_t Vector::GetX() const { return coordinate_.x; }
int64_t Vector::GetY() const { return coordinate_.y; }
double Vector::GetLen() const {
  return sqrt(coordinate_.x * coordinate_.x + coordinate_.y * coordinate_.y);
}

/* ------------Point-------------- */
Point::Point() = default;
Point::Point(int64_t coord_x, int64_t coord_y) : x_(coord_x), y_(coord_y){};
IShape& Point::Move(const Vector& vector) {
  x_ += vector.GetX();
  y_ += vector.GetY();
  return *this;
}
bool Point::ContainsPoint(const Point& point) const {
  return (x_ == point.GetX() && y_ == point.GetY());
}
bool Point::CrossSegment(const Segment& segment) const {
  return segment.ContainsPoint(*this);
}
IShape* Point::Clone() const {
  Point* clone = new Point(x_, y_);
  return clone;
}
Vector operator-(const Point& point_first, const Point& point_second) {
  Vector::Point first(point_first.GetX(), point_first.GetY());
  Vector::Point second(point_second.GetX(), point_second.GetY());
  Vector vector(second, first);
  return vector;
}
Point Point::operator+=(const Vector& vector) {
  x_ += vector.GetX();
  y_ += vector.GetY();
  return *this;
}
Point Point::operator-() {
  x_ *= -1;
  y_ *= -1;
  return *this;
}
int64_t Point::GetX() const { return x_; }
int64_t Point::GetY() const { return y_; }

/* ------------Segment----------- */
Segment::Segment() = default;
Segment::Segment(const Point& first, const Point& second)
    : start_(first), end_(second) {}
IShape& Segment::Move(const Vector& vector) {
  start_ += vector;
  end_ += vector;
  return *this;
}

bool Segment::ContainsPoint(const Point& point) const {
  Vector vector_first =
      Vector(point.GetX() - start_.GetX(), point.GetY() - start_.GetY());
  Vector vector_second = Vector(Vector::Point(start_.GetX(), start_.GetY()),
                                Vector::Point(end_.GetX(), end_.GetY()));
  return (vector_second ^ vector_first) == 0 &&
         point.GetX() <= std::max(start_.GetX(), end_.GetX()) &&
         point.GetX() >= std::min(start_.GetX(), end_.GetX()) &&
         point.GetY() <= std::max(start_.GetY(), end_.GetY()) &&
         point.GetY() >= std::min(start_.GetY(), end_.GetY());
}

double Segment::DistToSegment(const Point& point) const {
  Vector first_vector(Vector::Point(start_.GetX(), start_.GetY()),
                      Vector::Point(point.GetX(), point.GetY()));
  Vector second_vector(Vector::Point(start_.GetX(), start_.GetY()),
                       Vector::Point(end_.GetX(), end_.GetY()));
  Vector third_vector(Vector::Point(point.GetX(), point.GetY()),
                      Vector::Point(end_.GetX(), end_.GetY()));
  int64_t mul_12 = first_vector * second_vector;
  int64_t mul_32 = second_vector * third_vector;
  Line line(start_, end_);
  if (mul_12 >= 0 && mul_32 >= 0) {
    return line.DistToLine(point);
  }
  return std::min(first_vector.GetLen(), third_vector.GetLen());
}

int64_t Segment::Area(const Point& a_point, const Point& b_point,
                      const Point& c_point) {
  return (b_point.GetX() - a_point.GetX()) * (c_point.GetY() - a_point.GetY()) -
         (b_point.GetY() - a_point.GetY()) * (c_point.GetX() - a_point.GetX());
}
bool Segment::Intersect(int64_t a_point, int64_t b_point, int64_t c_point,
                        int64_t d_point) {
  if (a_point > b_point) {
    std::swap(a_point, b_point);
  }
  if (c_point > d_point) {
    std::swap(c_point, d_point);
  }
  return std::max(a_point, c_point) <= std::min(b_point, d_point);
}

bool Segment::CrossSegment(const Segment& segment) const {
  return Intersect(start_.GetX(), end_.GetX(), segment.start_.GetX(),
                   segment.end_.GetX()) &&
         Intersect(start_.GetY(), end_.GetY(), segment.start_.GetY(),
                   segment.end_.GetY()) &&
         Area(start_, end_, segment.start_) *
                 Area(start_, end_, segment.end_) <=
             0 &&
         Area(segment.start_, segment.end_, start_) *
                 Area(segment.start_, segment.end_, end_) <=
             0;
}
IShape* Segment::Clone() const {
  Segment* segment = new Segment(start_, end_);
  return segment;
}
Point Segment::GetA() const { return start_; }
Point Segment::GetB() const { return end_; }

/* ---------------Line---------------- */
Line::Line() = default;
Line::Line(const Point& first, const Point& second)
    : start_(first), end_(second) {
  A_ = second.GetY() - first.GetY();
  B_ = first.GetX() - second.GetX();
  C_ = first.GetY() * (second.GetX() - first.GetX()) -
       first.GetX() * (second.GetY() - first.GetY());
}
Line::Line(int64_t arg_a, int64_t arg_b, int64_t arg_c)
    : A_(arg_a), B_(arg_b), C_(arg_c) {}
IShape& Line::Move(const Vector& vector) {
  C_ = C_ - A_ * vector.GetX() - B_ * vector.GetY();
  return *this;
}
bool Line::ContainsPoint(const Point& point) const {
  return (A_ * point.GetX() + B_ * point.GetY() + C_) == 0;
}
bool Line::CrossSegment(const Segment& segment) const {
  Vector direction({0, 0}, {B_, -A_});
  Vector::Point point(start_.GetX(), start_.GetY());
  Vector first_vector(
      point, Vector::Point(segment.GetA().GetX(), segment.GetA().GetY()));
  Vector second_vector(
      point, Vector::Point(segment.GetB().GetX(), segment.GetB().GetY()));
  return (direction ^ first_vector) * (direction ^ second_vector) <= 0;
}
IShape* Line::Clone() const {
  Line* copy = new Line(A_, B_, C_);
  return copy;
}
double Line::DistToLine(const Point& point) const {
  double res = std::abs(A_ * point.GetX() + B_ * point.GetY() + C_);
  return res / sqrt(A_ * A_ + B_ * B_);
}

int64_t Line::GetA() const { return A_; }
int64_t Line::GetB() const { return B_; }
int64_t Line::GetC() const { return C_; }

/* -----------Ray------------- */

Ray::Ray(const Point& first, const Point& second)
    : start_(first), vector_(second.GetX(), second.GetY()) {}
Ray::Ray(const Point& first, const Vector& vector)
    : start_(first), vector_(vector) {}
IShape& Ray::Move(const Vector& vector) {
  Point point(start_.GetX(), start_.GetY());
  point.Move(vector);
  Point vec(vector_.GetX(), vector_.GetY());
  Vector sdvig(-point.GetX(), -point.GetY());
  vec.Move(vector);
  vec.Move(sdvig);
  start_ = point;
  vector_.SetCoordinate(vec.GetX(), vec.GetY());
  return *this;
}
bool Ray::ContainsPoint(const Point& point) const {
  Point tmp(point.GetX(), point.GetY());
  Line line(start_, Point(vector_.GetX(), vector_.GetY()));
  Vector first_vector(Vector::Point(start_.GetX(), start_.GetY()),
                      Vector::Point(tmp.GetX(), tmp.GetY()));
  Vector second_vector(Vector::Point(start_.GetX(), start_.GetY()),
                       Vector::Point(vector_.GetX(), vector_.GetY()));
  return (first_vector * second_vector >= 0 && line.ContainsPoint(point));
}
bool Ray::CrossSegment(const Segment& segment) const {
  Vector first_vector(Vector::Point(start_.GetX(), start_.GetY()),
                      Vector::Point(vector_.GetX(), vector_.GetY()));
  Vector second_vector(
      Vector::Point(segment.GetA().GetX(), segment.GetA().GetY()),
      Vector::Point(segment.GetB().GetX(), segment.GetB().GetY()));
  Vector third_vector(
      Vector::Point(segment.GetA().GetX(), segment.GetA().GetY()),
      Vector::Point(start_.GetX(), start_.GetY()));
  int64_t mul = first_vector ^ second_vector;
  if (mul == 0) {
    Point segment_start(segment.GetA().GetX(), segment.GetA().GetY());
    Point segment_end(segment.GetB().GetX(), segment.GetB().GetY());
    return ContainsPoint(segment_start) || ContainsPoint((segment_end));
  }
  int64_t mul1 = first_vector ^ third_vector;
  int64_t mul2 = second_vector ^ third_vector;
  const int64_t kNull = 0;
  if (static_cast<int>(mul > kNull) - static_cast<int>(mul < kNull) < 0) {
    mul *= -1;
    mul1 *= -1;
    mul2 *= -1;
  }
  return mul1 <= mul && mul1 >= 0 && mul2 >= 0;
}
IShape* Ray::Clone() const {
  Ray* copy = new Ray(start_, vector_);
  return copy;
}
Point Ray::GetA() const { return start_; }
Vector Ray::GetVector() const { return vector_; }

/* ---------------Circle--------------------- */
Circle::Circle(const Point& center, size_t radius)
    : center_(center), radius_(radius) {}
IShape& Circle::Move(const Vector& vector) {
  center_.Move(vector);
  return *this;
}
bool Circle::ContainsPoint(const Point& point) const {
  if (std::abs(point.GetX() - center_.GetX()) > radius_) {
    return false;
  }
  if (std::abs(point.GetY() - center_.GetY()) > radius_) {
    return false;
  }
  if (std::abs(point.GetX() - center_.GetX()) +
          std::abs(point.GetY() - center_.GetY()) <=
      radius_) {
    return true;
  }
  int64_t coord_x = std::abs(point.GetX() - center_.GetX());
  int64_t coord_y = std::abs(point.GetY() - center_.GetY());
  return (coord_x * coord_x + coord_y * coord_y <= radius_ * radius_);
}
bool Circle::CrossSegment(const Segment& segment) const {
  Point first(segment.GetA().GetX(), segment.GetA().GetY());
  Point second(segment.GetB().GetX(), segment.GetB().GetY());

  Vector first_vector(
      Vector::Point(center_.GetX(), center_.GetY()),
      Vector::Point(segment.GetA().GetX(), segment.GetA().GetY()));
  Vector second_vector(
      Vector::Point(center_.GetX(), center_.GetY()),
      Vector::Point(segment.GetB().GetX(), segment.GetB().GetY()));
  double distant = segment.DistToSegment(center_);
  return distant <= radius_ && (first_vector.GetLen() >= radius_ ||
                                second_vector.GetLen() >= radius_);
}

IShape* Circle::Clone() const {
  Circle* copy = new Circle(center_, radius_);
  return copy;
}
Point Circle::GetCentre() const { return center_; }
size_t Circle::GetRadius() const { return radius_; }
