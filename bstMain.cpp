
/*
  Jase Rodriguez
  Project 2
  10/31/23
  This program uses standard functions of BST (insert, search, and
  delete) and queries (height, count, and search tree).
*/
#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
using namespace std;

FILE *inFile;
FILE *outFile;

const int maxSize = 30;

class itemNode 
{
public:
  int count;
  char name[maxSize];
  itemNode *left, *right;

//Item class
  itemNode() //Default construct
  {
    name[0] = '\0';
    count = 0;
    left = NULL;
    right = NULL;
  }

  itemNode(char itemName[], int population)
  {
    if(strlen(itemName) >= maxSize)
      cout << "Name is too long\n";
    else
    {
      count = population;
      strcpy(name, itemName);
      left = NULL;
      right = NULL;
    }
  }
};

//Tree class
class treeNameNode
{
public:
  char treeName[maxSize];
  treeNameNode *left, *right;
  itemNode *theTree;

  treeNameNode() //Default construct
  {
    treeName[0] = '\0';
    theTree = NULL;
    left = NULL;
    right = NULL;
  }

  treeNameNode(char name[])
  {
    if(strlen(name) >= maxSize)
      cout << "Name is too long\n";
    else
    {
      strcpy(treeName, name);
      theTree = NULL;
      left = NULL;
      right = NULL;
    }
  }
};

//Inorder tree traversal
void inorder(itemNode *root) {
  //Traverse the node if it's not null
  if (root != NULL) {
    inorder(root->left); // Go Left.
    cout<< root->name << " ";
    fprintf(outFile, "%s ", root->name);
    inorder(root->right); // Go Right.
  }
  else
    return;
}

//Outputs tree's items in an inorder approach
void traverse_in_traverse(treeNameNode *root)
{
  if(root == NULL)
    return;
  traverse_in_traverse(root->left);
  cout << "*** " << root->treeName << " ***\n";
  fprintf(outFile, "*** %s ***\n", root->treeName);
  inorder(root->theTree);
  cout << "\n";
  fprintf(outFile, "\n");
  traverse_in_traverse(root->right);
}

//Insert node
itemNode *insertItem(itemNode *root, itemNode *item)
{
  if (root == NULL)
    return item;
  else
  {
    if (strcmp(item->name, root->name) < 0)
        root->left = insertItem(root->left, item);
    else if(strcmp(item->name, root->name) > 0)
        root->right = insertItem(root->right, item);
    else
    {
      cout << "Cannot insert duplicate item:" << item->name << "\n";
      fprintf(outFile, "Cannot insert duplicate item: %s\n", item->name);
      delete item;
    }
    return root;
  }
}

//Insert a tree to a root tree
treeNameNode *insertTree(treeNameNode *root, treeNameNode *tree)
{
  if(root == NULL)
    return tree;
  else
  {
    if(strcmp(root->treeName, tree->treeName) > 0)
        root->left = insertTree(root->left, tree);
    else if(strcmp(root->treeName, tree->treeName) < 0)
      root->right = insertTree(root->right, tree);
    else
    {
      cout << "Cannot insert duplicate tree: " << tree->treeName << "\n";
      fprintf(outFile, "Cannot insert duplicate tree %s\n", tree->treeName);
      delete tree;
    }
  }
  return root;
}

//Search for node and return it
treeNameNode *searchNameNode(treeNameNode *root, char treeName[50])
{
  if(root == NULL)
    return NULL;
  else
  {
    if(strcmp(root->treeName, treeName) == 0)
      return root;
    else if(strcmp(root->treeName, treeName) > 0)
      return searchNameNode(root->left, treeName);
    else
      return searchNameNode(root->right, treeName);
  }
}

//Count query that returns total num of items when given root
int count(itemNode *root) 
{
  if (root == NULL)
    return 0;
  return 1 + count(root->left) + count(root->right);
}

//Count items before query that returns num of items before reaching
//given item
int itemBefore(itemNode* root, char names[maxSize])
{
  if (root == NULL)
    return 0;
  if (strcmp(names, root->name) == 0) {
    return count(root->left);
  }
  if (strcmp(names, root->name) > 0) {
    return 1 + count(root->left) + itemBefore(root->right, names);
  }
  return itemBefore(root->left, names);
}

//Print Item before count
void item_before(itemNode *root, char names[maxSize])
{
  int numItemsBefore = itemBefore(root, names);
  cout << "Number of items before " << names << ": " << numItemsBefore << "\n";
  fprintf(outFile, "Number of items before %s: %d\n", names,  numItemsBefore);
}

//Query to return itemNode searched for
itemNode *search(itemNode *root, char name[maxSize])
{
  if(root == NULL)
    return root;
  if(strcmp(root->name, name) == 0)
    return root;
  else if(strcmp(root->name, name) > 0)
    return search(root->left, name);
  else
    return search(root->right, name);
}

//Print search
void search(itemNode* root, char name[maxSize], treeNameNode* tree) 
{
  itemNode* temp = search(root, name);
  
  if (temp == NULL) 
  {
    cout << name << " doesn't exist in " << tree->treeName << endl;
    fprintf(outFile, "%s doesn't exist in %s\n", name, tree->treeName);
  }
  else 
  {
    cout << temp->name << " item count: " << temp->count << endl;
    fprintf(outFile, "%s item count: %d\n", temp->name, temp->count);
  }
}

//Query to return height
int getHeightTree(itemNode *root)
{
  int leftHeight = 0;
  int rightHeight = 0;
  if (root == NULL)
    return 0;
  
  leftHeight = getHeightTree(root->left);
  rightHeight = getHeightTree(root->right);
  
  if (leftHeight > rightHeight)
    return 1 + leftHeight;
  else   
    return 1 + rightHeight;
}

//Print height, difference, and balance
void height_balance(treeNameNode *root)
{
  if (root == NULL || root->theTree == NULL) 
  {
    cout << "Tree is empty or does not exist." << endl;
    fprintf(outFile, "Tree is empty or does not exist.\n");
    return;
  }
  
  itemNode *temp = root->theTree;
  
  int height = getHeightTree(temp);
  int heightLeft = getHeightTree(temp->left);
  int heightRight = getHeightTree(temp->right);

  //Find difference between heights (must be positive)
  int diff = abs(heightLeft - heightRight);

  string isBalanced;

  //Height difference can only be 0 or 1 to be balanced
  if(diff > 1)
    isBalanced = "not balanced";
  else
    isBalanced = "balanced";

  cout << root->treeName << ":   left height " << heightLeft << ", right height " << heightRight << ", difference " << diff << ", " << isBalanced << endl;
  fprintf(outFile, "%s:   left height %d, right height %d, difference %d, %s\n", root->treeName, heightLeft, heightRight, diff, isBalanced);
}

//Read input and builds the tree (N, I, Q)
treeNameNode* buildTree()
{ 
  //Create nameTreenNode
  treeNameNode *root = NULL;
  char name[maxSize];

  //Define and scan for N, I, and Q
  int numTreeNames = 0, allNumItems = 0, numQueries = 0;
  
  fscanf(inFile, "%d %d %d", &numTreeNames, &allNumItems, &numQueries);

  for (int i = 0; i < numTreeNames; i++) //N
  {
    //Read tree name
    fscanf(inFile, "%s", name);
    treeNameNode *tNode = new treeNameNode(name);
    root = insertTree(root, tNode);
  }

  //Build tree
  for (int i = 0; i < allNumItems; i++) 
  {
    char treeName[maxSize];
    char itemName[maxSize];
    int itemCount;

    fscanf(inFile, "%s %s %d", treeName, itemName, &itemCount);
    treeNameNode *nameTree = searchNameNode(root, treeName);

    if (nameTree) 
    {
      itemNode *item = new itemNode(itemName, itemCount);
      itemNode *itemRoot = insertItem(nameTree->theTree, item);
      if(nameTree->theTree == NULL)
        nameTree->theTree = itemRoot;
    } 
    else 
    {
      cout << "Tree name " << treeName << " not found\n";
      fprintf(outFile, "Tree name %s not found\n", treeName);
    }
  }


  //Print tree
  traverse_in_traverse(root);

  for(int i = 0; i < numQueries; i++) //Q
  {
    char query[maxSize];
    char category[maxSize];
    char itemInTree[maxSize];
    
    fscanf(inFile, "%s %s %s", query, category, itemInTree);

    if(strcmp(query, "search") == 0) //Search
    {
      treeNameNode *nameTree = searchNameNode(root, category);
      if(nameTree == NULL)
      {
        cout << itemInTree << " does not exist\n";
        fprintf(outFile, "%s does not exist\n", itemInTree);
      }
      else
      {
        itemNode *temp = nameTree->theTree;
        search(temp, itemInTree, nameTree);
      }
    }
    else if(strcmp(query, "item_before") == 0) //Count items before
    {
      treeNameNode *nameTree = searchNameNode(root, category);
      if(nameTree == NULL)
      {
        cout << itemInTree << " does not exist\n";
        fprintf(outFile, "%s does not exist\n", itemInTree);
      }
      else
      {
        itemNode *temp = nameTree->theTree;
        item_before(temp, itemInTree);
      }
    }
    else if(strcmp(query, "height_balance") == 0) //Height
    {
      treeNameNode *nameTree = searchNameNode(root, category);
      if(nameTree == NULL)
      {
        cout << itemInTree << " does not exist\n";
        fprintf(outFile, "%s does not exist\n", itemInTree);
      }
      else
      {
        //itemNode *temp = nameTree->theTree;
        height_balance(nameTree);
      }
    }
    else if(strcmp(query, "count") == 0) //Count all
    {
      treeNameNode *nameTree = searchNameNode(root, category);
      if(nameTree == NULL)
      {
        cout << itemInTree << " does not exist\n";
        fprintf(outFile, "%s does not exist\n", itemInTree);
      }
      else
      {
        itemNode *temp = nameTree->theTree;
        cout << "Number of trees: " << count(temp) << "\n";
        fprintf(outFile, "Number of trees: %d\n", count(temp));
      }
    }
    else
    {
      cout << "Invalid query\n";
      fprintf(outFile, "Invalid query\n");
    }
  }
  return root;
}

int main() 
{
  //Open files (change the name if needed)
  inFile = fopen("inFile.txt", "r");
  outFile = fopen("outFile.txt", "w");

  if(inFile == NULL)
  {
    cout << "No input file present.";
    return 1;
  }

  if(outFile == NULL)
  {
    cout << "Error opening output file.";
    if(inFile != NULL)
      fclose(inFile);
    return 1;
  }
  
  treeNameNode* root = buildTree();

  fclose(inFile);
  fclose(outFile);

  return 0;
}
