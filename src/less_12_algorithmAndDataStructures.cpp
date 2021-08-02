//============================================================================
// Name        : less_12_algorithmAndDataStructures.cpp
// Author      : andry antonenko
// IDE         : Eclipse IDE
// Description : lesson 12 of the algorithms and data structures course
//============================================================================

#include <iostream>
#include <random>
#include <stdlib.h>
#include <cmath>
#include <chrono>
//----------------------------------------------------------------------------
using namespace std;
//----------------------------------------------------------------------------
//#define NDEBUG
#include <cassert>
//----------------------------------------------------------------------------
int getRandomNum(int min, int max)
{
  const static auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::mt19937_64 generator(seed);
  std::uniform_int_distribution<int> dis(min, max);
  return dis(generator);
}
//----------------------------------------------------------------------------
/*
1. Написать функцию проверяющую
является ли переданное в неё бинарное дерево сбалансированным и
написать программу, которая:
- создаст [50] деревьев по [10000] узлов и
заполнит узлы случайными целочисленными значениями;
- рассчитает, какой процент из созданных деревьев является сбалансированными.
*/

const int NO_BALANCED = -1;

struct TreeNode
{
  int key;
  TreeNode *left;
  TreeNode *right;
};

TreeNode* treeInsert(TreeNode *t, int key)
{
  TreeNode *newNode = nullptr;
  try{
    newNode = new TreeNode;
  }
  catch (const std::exception&) {
    cout << "No memory allocated." << endl;
  }

  if(newNode != nullptr)
  {
    newNode->key = key;
    newNode->left = nullptr;
    newNode->right = nullptr;

    TreeNode *current = t;
    TreeNode *parent = t;

    if(t == nullptr)
    {
      t = newNode;
    }
    else
    {
      while(current->key != key)
      {
        parent = current;
        if(current->key > key)
        {
          current = current->left;
          if(current == nullptr)
          {
            parent->left = newNode;
            return t;
          }
        }
        else
        {
          current = current->right;
          if(current == nullptr)
          {
            parent->right = newNode;
            return t;
          }
        }
      }
    }
  }

  return t;
}

void freeTree(TreeNode *t)
{
  if(t != nullptr)
  {
    freeTree(t->left);
    freeTree(t->right);
    delete t;
  }
}

void printTree(const TreeNode *t)
{
  if(t != nullptr)
  {
    cout << t->key;
    if(t->left != nullptr || t->right != nullptr)
    {
      cout << "(";
      if(t->left != nullptr)
        printTree(t->left);
      else
        cout << "N";
      cout << ",";
      if(t->right != nullptr)
        printTree(t->right);
      else
        cout << "N";
      cout << ")";
    }
  }
}

TreeNode* getSuccessor(TreeNode *node)
{
  TreeNode *current = node->right;
  TreeNode *parent = node;
  TreeNode *s = node;

  while(current != nullptr)
  {
    parent = s;
    s = current;
    current = current->left;
  }

  if(s != node->right)
  {
    parent->left = s->right;
    s->right = node->right;
  }

  return s;
}

bool deleteTreeNode(TreeNode *t, int key)
{
  assert(t != nullptr);

  TreeNode *current = t;
  TreeNode *parent = t;
  bool isLeftChild = true;

  while(current->key != key)
  {
    parent = current;
    if(key < current->key)
    {
      current = current->left;
      isLeftChild = true;
    }
    else
    {
      current = current->right;
      isLeftChild = false;
    }
    if(current == nullptr)
      return false;
  }
  //deleting leaves
  if(current->left == nullptr && current->right == nullptr)
  {
    if(current == t)
    {
      delete t;
      t = nullptr;
    }
    else if(isLeftChild)
    {
      delete parent->left;
      parent->left = nullptr;
    }
    else
    {
      delete parent->right;
      parent->right = nullptr;
    }
  }
  //
  else if(current->right == nullptr)
  {
    if(isLeftChild)
      parent->left = current->left;
    else
      parent->right = current->left;
    delete current;
  }
  else if(current->left == nullptr)
  {
    if(isLeftChild)
      parent->left = current->right;
    else
      parent->right = current->right;
    delete current;
  }
  //
  else
  {
    TreeNode *successor = getSuccessor(current);
    if(current == t)
    {
      t = successor;
    }
    else if(isLeftChild)
    {
      parent->left = successor;
    }
    else
    {
      parent->right = successor;
    }
    successor->left = current->left;
  }

  return true;
}

void preOrderTravers(TreeNode *t)
{
  if(t != nullptr)
  {
    cout << t->key << " ";
    preOrderTravers(t->left);
    preOrderTravers(t->right);
  }
}

void inOrderTravers(TreeNode *t)
{
  if(t != nullptr)
  {
    inOrderTravers(t->left);
    cout << t->key << " ";
    inOrderTravers(t->right);
  }
}

void postOrderTravers(TreeNode *t)
{
  if(t != nullptr)
  {
    inOrderTravers(t->left);
    inOrderTravers(t->right);
    cout << t->key << " ";
  }
}

int depthCount(TreeNode *t)
{
  if(t != nullptr)
  {
    int l = depthCount(t->left);
    int r = depthCount(t->right);
    if(l > r) return l + 1;
    else return r + 1;
  }
  else
  {
    return 0;
  }
}

int isBalanced(TreeNode *t)
{
  if(t == nullptr)
    return 0;

  int l = isBalanced(t->left);
  if(l == NO_BALANCED)
    return NO_BALANCED;
  int r = isBalanced(t->right);
  if(r == NO_BALANCED)
    return NO_BALANCED;
  if(abs(l - r) > 1)
    return NO_BALANCED;
  else if(l > r)
    return l + 1;
  else return r + 1;
}

//----------------------------------------------------------------------------
/*
2. Написать рекурсивную функцию бинарного поиска в дереве хранящемся в узлах,
а не в массиве.

Результатом работы должен стать один файл с кодом на языке С,
содержащий функцию main и все необходимые функции.
*/

bool searchTreeNode(TreeNode *t,int key)
{
  if(t == nullptr)
    return false;
  if(t->key == key)
    return true;
  if(searchTreeNode(t->left,key))
    return true;
  if(searchTreeNode(t->right,key))
    return true;
  return false;
}

//----------------------------------------------------------------------------
void task_1(void)
{
  cout << "Task 1\n" << endl;

  TreeNode *tree = nullptr;

  tree = treeInsert(tree,50);
  treeInsert(tree,30);
  treeInsert(tree,20);
  treeInsert(tree,70);
  treeInsert(tree,60);
  treeInsert(tree,90);
  treeInsert(tree,55);
  treeInsert(tree,80);

  printTree(tree);
  cout << endl;

  preOrderTravers(tree);
  cout << endl;
  inOrderTravers(tree);
  cout << endl;
  postOrderTravers(tree);
  cout << endl;

  cout << "Depth of tree: " << depthCount(tree) << endl;
  if(isBalanced(tree) == NO_BALANCED)
    cout << "Tree isn't balanced" << endl;
  else
    cout << "Tree is balanced" << endl;
  freeTree(tree);
  tree = nullptr;

  const int NUMBER_TREES = 50;
  const int NUMBER_NODES = 10000;

  cout << "Created " << NUMBER_TREES << " trees with " <<
      NUMBER_NODES << " nodes." << endl;

  TreeNode *trees[NUMBER_TREES] = {nullptr};

  const int MIN = 0;
  const int MAX = 100;

  int cnt = 0;
  for(int i = 0; i < NUMBER_TREES; i++)
  {
    int n = NUMBER_NODES;
    trees[i] = treeInsert(trees[i],(MAX-MIN)/2);
    while(n-- != 0)
    {
      treeInsert(trees[i],getRandomNum(MIN,MAX));
    }
    if(isBalanced(trees[i]) != NO_BALANCED)
    {
      cnt++;
    }
  }

  cout << "Number of balanced trees, %: " << cnt*100/NUMBER_TREES << endl;

  for(int i = 0; i < NUMBER_TREES; i++)
    freeTree(trees[i]);
}
//----------------------------------------------------------------------------
void task_2(void)
{
  cout << "\nTask 2\n" << endl;

  TreeNode *tree = nullptr;

  tree = treeInsert(tree,50);
  treeInsert(tree,30);
  treeInsert(tree,20);
  treeInsert(tree,70);
  treeInsert(tree,60);
  treeInsert(tree,90);
  treeInsert(tree,55);
  treeInsert(tree,80);

  printTree(tree);
  cout << endl;

  const int VALUE = 70;

  if(searchTreeNode(tree,VALUE))
    cout << "Value " << VALUE << " found." << endl;
  else
    cout << "Value " << VALUE << " not found." << endl;
  freeTree(tree);
  tree = nullptr;
}
//----------------------------------------------------------------------------
int main() {
//----------------------------------------------------------------------------
  // Task 1
  //*
  task_1();
  //*/
//----------------------------------------------------------------------------
  // Task 2
  /*
  task_2();
  //*/
//----------------------------------------------------------------------------
	return 0;
}
//----------------------------------------------------------------------------
