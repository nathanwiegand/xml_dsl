#include "xml_dsl.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::hash_map;

namespace xml_dsl {

Element::Element(const string& tag_name)
    : tag_name_(tag_name),
      hidden_(false),
      close_tag_(false),
      allocated_(false),
      mount_point_(NULL),
      children_(NULL),
      next_(NULL),
      prev_(NULL),
      parent_(NULL) {
}

Element::Element(const Element& src) : arguments_(src.arguments_) {
  tag_name_ = src.tag_name_;
  hidden_ = src.hidden_;
  close_tag_ = src.close_tag_;
  allocated_ = src.allocated_;
  mount_point_ = src.mount_point_;
  children_ = src.children_;
  next_ = src.next_;
  prev_ = src.prev_;
  parent_ = src.parent_;
}

Element& Element::operator()() {
  // Go all the way to the top of the document and return it
  if (prev_) {
    return (*prev_)();
  } else if (parent_) {
    return (*parent_)();
  } else {
    return *this;
  }
}

void Element::Print(int indent) {
  std::cout << ToString(indent);
}

string Element::ToString(int indent) {
  string output;
  ToString(indent, &output);
  return output;
}

void Element::ToString(int indent, string* output) {
  ElementToString(indent, output);
  if (next_) {
    next_->ToString(indent, output);
  }
}

void Element::ElementToString(int indent, string* output) {
  string tabs;
  for (int i = 0; i < indent; ++i) {
    tabs.push_back(' ');
    tabs.push_back(' ');
  }

  if (close_tag_) {
    output->append(tabs);
    output->append("</");
    output->append(tag_name_);
    output->append(">\n");
    return;
  }

  if (!hidden_) {
    output->append(tabs + "<");
    output->append(tag_name_);

    for (std::hash_map<string, string>::iterator it = arguments_.begin();
         it != arguments_.end(); ++it) {
      output->append(" " + it->first);
      output->append("=\"" + it->second + "\"");
    }
    output->append(">\n");
  }

  if (children_) {
    children_->ToString(indent + 1, output);
  }

  if (!hidden_) {
    output->append(tabs);
    output->append("</");
    output->append(tag_name_);
    output->append(">\n");
  }
}


TextElement::TextElement(const string& contents) : Element(""),
                                                   parent_(NULL),
                                                   contents_(contents) { }
void TextElement::ElementToString(int indent, string* output) {
  string tabs;
  for (int i = 0; i < indent; ++i) {
    tabs.push_back(' ');
    tabs.push_back(' ');
  }

  string contents = tabs + contents_;
  string::size_type pos = 0;
  while ((pos = contents.find("\n", pos)) != string::npos) {
    contents.replace(pos, 1, "\n" + tabs);
    pos += tabs.length() + 1;
  }

  output->append(contents + "\n");
}

Element& operator,(const Element& element, const Argument& arg) {
  Element* mutable_element = new Element(element);

  if (mutable_element->arguments_.find(arg.name) !=
    mutable_element->arguments_.end()) {
    cerr << "Duplication of argument name" << endl;
  }
  mutable_element->arguments_[arg.name] = arg.value;

  return *mutable_element;
}

Element& operator,(const Element& left, const string& contents) {
  Element* mutable_element = new Element(left);

  mutable_element->mount_point_->AddChild(TextElement(contents));
  return *mutable_element;
}

Element& operator,(const Element& left, const Element& right) {
  Element& mutable_left = const_cast<Element&>(left);

  if (right.close_tag_) {
    mutable_left.mount_point_ = mutable_left.parent_;
    return *mutable_left.parent_;
  }

  if (!mutable_left.mount_point_) {
    mutable_left.mount_point_ = &mutable_left;
  }

  Element& new_element = mutable_left.mount_point_->AddChild(right);
  new_element.mount_point_ = &new_element;
  return new_element;
}

}  // namespace xml_dsl
