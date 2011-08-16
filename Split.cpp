#include "Split.hpp"

#include <string>
#include <vector>

void Split(const std::string & S, std::vector<std::string> & V) {
    std::string Item;
    size_t profondeur = 0;
    for (char c : S) {
        if (c == '(') {
            ++profondeur;
            Item.push_back(c);
        } else if (c == ')') {
            --profondeur;
            Item.push_back(c);
        } else if (c == ',' && profondeur == 0) {
            V.push_back(Item);
            Item.clear();
        } else {
            Item.push_back(c);
        }
    }
    V.push_back(Item);
}

