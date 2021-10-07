/**
 * CPSC 221 - 2021S
 * PA3
 * Ternary Tree
 *
 * This file will be submitted for grading.
 *
 */

#include "teree.h"

// Node constructor, given.
Teree::Node::Node(pair<int, int> ul, int w, int h, Node* par)
	:upperleft(ul), width(w), height(h), parent(par), A(NULL), B(NULL), C(NULL)
{ }

// Teree destructor, given.
Teree::~Teree() {
	Clear();
}

// Teree copy constructor, given.
Teree::Teree(const Teree& other) {
	Copy(other);
}

// Teree assignment operator, given.
Teree& Teree::operator=(const Teree& rhs) {
	if (this != &rhs) {
		Clear();
		Copy(rhs);
	}
	return *this;
}


Teree::Teree(PNG& imIn) {
	std::pair <int, int> pair = std::make_pair(0, 0);

	root = BuildNode(imIn, pair, imIn.width(), imIn.height(), NULL);
	width = imIn.width();
	height = imIn.height();
}

void Teree::Clear() {
	clear(root);
	root = NULL;
}

void Teree::Copy(const Teree& other) {
	width = other.width;
	height = other.height;

	root = copy(other.root);
}

PNG Teree::Render() const {
	PNG canvas(width, height);
	Render(&canvas, root);
	return canvas;
}

void Teree::Prune(double tol) {
	if (root != NULL) Prune(tol, root);
}

int Teree::NumLeaves() const {
	return NumLeaves(root);
}

Teree::Node* Teree::BuildNode(PNG& im, pair<int, int> ul, int w, int h, Teree::Node* par) {
	Node* newNode = new Node(ul, w, h, par);
	if (w == 1 && h == 1) {
		newNode->avg = *(im.getPixel(ul.first, ul.second));
		return newNode;
	} 
	
	if (w >= h) {
		int x1, x2;
		std::pair<int, int> ul2, ul3;

		if (w % 3 == 0) {
			x1 = w/3;
			x2 = (w*2)/ 3;
		} else if (w % 3 == 1) {
			x1 = (w - 1) / 3;
			x2 = ((w - 1) * 2)/ 3 + 1;
		} else {
			x1 = (w - 2)/ 3 + 1; 
			x2 = ((w - 2) * 2)/ 3 + 1;
		}
		
		ul2 = std::make_pair(ul.first + x1, ul.second);
		ul3  = std::make_pair(ul.first + x2, ul.second);

		//std::cout << "w: " << w << " h: " << h << endl;
		//std::cout << "x1: " << x1 << " x2: " << x2 << endl;

		newNode->A = BuildNode(im, ul, x1, h, newNode);
		newNode->C = BuildNode(im, ul3, w - x2, h, newNode);
		if (x1 != x2) {
			newNode->B = BuildNode(im, ul2, x2 - x1, h, newNode);
		} else {
			newNode->B = NULL;
		}
	} else if (h > w) {
		int y1, y2;
		std::pair<int, int> ul2, ul3;

		if (h % 3 == 0) {
			y1 = h/3;
			y2 = (h*2) / 3;
		} else if (h % 3 == 1) {
			y1 = (h - 1) / 3;
			y2 = ((h - 1) * 2) / 3 + 1;
		} else {
			y1 = (h - 2) / 3 + 1;
			y2 = ((h - 2) * 2) / 3 + 1;
		}

		ul2 = std::make_pair(ul.first, y1 + ul.second);
		ul3 = std::make_pair(ul.first, y2 + ul.second);

		newNode->A = BuildNode(im, ul, w, y1, newNode);
		newNode->C = BuildNode(im, ul3, w, h - y2, newNode);
		if (y1 != y2) {
			newNode->B = BuildNode(im, ul2, w, y2 - y1, newNode);
		} else {
			newNode->B = NULL;
		}
	}

	double acArea = newNode->A->width * newNode->A->height;
	double totalArea = w * h;
	double sumH = 0.0;
	double sumS = 0.0;
	double sumL = 0.0;
	double sumA = 0.0;


	sumH += (newNode->A->avg.h * acArea) / totalArea;
	sumS += (newNode->A->avg.s * acArea) / totalArea;
	sumL += (newNode->A->avg.l * acArea) / totalArea;
	sumA += (newNode->A->avg.a * acArea) / totalArea;

	sumH += (newNode->C->avg.h * acArea) / totalArea;
	sumS += (newNode->C->avg.s * acArea) / totalArea;
	sumL += (newNode->C->avg.l * acArea) / totalArea;
	sumA += (newNode->C->avg.a * acArea) / totalArea;
 
	if (newNode->B != NULL) {
		double bArea = (double) newNode->B->width * (double) newNode->B->height;
	
		sumH += (newNode->B->avg.h * bArea) / totalArea;
		sumS += (newNode->B->avg.s * bArea) / totalArea;
		sumL += (newNode->B->avg.l * bArea) / totalArea;
		sumA += (newNode->B->avg.a * bArea) / totalArea;
	}
	
	newNode->avg.h = sumH;
	newNode->avg.s = sumS;
	newNode->avg.l = sumL;
	newNode->avg.a = sumA;

	return newNode;
}

/*==== ALSO IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE DECLARED ====*/

// Private helper function to recursively clear a teree
void Teree::clear(Teree::Node* subRoot) {
	if (subRoot == NULL) return;

	clear(subRoot->A);
	clear(subRoot->B);
	clear(subRoot->C);

	delete subRoot;
}

// Private helper function that recursively copies a teree
Teree::Node* Teree::copy(Teree::Node* subRoot) {
	if (subRoot == NULL) return NULL;

	Node* copiedNode = new Node(subRoot->upperleft, subRoot->width, subRoot->height, subRoot->parent);

	//cout << "sub height: " << subRoot->height << " new height: " << copiedNode->height << endl;

	copiedNode->avg = subRoot->avg;
	copiedNode->A = copy(subRoot->A);
	copiedNode->B = copy(subRoot->B);
	copiedNode->C = copy(subRoot->C);

	return copiedNode;
}

// Private helper function that recursively counts the leaves in the subroot
int Teree::NumLeaves(Teree::Node* subRoot) const {
	if (subRoot == NULL) return 0;
	if (subRoot->A == NULL && subRoot->B == NULL && subRoot->C == NULL) return 1;
	else {
		return NumLeaves(subRoot->A) + NumLeaves(subRoot->B) + NumLeaves(subRoot->C);
	}
}

// Private helper function that recursively renders the nodes in the subroot
void Teree::Render(PNG *canvas, Teree::Node* subRoot) const {
 	if (subRoot == NULL) return;
	if (subRoot->A == NULL && subRoot->B == NULL && subRoot->C == NULL) {
		for (int i = subRoot->upperleft.first; i < subRoot->upperleft.first + subRoot->width; i++) {
			for (int j = subRoot->upperleft.second; j < subRoot->upperleft.second + subRoot->height; j++) {
				HSLAPixel *pixel = canvas->getPixel(i, j);
				
				pixel->h = subRoot->avg.h;
				pixel->s = subRoot->avg.s;
				pixel->l = subRoot->avg.l;
				pixel->a = subRoot->avg.a;
			}
		}
		return;
	} 
	Render(canvas, subRoot->A);
	Render(canvas, subRoot->B);
	Render(canvas, subRoot->C); 
}

// Private helper function that recursively prunes in the subroot
void Teree::Prune(double tol, Teree::Node* subRoot) {
	if (subRoot == NULL) return;
	if (subRoot-> A == NULL && subRoot->B  == NULL && subRoot->C == NULL) return;
	//cout << "pruning" << endl;
	if (CheckLeaves(tol, subRoot, subRoot->avg)) {
		//cout << "clearing" << endl;
		clear(subRoot->A);
		subRoot->A = NULL;
		clear(subRoot->B);
		subRoot->B = NULL;
		clear(subRoot->C);
		subRoot->C = NULL;

		return;
	} else {
		Prune(tol, subRoot->A);
		Prune(tol, subRoot->B);
		Prune(tol, subRoot->C);
	}  
}

bool Teree::CheckLeaves(double tol, Teree::Node * subRoot, HSLAPixel avgParent) {
	if (subRoot == NULL) return true;
	if (subRoot->A == NULL && subRoot->B == NULL && subRoot->C == NULL) {
		return (subRoot->avg.dist(avgParent) <= tol);
	} else {
		return (CheckLeaves(tol, subRoot->A, avgParent) && CheckLeaves(tol, subRoot->B, avgParent) && CheckLeaves(tol, subRoot->C, avgParent));
	}
}