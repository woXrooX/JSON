#include <iostream>

#include "json.hpp"

int main(){

  std::string sampleJSON = R"(
    {
      "int": 5,
      "str": "Some Text",
      "arr": [1, 2, 3, "a", "b"],
      "obj": {
        "child": false
      }
    }
  )";

  ///// Examples
  // tokenizer
  // woXrooX::Tokenizer t("./source/test.json");

  // lexer
  woXrooX::Lexer::init(sampleJSON);



  // JSON
  // std::string a = "aaa";
  // woXrooX::JSON b;
  // woXrooX::JSON j;
  // j = b;
  // b = 5;
  // std::cout << b.getRealNumber() << '\n';
  // std::cout << "\nSize: " << sizeof(b) << '\n';

  // woXrooX::JSON a, b;

  // a["a"] = 10.5;
  // std::cout << a["a"].getRealNumber() << '\n';

  return 0;
}
