/**
 * ResourceManager.cpp 
 * Author: Yexiang Zhou
 * Platform: Mac/Linux
 */

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
using namespace std;

class Node
{
public:
	string name;//name of node
	bool usable;//usable status
	Node *pdependOn;//depend on which node (pointer)
	set<Node *> pproducts;//set of pointers of the nodes depend on it
	Node(string name, Node* pdependOn);
	~Node();
	void setUsable(bool ifUsable);//set the usable status of itself and all nodes depend on it
	void depend(Node *pdependOn);//set its dependence
	void produce(Node *pproduct);//set new node depend on it
};

set<Node *> pallNodes;

Node::Node(string name, Node* pdependOn)
{
	this->name = name;
	depend(pdependOn);
	if (pdependOn == NULL) setUsable(true);
	pallNodes.insert(this);

}

Node::~Node()
{
	//set its dependence to null and remove it from the product set of the node it depend on
	if (pdependOn != NULL)
	{
		pdependOn->pproducts.erase(this);
		pdependOn = NULL;
	}
	//for all nodes depend on it, set their usable to false and dependence to null
	if (pproducts.empty()) return;
	for(set<Node *>::iterator it = pproducts.begin(); it != pproducts.end(); it++)
	{
		(*it)->setUsable(false);
		(*it)->pdependOn = NULL;
	}
	cout<<name<<" deleted"<<endl;
}

void Node::depend(Node* pdependOn)
{
	this->pdependOn = pdependOn;

	if (pdependOn != NULL)
	{
		pdependOn->produce(this);
		setUsable(pdependOn->usable);
	}
}

void Node::produce(Node *pproduct)
{
	pproducts.insert(pproduct);
}


void Node::setUsable(bool ifUsable)
{
	usable = ifUsable;
	if (pproducts.empty()) return;
	for(set<Node *>::iterator it = pproducts.begin(); it != pproducts.end(); it++)
	{
		(*it)->setUsable(ifUsable);
	}
}

bool deleteNode(Node* n)
{
	if (pallNodes.empty()) return false;
	set<Node *>::iterator it;
	for(it = pallNodes.begin(); it != pallNodes.end();)
		{
			if ((*it) == n)
			{
				delete *it;
				pallNodes.erase(it++);
				return true;
			}
			else it++;
		}
	return false;
}

//find pointer of the node by its name
Node* findpNode(string name)
{
	if (pallNodes.empty()) return NULL;
	set<Node *>::iterator it;
	for(it = pallNodes.begin(); it != pallNodes.end(); it++)
		{
			if ((*it)->name == name)
			{
				return *it;
			}
		}
	return NULL;
}

vector<string> splitString(string originalStr)
{
	vector<string> result;
	int size = originalStr.size();
	int pos = 0;
	bool ifC = false;
	for (int i = 0; i < size; i++)
	{
		if (!ifC && originalStr[i] != ' ')
		{
			pos = i;
			ifC = true;
		} else if (ifC && originalStr[i] == ' ')
		{
			result.push_back(originalStr.substr(pos, i - pos));
			ifC = false;
		}
		if (ifC && i == (size - 1))
		{
			result.push_back(originalStr.substr(pos, i + 1 - pos));
		}
	}

	return result;
}

void printInitHelp()
{
	cout<<"--------------------------------------"<<endl;
	cout<<"Please input instruction for initiation: "<<endl;
	cout<<"    n                     create new graph from file \"resource.txt\""<<endl;
	cout<<"    l                     load saved graph from file \"save.txt\""<<endl;
	cout<<"--------------------------------------"<<endl;
}

bool createNewGraph()
{
	ifstream inputFile("resource.txt");
	if (!inputFile.is_open()) 
	{
		cout<<"can't open"<<endl;
		return false;
	}
	string str;
	while (getline(inputFile, str))
	{
		vector<string> info = splitString(str);
		if (info.size() != 2) break;
		if (findpNode(info[0]) == NULL && findpNode(info[1]) == NULL)
		{
			Node* nodeB = new Node(info[1], NULL);
			new Node(info[0], nodeB);
		} else if (findpNode(info[0]) == NULL && findpNode(info[1]) != NULL)
		{
			new Node(info[0], findpNode(info[1]));
		} else if (findpNode(info[0]) != NULL && findpNode(info[1]) == NULL)
		{
			Node* nodeB = new Node(info[1], NULL);
			findpNode(info[0])->depend(nodeB);
		} else if (findpNode(info[0]) != NULL && findpNode(info[1]) != NULL)
		{
			findpNode(info[0])->depend(findpNode(info[1]));
		}
	}
	return true;
}

bool loadGraph()
{
	ifstream inputFile("save.txt");
	if (!inputFile.is_open()) return false;
	string str;
	while (getline(inputFile, str))
	{
		vector<string> info = splitString(str);
		if (info.size() != 2) break;

		if (info[1] == "1")
		{
			if (findpNode(info[0]) == NULL) new Node(info[0], NULL);
			continue;
		} else if (info[1] == "0")
		{
			if (findpNode(info[0]) == NULL) new Node(info[0], NULL);
			findpNode(info[0])->setUsable(false);
			continue;
		}

		if (findpNode(info[0]) == NULL && findpNode(info[1]) == NULL)
		{
			Node* nodeB = new Node(info[1], NULL);
			new Node(info[0], nodeB);
		} else if (findpNode(info[0]) == NULL && findpNode(info[1]) != NULL)
		{
			new Node(info[0], findpNode(info[1]));
		} else if (findpNode(info[0]) != NULL && findpNode(info[1]) == NULL)
		{
			Node* nodeB = new Node(info[1], NULL);
			findpNode(info[0])->depend(nodeB);
		} else if (findpNode(info[0]) != NULL && findpNode(info[1]) != NULL)
		{
			findpNode(info[0])->depend(findpNode(info[1]));
		}
	}
	return true;
}

void printHelp()
{
	cout<<"--------------------------------------"<<endl;
	cout<<"Please input instruction: "<<endl;
	cout<<"    s                     show the current status of all nodes"<<endl;
	cout<<"    v                     visualise the graph with graphical image"<<endl;
	cout<<"    c nodeA nodeB         create new node \"nodeA\" depend on existed node \"nodeB\";"<<endl;
	cout<<"    c nodeA               create new independent node \"nodeA\" and set it usable"<<endl;
	cout<<"    cd nodeA nodeB        create dependence for existed node \"nodeA\" on existed node \"nodeB\";"<<endl;
	cout<<"    d nodeA               delete existed node \"nodeA\", all nodes depend on it will be unusable;"<<endl;
	cout<<"    us nodeA              set existed independent node \"nodeA\" usable"<<endl;
	cout<<"    un nodeA              set existed independent node \"nodeA\" unusable"<<endl;
	cout<<"    q                     save the modified graph and quit"<<endl;
	cout<<"--------------------------------------"<<endl;
}

void showAllNodes()
{
	cout<<"All nodes status: "<<endl;
	cout<<"--------------------------------------"<<endl;
	if (pallNodes.empty())
	{
		cout<<"No node exists"<<endl;
	}else 
	{
		set<Node *>::iterator it;
		for(it = pallNodes.begin(); it != pallNodes.end(); it++)
		{
			cout<<"Name: "<<(*it)->name<<", ";
			if ((*it)->usable) cout<<"usable"<<", ";
			else cout<<"unusable"<<", ";
			if ((*it)->pdependOn == NULL) cout<<"Depend on Nothing"<<endl;
			else cout<<"Depend on: "<<((*it)->pdependOn)->name<<endl;
		}
	}
	cout<<"--------------------------------------"<<endl;
}

void visualiseGraph()
{
	if (pallNodes.empty())
	{
		cout<<"No node exists"<<endl;
	}else 
	{
		ofstream outputFile("vis.dot");
		outputFile<<"digraph vis{"<<endl;
		set<Node *>::iterator it;
		for(it = pallNodes.begin(); it != pallNodes.end(); it++)
		{
			if ((*it)->usable) 
			{
				outputFile<<(*it)->name<<"[style=filled,color=green]"<<endl;
				if ((*it)->pdependOn != NULL) outputFile<<(*it)->name<<"->"<<((*it)->pdependOn)->name<<"[style=solid,color=green]"<<endl;
			} else
			{
				outputFile<<(*it)->name<<"[style=filled,color=red]"<<endl;
				if ((*it)->pdependOn != NULL) outputFile<<(*it)->name<<"->"<<((*it)->pdependOn)->name<<"[style=dashed,color=red]"<<endl;
			}
		}
		outputFile<<"}"<<endl;
		outputFile.close();
		system("dot -Tpng vis.dot -o vis.png");//mac/linux
		system("open vis.png");
	}
}

void createNodeWithDependence(string nodeName, string dependName)
{
	if (findpNode(nodeName) != NULL) 
	{
		cout<<"This name is already used. Creating node failed."<<endl;
		return;
	} else if (findpNode(dependName) == NULL)
	{
		cout<<"The node it depend on doesn't existed. Creating node failed."<<endl;
		return;
	} else
	{
		new Node(nodeName, findpNode(dependName));
	}
}

void createIndependentNode(string nodeName)
{
	if (findpNode(nodeName) != NULL) 
	{
		cout<<"This name already used. Creating node failed."<<endl;
		return;
	} else
	{
		new Node(nodeName, NULL);
	}
}

void createDependence(string nameA, string nameB)
{
	if (findpNode(nameA) == NULL) 
	{
		cout<<"The node doesn't existed. Creating dependence failed."<<endl;
		return;
	} else if (findpNode(nameB) == NULL)
	{
		cout<<"The node it depend on doesn't existed. Creating dependence failed."<<endl;
		return;
	} else if (findpNode(nameA)->pdependOn != NULL)
	{
		cout<<"The node is not independent. Creating dependence failed."<<endl;
		return;
	} else 
	{
		findpNode(nameA)->depend(findpNode(nameB));
		cout<<"Creating dependence succeed."<<endl;
	}
}

void deleteNodeByName(string name)
{
	if (findpNode(name) == NULL)
	{
		cout<<"The node doesn't existed. Deleting node failed."<<endl;
		return;
	} else 
	{
		if (deleteNode(findpNode(name)))
		{
			cout<<"Deleting node succeed."<<endl;
		}
	}
}

void setIndependentNodeUsable(string name, bool ifUsable)
{
	if (findpNode(name) == NULL)
	{
		cout<<"The node doesn't existed. Setting usable failed."<<endl;
		return;
	} else if (findpNode(name)->pdependOn != NULL)
	{
		cout<<"The node is not independent. Setting usable failed."<<endl;
		return;
	} else
	{
		findpNode(name)->setUsable(ifUsable);
		cout<<"Setting usable succeed."<<endl;
	}
}

void saveAndQuit()
{
	if (pallNodes.empty())
	{
		cout<<"No node exists, nothing to save."<<endl;
	}else 
	{
		ofstream outputFile("save.txt");
		set<Node *>::iterator it;
		for(it = pallNodes.begin(); it != pallNodes.end(); it++)
		{
			if ((*it)->pdependOn == NULL) 
			{
				if ((*it)->usable == true) outputFile<<(*it)->name<<" "<<"1"<<endl;
				else outputFile<<(*it)->name<<" "<<"0"<<endl;
			}
			else outputFile<<(*it)->name<<" "<<((*it)->pdependOn)->name<<endl;
		}
		outputFile.close();
		cout<<"Graph saved."<<endl;
	}
}


int main()
{
	while(true)
	{
		printInitHelp();
		string iCommand;
		getline(cin, iCommand);
		if(iCommand == "n")
		{
			if(createNewGraph()) break;
			else cout<<"Create new graph failed, please make sure the file \"resource.txt\" is in the same path here."<<endl;
		} else if(iCommand == "l")
		{
			if(loadGraph()) break;
			else cout<<"Load graph failed, please make sure the file \"save.txt\" is in the same path here."<<endl;
		} else cout<<"Invalid input."<<endl;
	}
	cout<<"Initiate successfully."<<endl;
	showAllNodes();

	while(true)
	{
		printHelp();
		string command;
		getline(cin, command);
		vector<string> instructions = splitString(command);

		if (instructions[0] == "s") 
		{
			if (instructions.size() == 1)
			{
				showAllNodes();
			} else cout<<"Invalid input."<<endl;
			continue;
		} else if (instructions[0] == "v") 
		{
			if (instructions.size() == 1)
			{
				visualiseGraph();
			} else cout<<"Invalid input."<<endl;
			continue;
		} else if (instructions[0] == "c")
		{
			if (instructions.size() == 3)
			{
				createNodeWithDependence(instructions[1], instructions[2]);
			} else if (instructions.size() == 2)
			{
				createIndependentNode(instructions[1]);
			} else cout<<"Invalid input."<<endl;
			continue;
		} else if (instructions[0] == "cd")
		{
			if (instructions.size() == 3)
			{
				createDependence(instructions[1], instructions[2]);
			} else cout<<"Invalid input."<<endl;
			continue;
		} else if (instructions[0] == "d")
		{
			if (instructions.size() == 2)
			{
				deleteNodeByName(instructions[1]);
			} else cout<<"Invalid input."<<endl;
			continue;
		} else if (instructions[0] == "us")
		{
			if (instructions.size() == 2)
			{
				setIndependentNodeUsable(instructions[1], true);
			} else cout<<"Invalid input."<<endl;
			continue;
		} else if (instructions[0] == "un")
		{
			if (instructions.size() == 2)
			{
				setIndependentNodeUsable(instructions[1], false);
			} else cout<<"Invalid input."<<endl;
			continue;
		} else if (instructions[0] == "q")
		{
			if (instructions.size() == 1)
			{
				saveAndQuit();
				break;
			} else cout<<"Invalid input."<<endl;
		} else cout<<"Invalid input."<<endl;
	}

	return 0;
}