#pragma once
#include <cassert>
#include <memory>
#include <string>
#include <ostream>
#include <iomanip>

template<typename KeyType, typename ValueType>
class AVLTree
{

	class Node {
	public:
		KeyType key;
		ValueType value;
		Node* left = nullptr;
		Node* right = nullptr;

		Node(KeyType k, ValueType v, Node* l = nullptr, Node* r = nullptr) {
			key = k;
			value = v;
			left = l;
			right = r;
		}

		std::string toString() const {
			std::string result = "([";
			result += std::to_string(key);
			result += ",";
			result += std::to_string(value);
			result += "],";
			if (left != nullptr) {
				result += left->toString();
			}
			result += ",";
			if (right != nullptr) {
				result += right->toString();
			}
			result += ")";
			return result;
		}

	};

	size_t num_elements = 0;

	Node* root = nullptr;

	void insert(Node* n, KeyType const& key, ValueType const& value);

	void removeAll(Node* n);

	void balance();

	void balance(Node* n);

	int height(Node *n);

	int balanceFactor(Node* n);

	void rrot(Node* n, Node* p);

	void lrot(Node *n, Node* p);

	ValueType* find(Node* n, KeyType const& key);

	template<typename StreamType>
	void print(StreamType& stream, Node* n, int indent) const;

	public:

	AVLTree();

	~AVLTree();

	size_t size() const;

	void insert(KeyType const& key, ValueType const& value);

	ValueType* find(KeyType const& key);

	std::string toString() const;

	template<typename StreamType>
	void print(StreamType& stream) const;
};

template<typename KeyType, typename ValueType>
std::ostream& operator <<(std::ostream& stream, AVLTree<KeyType, ValueType> const& tree)
{
	tree.print<std::ostream>(stream);
	return stream;
}

template<typename KeyType, typename ValueType>
size_t AVLTree<KeyType, ValueType>::size() const {
	return num_elements;
}

template<typename KeyType, typename ValueType>
AVLTree<KeyType, ValueType>::AVLTree() {
	root = nullptr;
}

template<typename KeyType, typename ValueType>
AVLTree<KeyType, ValueType>::~AVLTree() {
	removeAll(root);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::removeAll(Node* n) {
	num_elements = 0;
	if (n == nullptr) return;
	if (n->left != nullptr) {
		removeAll(n->left);
		n->left = nullptr;
	}
	if (n->right != nullptr) {
		removeAll(n->right);
		n->right = nullptr;
	}	
	delete n;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::insert(KeyType const& key, ValueType const& value) {
	if (root == nullptr) {
		root = new Node(key, value);
		return;
	}
	insert(root, key, value);
	balance();
	num_elements += 1;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::insert(Node* n, KeyType const& key, ValueType const& value) {
	if (key < n->key && n->left == nullptr) {
		n->left = new Node(key, value);
		return;
	}

	if (key < n->key && n->left != nullptr) {
		insert(n->left, key, value);
		return;
	}

	if (key > n->key && n->right == nullptr) {
		n->right = new Node(key, value);
		return;
	}

	if (key > n->key && n->right != nullptr) {
		insert(n->right, key, value);
		return;
	}

	if (key == n->key) {
		n->value++;
		return;
	}
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::balance() {
	balance(root);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::balance(Node* n) {
	if (n == nullptr) return;
	if (n != nullptr) balance(n->left);
	if (n != nullptr) balance(n->right);
	//cases when rotating node that is not a root
	if (balanceFactor(n->left) > 1) rrot(n->left, n);
	if (balanceFactor(n->right) > 1) {
		if (balanceFactor(n->right->left) <= -1) {
			lrot(n->right->left, n->right);
		}
		rrot(n->right, n);
	}
	if (balanceFactor(n->left) < -1) {
		if (balanceFactor(n->left->right) >= 1) {
			rrot(n->left->right, n->left);
		}
		lrot(n->left, n);
	}
	if (balanceFactor(n->right) < -1) lrot(n->right, n);
	//cases when rotating root
	if (n == root && balanceFactor(n) < -1) {
		if (balanceFactor(n->right) == 1) rrot(n->right, n);
		lrot(n, nullptr);
	}
	if (n == root && balanceFactor(n) > 1) {
		if (balanceFactor(n->left) == -1) lrot(n->left, n);
		rrot(n, nullptr);
	}
}

template<typename KeyType, typename ValueType>
int AVLTree<KeyType, ValueType>::height(Node* n) {
	if (n == nullptr) return 0;
	int l = height(n->left);
	int r = height(n->right);
	return 1 + (l > r ? l :r);
}

template<typename KeyType, typename ValueType>
int AVLTree<KeyType, ValueType>::balanceFactor(Node* n) {
	if (n == nullptr) return 0;
	return height(n->left) - height(n->right);
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::rrot(Node* n, Node* p) {
	if (n == root) {
		Node* tempx = n;
		Node* lefttempx = tempx->left;
		tempx->left = lefttempx->right;
		lefttempx->right = tempx;
		root = lefttempx;
		return;
	}
	Node* tempx = n;
	Node* lefttempx = tempx->left;
	tempx->left = lefttempx->right;
	lefttempx->right = tempx;
	if (p->left == n) p->left = lefttempx;
	else if (p->right == n) p->right = lefttempx;
}

template<typename KeyType, typename ValueType>
void AVLTree<KeyType, ValueType>::lrot(Node* n, Node* p) {
	if (n == root) {
		Node* tempx = n;
		Node* righttempx = tempx->right;
		tempx->right = righttempx->left;
		righttempx->left = tempx;
		root = righttempx;
		return;
	}
	Node* tempx = n;
	Node* righttempx = tempx->right;
	tempx->right = righttempx->left;
	righttempx->left = tempx;
	if (p->left == n) p->left = righttempx;
	else if (p->right == n) p->right = righttempx;
}

template<typename KeyType, typename ValueType>
typename ValueType* AVLTree<KeyType, ValueType>::find(KeyType const& key) {
	return find(root, key);
}

template<typename KeyType, typename ValueType>
typename ValueType* AVLTree<KeyType, ValueType>::find(Node* n, KeyType const& key) {
	if (n == nullptr) {
		return nullptr;
	}

	if (key == n->key) {
		return &(n->value);
	}

	if (key < n->key) {
		return find(n->left, key);
	}

	if (key > n->key) {
		return find(n->right, key);
	}

	return nullptr;
}

template<typename KeyType, typename ValueType>
std::string AVLTree<KeyType, ValueType>::toString() const {
	std::string result = "";
	if (root == nullptr) {
		return result;
	}
	return root->toString();
}

template<typename KeyType, typename ValueType>
template<typename StreamType>
void AVLTree<KeyType, ValueType>::print(StreamType& stream) const {
	print(stream, root, 0);
}

template<typename KeyType, typename ValueType>
template<typename StreamType>
void AVLTree<KeyType, ValueType>::print(StreamType& stream, Node* n, int indent) const {
	if (n == nullptr) {
		return;
	}
	stream << "[" << n->key << ", " << n->value << "]\n";
	if (n->left != nullptr) {
		for (int i = indent + 1; i > 0; i--) {
			stream << "    ";
		}
		stream << "L: ";
		print(stream, n->left, indent + 1);
	}
	if (n->right != nullptr) {
		for (int i = indent + 1; i > 0; i--) {
			stream << "    ";
		}
		stream << "R: ";
		print(stream, n->right, indent + 1);
	}
}
