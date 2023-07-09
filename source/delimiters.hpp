#ifndef DELIMITERS_HPP
#define DELIMITERS_HPP

namespace woXrooX{
  enum class Delimiters{
    // Bracket Delimiters
    curlyBracketOpen,
    curlyBracketClose,

    squareBracketOpen,
    squareBracketClose,

    doubleQuotesOpen,
    doubleQuotesClose,

    // Field and Record Delimiters
    // Field delimiters separate data fields
    // Record delimiters separate groups of fields
    // For example, the CSV file format uses a comma as the delimiter between fields,
    // and an end-of-line indicator as the delimiter between records
    // fname,lname,age,salary
    // nancy,davolio,33,$30000
    whiteSpace,
    colon,
    comma,
    dot,

    unset

  };

}

#endif
