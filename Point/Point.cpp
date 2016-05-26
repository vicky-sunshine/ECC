#include "Point.hpp"

/*
 Constructor
 */
Point::Point() {
  this->x = 0;
  this->y = 0;
}
Point::Point(const FiniteFieldElement& x, const FiniteFieldElement& y) {
  this->x = x;
  this->y = y;
}


/*
 Logical Operation
 */
bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator!=(const Point& lhs, const Point& rhs) {
  return !(lhs == rhs);
}

/*
 Output Format
 */
std::ostream& operator<<(std::ostream& os, const Point& p) {
  if (p.x == 0 && p.y == 0) {
    os << "ZERO";
  } else {
    os << "(" << p.x << ", " << p.y << ")";
  }
  return os;
}