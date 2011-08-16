#include <algorithm>
#include <iostream>
#include <string>
#include <typeinfo>

#include "Expression.hpp"
#include "Parse.hpp"
#include "Split.hpp"

void help();

int main(int, char**)
{
    Context c;
    while (true) {
        std::string expr;

        std::cout << ">>> ";
        std::getline(std::cin, expr);

        if (!std::cin.good()) {
            return 0;
        }

        if (expr == "?" || expr == "help") {
            help();
        } else if (expr == "quit") {
            return 0;
        } else {
            // Les espaces ne servent a rien.
            expr.erase(
                    std::remove(std::begin(expr), std::end(expr), ' '),
                    std::end(expr)
                    );
            // On a une expression.
            size_t posEgal = expr.find_first_of('=');
            if (posEgal == std::string::npos) {
                // Expression toute bete
                std::cout << (*Parse(expr, c))(c) << std::endl;
            } else {
                const std::string name = expr.substr(0, posEgal);
                const std::string expression = expr.substr(posEgal + 1);
                ExprPtr parsed = Parse(expression, c);
                size_t posParen = expr.find_first_of('(');
                if (posParen == std::string::npos) {
                    // Une simple variable
                    const ExprValue value = (*parsed)(c);
                    c.set(name, value);
                    std::cout << "Variable " << name << " "
                              << "prend la valeur " << value << std::endl;
                } else {
                    // Une fonction, le cas le plus complexe
                    const std::string funcName = name.substr(0, posParen);
                    const std::string args = name.substr(
                            posParen + 1,
                            name.size() - posParen - 2
                            );
                    std::vector<std::string> argsV;
                    Split(args, argsV);
                    c.setFunc(funcName, argsV, parsed);
                    std::cout << "Fonction " << funcName << " "
                              << "creee" << std::endl;
                }
            }
        }
    }
}

void help()
{
    std::cout <<
        "Aide :" << std::endl <<
        "Utilisez '?' ou 'help' pour voir ce message" << std::endl <<
        "Ou bien 'quit' pour arreter" << std::endl <<
        "Ou encore entrez une expression mathematique" << std::endl <<
        "a evaluer." << std::endl <<
        "Vous pouvez entrer des variables, ou meme des" << std::endl <<
        "fonctions, pour les utiliser plus tard !" << std::endl <<
        "Bonne chance !" << std::endl <<
        std::endl;
}

