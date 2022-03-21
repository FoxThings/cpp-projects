#include <assert.h>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace Geometry {

class Vector;

class IShape;
class Point;
class Segment;
class Ray;
class Line;
class Circle;
class Polygon;

//////////////////////////////////////////////////////////////////////////////////

class Vector {
 public:
  Vector() = default;
  Vector(int x, int y);
  std::string ToString();

  Vector& operator+=(const Vector& other);
  Vector& operator-=(const Vector& other);
  Vector& operator*=(int num);
  Vector& operator-();

  friend long long operator*(const Vector& first, const Vector& second);
  friend long long operator^(const Vector& first, const Vector& second);
  friend Vector operator*(const Vector& vec, int num);
  friend Vector operator*(int num, const Vector& vec);
  friend Vector operator-(const Vector& first, const Vector& second);
  friend Vector operator+(const Vector& first, const Vector& second);
  friend bool operator==(const Vector& first, const Vector& second);

  int x;
  int y;
};

//////////////////////////////////////////////////////////////////////////////////

class IShape {
 public:
  virtual ~IShape() = default;

  virtual IShape& Move(const Vector& shift) = 0;
  virtual bool ContainsPoint(const Point& other) const = 0;
  virtual bool CrossesSegment(const Segment& other) const = 0;
  virtual IShape* Clone() const = 0;
  virtual std::string ToString() = 0;
};

//////////////////////////////////////////////////////////////////////////////////

class Point : public IShape {
 public:
  Point() = default;
  Point(const Vector& point);
  Point(int x, int y);

  IShape& Move(const Vector& shift) override;
  bool ContainsPoint(const Point& other) const override;
  bool CrossesSegment(const Segment& other) const override;
  IShape* Clone() const override;
  std::string ToString() override;

  friend Vector operator-(const Point& first, const Point& second);

  Vector point;
};

//////////////////////////////////////////////////////////////////////////////////

class Segment : public IShape {
 public:
  Segment() = default;
  Segment(const Point& begin, const Point& end);

  IShape& Move(const Vector& shift) override;
  bool ContainsPoint(const Point& other) const override;
  bool CrossesSegment(const Segment& other) const override;
  IShape* Clone() const override;
  std::string ToString() override;
  std::pair<Point, Point> GetBorders() const;

 private:
  Point begin_;
  Point end_;
};

//////////////////////////////////////////////////////////////////////////////////

class Ray : public IShape {
 public:
  Ray() = default;
  Ray(const Point& begin, const Point& end);
  Ray(const Point& begin, const Vector& dir);

  IShape& Move(const Vector& shift) override;
  bool ContainsPoint(const Point& other) const override;
  bool CrossesSegment(const Segment& other) const override;
  IShape* Clone() const override;
  std::string ToString() override;

 private:
  Point begin_;
  Vector direction_;
};

//////////////////////////////////////////////////////////////////////////////////

class Line : public IShape {
 public:
  Line() = default;
  Line(const Point& begin, const Point& end);

  IShape& Move(const Vector& shift) override;
  bool ContainsPoint(const Point& other) const override;
  bool CrossesSegment(const Segment& other) const override;
  IShape* Clone() const override;
  std::string ToString() override;

 private:
  int a_;
  int b_;
  int c_;

  Point first_;
  Point second_;
};

//////////////////////////////////////////////////////////////////////////////////

class Circle : public IShape {
 public:
  Circle() = default;
  Circle(const Point& k_center, int radius);

  IShape& Move(const Vector& shift) override;
  bool ContainsPoint(const Point& other) const override;
  bool CrossesSegment(const Segment& other) const override;
  IShape* Clone() const override;
  std::string ToString() override;

 private:
  Point center_;
  int radius_;
};

//////////////////////////////////////////////////////////////////////////////////

class Polygon : public IShape {
 public:
  Polygon() = default;
  Polygon(const std::vector<Point>& vertexes);

  IShape& Move(const Vector& shift) override;
  bool ContainsPoint(const Point& other) const override;
  bool CrossesSegment(const Segment& other) const override;
  IShape* Clone() const override;
  std::string ToString() override;

 private:
  std::vector<Point> vertexes_;
};

//////////////////////////////////////////////////////////////////////////////////

// ---------------------------------> Vector <---------------------------------

Vector::Vector(int x, int y) : x(x), y(y) {}

Vector& Vector::operator+=(const Vector& other) {
  x += other.x;
  y += other.y;
  return *this;
}

Vector& Vector::operator-=(const Vector& other) {
  x -= other.x;
  y -= other.y;
  return *this;
}

Vector& Vector::operator*=(int num) {
  x *= num;
  y *= num;
  return *this;
}

Vector& Vector::operator-() { return (*this *= -1); }

long long operator*(const Vector& first, const Vector& second) {
  return first.x * second.x + first.y * second.y;
}

Vector operator+(const Vector& first, const Vector& second) {
  Vector temp(first);
  temp += second;
  return temp;
}

Vector operator-(const Vector& first, const Vector& second) {
  Vector temp(first);
  temp -= second;
  return temp;
}

long long operator^(const Vector& first, const Vector& second) {
  return first.x * second.y - second.x * first.y;
}

Vector operator*(const Vector& vec, int num) {
  Vector temp(vec);
  temp *= num;
  return temp;
}

Vector operator*(int num, const Vector& vec) { return (vec * num); }

bool operator==(const Vector& first, const Vector& second) {
  return (first.x == second.x && first.y == second.y);
}

std::string Vector::ToString() {
  std::ostringstream string_stream;
  string_stream << "Vector(" << x << ", " << y << ")";
  return string_stream.str();
}

// ---------------------------------> Point <---------------------------------

Point::Point(const Vector& point) : point(point) {}

Point::Point(int x, int y) : point(x, y) {}

IShape& Point::Move(const Vector& shift) {
  point += shift;
  return *this;
}

bool Point::ContainsPoint(const Point& other) const {
  return point == other.point;
}

bool Point::CrossesSegment(const Segment& other) const {
  return other.ContainsPoint(*this);
}

IShape* Point::Clone() const { return new Point(*this); }

std::string Point::ToString() {
  std::ostringstream string_stream;
  string_stream << "Point(" << point.x << ", " << point.y << ")";
  return string_stream.str();
}

Vector operator-(const Point& first, const Point& second) {
  return first.point - second.point;
}

// ---------------------------------> Segment <---------------------------------

Segment::Segment(const Point& begin, const Point& end)
    : begin_(begin), end_(end) {}

IShape& Segment::Move(const Vector& shift) {
  begin_.Move(shift);
  end_.Move(shift);
  return *this;
}

bool Segment::ContainsPoint(const Point& other) const {
  Line temp(begin_, end_);
  return (temp.ContainsPoint(other) &&
          (other.point.x >= std::min(begin_.point.x, end_.point.x) &&
           other.point.x <= std::max(begin_.point.x, end_.point.x)) &&
          (other.point.y >= std::min(begin_.point.y, end_.point.y) &&
           other.point.y <= std::max(begin_.point.y, end_.point.y)));
}

bool Segment::CrossesSegment(const Segment& other) const {
  auto borders = other.GetBorders();
  long long first_result = ((end_ - begin_) ^ (borders.first - begin_)) *
                           ((end_ - begin_) ^ (borders.second - begin_));
  long long second_result =
      ((borders.second - borders.first) ^ (begin_ - borders.first)) *
      ((borders.second - borders.first) ^ (end_ - borders.first));
  return (first_result < 0 && second_result < 0) ||
         this->ContainsPoint(borders.first) ||
         this->ContainsPoint(borders.second) || other.ContainsPoint(begin_) ||
         other.ContainsPoint(end_);
}

IShape* Segment::Clone() const { return new Segment(begin_, end_); }

std::string Segment::ToString() {
  std::ostringstream string_stream;
  string_stream << "Segment(" << begin_.ToString() << ", " << end_.ToString()
                << ")";
  return string_stream.str();
}

std::pair<Point, Point> Segment::GetBorders() const {
  return std::pair<Point, Point>(begin_, end_);
}

// ---------------------------------> Ray <---------------------------------

Ray::Ray(const Point& begin, const Point& end) : begin_(begin) {
  direction_ = end.point - begin.point;
}

Ray::Ray(const Point& begin, const Vector& dir)
    : begin_(begin), direction_(dir) {}

IShape& Ray::Move(const Vector& shift) {
  begin_.Move(shift);
  return *this;
}

bool Ray::ContainsPoint(const Point& other) const {
  Line temp(begin_, begin_.point + direction_);
  return (temp.ContainsPoint(other) && ((other - begin_) * direction_) >= 0);
}

bool Ray::CrossesSegment(const Segment& other) const {
  auto borders = other.GetBorders();
  if (ContainsPoint(borders.first) || ContainsPoint(borders.second)) {
    return true;
  }

  return ((borders.first - begin_) ^ direction_) *
                 ((borders.second - begin_) ^ direction_) <
             0 &&
         !Line(begin_, borders.first)
              .CrossesSegment(Segment((borders.second),
                                      Point(begin_.point.x + direction_.x,
                                            begin_.point.y + direction_.y)));
}

IShape* Ray::Clone() const { return new Ray(*this); }

std::string Ray::ToString() {
  std::ostringstream string_stream;
  string_stream << "Ray(" << begin_.ToString() << ", " << direction_.ToString()
                << ")";
  return string_stream.str();
}

// ---------------------------------> Line <---------------------------------

Line::Line(const Point& begin, const Point& end) : first_(begin), second_(end) {
  a_ = end.point.y - begin.point.y;  // y2 - y1
  b_ = begin.point.x - end.point.x;  // x1 - x2
  c_ = begin.point.x * (begin.point.y - end.point.y) +
       begin.point.y * (end.point.x - begin.point.x);
  // x1 * (y1 - y2) + y1 * (x2 - x1)
}

IShape& Line::Move(const Vector& shift) {
  c_ -= (a_ * shift.x + b_ * shift.y);
  return *this;
}

bool Line::ContainsPoint(const Point& other) const {
  return (a_ * other.point.x + b_ * other.point.y + c_ == 0);
}

bool Line::CrossesSegment(const Segment& other) const {
  auto borders = other.GetBorders();

  Vector direction = second_ - first_;
  Vector first = borders.first - first_;
  Vector second = borders.second - first_;

  return ((direction ^ first) * (direction ^ second)) <= 0;
}

IShape* Line::Clone() const { return new Line(*this); }

std::string Line::ToString() {
  std::ostringstream string_stream;
  string_stream << "Line(" << a_ << ", " << b_ << ", " << c_ << ")";
  return string_stream.str();
}

// ---------------------------------> Circle <---------------------------------

static int LinearEquation(double k, double b, double* x) {
  if (k == 0) {
    return -1;
  }

  *x = -b / k;

  return 1;
}

static int SquareEquat(double a, double b, double c, double* x1, double* x2) {
  if (a == 0) {
    return LinearEquation(b, c, x1);
  }

  double d = (b * b) - (4 * a * c);
  if (d < 0) {
    return -1;
  }

  double cached_sqrt = std::sqrt(d);

  *x1 = (-b + cached_sqrt) / (2 * a);
  *x2 = (-b - cached_sqrt) / (2 * a);

  return 2;
}

Circle::Circle(const Point& k_center, int radius)
    : center_(k_center), radius_(radius) {}

IShape& Circle::Move(const Vector& shift) {
  center_.Move(shift);
  return *this;
}

bool Circle::ContainsPoint(const Point& other) const {
  return (other.point.x - center_.point.x) * (other.point.x - center_.point.x) +
             (other.point.y - center_.point.y) *
                 (other.point.y - center_.point.y) <=
         radius_ * radius_;
}

bool Circle::CrossesSegment(const Segment& other) const {
  auto borders = other.GetBorders();
  long long x1 = borders.first.point.x;
  long long x2 = borders.second.point.x;
  long long y1 = borders.first.point.y;
  long long y2 = borders.second.point.y;

  long long a = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
  long long b = 2 * (x2 - x1) * x1 + 2 * (y2 - y1) * y1 -
                2 * center_.point.x * (x2 - x1) -
                2 * center_.point.y * (y2 - y1);
  long long c = x1 * x1 + y1 * y1 - 2 * x1 * center_.point.x -
                2 * y1 * center_.point.y + center_.point.x * center_.point.x +
                center_.point.y * center_.point.y - radius_ * radius_;

  double p1 = -1;
  double p2 = -1;

  int answer = SquareEquat(a, b, c, &p1, &p2);
  if (answer == 2) {
    return (p1 >= 0. && p1 <= 1.) || (p2 >= 0. && p2 <= 1.);
  }
  if (answer == 1) {
    return (p1 >= 0. && p1 <= 1.);
  }
  return false;
}

IShape* Circle::Clone() const { return new Circle(*this); }

std::string Circle::ToString() {
  std::ostringstream string_stream;
  string_stream << "Circle(" << center_.ToString() << ", " << radius_ << ")";
  return string_stream.str();
}

// ---------------------------------> Polygon <---------------------------------

Polygon::Polygon(const std::vector<Point>& vertexes) : vertexes_(vertexes) {}

IShape& Polygon::Move(const Vector& shift) {
  for (auto it = vertexes_.begin(); it != vertexes_.end(); ++it) {
    it->Move(shift);
  }

  return *this;
}

bool Polygon::ContainsPoint(const Point& other) const {
  size_t size = vertexes_.size();

  for (size_t i = 0; i < size; ++i) {
    if (Segment(vertexes_[i], vertexes_[(i + 1) % size]).ContainsPoint(other)) {
      return true;
    }
  }

  bool is_found = false;
  int count = 0;
  Ray ray;

  while (!is_found) {
    Vector rand_dir(rand() % 80 + 1, rand() % 80 + 1);
    ray = Ray(other, rand_dir);
    is_found = true;
    for (auto it = vertexes_.begin(); it != vertexes_.end(); ++it) {
      if (ray.ContainsPoint(*it)) {
        is_found = false;
      }
    }
  }

  for (size_t i = 0; i < size; ++i) {
    if (ray.CrossesSegment(Segment(vertexes_[i], vertexes_[(i + 1) % size]))) {
      ++count;
    }
  }

  return (bool)(count % 2);
}

bool Polygon::CrossesSegment(const Segment& other) const {
  for (size_t i = 0; i < vertexes_.size() - 1; ++i) {
    Segment temp(vertexes_[i], vertexes_[i + 1]);
    if (temp.CrossesSegment(other)) {
      return true;
    }
  }

  return false;
}

IShape* Polygon::Clone() const { return new Polygon(*this); }

std::string Polygon::ToString() {
  std::ostringstream string_stream;
  string_stream << "Polygon(";

  for (size_t i = 0; i < vertexes_.size() - 1; ++i) {
    string_stream << vertexes_[i].ToString() << ", ";
  }
  string_stream << vertexes_[vertexes_.size() - 1].ToString() << ")";

  return string_stream.str();
}

}  // namespace Geometry
