#ifndef __XML_DSL_H__
#define __XML_DSL_H__

#include <assert.h>
#include <ext/hash_map>
#include <iostream>
#include <string>
#include <vector>

namespace std { using namespace __gnu_cxx; }

using std::hash;
using std::string;

namespace __gnu_cxx {
  template<>
  struct hash<std::string> {
    size_t operator()( const std::string& x ) const {
      return hash< const char* >()( x.c_str() );
    }
  };
}

namespace xml_dsl {

class TextElement;

struct Argument {
 public:
  Argument(const string& arg_name) : name(arg_name), value("") { }
  Argument(const string& arg_name, const string& arg_value)
      : name(arg_name), value(arg_value) { }

  Argument& operator()(const string& arg_value) {
    Argument* new_argument = new Argument(name);
    new_argument->value = arg_value;
    return *new_argument;
  }

  string name;
  string value;
};

class Element {
 public:
  Element(const string& tag_name);

  Element(const Element& src);

  void SetHidden(bool hidden) { hidden_ = hidden; }
  void SetCloseTag(bool close_tag) { close_tag_ = close_tag; }

  Element& operator()();

  void Print(int indent = 0);
  string ToString(int indent = 0);

 protected:
  virtual void ToString(int indent, string* output);
  virtual void ElementToString(int indent, string* output);

  template<class ElementType>
  Element& AddChild(const ElementType& child) {
    Element* new_child =  new ElementType(child);

    if (children_) {
      Element* last_child = children_;
      for (; last_child->next_; last_child = last_child->next_) { }

      last_child->next_ = new_child;
      new_child->prev_ = last_child;
    } else {
      children_ = new_child;
    }

    new_child->parent_ = this;

    return *new_child;
  }

  string tag_name_;
  bool hidden_;
  bool close_tag_;
  bool allocated_;
  Element* mount_point_;
  Element* children_;
  Element* next_;
  Element* prev_;
  Element* parent_;
  std::hash_map<string, string> arguments_;
  std::vector<Element*> allocations_;

  friend Element& operator,(const Element& left, const Element& right);
  friend Element& operator,(const Element& left, const string& contents);
  friend Element& operator,(const Element& element, const Argument& right);
};

class TextElement : public Element {
 public:
  TextElement(const string& contents);

 protected:
  virtual void ElementToString(int indent, string* output);

 private:
  Element* parent_;
  string contents_;
};


class Document : public Element {
 public:
  Document() : Element("Document") {
    SetHidden(true);
  }
};

#define TAG(TAGNAME) \
class Element_ ## TAGNAME : public Element { \
 public: \
   Element_ ## TAGNAME () : Element(#TAGNAME) { } \
   explicit Element_ ## TAGNAME (const string& name) : Element(name) { } \
}; extern Element_ ## TAGNAME TAGNAME; \
class CloseElement_ ## TAGNAME : public Element_ ## TAGNAME { \
 public: \
   CloseElement_ ## TAGNAME () : Element_ ## TAGNAME(#TAGNAME) {\
    SetCloseTag(true); \
   } \
};  extern CloseElement_ ## TAGNAME _ ## TAGNAME

#define TAG_DECL(TAGNAME) Element_ ## TAGNAME TAGNAME; \
    CloseElement_##TAGNAME _ ## TAGNAME

#define ARGUMENT(name) extern Argument name
#define ARGUMENT_DECL(name) Argument name(#name)

#define TEXT_NODE(x) #x

}  // namespace xml_dsl

#endif // __XML_DSL_H__
