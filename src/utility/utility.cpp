#include "utility.h"

namespace Utility {
namespace Debug {
void log(const std::string &msg) {
  std::cout << msg << "\n";
}
}  // namespace Debug

namespace Parser {
std::size_t parseHashKey(const std::string &raw) {
  std::string clean = raw.substr(raw.find(":") + 2);
  return std::hash<std::string>{}(clean);
}

void deduceSpecifics(
    std::vector<std::unique_ptr<Shape>> &shapes, std::fstream &fin, const std::string &id,
    const std::string &type, const int &index) {
  std::string cursor;

  std::getline(fin, cursor);
  cursor = cursor.substr(cursor.find(':') + 2);
  std::stringstream sstream(cursor);

  auto nXY = [&]() {
    while (!sstream.eof()) {
      QPoint point;
      std::string x;
      std::string y;

      std::getline(sstream, x, ',');
      std::getline(sstream, y, ',');

      point.setX(std::stoi(x));
      point.setY(std::stoi(y));
      shapes[index]->pushPoint(point);
    }
  };

  auto setXY = [&]() {
    QPoint point;
    std::string x;
    std::string y;

    std::getline(sstream, x, ',');
    std::getline(sstream, y, ',');

    point.setX(std::stoi(x));
    point.setY(std::stoi(y));
    shapes[index]->pushPoint(point);
  };

  switch (strToShapeType.at(type)) {
    case GM::ShapeType::Line: {
      shapes.push_back(std::make_unique<Shape>());
      nXY();
      break;
    }
    case GM::ShapeType::Polyline: {
      shapes.push_back(std::make_unique<Shape>());
      nXY();
      break;
    }
    case GM::ShapeType::Polygon: {
      shapes.push_back(std::make_unique<Shape>());
      nXY();
      break;
    }
    case GM::ShapeType::Rectangle: {
      shapes.push_back(std::make_unique<Shape>());
      setXY();
      // Read rectangle specifics
      break;
    }
    case GM::ShapeType::Square: {
      shapes.push_back(std::make_unique<Shape>());
      setXY();
      // Read square specifics
      break;
    }
    case GM::ShapeType::Ellipse: {
      shapes.push_back(std::make_unique<Shape>());
      setXY();
      // Read ellipse specifics
      break;
    }
    case GM::ShapeType::Circle: {
      shapes.push_back(std::make_unique<Shape>());
      setXY();
      // Read circle specifics
      break;
    }
    default: {
      Debug::log("Shape type invalid.");
      break;
    }
  }
}

void deduceFields(
    std::vector<std::unique_ptr<Shape>> &shapes, std::fstream &fin,
    const std::string &type, const int &index) {
  std::string cursor;
  auto parse = [](std::string &raw) { return raw.substr(raw.find(':') + 2); };

  std::getline(fin, cursor);
  cursor = parse(cursor);
  shapes[index]->setPenColor(QColor(cursor.c_str()));

  std::getline(fin, cursor);
  cursor = parse(cursor);
  shapes[index]->setPenWidth(std::stoi(cursor));

  std::getline(fin, cursor);
  cursor = parse(cursor);
  shapes[index]->setPenStyle(strToPenStyle.at(cursor));

  std::getline(fin, cursor);
  cursor = parse(cursor);
  shapes[index]->setPenCap(strToPenCapStyle.at(cursor));

  std::getline(fin, cursor);
  cursor = parse(cursor);
  shapes[index]->setPenJoin(strToPenJoinStyle.at(cursor));

  if (strToShapeType.at(type) != GM::ShapeType::Line &&
      strToShapeType.at(type) != GM::ShapeType::Polyline) {
    QBrush brush;

    std::getline(fin, cursor);
    cursor = parse(cursor);
    brush.setColor(QColor(cursor.c_str()));

    std::getline(fin, cursor);
    cursor = parse(cursor);
    brush.setStyle(strToBrushStyle.at(cursor));

    shapes[index]->setPenBrush(brush);
  }
}

void parseShapes(std::vector<std::unique_ptr<Shape>> &shapes) {
  std::fstream fin("../src/db/shapes.db", std::ios::in);

  if (!fin.is_open()) {
    Debug::log("File was not able to be opened.");
    return;
  }

  std::string id;
  std::string type;
  int index = 0;
  auto parse = [](std::string &raw) { return raw.substr(raw.find(':') + 2); };

  while (!fin.eof()) {
    std::getline(fin, id);
    id = parse(id);

    std::getline(fin, type);
    type = parse(type);

    deduceSpecifics(shapes, fin, id, type, index);
    deduceFields(shapes, fin, type, index);

    fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    ++index;
  }
}
}  // namespace Parser

namespace Login {
bool isValid(const std::size_t &hash) {
  std::fstream fin("../src/db/users.db", std::ios::in);

  if (!fin.is_open()) {
    Debug::log("File was not able to be opened.");
    return false;
  }

  std::string key;
  for (int i = 0; i < 2; i++) {
    if (!fin.eof()) {
      std::getline(fin, key);
    }
  }
  fin.close();

  try {
    if (Parser::parseHashKey(key) != hash) {
      return false;
    }
  } catch (const std::invalid_argument &e) {
    Debug::log(e.what() + std::string("."));
    return false;
  }

  return true;
}
}  // namespace Login
}  // namespace Utility