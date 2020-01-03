#pragma once

#include <string>

namespace Graphics {
  class Shader {
    unsigned _type, _id;

  public:
    Shader(unsigned type, const std::string &path);
    ~Shader();

    unsigned GetId() const { return _id; }

    void Delete() const;
  };

}
