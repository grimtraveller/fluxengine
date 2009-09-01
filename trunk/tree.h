#ifndef _TREE_H_
#define _TREE_H_

#include "dlist.h"

enum TreeNodeType {
	NODE_ROOT = 0,
	NODE_FUNCTION,
	NODE_DECLARATION_BLOCK,
	NODE_DECLARATION,
	NODE_GLOBAL
};


template<class T>
class TreeNode {
public:
	T					m_data;
	TreeNode			*m_parent;
	dlist<Token>		m_children;
};

template<class T>
class Tree {
public:
	Tree() {}
	virtual ~Tree() {  }

	TreeNode<T>* AddNode(T _data) {
		TreeNode<Token>* node = new TreeNode<Token>;

		node->m_data = _data;

		return node;
	}

	TreeNode<T>* AddNode(int _nodetype) {
		TreeNode<Token>* node = new TreeNode<Token>;

		Token new_token;
		new_token.type = _nodetype;

		node->m_data = new_token;

		return node;
	}

	void AddChild(TreeNode<T>* _parent, TreeNode<T>* _node) {
		if(_node == NULL)
			return;

		if(_parent == NULL)
			_node->m_parent = NULL;
		else
		{
			_node->m_parent = _parent;
			_parent->m_children.append(_node->m_data);
		}
	}
};



#endif