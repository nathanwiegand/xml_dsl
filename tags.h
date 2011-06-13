#ifndef __HTML_H__
#define __HTML_H__

#include "xml_dsl.h"

namespace html {
using namespace xml_dsl;

TAG(HTML);
TAG(HEAD);
TAG(STYLE);
TAG(SCRIPT);
TAG(BODY);
TAG(FORM);
TAG(DIV);
TAG(A);

ARGUMENT(src);
ARGUMENT(name);
ARGUMENT(type);

}  // namespace html

#endif // __HTML_H__
