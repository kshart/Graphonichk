#include "grDOM.h"

using namespace DOM;

DOMException::DOMException(ExceptionCode c) :code(c) {
	
};
const char* DOMException::what() const throw() {
	switch(code) {
		case ExceptionCode::INDEX_SIZE_ERR:				return "DOMException INDEX_SIZE_ERR";
		case ExceptionCode::DOMSTRING_SIZE_ERR:			return "DOMException DOMSTRING_SIZE_ERR";
		case ExceptionCode::HIERARCHY_REQUEST_ERR:		return "DOMException HIERARCHY_REQUEST_ERR";
		case ExceptionCode::WRONG_DOCUMENT_ERR:			return "DOMException WRONG_DOCUMENT_ERR";
		case ExceptionCode::INVALID_CHARACTER_ERR:		return "DOMException INVALID_CHARACTER_ERR";
		case ExceptionCode::NO_DATA_ALLOWED_ERR:		return "DOMException NO_DATA_ALLOWED_ERR";
		case ExceptionCode::NO_MODIFICATION_ALLOWED_ERR:return "DOMException NO_MODIFICATION_ALLOWED_ERR";
		case ExceptionCode::NOT_FOUND_ERR:				return "DOMException NOT_FOUND_ERR";
		case ExceptionCode::NOT_SUPPORTED_ERR:			return "DOMException NOT_SUPPORTED_ERR";
		case ExceptionCode::INUSE_ATTRIBUTE_ERR:		return "DOMException INUSE_ATTRIBUTE_ERR";
		#if defined(DOM2)
			case ExceptionCode::INVALID_STATE_ERR:			return "DOMException INVALID_STATE_ERR";
			case ExceptionCode::SYNTAX_ERR:					return "DOMException SYNTAX_ERR";
			case ExceptionCode::INVALID_MODIFICATION_ERR:	return "DOMException INVALID_MODIFICATION_ERR";
			case ExceptionCode::NAMESPACE_ERR:				return "DOMException NAMESPACE_ERR";
			case ExceptionCode::INVALID_ACCESS_ERR:			return "DOMException INVALID_ACCESS_ERR";
		#endif
	}
	return nullptr;
}

DOMImplementation::DOMImplementation(unsigned char core) :Core(core) {
	
}
bool DOMImplementation::hasFeature(DOMString feature, DOMString version) {
	if (feature == "Core") {
		if (version == "1.0" && Core>=10) return true;
	}else if (feature == "XML") {
	}else if (feature == "HTML") {
	}else if (feature == "Views") {
	}else if (feature == "StyleSheets") {
	}else if (feature == "CSS") {
	}else if (feature == "CSS2") {
	}else if (feature == "Events") {
	}else if (feature == "UIEvents") {
	}else if (feature == "MouseEvents") {
	}else if (feature == "MutationEvents") {
	}else if (feature == "HTMLEvents") {
	}else if (feature == "Range") {
	}else if (feature == "Traversal") {
	}
	return false;
}

inline NodeList::iterator NodeList::begin() noexcept {
	return this->vector<Node*>::begin();
}
inline NodeList::iterator NodeList::end() noexcept {
	return this->vector<Node*>::end();
}
inline Node* NodeList::item(size_t index) {
	if (index >= this->vector<Node*>::size()) return nullptr; 
	return this->vector<Node*>::at(index);
}
inline const size_t NodeList::length() const noexcept {
	return this->vector<Node*>::size();
}

inline NamedNodeMap::iterator NamedNodeMap::begin() noexcept {
	return this->vector<Node*>::begin();
}
inline NamedNodeMap::iterator NamedNodeMap::end() noexcept {
	return this->vector<Node*>::end();
}
Node* NamedNodeMap::getNamedItem(DOMString name) const {
	for(auto node=this->vector<Node*>::begin(); node!=this->vector<Node*>::end(); ++node) {
		if ((*node)->nodeName() == name) return *node;
	}
	return nullptr;
};
Node* NamedNodeMap::setNamedItem(Node* arg) {
	if (arg == nullptr) return nullptr;
	for(auto node=this->vector<Node*>::begin(); node!=this->vector<Node*>::end(); ++node) {
		if ((*node)->nodeName() == arg->nodeName()) {
			(*node) = arg;
			return arg;
		}
	}
	this->vector<Node*>::push_back(arg);
	return arg;
};
Node* NamedNodeMap::removeNamedItem(DOMString name) {
	if (name.empty()) return nullptr;
	for(auto node=this->vector<Node*>::begin(); node!=this->vector<Node*>::end(); ++node) {
		if ((*node)->nodeName() == name) {
			this->vector<Node*>::erase(node);
			return (*node);
		}
	}
	return nullptr;
};
inline const size_t NamedNodeMap::length() const noexcept {
	return this->vector<Node*>::size();
}
inline Node* NamedNodeMap::item(size_t index) {
	if (index >= this->vector<Node*>::size()) return nullptr; 
	return this->vector<Node*>::at(index);
}

Node::Node(Node::NodeType nt) :nodeType(nt) {
	
};
const DOMString	&Node::nodeName() const noexcept {
	
}
DOMString &Node::nodeValue() noexcept {
	
}
inline const Node* Node::parentNode() const noexcept {
	return _parentNode;
}
inline const Node* Node::firstChild() const noexcept {
	if (childNodes.empty()) return nullptr;
	return childNodes.front();
}
inline const Node* Node::lastChild() const noexcept {
	if (childNodes.empty()) return nullptr;
	return childNodes.back();
}
inline const Node* Node::previousSibling() const noexcept {
	if (_id==0 || _parentNode==nullptr) return nullptr;
	return _parentNode->childNodes[_id-1];
}
inline const Node* Node::nextSibling() const noexcept {
	if (_parentNode==nullptr || _id>=_parentNode->childNodes.size()-1) return nullptr;
	return _parentNode->childNodes[_id+1];
}
Node* Node::insertBefore(Node* newChild, Node* refChild) {
	if (newChild == nullptr || newChild==this || refChild==this) {
		throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
		return nullptr;
	}
	if (refChild == nullptr) return this->appendChild(newChild);
	if (refChild->_parentNode != this) {
		throw DOMException(DOMException::NOT_FOUND_ERR);
		return nullptr;
	}
	if (newChild->_parentNode != nullptr) newChild->_parentNode->removeChild(newChild);
	childNodes.insert(childNodes.begin()+refChild->_id, newChild);
	return newChild;
	
};//DocumentFragment
Node* Node::replaceChild(Node* newChild, Node* oldChild) {
	if (newChild == nullptr || oldChild == nullptr) return nullptr;
	if (oldChild->_parentNode != this) return nullptr;
	if (newChild->_parentNode != nullptr) newChild->_parentNode->removeChild(newChild);
	newChild->_parentNode = this;
	newChild->_id = oldChild->_id;
	childNodes[newChild->_id] = newChild;
	oldChild->_parentNode = nullptr;
	oldChild->_id = 0;
	return oldChild;
};
Node* Node::removeChild(Node* oldChild) {
	if (oldChild==nullptr || oldChild==this) {
		throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
		return nullptr;
	}
	if (oldChild->_parentNode == nullptr) {
		throw DOMException(DOMException::NOT_FOUND_ERR);
		return nullptr;
	}
	childNodes.erase(childNodes.begin()+oldChild->_id);
	for(size_t i=oldChild->_id; i<childNodes.size(); i++) {
		childNodes[i]->_id = i;
	}
	oldChild->_id = 0;
	oldChild->_parentNode = nullptr;
	return oldChild;
};
Node* Node::appendChild(Node* newChild) {
	if (newChild == nullptr || newChild==this)  {
		throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
		return nullptr;
	}
	if (newChild->_parentNode != nullptr) newChild->_parentNode->removeChild(newChild);
	newChild->_parentNode = this;
	newChild->_id = childNodes.size();
	childNodes.push_back(newChild);
	return newChild;
};
inline bool Node::hasChildNodes() const noexcept {
	return childNodes.empty();
}
Node* Node::cloneNode(bool deep) {
	return nullptr;
};
void Node::trace() {
	printf("<node id='%i' name='%s' value='%s' parent='%i'", this->_id, this->nodeName().c_str(), this->nodeValue().c_str(), _parentNode);
	for(auto attr : attributes) {
		printf("\n\t%s='%s'", attr->nodeName().c_str(), attr->nodeValue().c_str());
	}
	printf(">\n");
	for(auto node : childNodes) {
		node->trace();
	}
	puts("</node>");
};

CharacterData::CharacterData(Node::NodeType type) :Node(type) {
	
}
inline const size_t CharacterData::length() const noexcept {
	return data.size();
}
DOMString CharacterData::substringData(size_t offset, size_t count) {
	return data.substr(offset, count);
};
void CharacterData::appendData(DOMString arg) {
	//if (_readonly) throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
	data += arg;
};
void CharacterData::deleteData(size_t offset, size_t count) {
	//if (_readonly) throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
	//if (offset > data.length()) throw DOMException(DOMException::INDEX_SIZE_ERR);
	//if (offset+count > data.length()) throw DOMException(DOMException::INDEX_SIZE_ERR);
	data.erase(offset, count);
};
void CharacterData::insertData(size_t offset, DOMString &arg) {
	//if (_readonly) throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
	//if (offset > data.length()) throw DOMException(DOMException::INDEX_SIZE_ERR);
	data.insert(offset, arg);
};
void CharacterData::replaceData(size_t offset, size_t count, DOMString &arg) {
	//if (_readonly) throw DOMException(DOMException::NO_MODIFICATION_ALLOWED_ERR);
	//if (offset > data.length()) throw DOMException(DOMException::INDEX_SIZE_ERR);
	//if (offset+count > data.length()) throw DOMException(DOMException::INDEX_SIZE_ERR);
	data.replace(offset, count, arg);
};

const DOMString &Attr::nodeName() const noexcept {
	return this->name;
};
DOMString &Attr::nodeValue() noexcept {
	return this->value;
};

void Element::getElementsByTagName(NodeList &list, DOMString &name) {
	for(auto node : this->childNodes) {
		Element *element = dynamic_cast<Element*>(node);
		if (element != nullptr) {
			if (element->tagName == name) list.push_back(element);
			if (!element->childNodes.empty()) element->getElementsByTagName(list, name);
		}
	}
}
const DOMString Element::getAttribute(DOMString name) const noexcept {
	Attr *attr = dynamic_cast<Attr*>(this->attributes.getNamedItem(name));
	if (attr!=nullptr) return attr->value;
}
void Element::setAttribute(DOMString name, DOMString value){
	Attr *attr = dynamic_cast<Attr*>(this->attributes.getNamedItem(name));
	if (attr != nullptr) attr->value = value;
}
void Element::removeAttribute(DOMString name) {
	this->attributes.removeNamedItem(name);
}
Attr* Element::getAttributeNode(DOMString name) {
	return dynamic_cast<Attr*>(this->attributes.getNamedItem(name));
}
Attr* Element::setAttributeNode(Attr* newAttr) {
	return (Attr*)this->attributes.setNamedItem(newAttr);
}
Attr* Element::removeAttributeNode(Attr* oldAttr) {
	if (oldAttr == nullptr) return nullptr;
	return (Attr*)this->attributes.removeNamedItem(oldAttr->value);
}
NodeList Element::getElementsByTagName(DOMString name) {
	NodeList list;
	this->getElementsByTagName(list, name);
	return list;
}
Element::Element(DOMString tag) :Node(Node::ELEMENT_NODE), tagName(tag) {
	
};
const DOMString &Element::nodeName() const noexcept {
	return tagName;
};
Node* Element::appendChild(Node* newChild) {
	if (newChild == nullptr || newChild==this)  {
		throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);
		return nullptr;
	}
	if (newChild->_parentNode != nullptr) newChild->_parentNode->removeChild(newChild);
	switch (newChild->nodeType) {
		case Node::ELEMENT_NODE:
			newChild->_parentNode = this;
			newChild->_id = childNodes.size();
			childNodes.push_back(newChild);
			return newChild;
		case Node::TEXT_NODE:
			newChild->_parentNode = this;
			newChild->_id = childNodes.size();
			childNodes.push_back(newChild);
			return newChild;
	}
	return nullptr;
};

Text::Text(DOMString str) :CharacterData(Node::TEXT_NODE) {
	this->data = str;
};


DocumentFragment::DocumentFragment() :Node(Node::DOCUMENT_FRAGMENT_NODE) {
	
}

DocumentType::DocumentType() :Node(Node::DOCUMENT_TYPE_NODE) {
	
};

Document::Document() :Node(Node::DOCUMENT_NODE), simplementation(10), documentElement("main") {
	
}