#ifndef SKIP_COMMENTS_HPP_
#define SKIP_COMMENTS_HPP_
#include <cstring>
#include <istream>
namespace {
//-----------------------------------------------------------------------
//! skip comments from stream 's'.
/*! \author Motchy, Kameda \date 2009-04-25 Sat.*/
inline std::istream& skipComments(std::istream& s) {
  while (1) {
    while (isspace(s.peek())) // skip white spaces
      s.ignore();
    if (s.peek() == '#')	  // If '#' is found, skip the whole line.
      while (s.get() != '\n') {} // ignore characters until a '\n' appears.
    else
      return s;
  }
}
}
#endif // SKIP_COMMENTS_HPP_
