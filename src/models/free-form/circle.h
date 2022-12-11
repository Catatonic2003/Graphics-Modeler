#pragma once

#include <QBrush>
#include <QPainter>
#include <QPen>
#include <memory>
#include <vector>

#include "models/shape.h"

extern std::vector<std::unique_ptr<Shape>> shapes;

class Circle : public Shape {
 public:
  Circle();
  virtual ~Circle() noexcept;

 public:
  void setRadius(const int &radius);
  int getRadius() const;

 public:
  float area() const override;
  float perimeter() const override;
  void paint(QPainter* painter) override;

 private:
  int radius_ = 0;
};
