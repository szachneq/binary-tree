#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>
#include "..\AVLTreeLib\AVLTree.h"
#include "..\benchmark\benchmark.h"

class Key
{
private:
	unsigned long value = 0UL;
	static bool performDelay;
	void delay() const
	{
		if (performDelay)
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
public:
	Key() = default;

	Key(Key const&) = default;

	Key(unsigned long v) : value(v)
	{
	}

	unsigned long getKey() const
	{
		return value;
	}

	static void delayOn()
	{
		performDelay = true;
	}

	static void delayOff()
	{
		performDelay = false;
	}


	bool operator==(Key const& v) const
	{
		delay();
		return value == v.value;
	}

	bool operator!=(Key const& v) const
	{
		delay();
		return value != v.value;
	}

	bool operator<(Key const& v) const
	{
		delay();
		return value < v.value;
	}
	bool operator<=(Key const& v) const
	{
		delay();
		return value <= v.value;
	}
	bool operator>(Key const& v) const
	{
		delay();
		return value > v.value;
	}
	bool operator>=(Key const& v) const
	{
		delay();
		return value >= v.value;
	}

};

bool Key::performDelay = false;

std::ostream & operator << (std::ostream& stream, Key const& key)
{
	stream << key.getKey();
	return stream;
}
// The function "counter" creates a Dictionary of words read from a text file (counts word occurrences).
// The Dictionary should contain all words encountered in the text file together with the word occurrence count, ordered lexically.
AVLTree<std::string, int> counter(const std::string& fileName) {
	auto tree = AVLTree<std::string, int>();

	std::ifstream is(fileName);
	if (!is.good())
	{
		std::cout << "File opening error :(\n";
		return tree;
	}

	std::string word;
	while (is >> word) {
		tree.insert(word, 1);
	}

	return tree;
}

// The "listing" function that displays(std::cout) the words from the dictionary
// ordered by the number of occurrences of the words.
// Words with the same counter value should be ordered lexically.
void listing(const AVLTree <std::string, int>& tree);

int main()
{
	auto tree = counter("Text.txt");
	std::stringstream stream;
	tree.print(stream);
	auto s = stream.str();
	std::cout << s << std::endl;
	std::cout << tree.size() << std::endl;
	std::cout << "end" << std::endl;
	/*
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator((unsigned long)seed);

	size_t size;
	do
	{
		std::cout << "Enter size:";
		std::cin >> size;
		if (size > 0)
		{
			Key::delayOff();
			AVLTree<Key, unsigned long> tree;
			while (tree.size() < size)
			{
				unsigned long n = generator();

				Key key(n);

				tree.insert(key, n);
			}
			if (tree.size() <= 100)
				std::cout << tree;

			Key::delayOn();
			unsigned long long sum = 0;
			int i;
			for (i = 0; i < 10; ++i)
			{
				unsigned long n = generator();
				Key key(n);

				Benchmark<std::chrono::nanoseconds> b;
				tree.find(Key(key));
				sum += b.elapsed();
			}
			std::cout << "Time: " << sum / i << "\n";
		}
	} while (size > 0);
	*/
	return 0;
}
