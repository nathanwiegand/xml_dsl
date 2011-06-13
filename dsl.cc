#include <iostream>
#include "html.h"

int main() {
  using namespace html;

  // TODO(nathanwiegand): Need to make sure everything returns subtrees
  // correctly. If we take the parens off of the script tag below, the argument
  // gets lost.

  Document document;
  document
   , HTML
   ,   HEAD
   ,     STYLE
   ,     _STYLE
   ,     (SCRIPT, src("http://google.com/jquery.js"), type("text/javascript"))
   ,     _SCRIPT
   ,     SCRIPT
   ,       TEXT_NODE(
            function hello() {
              alert('This is a string with "quotes" in it');
            }
           )
   ,     _SCRIPT
   ,   _HEAD
   ,   BODY
   ,     TEXT_NODE(
          This is an exmaple of a really really\nlong string.
          \nThis thing is rediculous.
         )
   ,    "This is also a text node, but just implied"
  ;

  std::cout << document.ToString() << std::endl;

  return 0;
}
