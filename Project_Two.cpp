/*	==========================================
*	Name:		Project_two
*	Author:		Jason Harmon
*	Version:	1.0
*	Copyright:  Copyright 2024 SNHU CODE
*	Description:Load a CSV file and pull data
*				from it while organizing it in
*				a specific order.
* =============================================
*/

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

//Builds and displays the menu
void Menu() {
    // used "-" and | to help it look more like a menu
	cout << "---------------------------" << endl; // endl to make a new line
	cout << "|1. | Load Data Structure |" << endl;
	cout << "|2. | Print Course List   |" << endl;
	cout << "|3. | Print Course        |" << endl;
	cout << "|9. | Exit                |" << endl;
	cout << "---------------------------" << endl;
	cout << "What would you like to do?" << endl;
};

// builds structure to hold the course information 
struct Course {
	string courseID; // builds a courseID to hold data relating to course id
	string courseName; // builds a courseName to hold data relating to course name
	vector<string> preReq; // builds a list called preReq to add the preReq classes to it

};


// Builds the structure for the spanning tree nodes
struct Node {
	Course course; // builds a course from structure course
	Node* left; // builds a pointer to go to the left 
	Node* right; // builds a pointer to go to the right

	// builds the default constructor
	Node() {
		left = nullptr; // sets the left to the nullptr
		right = nullptr; // sets the right to the nullptr
	}
	// asssign a course
	Node(Course aCourse) : Node() {
		this->course = aCourse; // assigns current node to the course
	}
};

// Builds class to contain members and build Search Tree
class SearchTree {

private: 
	void Remove(Node* node); // made to delete nodes

public:
	Node* root; // builds a node for the root
	SearchTree(); // calls the search function
	virtual ~SearchTree(); 
	void Insert(SearchTree* tree, Node* node); // builds insert method
	void Search(string courseID); // builds search method
	void PrintCourse(Node* node); // builds print course method
};

// Build constructor
SearchTree::SearchTree() {
	root = nullptr; // assigns root to nullptr
}

// destructor
SearchTree::~SearchTree() {
    Remove(root); // deletes root node
};

// follow left and right nodes and delets
void SearchTree::Remove(Node* node) {
    if (node != nullptr) { // if the node is not equal to nullptr
        Remove(node->left); // removes left node
        node->left = nullptr; // left node is now nullptr
        Remove(node->right); // removes right node
        node->right = nullptr; // right node is now nullptr
        delete node; // deltes node
    }
};

// Search for a course name or CourseID
void SearchTree::Search(string courseID) {
    Node* curNode = root; // assigns a temp node named current

    while (curNode != nullptr) { // while curNode is not equal to null ptr it will loop through
        if (curNode->course.courseID == courseID) {// if the data in the current node is equal to the data in courseID 
            // print the courseID and courseName
            cout << curNode->course.courseID << ", ";
            cout << curNode->course.courseName;
            cout << endl;
            cout << " The Prerequisites Are: ";
            // prints thw preReq 
            for (string preReq : curNode->course.preReq) { // loops through the curnode until it has found all of the preReqs
                if (preReq == curNode->course.preReq.back()) {
                    cout << preReq << endl; // outputs the preReqs
                }
                else {
                    cout << preReq << ", ";// adds a coma to seperate the preReqs
                }
            }
            return;
        }
        //searches for the left node
        else if (courseID < curNode->course.courseID) { // if the courseID is less than that of the node on the left it assigns it to the left
            if (curNode->left != nullptr) {
                curNode = curNode->left;
            }
        }
        //searches for the right node
        else {
            curNode = curNode->right; // if the courseID is bigger than the node on the left it assigns it to the right
        }
    }
    // if the cours is not in the list
    cout << "Course " << courseID << "not found. " << endl;
    return;
}

// inserts the data into a courselist
void SearchTree::Insert(SearchTree* tree, Node* node) {
    if (tree->root == nullptr) {// if the current node is null it assigns a node
        tree->root = node;
    }
    else {
        Node* cur = tree->root; // assigns a temp node to the current spot in the list
        while (cur != nullptr) {

            if (node->course.courseID < cur->course.courseID) { // if the cousre id of the node is smaller than the current node
                if (cur->left == nullptr) { // and the node on the left is null
                    cur->left = node; // assins courseID to that node
                    cur = nullptr; // assigns temp node to null
                }
                else {
                    cur = cur->left; // assigns the temp node to the node on the left
                }
            }
            else {

                if (cur->right == nullptr) { // if the node on the right is null 
                    cur->right = node; // assigns the temp node to current node
                    cur = nullptr; // puts temp node to null
                }
                else {
                    cur = cur->right; // assigns the temp node to the nude on the right
                }
            }

        }

    }
}
//prints the data in course list
void SearchTree::PrintCourse(Node* node) {

    //Traverse to print tree
    if (node == nullptr) {
        return;
    }
    //looks through the left node for courseID, cousreName and prints course
    PrintCourse(node->left);
    cout << node->course.courseID << ", ";
    cout << node->course.courseName << endl << endl;
    PrintCourse(node->right);
};

//loads file and creates course list
void loadCourse(string filename, SearchTree* bst) {
    ifstream file(filename); // attempts to open the file
    if (file.is_open()) { // if the file was oppened 
        cout << "The File Has Loaded Properly" << endl; // output file was opened 

        string ln;
        string word;

        while (getline(file, ln)) { // while the file isnt empty

            Node* node = new Node();
            stringstream str(ln);

           for(int i = 0; i < 2; i++){ // loops through the first two items in the each line of the file
                getline(str, word, ',');
                if (i == 0) {
                    node->course.courseID = word; // assigns the data at the current node to the temp string 
                }
                else {
                    node->course.courseName = word; // assigns the data at current node to the temp string
                }
            }
            while (getline(str, word, ',')) {
                node->course.preReq.push_back(word); // adds courses to the preReq
            }

            //inserts node into bst
            bst->Insert(bst, node);
        }
    }
    //file error if file not found
    else 
    {
        cout << "Error Loading File, Please Try Again!" << endl;// outputs error while loading file
        return;
    }

}

void main() { // main method of the program

    SearchTree* bst = new SearchTree();

    string file; // file that is going to be opened
    string courseId;// user input for the course id they want

    int userInput = 0;

    cout << "Welcome to the course planner!" << endl << endl;
    //main while loop to keep program open while choice is not 9
    while (userInput != 9) {
        Menu();
        cin >> userInput;

        switch (userInput) {
            //loads the courses file
        case 1:
            cout << endl;
            cout << "What file do you want to read from?" << endl;
            cin >> file;
            loadCourse(file, bst);
            cout << endl;
            break;

            //prints the courses in order
        case 2:
            cout << endl;
            cout << "----------------------------------------------" << endl;
            cout << "| COURSE ID |     COURSE NAME                 |" << endl;
            cout << "----------------------------------------------" << endl;
            //prints the courses starting at the root   	
            bst->PrintCourse(bst->root);
            cout << endl;
            break;

            //prints the preReqs that user picks with the Course ID 
        case 3: 
            cout << endl;
            cout << "What course do you want the prerequisets for? ";
            cin >> courseId;// takes user input and assigns it to courseId
            cout << endl;

            std::transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);
            bst->Search(courseId); // takes the user input and searches for the courseID and prints

            cout << endl;
            break;

            //exits the program
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

            // if one of the inputs is not an option on the menu 
        default:
            cout << userInput << " is not a valid option." << endl << endl;
            break;
        }
    }
}