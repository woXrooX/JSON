#ifndef JSON_HPP
#define JSON_HPP

#include <string>
#include <vector>
#include <map>
#include <stdexcept> // Throw

#include "delimiters.hpp"
#include "types.hpp"
#include "token.hpp"
#include "lexer.hpp"
#include "tokenizer.hpp"

namespace woXrooX{
  class JSON{
  public:
    /////////// Setters
    //// Key
    JSON& operator[](const std::string& key){
      this->key = key;
      return *this;
    }

    //// realNumber
    void operator=(float realNumber){
      this->value.realNumber = realNumber;
      this->type = Types::realNumber;
    }

    /////////// Getters
    // Real Number
    float getRealNumber() const{
      if(this->type == Types::realNumber) return value.realNumber;
      throw std::logic_error("Ivalid Type Get Request");
    }

  private:
    union Values{
      std::map<std::string, JSON>* object;
      std::vector<JSON>* array;
      std::string* string;
      float realNumber;
      bool boolean;
    };

    Types type;
    Values value;
    std::string key;

  };

}

#endif
