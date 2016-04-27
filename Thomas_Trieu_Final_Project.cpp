//Thomas Trieu
//CSCI 2270, Kos
//Final Project

#include "stdafx.h"	//For VS
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Header1.h"
using namespace std;

void removeAllNodes(BookNode* theRoot)
{
	if (theRoot->leftChild != NULL || theRoot->rightChild != NULL)
	{
		if (theRoot->leftChild != NULL)
			removeAllNodes(theRoot->leftChild);
		if (theRoot->rightChild != NULL)
			removeAllNodes(theRoot->rightChild);
	}
	delete theRoot;
}

BinaryTree::~BinaryTree()
{
	if (numberOfNodes > 0)
		removeAllNodes(root);
}

BookNode* BinaryTree::findMinOf(BookNode* source)
{
	if (source->leftChild == NULL)
	{
		return source;
	}
	else
	{
		findMinOf(source->leftChild);
	}
}

BookNode* BinaryTree::findMaxOf(BookNode* source)
{
	if (source->rightChild == NULL)
	{
		return source;
	}
	else
	{
		findMaxOf(source->rightChild);
	}
}

void BinaryTree::fillTree(string& fileName, int& ticker, int& lineCounter)
{
	string line;
	int _isbn;
	string _title;
	string _authorF;
	string _authorL;
	string _genre;
	double _price;
	int _quantity;
	bool _onSale;
	ifstream invFile;
	invFile.open(fileName);
	if (invFile.is_open())
	{
		getline(invFile, line);	//Getting rid of column labels
		while (invFile.good())
		{
			for (int i = 0; i < 7; i++)
			{
				getline(invFile, line, ',');
				stringstream ss(line);
				switch (i)
				{
				case 0:
					ss >> _isbn;
					ticker += 2;
					break;
				case 1:
					_title = line;
					ss.flush();
					ticker += 4;
					break;
				case 2:
					ss >> _authorF;
					ticker += 4;
					break;
				case 3:
					ss >> _authorL;
					ticker += 5;
					break;
				case 4:
					ss >> _genre;
					ticker += 6;
					break;
				case 5:
					ss >> _price;
					ticker += 7;
					break;
				case 6:
					ss >> _quantity;
					ticker += 8;
					break;
				}
				ticker += 4;
			}
			getline(invFile, line);
			lineCounter++;
			stringstream rr(line);
			rr >> _onSale;
			addBook(_isbn, _title, _authorF, _authorL, _genre, _price, _quantity, _onSale, ticker);
			ticker += 7;
		}
		ticker += 2;
	}
	ticker += 3;
}

void BinaryTree::addBook(int& theISBN, string& theTitle, string& theAuthorF, string& theAuthorL,
	string& theGenre, double& thePrice, int& theQuantity, bool& isSale, int& ticker)
{
	int tickerPlaceHolder;
	BookNode* newBook = new BookNode;
	newBook->isbn = theISBN;
	newBook->title = theTitle;
	newBook->authorF = theAuthorF;
	newBook->authorL = theAuthorL;
	newBook->genre = theGenre;
	newBook->price = thePrice;
	newBook->quantity = theQuantity;
	newBook->onSale = isSale;
	BookNode* temp = root;
	BookNode* parent = NULL;
	if (findBook(theISBN, tickerPlaceHolder) != NULL)
	{
		cout << "A book already exists with ISBN: " << theISBN <<"." << endl;
		delete newBook;
		ticker++;
	}
	else
	{
		while (temp != NULL)
		{
			parent = temp;
			if (newBook->isbn < temp->isbn)
			{
				temp = temp->leftChild;
				ticker++;
			}
			else
			{
				temp = temp->rightChild;
				ticker++;
			}
			ticker += 3;
		}
		if (parent == NULL)
		{
			root = newBook;
			ticker++;
		}
		else
		{
			if (newBook->isbn < parent->isbn)
			{
				parent->leftChild = newBook;
				newBook->parent = parent;
				ticker += 2;
			}
			else
			{
				parent->rightChild = newBook;
				newBook->parent = parent;
				ticker += 2;
			}
			ticker++;
		}
		numberOfNodes = numberOfNodes + 1;
		ticker += 3;
	}
	ticker += 13;
}

void BinaryTree::sellBook(BookNode* toSell)
{
	if (toSell != NULL)
	{
		if (toSell->quantity == 0)
			cout << "There are no more copies of this book in stock." << endl;
		else
			toSell->quantity--;
	}
}

void BinaryTree::removeBookNode(BookNode* toRemove)
{
	int isbnHolder = 0;
	if (toRemove == root && toRemove->leftChild == NULL && toRemove->rightChild == NULL)
	{
		delete toRemove;
		numberOfNodes--;
		return;
	}
	if (toRemove->leftChild == NULL && toRemove->rightChild == NULL)
	{	//If leaf
		if (toRemove->parent->isbn < toRemove->isbn)
		{
			toRemove->parent->rightChild = NULL;
		}
		else
		{
			toRemove->parent->leftChild = NULL;
		}
		delete toRemove;
		numberOfNodes--;
	}
	else
	{
		if (toRemove->rightChild != NULL)
		{	//If node has either 2 children or 1 right child
			BookNode* temp = findMinOf(toRemove->rightChild);
			isbnHolder = temp->isbn;
			toRemove->title = temp->title;
			toRemove->authorF = temp->authorF;
			toRemove->authorL = temp->authorL;
			toRemove->genre = temp->genre;
			toRemove->price = temp->price;
			toRemove->quantity = temp->quantity;
			toRemove->onSale = temp->onSale;
			removeBookNode(temp);
			toRemove->isbn = isbnHolder;
		}
		else
		{	//If node has 1 left child only
			BookNode* temp = findMaxOf(toRemove->leftChild);
			isbnHolder = temp->isbn;
			toRemove->title = temp->title;
			toRemove->authorF = temp->authorF;
			toRemove->authorL = temp->authorL;
			toRemove->genre = temp->genre;
			toRemove->price = temp->price;
			toRemove->quantity = temp->quantity;
			toRemove->onSale = temp->onSale;
			removeBookNode(temp);
			toRemove->isbn = isbnHolder;
		}
	}
}

BookNode* BinaryTree::findBook(string& _title, int& ticker)
{
	BookNode* answer = NULL;
	if (root->title == _title)
	{
		answer = root;
		ticker++;
	}
	else
	{
		if (root->leftChild != NULL)
		{
			findBook(root->leftChild, answer, _title, ticker);
		}
		if (root->rightChild != NULL && answer == NULL)
		{
			findBook(root->rightChild, answer, _title, ticker);
		}
		ticker += 3;
 	}
	ticker += 4;
	return answer;
}

void BinaryTree::findBook(BookNode* start, BookNode*& answer, string& _title, int& ticker)
{
	if (start->title == _title)
	{
		answer = start;
		ticker++;
	}
	else
	{
		if (start->leftChild != NULL)
		{
			findBook(start->leftChild, answer, _title, ticker);
		}
		if (start->rightChild != NULL && answer == NULL)
		{
			findBook(start->rightChild, answer, _title, ticker);
		}
		ticker += 3;
	}
	ticker += 2;
}

BookNode* BinaryTree::findBook(int _isbn, int& ticker)
{
	BookNode* current = root;
	BookNode* answer = NULL;
	while (current != NULL)
	{
		if (current->isbn == _isbn)
		{
			answer = current;
			ticker += 2;
			break;
		}
		else
		{
			if (current->isbn > _isbn)
			{
				current = current->leftChild;
				ticker++;
			}
			else
			{
				current = current->rightChild;
				ticker++;
			}
			ticker++;
		}
		ticker += 2;
	}
	ticker += 5;
	return answer;
}

void BinaryTree::printBook(BookNode* target)
{
	if (target != NULL)
	{
		cout << "Title:    " << target->title << "\nAuthor:   " << target->authorF << " " << target->authorL <<
			"\nISBN:     " << target->isbn << "\nGenre:    " << target->genre <<
			"\nQuantity: " << target->quantity << "\nOn sale?  " << boolalpha << target->onSale <<
			"\nPrice:    " << target->price << "\n\n";
	}
}

void BinaryTree::printAll(int& ticker)
{	//Got lazy and didn't want to write a function to get private member root.
	printAll(root, ticker);
	ticker += 2;
}	//So instead, I used function overloads to gain access.

void BinaryTree::printAll(BookNode* theRoot, int& ticker)
{
	if (theRoot->leftChild != NULL)
	{
		printAll(theRoot->leftChild, ticker);
	}
	if (theRoot->quantity > 0)
	{
		cout << "Title:    " << theRoot->title << "\nAuthor:   " << theRoot->authorF << " " << theRoot->authorL <<
			"\nISBN:     " << theRoot->isbn << "\nGenre:    " << theRoot->genre <<
			"\nQuantity: " << theRoot->quantity << "\nOn sale?  " << boolalpha << theRoot->onSale <<
			"\nPrice:    " << theRoot->price << "\n\n";
	}
	if (theRoot->rightChild != NULL)
	{
		printAll(theRoot->rightChild, ticker);
	}
	ticker += 4;
}

void BinaryTree::printBargain(double& budget)
{	//Again, I got lazy.
	int ticker = 0;
	if (!(isEmpty()))
	{
		printBargain(root, budget, ticker);
	}
	else
		cout << "\nYour inventory is empty.\n\n";
	ticker += 3;
	cout << "Print bargain books, " << nodeCount() << " nodes, " << ticker << " operations.\n";

}

void BinaryTree::printBargain(BookNode* theRoot, double& budget, int& ticker)
{
	if (theRoot->leftChild != NULL)
	{
		printBargain(theRoot->leftChild, budget, ticker);
	}
	if (theRoot->quantity > 0 && theRoot->price <= budget)
	{
		cout << "Title:    " << theRoot->title << "\nAuthor:   " << theRoot->authorF << " " << theRoot->authorL <<
			"\nISBN:     " << theRoot->isbn << "\nGenre:    " << theRoot->genre <<
			"\nQuantity: " << theRoot->quantity << "\nOn sale?  " << boolalpha << theRoot->onSale <<
			"\nPrice:    " << theRoot->price << "\n\n";
	}
	if (theRoot->rightChild != NULL)
	{
		printBargain(theRoot->rightChild, budget, ticker);
	}
	ticker += 5;
}

void BinaryTree::printSale()
{
	int ticker = 0;
	if (!(isEmpty()))
	{
		printSale(root, ticker);
	}
	else
		cout << "Your inventory is empty.\n\n";
	ticker += 2;
	cout << "Print on sale books, " << nodeCount() << " nodes, " << ticker << " operations.\n";
}

void BinaryTree::printSale(BookNode* theRoot, int& ticker)
{
	if (theRoot->leftChild != NULL)
	{
		printSale(theRoot->leftChild, ticker);
	}
	if (theRoot->quantity > 0 && theRoot->onSale == true)
	{
		cout << "Title:    " << theRoot->title << "\nAuthor:   " << theRoot->authorF << " " << theRoot->authorL <<
			"\nISBN:     " << theRoot->isbn << "\nGenre:    " << theRoot->genre <<
			"\nQuantity: " << theRoot->quantity << "\nOn sale?  " << boolalpha << theRoot->onSale <<
			"\nPrice:    " << theRoot->price << "\n\n";
	}
	if (theRoot->rightChild != NULL)
	{
		printSale(theRoot->rightChild, ticker);
	}
	ticker += 5;
}

bool BinaryTree::isEmpty() const
{
	return (numberOfNodes == 0);
}

int BinaryTree::nodeCount() const
{
	return numberOfNodes;
}

void ShoppingCart::addBookToCart(string target, BinaryTree& bookstore)
{
	int ticker;
	BookNode* book = bookstore.findBook(target, ticker);
	if (book == NULL)
	{
		cout << "\nCould not find book with that title.\n\n";
	}
	else
	{
		if (numberOfBooks < maxSize)
		{
			cart[numberOfBooks] = book;
			numberOfBooks++;
			cout << endl << target << " has been added to your cart.\n\n";
		}
		else
		{
			cout << "\nYour cart has reached its maximum capacity.\n\n";
		}
	}
}

void ShoppingCart::addBookToCart(int target, BinaryTree& bookstore)
{
	int ticker;
	BookNode* book = bookstore.findBook(target, ticker);
	if (book == NULL)
	{
		cout << "\nCould not find book with that ISBN.\n\n";
	}
	else
	{
		if (numberOfBooks < maxSize)
		{
			cart[numberOfBooks] = book;
			numberOfBooks++;
			cout << "\nBook with ISBN: " << target << " has been added to your cart.\n\n";
		}
		else
		{
			cout << "\nYour cart has reached its maximum capacity.\n\n";
		}
	}
}

void ShoppingCart::removeBookFromCart(string target)
{
	bool removed = false;
	for (int i = 0; i < numberOfBooks; i++)
	{
		if (cart[i]->title == target)
		{
			cart[i] = cart[numberOfBooks - 1];
			numberOfBooks--;
			removed = true;
		}
	}
	if (removed)
	{
		cout << endl << target << " has been removed from the shopping cart.\n\n";
	}
	else
	{
		cout << endl << target << " was not found in the shopping cart.\n\n";
	}
}

void ShoppingCart::removeBookFromCart(int target)
{
	bool removed = false;
	for (int i = 0; i < numberOfBooks; i++)
	{
		if (cart[i]->isbn == target)
		{
			cart[i] = cart[numberOfBooks - 1];
			numberOfBooks--;
			removed = true;
		}
	}
	if (removed)
	{
		cout << "Book with ISBN of " << target << " has been removed from the shopping cart." << endl;
	}
	else
	{
		cout << "Book with ISBN of " << target << " was not found in the shopping cart." << endl;
	}
}

void ShoppingCart::sellBook(BookNode* toSell)
{
	if (toSell->quantity > 0)
	{
		toSell->quantity--;
	}
	else
	{
		cout << endl << toSell->title << " is out of stock. Unable to purchase.\n\n";
	}
}

void ShoppingCart::printBook(BookNode* target)
{
	if (target != NULL)
	{
		cout << "Title:    " << target->title << "\nAuthor:   " << target->authorF << " " << target->authorL <<
			"\nISBN:     " << target->isbn << "\nGenre:    " << target->genre <<
			"\nQuantity: " << target->quantity << "\nOn sale?  " << boolalpha << target->onSale <<
			"\nPrice:    " << target->price << "\n\n";
	}
}

void ShoppingCart::sellAll(BinaryTree& bookstore)
{
	bool okChoice = false;
	for (int i = 0; i < numberOfBooks; i++)
	{
		sellBook(cart[i]);
	}
	for (int j = 0; j < numberOfBooks; j++)
	{
		printBook(cart[j]);
	}
	for (int k = 0; k < numberOfBooks; k++)
	{
		if (cart[k]->quantity == 0)
		{
			int willRemove = 0;
			okChoice = false;
			while (!okChoice)
			{
				cout << cart[k]->title << " is out of stock. Would you like to\n1. Delete it from " <<
					"the database\n2. Leave it in the database\n\nYour choice: ";
				cin >> willRemove;
				okChoice = true;
				if (willRemove != 1 && willRemove != 2)
				{
					cout << "\nInvalid choice.\n\n";
					okChoice = false;
				}
			}
			if (willRemove == 1)
				bookstore.removeBookNode(cart[k]);
		}
	}
}

void ShoppingCart::printCart()
{
	double subtotal = 0;
	if (numberOfBooks == 0)
		cout << "\nYour cart is empty.\n";
	else
	{
		for (int i = 0; i < numberOfBooks; i++)
		{
			cout << "Title:    " << cart[i]->title << "\nAuthor:   " << cart[i]->authorF << " " << cart[i]->authorL <<
				"\nISBN:     " << cart[i]->isbn << "\nGenre:    " << cart[i]->genre <<
				"\nQuantity: " << cart[i]->quantity << "\nOn sale?  " << boolalpha << cart[i]->onSale <<
				"\nPrice:    " << cart[i]->price << "\n\n";
			subtotal = subtotal + cart[i]->price;
		}
	}
	cout << "\nSubtotal: " << subtotal << "\nTotal items: " << numberOfBooks << "\n\n";
}

void fillInventory(BinaryTree& shop)
{
	string file;
	int ticker = 0;
	int lineCounter = 0;
	cout << "Name of the data file: ";
	cin >> file;
	shop.fillTree(file, ticker, lineCounter);
	ticker += 2;
	cout << "\nAdd all books from inventory, " << lineCounter << " lines, " << ticker << " operations.\n";
}

void addABook(BinaryTree& shop)
{
	int ticker = 0;
	int _isbn;
	string _title;
	string _authorF;
	string _authorL;
	string _genre;
	double _price;
	int _quantity;
	bool _onSale;
	cin.sync(); //Used to make sure getline does not skip input.
	cout << "Title: "; getline(cin, _title);
	cout << "Author's first name: "; getline(cin, _authorF);
	cout << "Author's last name: "; getline(cin, _authorL);
	cout << "ISBN: "; cin >> _isbn;
	cout << "Genre: "; cin >> _genre;
	cout << "Price: "; cin >> _price;
	cout << "Quantity: "; cin >> _quantity;
	cout << "On sale? (1 for yes, 0 for no): "; cin >> _onSale;
	shop.addBook(_isbn, _title, _authorF, _authorL, _genre, _price, _quantity, _onSale, ticker);
	cout << endl;
}

void sellStuff(BinaryTree& shop)
{
	int tickerPlaceHolder;
	BookNode* item = NULL;
	string target;
	int isbntity;
	int decision = 0;
	int isbnOrTitle = 0;
	bool validChoice = false;
	while (!validChoice)
	{
		cout << "Would you like to\n1. Sell a single item\n2. Sell multiple items\n\nYour choice: ";
		cin >> decision;
		validChoice = true;
		if (decision != 1 && decision != 2)
		{
			cout << "\nInvalid choice.\n\n";
			validChoice = false;
		}
	}
	if (decision == 1)
	{
		isbnOrTitle = 0;
		validChoice = false;
		while (!validChoice)
		{
			cout << "Will you identify with the product's\n1. Title\n2. ISBN\n\nYour choice: ";
			cin >> isbnOrTitle;
			validChoice = true;
			if (isbnOrTitle != 1 && isbnOrTitle != 2)
			{
				cout << "\nInvalid choice.\n\n";
				validChoice = false;
			}
		}
		if (isbnOrTitle == 1)
		{
			cout << "The title of the book to be sold: ";
			cin.sync();
			getline(cin, target);
			item = shop.findBook(target, tickerPlaceHolder);
		}
		else
		{
			cout << "The ISBN of the book to be sold: ";
			cin >> isbntity;
			item = shop.findBook(isbntity, tickerPlaceHolder);
		}
		if (item == NULL)
		{
			cout << "\nThe book you wanted to sell could not be found.\n";
		}
		else
		{
			shop.sellBook(item);
			if (item->quantity == 0)
			{
				int willRemove = 0;
				validChoice = false;
				while (!validChoice)
				{
					cout << endl << item->title << " is now out of stock. Would you like to\n1. Delete it from " <<
						"the database\n2. Leave it in the database\n\nYour choice: ";
					cin >> willRemove;
					if (willRemove == 1 || willRemove == 2)
					{
						validChoice = true;
					}
					else
					{
						cout << "\nInvalid choice.\n\n";
					}
				}
				if (willRemove == 1)
				{
					shop.removeBookNode(item);
					item = NULL;
				}
			}
		}
	}
	else
	{
		ShoppingCart theCart;
		int choice = 0;
		cout << "\nA shopping cart has been formed.\n\n";
		while (choice != 4)
		{
			validChoice = false;
			while (!validChoice)
			{
				cout << "Would you like to\n1. Add an item\n2. Remove an item\n3. View your cart\n" <<
					"4. Checkout\n\nYour choice: ";
				cin >> choice;
				validChoice = true;
				if (choice != 1 && choice != 2 && choice != 3 && choice != 4)
				{
					cout << "\nInvalid choice.\n\n";
					validChoice = false;
				}
			}
			switch (choice)
			{
			case 1:
				isbnOrTitle = 0;
				validChoice = false;
				while (!validChoice)
				{
					cout << "Will you identify with the product's\n1. Title\n2. ISBN\n\nYour choice: ";
					cin >> isbnOrTitle;
					validChoice = true;
					if (isbnOrTitle != 1 && isbnOrTitle != 2)
					{
						cout << "\nInvalid choice.\n\n";
						validChoice = false;
					}
				}
				if (isbnOrTitle == 1)
				{
					cout << "The title of the book to be sold: ";
					cin.sync();
					getline(cin, target);
					theCart.addBookToCart(target, shop);
				}
				else
				{
					cout << "The ISBN of the book to be sold: ";
					cin >> isbntity;
					theCart.addBookToCart(isbntity, shop);
				}
				break;
			case 2:
				isbnOrTitle = 0;
				validChoice = false;
				while (!validChoice)
				{
					cout << "Will you identify with the product's\n1. Title\n2. ISBN\n\nYour choice: ";
					cin >> isbnOrTitle;
					validChoice = true;
					if (isbnOrTitle != 1 && isbnOrTitle != 2)
					{
						cout << "\nInvalid choice.\n\n";
						validChoice = false;
					}
				}
				if (isbnOrTitle == 1)
				{
					cout << "The title of the book to be sold: ";
					cin.sync();
					getline(cin, target);
					theCart.removeBookFromCart(target);
				}
				else
				{
					cout << "The ISBN of the book to be sold: ";
					cin >> isbntity;
					theCart.removeBookFromCart(isbntity);
				}
				break;
			case 3:
				theCart.printCart();
				break;
			case 4:
				theCart.sellAll(shop);
				break;
			}
		}
	}
}

void findStuff(BinaryTree& shop)
{
	BookNode* item = NULL;
	char yn;
	string target;
	string target2;
	double newPrice;
	int isbntity;
	int ticker = 0;
	int isbnOrTitle = 0;
	bool validChoice = false;
	while (!validChoice)
	{
		cout << "Will you identify with the product's\n1. Title\n2. ISBN\n\nYour choice: ";
		cin >> isbnOrTitle;
		if (isbnOrTitle == 1 || isbnOrTitle == 2)
		{
			validChoice = true;
			ticker++;
		}
		else
		{
			cout << "\nInvalid input.\n\n";
			cin.clear();
			cin.ignore();
			ticker += 2;
		}
		ticker += 3; //Or 4 depending on the if comparisons.
	}
	if (isbnOrTitle == 1)
	{
		cout << "The title of the book to be found: ";
		cin.sync();
		getline(cin, target);
		if (shop.isEmpty())
		{
			cout << "\nYour inventory is empty.\n\n";
			ticker += 11;
			cout << "Find a book, " << shop.nodeCount() << " nodes, " << ticker << " operations.\n\n";
			return;
		}
		item = shop.findBook(target, ticker);
		ticker += 4;
	}
	else
	{
		cout << "The ISBN of the book to be found: ";
		cin >> isbntity;
		if (shop.isEmpty())
		{
			cout << "\nYour inventory is empty.\n\n";
			ticker += 11;
			cout << "Find a book, " << shop.nodeCount() << " nodes, " << ticker << " operations.\n\n";
			return;
		}
		item = shop.findBook(isbntity, ticker);
		ticker += 4;
	}
	cout << endl;
	if (item == NULL)
	{
		cout << "\nThat book could not be found.\n\n";
		ticker += 12;
		cout << "Find a book, " << shop.nodeCount() << " nodes, " << ticker << " operations.\n\n";
		return;
	}
	else
	{
		cout << "Find a book, " << shop.nodeCount() << " nodes, " << ticker << " operations.\n\n";
	}
	shop.printBook(item);
	validChoice = false;
	while (!validChoice)
	{
		cout << "Would you like to\n1. Change title\n2. Change author\n3. Change genre\n" <<
			"4. Change price\n5. Change sale status\n6. Do nothing\n\nYour choice: ";
		cin >> isbnOrTitle;
		validChoice = true;
		if (validChoice != 1 && validChoice != 2 && validChoice != 3 && validChoice != 4 && validChoice != 5 && validChoice != 6)
		{
			cout << "\nInvalid choice.\n\n";
			validChoice = false;
		}
	}
	switch (isbnOrTitle)
	{
	case 1:
		cout << "What would you like to change the title to? ";
		cin.sync();
		getline(cin, target);
		validChoice = false;
		while (!validChoice)
		{
			cout << "You wish to change this book's title to " << target << ", is that correct? (y/n) ";
			cin >> yn;
			cin.sync();
			validChoice = true;
			if (yn != 'y' && yn != 'n')
			{
				cout << "\nInvalid choice. Please enter either 'y' or 'n'.\n\n";
				validChoice = false;
			}
		}
		item->title = target;
		break;
	case 2:
		cout << "What would you like to change the author's first name to? ";
		cin.sync();
		getline(cin, target);
		cout << "What would you like to change the author's last name to? ";
		cin.sync();
		getline(cin, target2);
		validChoice = false;
		while (!validChoice)
		{
			cout << "You wish to change this book's author to " << target << " " <<
				target2 << ", is that correct? (y/n) ";
			cin >> yn;
			cin.sync();
			validChoice = true;
			if (yn != 'y' && yn != 'n')
			{
				cout << "\nInvalid choice. Please enter either 'y' or 'n'.\n\n";
				validChoice = false;
			}
		}
		item->authorF = target;
		item->authorL = target2;
		break;
	case 3:
		cout << "What would you like to change the genre to? ";
		cin >> target;
		validChoice = false;
		while (!validChoice)
		{
			cout << "You wish to change this book's genre to " << target << ", is that correct? (y/n) ";
			cin >> yn;
			validChoice = true;
			if (yn != 'y' && yn != 'n')
			{
				cout << "\nInvalid choice. Please enter either 'y' or 'n'.\n\n";
				validChoice = false;
			}
		}
		item->genre = target;
		break;
	case 4:
		cout << "What would you like to change the price to? ";
		cin >> newPrice;
		validChoice = false;
		while (!validChoice)
		{
			cout << "You wish to change this book's title to " << newPrice << ", is that correct? (y/n) ";
			cin >> yn;
			validChoice = true;
			if (yn != 'y' && yn != 'n')
			{
				cout << "\nInvalid choice. Please enter either 'y' or 'n'.\n\n";
				validChoice = false;
			}
		}
		item->price = newPrice;
		break;
	case 5:
		item->onSale = !(item->onSale);
		if (item->onSale)
			cout << "The item has been marked as on sale." << endl;
		else
			cout << "The item has been marked as not on sale." << endl;
		cout << "What would you like to change the price to? ";
		cin >> newPrice;
		validChoice = false;
		while (!validChoice)
		{
			cout << "You wish to change this book's title to " << newPrice << ", is that correct? (y/n) ";
			cin >> yn;
			validChoice = true;
			if (yn != 'y' && yn != 'n')
			{
				cout << "\nInvalid choice. Please enter either 'y' or 'n'.\n\n";
				validChoice = false;
			}
		}
		item->price = newPrice;
		break;
	case 6:
		break;
	}
}

int main()
{
	int opCount;
	int userChoice = 0;
	BinaryTree shop;
	cout << "Welcome to your bookstore manager.\n";
	while (userChoice != 8)
	{
		opCount = 0;
		userChoice = 0;
		cout << "\nWhat would you like to do?\n1. Add all books from inventory\n2. Add a single book\n3. Sell books\n" <<
			"4. Find a book\n5. Print the entire inventory\n6. Print bargain books\n7. Print on sale books\n8. Terminate " <<
			"program\n\nYour choice: ";
		cin >> userChoice;
		cout << "\n";
		switch (userChoice)
		{
		case 1:
			fillInventory(shop);
			break;
		case 2:
			addABook(shop);
			break;
		case 3:
			sellStuff(shop);
			break;
		case 4:
			findStuff(shop);
			break;
		case 5:
			if (!(shop.isEmpty()))
				shop.printAll(opCount);
			else
				cout << "Your inventory is empty.\n\n";
			opCount++;
			cout << "Print the entire inventory, " << shop.nodeCount() << " nodes, " << opCount << " operations.\n";
			break;
		case 6:
			double budget;
			cout << "What is the maximum spendable amount? ";
			cin >> budget;
			cout << endl;
			shop.printBargain(budget);
			break;
		case 7:
			shop.printSale();
			break;
		case 8:
			break;
		}
	}
}