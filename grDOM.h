#ifndef GRDOM_H
#define	GRDOM_H

#include <exception>
#include <vector>
#include <string>

#include <stdio.h>
//#define DOM2
namespace DOM {
	using std::string;
	using std::exception;
	using std::vector;
	//using Text = string;
	using DOMString = string;
	using DOMTimeStamp = unsigned long long;
	
	
	class DOMException;
	class DOMImplementation;
	class DocumentFragment;
	class Document;
	class Node;
	class NodeList;
	class NamedNodeMap;
	class CharacterData;
	class Attr;
	class Element;
	class Text;
	class Comment;
	class CDATASection;
	class DocumentType;
	class Entity;
	class EntityReference;
	class Notation;
	class ProcessingInstruction;
	
	class DOMException final	:public exception {
	public:
		enum ExceptionCode:unsigned short {
			INDEX_SIZE_ERR                 = 1,
			DOMSTRING_SIZE_ERR             = 2,
			HIERARCHY_REQUEST_ERR          = 3,
			WRONG_DOCUMENT_ERR             = 4,
			INVALID_CHARACTER_ERR          = 5,
			NO_DATA_ALLOWED_ERR            = 6,
			NO_MODIFICATION_ALLOWED_ERR    = 7,
			NOT_FOUND_ERR                  = 8,
			NOT_SUPPORTED_ERR              = 9,
			INUSE_ATTRIBUTE_ERR            = 10,
			#if defined(DOM2)	
				INVALID_STATE_ERR              = 11,
				SYNTAX_ERR                     = 12,
				INVALID_MODIFICATION_ERR       = 13,
				NAMESPACE_ERR                  = 14,
				INVALID_ACCESS_ERR             = 15,
			#endif
		};
		ExceptionCode code;
		
		DOMException(ExceptionCode c);
		virtual const char* what() const throw();
	};//END
	class DOMImplementation		{
	private:
		const unsigned char Core			= 0;
		const unsigned char XML				= 0;
		const unsigned char HTML			= 0;
		const unsigned char Views			= 0;
		const unsigned char StyleSheets		= 0;
		const unsigned char CSS				= 0;
		const unsigned char CSS2			= 0;
		const unsigned char Events			= 0;
		const unsigned char UIEvents		= 0;
		const unsigned char MouseEvents		= 0;
		const unsigned char MutationEvents	= 0;
		const unsigned char HTMLEvents		= 0;
		const unsigned char Range			= 0;
		const unsigned char Traversal		= 0;
		
		DOMImplementation(unsigned char core);
		friend Document;
	public:
		bool hasFeature(DOMString feature, DOMString version);
		#if defined(DOM2)
			DocumentType	createDocumentType(DOMString qualifiedName, DOMString publicId, DOMString systemId);//raises(DOMException);
			Document		createDocument(DOMString namespaceURI, DOMString qualifiedName, DocumentType doctype);//raises(DOMException);
		#endif
	};
	class NodeList		:protected vector<Node*> {
	private:
		friend Node;
		friend Element;
	public:
		using iterator = vector<Node*>::iterator;
		
		inline iterator		begin	() noexcept;
		inline iterator		end		() noexcept;
		inline Node*		item	(size_t index);
		inline const size_t	length	() const noexcept;
	};
	class NamedNodeMap	:protected vector<Node*> {
	protected:
		friend Node;
	public:
		using iterator = vector<Node*>::iterator;
		inline iterator	begin			() noexcept;
		inline iterator	end				() noexcept;
				Node*	getNamedItem	(DOMString name) const;
				Node*	setNamedItem	(Node* arg);
				Node*	removeNamedItem	(DOMString name);
		inline	const size_t length		() const noexcept;
		inline	Node*	item			(size_t index);
		#if defined(DOM2)
			Node* getNamedItemNS(DOMString namespaceURI, DOMString localName);
			Node* setNamedItemNS(Node arg);
			Node* removeNamedItemNS(DOMString namespaceURI, DOMString localName);
		#endif		
};
	class Node			{
	public:
		enum NodeType:unsigned short { 
			ELEMENT_NODE                   = 1,
			ATTRIBUTE_NODE                 = 2,
			TEXT_NODE                      = 3,
			CDATA_SECTION_NODE             = 4,
			ENTITY_REFERENCE_NODE          = 5,
			ENTITY_NODE                    = 6,
			PROCESSING_INSTRUCTION_NODE    = 7,
			COMMENT_NODE                   = 8,
			DOCUMENT_NODE                  = 9,
			DOCUMENT_TYPE_NODE             = 10,
			DOCUMENT_FRAGMENT_NODE         = 11,
			NOTATION_NODE                  = 12,
		};
		
		virtual const	DOMString	&nodeName		() const noexcept;
		virtual			DOMString	&nodeValue		() noexcept;
		const			NodeType	nodeType;
		inline const	Node*		parentNode		() const noexcept;
						NodeList	childNodes;
		inline const	Node*		firstChild		() const noexcept;
		inline const	Node*		lastChild		() const noexcept;
		inline const	Node*		previousSibling	() const noexcept;
		inline const	Node*		nextSibling		() const noexcept;
						NamedNodeMap attributes;
						Node*		insertBefore	(Node* newChild, Node* refChild);//DocumentFragment
						Node*		replaceChild	(Node* newChild, Node* oldChild);
						Node*		removeChild		(Node* oldChild);
		virtual			Node*		appendChild		(Node* newChild);
		inline			bool		hasChildNodes	() const noexcept;
						Node*		cloneNode		(bool deep);
		
		#if defined(DOM2)
			Document	ownerDocument;
			void		normalize		();
			bool		isSupported		(DOMString feature, DOMString version);
			DOMString	namespaceURI;
			DOMString	prefix; // raises(DOMException) on setting
			DOMString	localName;
			bool		hasAttributes	();
		#endif
		
		virtual void trace();
	protected:
		Node *_parentNode = nullptr;
		size_t _id = 0;
		Node(NodeType nt);
		
		friend Element;
		friend NodeList;
		friend NamedNodeMap;
};
	class CharacterData :public Node {
	protected:
		CharacterData(Node::NodeType type);
	public:
		DOMString			data;															// raises(DOMException) on setting raises(DOMException) on retrieval
		inline const size_t	length			() const noexcept;
		DOMString			substringData	(size_t offset, size_t count);					//raises(DOMException);
		void				appendData		(DOMString arg);								//raises(DOMException);
		void				insertData		(size_t offset, DOMString &arg);				//raises(DOMException);
		void				deleteData		(size_t offset, size_t count);					//raises(DOMException);
		void				replaceData		(size_t offset, size_t count, DOMString &arg);	//raises(DOMException);
	};
	class Attr			:public Node {
	public:
		const DOMString	name;
		const bool		specified;
		DOMString		value;
		#if defined(DOM_LEVEL_2)
			const Element* ownerElement;
		#else
			Attr(DOMString name, bool specified);
		#endif
		
		const	DOMString &nodeName() const noexcept override;
				DOMString &nodeValue() noexcept override;
	};
	class Element		:public Node {
	private:
		void getElementsByTagName(NodeList &list, DOMString &name);
	public:
		const DOMString tagName;
		const DOMString getAttribute		(DOMString name) const noexcept;
		void			setAttribute		(DOMString name, DOMString value);
		void			removeAttribute		(DOMString name);
		Attr*			getAttributeNode	(DOMString name);
		Attr*			setAttributeNode	(Attr* newAttr);
		Attr*			removeAttributeNode	(Attr* oldAttr);
		NodeList		getElementsByTagName(DOMString name);
		
		#if defined(DOM2)
			DOMString          getAttributeNS			(DOMString namespaceURI, DOMString localName);
			void               setAttributeNS			(DOMString namespaceURI, DOMString qualifiedName, DOMString value);
			void               removeAttributeNS		(DOMString namespaceURI, DOMString localName);
			Attr               getAttributeNodeNS		(DOMString namespaceURI, DOMString localName);
			Attr               setAttributeNodeNS		(Attr newAttr);
			NodeList           getElementsByTagNameNS	(DOMString namespaceURI, DOMString localName);
			boolean            hasAttribute				(DOMString name);
			boolean            hasAttributeNS			(DOMString namespaceURI, DOMString localName);
		#endif
		
		Element(DOMString tag);
		const DOMString &nodeName() const noexcept override;
		Node* appendChild(Node* newChild) override;
	};
	class Text			:public CharacterData {
	public:
		Text(DOMString str);
		Text splitText(size_t offset);// raises(DOMException);
	};
	class Comment		:public CharacterData  {
	};
	class CDATASection	:public Text {
	};
	class Notation		:public Node {
	public:
		DOMString publicId;
		DOMString systemId;
	};
	class Entity		:public Node {
	public:
		DOMString publicId;
		DOMString systemId;
		DOMString notationName;
	};
	class EntityReference		:public Node {
	};
	class ProcessingInstruction	:public Node {
	public:
		DOMString target;
		DOMString data;// raises(DOMException) on setting
	};
	class DocumentFragment		:public Node {
	public:
		DocumentFragment();
	};
	class DocumentType	:public Node {
	private:
		DocumentType();
		friend Document;
	public:
		const DOMString		name;
		const NamedNodeMap	entities;
		const NamedNodeMap	notations;
		
		//DocumentType(DOMString &name, NamedNodeMap entities, NamedNodeMap notations) :Node(Node::DOCUMENT_TYPE_NODE), name(name), entities(entities), notations(notations) {};
		#if defined(DOM2)
			DOMString publicId;
			DOMString systemId;
			DOMString internalSubset;
		#endif
	};
	class Document		:public Node {
	public:
		Document();
		const DocumentType		doctype;
		const DOMImplementation	simplementation;
		const Element			documentElement;
		
		Element					createElement				(DOMString tagName);//raises(DOMException);
		DocumentFragment		createDocumentFragment		();
		Text					createTextNode				(DOMString data);
		Comment					createComment				(DOMString data);
		CDATASection			createCDATASection			(DOMString data);//raises(DOMException);
		ProcessingInstruction	createProcessingInstruction	(DOMString target, DOMString data);//raises(DOMException);
		Attr					createAttribute				(DOMString name);//raises(DOMException);
		EntityReference			createEntityReference		(DOMString name);//raises(DOMException);
		NodeList				getElementsByTagName		(DOMString tagname);
		#if defined(DOM2)
			Node		importNode				(Node importedNode, boolean deep);//raises(DOMException);
			Element		createElementNS			(DOMString namespaceURI, DOMString qualifiedName);//raises(DOMException);
			Attr		createAttributeNS		(DOMString namespaceURI, DOMString qualifiedName);//raises(DOMException);
			NodeList	getElementsByTagNameNS	(DOMString namespaceURI, DOMString localName);
			Element		getElementById			(DOMString elementId);
		#endif
	};
	
	
	
#if defined(DOM2)
	class MediaList {
		DOMString	mediaText;// raises(DOMException) on setting
		size_t		length;
		DOMString	item(size_t index);
		void		deleteMedium(DOMString oldMedium);//raises(DOMException);
		void		appendMedium(DOMString newMedium);//raises(DOMException);
	};
	
	
	class CSSValue;
	class CSSPrimitiveValue;
	class Rect;
	class RGBColor;
	class Counter;
	
	class StyleSheet {
		DOMString	type;
		boolean		disabled;
		Node		ownerNode;
		StyleSheet	parentStyleSheet;
		DOMString	href;
		DOMString	title;
		MediaList	media;
	};
	class CSSValue {
	public:
		enum Type:unsigned short {
			CSS_INHERIT			= 0,
			CSS_PRIMITIVE_VALUE	= 1,
			CSS_VALUE_LIST		= 2,
			CSS_CUSTOM			= 3,
		};
		
		CSSValue() :cssValueType(Type::CSS_INHERIT) {};
		CSSValue(Type type, DOMString str="") :cssValueType(type), cssText(str) {};
		Type cssValueType;
		DOMString cssText;// raises(DOMException) on setting
	};
	class CSSPrimitiveValue :public CSSValue {
		enum Type:unsigned short {
			CSS_UNKNOWN		= 0,
			CSS_NUMBER		= 1,
			CSS_PERCENTAGE	= 2,
			CSS_EMS			= 3,
			CSS_EXS			= 4,
			CSS_PX			= 5,
			CSS_CM			= 6,
			CSS_MM			= 7,
			CSS_IN			= 8,
			CSS_PT			= 9,
			CSS_PC			= 10,
			CSS_DEG			= 11,
			CSS_RAD			= 12,
			CSS_GRAD		= 13,
			CSS_MS			= 14,
			CSS_S			= 15,
			CSS_HZ			= 16,
			CSS_KHZ			= 17,
			CSS_DIMENSION	= 18,
			CSS_STRING		= 19,
			CSS_URI			= 20,
			CSS_IDENT		= 21,
			CSS_ATTR		= 22,
			CSS_COUNTER		= 23,
			CSS_RECT		= 24,
			CSS_RGBCOLOR	= 25,
		};
		
		Type		primitiveType;
		void		setFloatValue(Type unitType, float floatValue);		//raises(DOMException);
		float		getFloatValue(Type unitType);							//raises(DOMException);
		void		setStringValue(Type stringType, DOMString stringValue);//raises(DOMException);
		DOMString	getStringValue();	//raises(DOMException);
		Counter		getCounterValue();	//raises(DOMException);
		Rect		getRectValue();		//raises(DOMException);
		RGBColor	getRGBColorValue();	//raises(DOMException);
};
	class CSSRule {

		enum RuleType:unsigned short {
			UNKNOWN_RULE	= 0,
			STYLE_RULE		= 1,
			CHARSET_RULE	= 2,
			IMPORT_RULE		= 3,
			MEDIA_RULE		= 4,
			FONT_FACE_RULE	= 5,
			PAGE_RULE		= 6,
		};
		RuleType		type;
		DOMString		cssText;// raises(DOMException) on setting
		CSSStyleSheet	parentStyleSheet;
		CSSRule			parentRule;
	};
	class CSSRuleList {
		size_t	length;
		CSSRule	item(size_t index);
	};
	class CSSStyleSheet : StyleSheet {
		CSSRule		ownerRule;
		CSSRuleList	cssRules;
		size_t		insertRule(DOMString rule, size_t index);//raises(DOMException);
		void		deleteRule(size_t index);//raises(DOMException);
	};
	class CSSStyleDeclaration {
		DOMString	cssText;// raises(DOMException) on setting
		DOMString	getPropertyValue	(DOMString propertyName);
		CSSValue	getPropertyCSSValue	(DOMString propertyName);
		DOMString	removeProperty		(DOMString propertyName);//raises(DOMException);
		DOMString	getPropertyPriority	(DOMString propertyName);
		void		setProperty			(DOMString propertyName, DOMString value, DOMString priority);//raises(DOMException);
		size_t		length;
		DOMString	item(size_t index);
		CSSRule		parentRule;
	};

	class Rect {
		CSSPrimitiveValue  top;
		CSSPrimitiveValue  right;
		CSSPrimitiveValue  bottom;
		CSSPrimitiveValue  left;
	};
	class RGBColor {
		CSSPrimitiveValue  red;
		CSSPrimitiveValue  green;
		CSSPrimitiveValue  blue;
	};
	class Counter {
		DOMString  identifier;
		DOMString  listStyle;
		DOMString  separator;
	};
#endif
}

#endif	/* GRDOM_H */

