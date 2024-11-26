#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

struct TreeNode {
    string word;
    string translation;
    int counter;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const string& w, const string& t)
        : word(w), translation(t), counter(0), left(nullptr), right(nullptr) {}
};

class Dictionary {
private:
    TreeNode* root;

    TreeNode* addNode(TreeNode* node, const string& word, const string& translation) {
        if (!node) {
            return new TreeNode(word, translation);
        }
        if (word < node->word) {
            node->left = addNode(node->left, word, translation);
        }
        else if (word > node->word) {
            node->right = addNode(node->right, word, translation);
        }
        else {
            node->translation = translation;
        }
        return node;
    }

    TreeNode* findNode(TreeNode* node, const string& word) {
        if (!node || node->word == word) {
            return node;
        }
        if (word < node->word) {
            return findNode(node->left, word);
        }
        return findNode(node->right, word);
    }

    void inOrderTraversal(TreeNode* node, vector<TreeNode*>& nodes) {
        if (!node) return;
        inOrderTraversal(node->left, nodes);
        nodes.push_back(node);
        inOrderTraversal(node->right, nodes);
    }

    void deleteTree(TreeNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    TreeNode* deleteNode(TreeNode* node, const string& word) {
        if (!node) return nullptr;
        if (word < node->word) {
            node->left = deleteNode(node->left, word);
        }
        else if (word > node->word) {
            node->right = deleteNode(node->right, word);
        }
        else {
            if (!node->left) {
                TreeNode* rightChild = node->right;
                delete node;
                return rightChild;
            }
            if (!node->right) {
                TreeNode* leftChild = node->left;
                delete node;
                return leftChild;
            }
            TreeNode* minLargerNode = getMinNode(node->right);
            node->word = minLargerNode->word;
            node->translation = minLargerNode->translation;
            node->counter = minLargerNode->counter;
            node->right = deleteNode(node->right, minLargerNode->word);
        }
        return node;
    }

    TreeNode* getMinNode(TreeNode* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

public:
    Dictionary() : root(nullptr) {}

    ~Dictionary() {
        deleteTree(root);
    }

    void addWord(const string& word, const string& translation) {
        root = addNode(root, word, translation);
    }

    string getTranslation(const string& word) {
        TreeNode* node = findNode(root, word);
        if (node) {
            ++node->counter;
            return node->translation;
        }
        return "—лово не знайдено";
    }

    void displayAllWords() {
        vector<TreeNode*> nodes;
        inOrderTraversal(root, nodes);
        for (auto node : nodes) {
            cout << node->word << " Ч " << node->translation << " (звернень: " << node->counter << ")\n";
        }
    }

    void displayTopWords(bool popular = true) {
        vector<TreeNode*> nodes;
        inOrderTraversal(root, nodes);
        sort(nodes.begin(), nodes.end(), [popular](TreeNode* a, TreeNode* b) {
            return popular ? a->counter > b->counter : a->counter < b->counter;
            });
        cout << (popular ? "“оп-3 найпопул€рн≥ших сл≥в:\n" : "“оп-3 найменш попул€рних сл≥в:\n");
        for (size_t i = 0; i < min(size_t(3), nodes.size()); ++i) {
            cout << nodes[i]->word << " Ч " << nodes[i]->translation << " (звернень: " << nodes[i]->counter << ")\n";
        }
    }

    void deleteWord(const string& word) {
        root = deleteNode(root, word);
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Dictionary dict;

    dict.addWord("hello", "прив≥т");
    dict.addWord("world", "св≥т");
    dict.addWord("programming", "програмуванн€");

    cout << "ѕереклад слова 'hello': " << dict.getTranslation("hello") << endl;

    cout << "\n”с≥ слова:\n";
    dict.displayAllWords();

    cout << "\n";
    dict.displayTopWords();
    cout << "\n";
    dict.displayTopWords(false);

    return 0;
}
