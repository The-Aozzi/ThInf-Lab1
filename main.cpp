#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// @brief Represents a node of a code tree.
struct Node{

    // @brief Child nodef for symbols with lower and greater probability respectively. 
    Node *lower, *greater;

    // @brief Probability of a ymbol or group of symbols.
    double value;

    // @brief true if it is a leaf node.
    bool leaf;

    // @brief Index of symbol input.Only applicable for leaf nodes.
    size_t index;
};

// @brief Compares nodes in non-increasing order.
bool cmp(const Node* left, const Node* right){
    return left->value >= right->value;
}

// @brief Traverses a code tree and creates a path for avery leaf node.
void treeTravesal(Node* root, std::vector<std::string> &paths, std::string path = ""){
    if(root->leaf){
        paths[root->index] = path;
    }else{
        treeTravesal(root->greater, paths, path + '1');
        treeTravesal(root->lower, paths, path + '0');
    }
}

int main(int, char**){
    size_t n;
    std::vector<Node*> ensemble;
    std::vector<double> probabilities;
    std::cin >> n;
    for(size_t i = 0; i < n; i++){
        probabilities.push_back(0);
        std::cin >> probabilities.back();
        ensemble.push_back(new Node {nullptr, nullptr, probabilities.back(), true, i});
    }

    // Constructing the code tree.
    for(size_t i = n-1; i > 0; --i){
        double sum_value = ensemble[i]->value + ensemble[i-1]->value;
        Node* node = new Node{ensemble[i], ensemble[i-1], sum_value, false};
        ensemble.pop_back();
        ensemble.pop_back();
        auto iter = std::lower_bound(ensemble.begin(), ensemble.end(), node, cmp);
        ensemble.emplace(iter, node);
    }

    std::vector<std::string> codes(8, "");
    treeTravesal(ensemble[0], codes);
    double average_len = 0;
    double entropy = 0;
    std::cout << "\nCorresponding codes:\n";
    for (size_t i = 0; i < n; i++){
        std::cout << codes[i] << std::endl;
        average_len += codes[i].size() * probabilities[i];
        entropy -= probabilities[i] * std::log2(probabilities[i]);
    }
    std::cout << "\nAverage length: " << average_len << std::endl;
    std::cout << "Entropy:        " << entropy << std::endl;
    std::cout << "Redundancy:     " << average_len - entropy << std::endl;
    
}
