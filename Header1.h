//Thomas Trieu
//CSCI 2270, Kos
//Final Project

#ifndef _HEADER1
#define _HEADER1

using namespace std;

struct BookNode
{
	BookNode* parent = NULL;
	BookNode* leftChild = NULL;
	BookNode* rightChild = NULL;
	int isbn;
	string title;
	string authorF;
	string authorL;
	string genre;
	double price;
	int quantity;
	bool onSale;
	BookNode() { parent = NULL; leftChild = NULL; rightChild = NULL; }
};

class BinaryTree
{
private:
	BookNode* root;
	int numberOfNodes;
public:
	BinaryTree() { root = NULL; numberOfNodes = 0; }
	~BinaryTree();
	BookNode* findMinOf(BookNode* source);
	BookNode* findMaxOf(BookNode* source);
	void fillTree(string& fileName, int& ticker, int& lineCounter);
	void addBook(int& theISBN, string& theTitle, string& theAuthorF, string& theAuthorL, 
		string& theGenre, double& thePrice, int& theQuantity, bool& isSale, int& ticker);
	void sellBook(BookNode* toSell);
	void removeBookNode(BookNode* toRemove);
	BookNode* findBook(string& _title, int& ticker);
	void findBook(BookNode* start, BookNode*& answer, string& _title, int& ticker);
	BookNode* findBook(int _isbn, int& ticker);
	void printBook(BookNode* target);
	void printAll(int& ticker);
	void printAll(BookNode* theRoot, int& ticker);
	void printBargain(double& budget);
	void printBargain(BookNode* theRoot, double& budget, int& ticker);
	void printSale();
	void printSale(BookNode* theRoot, int& ticker);
	bool isEmpty() const;
	int nodeCount() const;
};

class ShoppingCart
{
private:
	static const int maxSize = 15;
	BookNode* cart[maxSize];
	int numberOfBooks = 0;
public:
	void addBookToCart(string target, BinaryTree& bookstore);
	void addBookToCart(int target, BinaryTree& bookstore);
	void removeBookFromCart(string target);
	void removeBookFromCart(int target);
	void sellBook(BookNode* toSell);
	void printBook(BookNode* target);
	void sellAll(BinaryTree& bookstore);
	void printCart();
};

#endif