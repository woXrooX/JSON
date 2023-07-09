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
  class Json{
  public:

    /////////// Methods


    /////////// Setters
    //// Object | Dictionary
    void operator=(std::map<std::string, Json> object){
      this->value.object = &object;
      this->type = Types::object;

    }

    void setObject(std::map<std::string, Json>& object){
      this->value.object = &object;
      this->type = Types::object;

    }

    // Array | List
    void setObject(std::vector<Json>& array){
      this->value.array = &array;
      this->type = Types::array;

    }

    // String
    void setString(std::string& string){
      this->value.string = &string;
      this->type = Types::string;

    }

    //// Real Number
    void operator=(float realNumber){
      this->setRealNumber(realNumber);

    }

    void setRealNumber(float realNumber){
      this->value.realNumber = realNumber;
      this->type = Types::realNumber;

    }

    //// Boolean
    void operator=(bool boolean){
      this->setBoolean(boolean);

    }

    void setBoolean(bool boolean){
      this->value.boolean = boolean;
      this->type = Types::boolean;

    }

    /////////// Getters
    // Object
    auto getObject() const{if(this->type == Types::object) return *value.object;}

    // Array
    auto getArray() const{if(this->type == Types::array) return *value.array;}

    // String
    auto getString() const{if(this->type == Types::string) return *this->value.string;}

    // Real Number
    float getRealNumber() const{
      if(this->type == Types::realNumber) return value.realNumber;
      // control reaches end of non-void function
      // https://stackoverflow.com/questions/6171500/what-does-control-reaches-end-of-non-void-function-mean
      // Func needs to return something since it's not void func after if it has no return
      throw std::logic_error("Ivalid Type Get Request");
    }

    // Boolean
    auto getBoolean() const{if(this->type == Types::boolean) return value.boolean;}


  private:
    ///// Private Variables
    union Values{
      std::map<std::string, Json>* object;
      std::vector<Json>* array;
      std::string* string;
      float realNumber;
      bool boolean;

    };


    ///// Variables
    Types type;
    Values value;


    ///// Methods


  };

}

#endif
