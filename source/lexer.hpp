#ifndef LEXER_HPP
#define LEXER_HPP
#include <iostream>
#include <cstdlib>


namespace woXrooX{
  class Lexer{
  public:
    static void init(const std::string& data){
      Lexer::data = data;

      Lexer::lex();

      Lexer::dump();

    }

  private:
    static void lex(){
      for(; Lexer::currentIndex < Lexer::data.length(); Lexer::currentIndex++){
        if(Lexer::hasError == true) return;

        switch(Lexer::data[Lexer::currentIndex]){
          //// Objects
          // curlyBracketOpen
          case '{':
            if(Lexer::currentType == Types::string){
              Lexer::handleString();
              break;
            }

            if(Lexer::currentType == Types::unset){
              Lexer::handleObject();
              break;
            }

            break;

          // curlyBracketClose
          case '}':
            if(Lexer::currentType == Types::string){
              Lexer::handleString();
              break;
            }

            break;

          //// Arrays
          // squareBracketOpen
          case '[':
            if(Lexer::currentType == Types::string){
              Lexer::handleString();
              break;
            }

            if(Lexer::currentType == Types::unset){
              Lexer::handleArray();
              break;
            }

            break;

          // squareBracketClose
          case ']':
            if(Lexer::currentType == Types::string){
              Lexer::handleString();
              break;
            }

            break;

          //// Real Numbers
          // Negative
          case '-':
          // Decimal Point
          case '.':
          // 0 - 9
          case '0' ... '9':
            if(Lexer::currentType == Types::string){
              Lexer::handleString();
              break;
            }

            if(Lexer::currentType == Types::realNumber){
              Lexer::handleRealNumber();
              break;
            }

            if(Lexer::currentType == Types::unset){
              Lexer::currentType = Types::realNumber;
              Lexer::handleRealNumber();
              break;
            }

            break;

          //// doubleQuotes
          case '"':
            // String Start
            if(Lexer::currentType == Types::unset){
              Lexer::currentDelimiter = Delimiters::doubleQuotesOpen;
              Lexer::currentType = Types::string;

              Lexer::handleString();

              break;
            }

          //// Boolean
          // True
          case 't':
            if(Lexer::currentType == Types::string){
              Lexer::handleString();
              break;
            }

            if(Lexer::currentType == Types::boolean){
              Lexer::handleBooleanTrue();
              break;
            }

            if(Lexer::currentType == Types::unset){
              Lexer::currentType = Types::boolean;
              Lexer::handleBooleanTrue();
              break;
            }

            break;

          // True
          case 'f':
            if(Lexer::currentType == Types::string){
              Lexer::handleString();
              break;
            }

            if(Lexer::currentType == Types::boolean){
              Lexer::handleBooleanFalse();
              break;
            }

            if(Lexer::currentType == Types::unset){
              Lexer::currentType = Types::boolean;
              Lexer::handleBooleanFalse();
              break;
            }

            break;

          // Null
          case 'n':
            if(Lexer::currentType == Types::string){
              Lexer::handleString();
              break;
            }

            if(Lexer::currentType == Types::null){
              Lexer::handleBooleanFalse();
              break;
            }

            if(Lexer::currentType == Types::unset){
              Lexer::currentType = Types::null;
              Lexer::handleNull();
              break;
            }

            break;

          default:
            break;

        }
      }

    }

    ///////////// Helpers
    static void dump(){
      for(auto &token: Lexer::tokens){
        switch(token.type){
          case Types::object:
            std::cout << "Type: object";
            break;

          case Types::array:
            std::cout << "Type: array";
            break;

          case Types::string:
            std::cout << "Type: string";
            break;

          case Types::realNumber:
            std::cout << "Type: realNumber";
            break;

          case Types::boolean:
            std::cout << "Type: boolean";
            break;

          case Types::null:
            std::cout << "Type: null";
            break;

          case Types::unset:
            std::cout << "Type: unset";
            break;

          default:
            std::cout << "Default";
            break;
        }
        std::cout << " - Value: " << token.value << '\n';
        std::cout << "-----------------------------" << '\n';
      }

      std::cout << Lexer::errorMessage << '\n';

    }

    static void saveToken(){
      Lexer::token.value = Lexer::currentValue;
      Lexer::token.type = Lexer::currentType;
      Lexer::tokens.push_back(Lexer::token);

      Lexer::cleanCurrents();
    }

    static void cleanCurrents(){
      Lexer::currentType = Types::unset;
      Lexer::currentDelimiter = Delimiters::unset;
      Lexer::currentValue = "";
    }

    ////// Handlers
    static void handleObject(){
      // Skip the squareBracketOpen '{'
      Lexer::currentIndex++;

      while(true){
        // Check if end of the data
        if(Lexer::currentIndex >= Lexer::data.length()) break;

        Lexer::lex();

        // Lexer::currentValue += Lexer::data[Lexer::currentIndex];
        Lexer::currentIndex++;
      }
    }

    static void handleArray(){
      // Skip the squareBracketOpen '['
      Lexer::currentIndex++;

      while(true){
        // Check if end of the data
        if(Lexer::currentIndex >= Lexer::data.length()) break;

        // Skip the comma ','
        if(Lexer::data[Lexer::currentIndex] == ','){
          Lexer::currentIndex++;
          continue;
        }

        Lexer::lex();

        // Lexer::currentValue += Lexer::data[Lexer::currentIndex];
        Lexer::currentIndex++;
      }
    }

    static void handleString(){
      // Skip the doubleQuotesOpen '"'
      Lexer::currentIndex++;

      while(true){
        // Check if end of the data
        if(Lexer::currentIndex == Lexer::data.length()) break;

        if(
          // Check if end of the string
          Lexer::data[Lexer::currentIndex] == '"' &&

          // Check If not escaped double quotes inside string
          (Lexer::currentIndex - 1) >= 0 &&
          Lexer::data[Lexer::currentIndex - 1] != '\\'
        ) break;

        Lexer::currentValue += Lexer::data[Lexer::currentIndex];
        Lexer::currentIndex++;
      }

      Lexer::saveToken();

      // Skip the doubleQuotesClose '"'
      Lexer::currentIndex++;

    }

    static void handleRealNumber(){
      bool dot_used = false;

      while(true){
        if(
          // Check if numeric value
          (Lexer::data[Lexer::currentIndex] > '9' || Lexer::data[Lexer::currentIndex] < '0') &&

          // '-'
          Lexer::data[Lexer::currentIndex] != '-' &&

          // '.'
          Lexer::data[Lexer::currentIndex] != '.'

        ) break;

        // Allow only one floating point in numeric values
        if(Lexer::data[Lexer::currentIndex] == '.'){
          if(dot_used == true){
            Lexer::hasError = true;
            Lexer::errorMessage = "ERROR: Invalid JSON. More than one dots used in float numbers!\n";
            return;
          }

          dot_used = true;
        }

        Lexer::currentValue += Lexer::data[Lexer::currentIndex];
        Lexer::currentIndex++;
      }

      Lexer::saveToken();

    }

    static void handleBooleanTrue(){
      if(
        (Lexer::currentIndex + 3) <= Lexer::data.length() &&
        Lexer::data[Lexer::currentIndex + 1] == 'r' &&
        Lexer::data[Lexer::currentIndex + 2] == 'u' &&
        Lexer::data[Lexer::currentIndex + 3] == 'e'
      ){
        Lexer::currentIndex += 3;
        Lexer::currentValue = "true";
        Lexer::saveToken();
      }else{
        Lexer::hasError = true;
        Lexer::errorMessage = "ERROR: Invalid JSON. Invalid boolean value! (true)\n";
        return;
      }
    }

    static void handleBooleanFalse(){
      if(
        (Lexer::currentIndex + 4) <= Lexer::data.length() &&
        Lexer::data[Lexer::currentIndex + 1] == 'a' &&
        Lexer::data[Lexer::currentIndex + 2] == 'l' &&
        Lexer::data[Lexer::currentIndex + 3] == 's' &&
        Lexer::data[Lexer::currentIndex + 4] == 'e'
      ){
        Lexer::currentIndex += 4;
        Lexer::currentValue = "false";
        Lexer::saveToken();
      }else{
        Lexer::hasError = true;
        Lexer::errorMessage = "ERROR: Invalid JSON. Invalid boolean value! (false)\n";
        return;
      }
    }

    static void handleNull(){
      if(
        (Lexer::currentIndex + 3) <= Lexer::data.length() &&
        Lexer::data[Lexer::currentIndex + 1] == 'u' &&
        Lexer::data[Lexer::currentIndex + 2] == 'l' &&
        Lexer::data[Lexer::currentIndex + 3] == 'l'
      ){
        Lexer::currentIndex += 3;
        Lexer::currentValue = "null";
        Lexer::saveToken();
      }else{
        Lexer::hasError = true;
        Lexer::errorMessage = "ERROR: Invalid JSON. Invalid null value!\n";
        return;
      }
    }

    static bool hasError;
    static std::string errorMessage;

    static std::string data;

    static std::string currentValue;
    static unsigned int currentIndex;
    static unsigned int previousIndex;

    static Types currentType;
    static Delimiters currentDelimiter;

    static Token token;
    static std::vector<Token> tokens;
  };

  bool Lexer::hasError = false;
  std::string Lexer::errorMessage = "";
  std::string Lexer::data = "";
  std::string Lexer::currentValue = "";
  unsigned int Lexer::currentIndex = 0;
  unsigned int Lexer::previousIndex = 0;
  Types Lexer::currentType = Types::unset;
  Delimiters Lexer::currentDelimiter = Delimiters::unset;

  Token Lexer::token;
  std::vector<Token> Lexer::tokens;

}


#endif
