#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;



class node {
public:
    string Label;
    int letters_count;
    node* Left;
    node* Right;
    node() { }
    node(string label, int lettersCount, node* left = NULL, node* right = NULL) : Left(left), Right(right)
    {
        Label = label;
        letters_count = lettersCount;
    }
};

string openFile()
{
    ifstream file;
    stringstream buffer;
    file.open("tekst.txt");
    buffer << file.rdbuf();
    return buffer.str();
}

void MinHeapify(vector<node>& tree, int heap_size, int i)
{
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int lowestValue = i;

    if (left < heap_size && tree[left].letters_count < tree[lowestValue].letters_count)
    {
        lowestValue = left;
    }


    if (right < heap_size && tree[right].letters_count < tree[lowestValue].letters_count)
    {
        lowestValue = right;
    }

    if (lowestValue != i)
    {
        swap(tree[i], tree[lowestValue]);
    }

}

void BuildMinHeap(vector<node>& tree)
{
    int initial_size = tree.size();
    for (int i = tree.size() / 2 - 1; i >= 0; i--)
    {
        MinHeapify(tree, initial_size, i);
    }

}

map<char, int> countLetters(string input)
{
    auto letterWithCounts = map<char, int>();

    for (unsigned int i = 0; i < input.length(); i++)
    {
        if (letterWithCounts.contains(input[i]))
        {
            letterWithCounts[input[i]]++;
        }
        else
        {
            letterWithCounts.insert(pair(input[i], 1));
        }
    }

    return letterWithCounts;
}

void saveDictionary(map<char, int> map)
{
    ofstream DictionaryFile("dictionary.txt");

    for (auto it = map.begin(); it != map.end(); it++)
    {
        DictionaryFile << "\"" << it->first << "\": \"" << it->second << "\",\n";
    }

   DictionaryFile.close();
}
vector<node> makeTree()
{
    vector<node> tree;
    string str = openFile();
    map<char, int> map = countLetters(str);

    saveDictionary(map);

    auto it = map.begin();

    while (it != map.end())
    {
        tree.push_back(node(string(1, it->first), it->second));
        it++;
    }
    return tree;
}

node extractMin(vector<node>& tree)
{
    auto minimum = tree.front();
    auto minimumIt = tree.begin();
    auto it = tree.begin();
    for (int i = 0; i < tree.size(); i++)
    {
        if (i != tree.size() - 1)
        {
            it++;
        }
        if (tree[i].letters_count < minimum.letters_count)
        {
            minimumIt = it;
            minimum = tree[i];
        }
    }

    iter_swap(minimumIt, --tree.end());
    auto extractedElement = tree.at(tree.size() - 1);
    node elemToReturn = extractedElement;

    tree.resize(tree.size() - 1);

    return elemToReturn;
}

void createFullLable(vector<node>& tree)
{
    BuildMinHeap(tree);
    for (int i = 0; tree.size() > 1; i++)
    {
        node newnode = node();
        node last = extractMin(tree);
        newnode.letters_count += last.letters_count;
        newnode.Left = new node(last.Label, last.letters_count, last.Left, last.Right);

        node secodLast = extractMin(tree);
        newnode.Right = new node(secodLast.Label, secodLast.letters_count, secodLast.Left, secodLast.Right);
        newnode.letters_count += secodLast.letters_count;

        auto labelVal = newnode.Left->Label + newnode.Right->Label;
        newnode.Label = labelVal;

        tree.push_back(newnode);
        BuildMinHeap(tree);
    }


}
int main()
{
    vector<node> tree = makeTree();
    createFullLable(tree);
    cout << "test";
}