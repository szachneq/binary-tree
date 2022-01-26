#pragma once
#include <cassert>
#include <memory>
#include <string>
#include <ostream>
#include <iostream>
#include <iomanip>

template<typename KeyType, typename ValueType>
class BinarySearchTree
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

		void insert(KeyType k, ValueType v) {
			if (k < key && left == nullptr) {
				left = new Node(k, v);
				return;
			}

			if (k < key && left != nullptr) {
				left->insert(k, v);
				return;
			}

			if (k > key && right == nullptr) {
				right = new Node(k, v);
				return;
			}

			if (k > key && right != nullptr) {
				right->insert(k, v);
				return;
			}

			if (k == key) {
				value++;
				return;
			}
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

	void removeAll(Node *n);

	void remove(Node* node, KeyType const& key);

	ValueType* find(Node* n, KeyType const& key);

	template<typename StreamType>
	void print(StreamType& stream, Node* n, int indent) const;

	public:

	BinarySearchTree();

	~BinarySearchTree();

	size_t size() const;

	void insert(KeyType const& key, ValueType const& value);

	void remove(KeyType const& key);

	ValueType* find(KeyType const& key);

	std::string toString() const;

	template<typename StreamType>
	void print(StreamType& stream) const;

};

template<typename KeyType, typename ValueType>
std::ostream& operator <<(std::ostream& stream, BinarySearchTree<KeyType, ValueType> const& tree)
{
	tree.print<std::ostream>(stream);
	return stream;
}

template<typename KeyType, typename ValueType>
BinarySearchTree<KeyType, ValueType>::~BinarySearchTree() {
	removeAll(root);
}

template<typename KeyType, typename ValueType>
void BinarySearchTree<KeyType, ValueType>::removeAll(Node* n) {
	if (n == nullptr) return;
	removeAll(n->left);
	removeAll(n->right);
	n->left = nullptr;
	n->right = nullptr;
	delete n;
	n = nullptr;
}

template<typename KeyType, typename ValueType>
BinarySearchTree<KeyType, ValueType>::BinarySearchTree() {
	root = nullptr;
}

template<typename KeyType, typename ValueType>
size_t BinarySearchTree<KeyType, ValueType>::size() const {
	return num_elements;
}

template<typename KeyType, typename ValueType>
void BinarySearchTree<KeyType, ValueType>::insert(KeyType const& key, ValueType const& value) {
	num_elements += 1;
	if (root == nullptr) {
		root = new Node(key, value);
		return;
	}
	root->insert(key, value);
}

template<typename KeyType, typename ValueType>
void BinarySearchTree<KeyType, ValueType>::remove(KeyType const& key) {
	if (root == nullptr) {
		return;
	}
	if (root->key == key && root->left == nullptr && root->right == nullptr) {
		delete root;
		root = nullptr;
		return;
	}
	remove(root, key);
	num_elements -= 1;
}

template<typename KeyType, typename ValueType>
void BinarySearchTree<KeyType, ValueType>::remove(Node* node, KeyType const& key) {
	if (node == nullptr) {
		return;
	}
	remove(node->left, key);
	remove(node->right, key);

	if (node->left != nullptr && node->left->key == key) {
		// no children
		if (node->left->left == nullptr && node->left->right == nullptr) {
			delete node->left;
			node->left = nullptr;
			return;
		}

		// only left child
		if (node->left->left != nullptr && node->left->right == nullptr) {
			Node* toRemove = node->left;
			node->left = node->left->left;
			delete toRemove;
			return;
		}

		// only right child
		if (node->left->left == nullptr && node->left->right != nullptr) {
			Node* toRemove = node->left;
			node->left = node->left->right;
			delete toRemove;
			return;
		}

		// both children
		Node* toRemove = node->left;
		Node *predecessor = toRemove->left;
		while (predecessor->right != nullptr) {
			predecessor = predecessor->right;
		}
		toRemove->key = predecessor->key;
		toRemove->value = predecessor->value;
		remove(toRemove, predecessor->key);
		return;

	}

	if (node->right != nullptr && node->right->key == key) {
		// no children
		if (node->right->left == nullptr && node->right->right == nullptr) {
			delete node->right;
			node->right = nullptr;
			return;
		}

		// only left child
		if (node->right->left != nullptr && node->right->right == nullptr) {
			Node* toRemove = node->right;
			node->right = node->right->left;
			delete toRemove;
			return;
		}

		// only right child
		if (node->right->left == nullptr && node->right->right != nullptr) {
			Node* toRemove = node->right;
			node->right = node->right->right;
			delete toRemove;
			return;
		}

		// both children
		Node* toRemove = node->right;
		Node* predecessor = toRemove->left;
		while (predecessor->right != nullptr) {
			predecessor = predecessor->right;
		}
		toRemove->key = predecessor->key;
		toRemove->value = predecessor->value;
		remove(toRemove, predecessor->key);
		return;
	}
}

template<typename KeyType, typename ValueType>
typename ValueType* BinarySearchTree<KeyType, ValueType>::find(KeyType const& key) {
	return find(root, key);
}

template<typename KeyType, typename ValueType>
typename ValueType* BinarySearchTree<KeyType, ValueType>::find(Node* n, KeyType const& key) {
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
std::string BinarySearchTree<KeyType, ValueType>::toString() const {
	std::string result = "";
	if (root == nullptr) {
		return result;
	}
	return root->toString();
}

template<typename KeyType, typename ValueType>
template<typename StreamType>
void BinarySearchTree<KeyType, ValueType>::print(StreamType& stream) const {
	print(stream, root, 0);
}

template<typename KeyType, typename ValueType>
template<typename StreamType>
void BinarySearchTree<KeyType, ValueType>::print(StreamType& stream, Node *n, int indent) const {
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
