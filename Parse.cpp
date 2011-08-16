#include "Parse.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "Context.hpp"
#include "Expression.hpp"
#include "Function.hpp"
#include "Op0.hpp"
#include "Op1.hpp"
#include "Op2.hpp"
#include "Split.hpp"

enum Associativite{ DG, GD }; // De droite a gauche ou l'inverse ?
typedef unsigned short Priorite;

std::set<char> operateurs{'+', '-', '*', '/', '%', '^'};
std::set<std::string> fonctions{"pow", "sqrt", "rt", "cos", "sin", "tan",
                                "exp", "log" , "ln"};
std::set<std::string> constantes{"pi", "e"};

std::map<char, Priorite> priorites{
    {'+', 9}, {'-', 9},
    {'*', 8}, {'/', 8}, {'%', 8},
    {'^', 7}
};
std::map<char, Associativite> associativites{
    {'+', GD}, {'-', GD},
    {'*', GD}, {'/', GD}, {'%', GD},
    {'^', DG}
};

static void Clean(std::string & s, const Context & c);
static ExprPtr DoParse(const std::string & e, const Context & c);
static bool ExprClote(const std::string & e);

ExprPtr Parse(std::string e, const Context & c)
{
    Clean(e, c);
    return DoParse(e, c);
}

static void Clean(std::string & s, const Context & c)
{
    // On retire tous les espaces.
    s.erase(std::remove(std::begin(s), std::end(s), ' '), std::end(s));
    // On remplace les [] par des ()
    std::replace(std::begin(s), std::end(s), '[', '(');
    std::replace(std::begin(s), std::end(s), ']', ')');
    // On va ajouter les * si necessaire
    char dernierCaractere = '+';
    std::string nom;
    for (size_t i = 0 ; i < s.size() ; ++i) {
        if (std::isdigit(s[i]) && std::isdigit(dernierCaractere)) {
            // On est au milieu d'un chiffre - on saute.
            nom.clear();
            dernierCaractere = s[i];
            continue;
        }
        if ((dernierCaractere == ')' && s[i] != ')' && s[i] != ','
                && operateurs.find(s[i]) == operateurs.end())
            || (s[i] == '(' && fonctions.find(nom) == fonctions.end()
                && !c.hasFunc(nom) && dernierCaractere != '('
                && operateurs.find(dernierCaractere) == operateurs.end())
            || (std::isdigit(dernierCaractere) && !std::isdigit(s[i])
                && s[i] != ')' && s[i] != ',' && s[i] != 'E' && s[i] != '.'
                && operateurs.find(s[i]) == operateurs.end())
            || (std::isdigit(s[i]) && !std::isdigit(dernierCaractere)
                && dernierCaractere != '(' && dernierCaractere != ','
                && dernierCaractere != 'E' && dernierCaractere != '.'
                && operateurs.find(dernierCaractere) == operateurs.end())
            ) {
            s.insert(i, 1, '*');
            ++i;
            nom.clear();
        }
        if (dernierCaractere == '(' || dernierCaractere == ')'
                || dernierCaractere == ','
                || operateurs.find(dernierCaractere) != operateurs.end()) {
            nom.clear();
        }
        nom.push_back(s[i]);
        dernierCaractere = s[i];
    }
}

static ExprPtr DoParse(const std::string & expr, const Context & c)
{
    // Si on n'a pas d'expression, on retourne 0 (utile pour -N)
    if (expr.empty()) {
        return New<Const>(0);
    }
    // Si on est en train de calculer l'interieur de parentheses, on
    // recommence sans.
    if (expr.front() == '(' && expr.back() == ')' && ExprClote(expr)) {
        return DoParse(expr.substr(1, expr.size() - 2), c);
    }
    // D'abord, on cherche le dernier operateur, de priorite la plus
    // grande, en sautant les parentheses.
    Priorite max_prio = 0;
    size_t max_i = 0;
    size_t profondeur = 0;
    for (size_t i = 0 ; i < expr.size() ; ++i) {
        // Si on est dans une parenthese, on va a sa fin
        if (profondeur != 0 || expr[i] == '(') {
            if (expr[i] == '(') ++profondeur;
            if (expr[i] == ')') --profondeur;
            continue;
        }
        // Sinon, on va voir si on a le nouvel operateur a placer.
        Priorite prio = priorites[expr[i]];
        if (prio > max_prio) {
            max_prio = prio;
            max_i = i;
        } else if (prio == max_prio) {
            if (associativites[expr[i]] == GD) {
                max_i = i;
            }
        }
    }
    // Si on n'a pas trouve d'operateur, on n'a qu'un nombre, une
    // fonction ou encore une constante.
    if (max_prio < 1) {
        // On essaie de voir si on a un nombre.
        ExprValue val;
        std::istringstream iss(expr);
        if (iss >> val) {
            // On a bien un nombre. cf.
//http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.2
            // pour plus d'informations sur le pourquoi du comment.
            return New<Const>(val);
        } else if (constantes.find(expr) != constantes.end()) {
            // On a une constante
            if (expr == "e") {
                return New<E>();
            } else if (expr == "pi") {
                return New<Pi>();
            } else {
                throw EpicFail("Constante inconnue");
            }
        } else {
            // On a une fonction
            size_t pos = expr.find('(');
            if (pos == std::string::npos) {
                // Finalement non, on a une variable.
                return New<Variable>(expr);
            }
            std::string nom = expr.substr(0, pos);
            std::string argumentsString = expr.substr(
                    pos + 1,
                    expr.size() - pos - 2
                    );
            std::vector<std::string> argumentsVec;
            Split(argumentsString, argumentsVec);
            std::vector<ConstExprPtr> arguments;
            for (size_t i = 0 ; i < argumentsVec.size() ; ++i) {
                arguments.push_back(DoParse(argumentsVec[i], c));
            }
            if (nom == "sqrt") {
                if (arguments.size() < 1) {
                    throw EpicFail("Pas assez d'arguments a sqrt");
                }
                return New<Sqrt>(arguments[0]);
            } else if (nom == "pow") {
                if (arguments.size() < 2) {
                    throw EpicFail("Pas assez d'arguments a pow");
                }
                return New<Pow>(arguments[0], arguments[1]);
            } else if (nom == "rt") {
                if (arguments.size() < 2) {
                    throw EpicFail("Pas assez d'arguments a rt");
                }
                return New<Pow>(
                        arguments[0],
                        New<Divides>(
                            New<Const>(1),
                            arguments[1]
                            )
                        ); // rt(A, B) <=> pow(A, 1/B)
            } else if (nom == "cos") {
                if (arguments.size() < 1) {
                    throw EpicFail("Pas assez d'arguments a cos");
                }
                return New<Cos>(arguments[0]);
            } else if (nom == "sin") {
                if (arguments.size() < 1) {
                    throw EpicFail("Pas assez d'arguments a sin");
                }
                return New<Sin>(arguments[0]);
            } else if (nom == "tan") {
                if (arguments.size() < 1) {
                    throw EpicFail("Pas assez d'arguments a tan");
                }
                return New<Tan>(arguments[0]);
            } else if (nom == "exp") {
                if (arguments.size() < 1) {
                    throw EpicFail("Pas assez d'arguments a exp");
                }
                return New<Pow>(New<E>(), arguments[0]);
            } else if (nom == "log") {
                if (arguments.size() < 2) {
                    throw EpicFail("Pas assez d'arguments a log");
                }
                return New<Divides>(
                        New<Ln>(arguments[0]),
                        New<Ln>(arguments[1])
                        );
            } else if (nom == "ln") {
                if (arguments.size() < 1) {
                    throw EpicFail("Pas assez d'arguments a ln");
                }
                return New<Ln>(arguments[0]);
            } else {
                // Une fonction declaree par l'utilisateur, peut-etre ?
                if (c.hasFunc(nom)) {
                    if (arguments.size() < c.funcArgs(nom).size()) {
                        throw EpicFail("Pas assez d'arguments a " + nom);
                    }
                    return New<Function>(nom, arguments);
                } else {
                    throw EpicFail("Fonction inconnue");
                }
            }
        }
    }
    // Sinon, on va calculer le resultat de l'expression.
    // On recherche les parties a gauche et a droite.
    ExprPtr lhs = DoParse(expr.substr(0, max_i), c);
    ExprPtr rhs = DoParse(expr.substr(max_i + 1), c);
    // Et on renvoie le resultat.
    switch(expr[max_i]) {
        case '+': return New<Plus   >(lhs, rhs);
        case '-': return New<Minus  >(lhs, rhs);
        case '*': return New<Times  >(lhs, rhs);
        case '/': return New<Divides>(lhs, rhs);
        case '%': return New<Modulo >(lhs, rhs);
        case '^': return New<Pow    >(lhs, rhs);
    }

    // Il y a eu un big bug : On a defini un operateur dans les maps tout
    // en haut, mais on ne l'a pas implemente ici.
    throw EpicFail("Operateur inconnu");
}

static bool ExprClote(const std::string & s)
{
    int profondeur = 1;
    for (char c : s.substr(1, s.size() - 2)) {
        switch (c) {
            case '(':
                ++profondeur;
                break;
            case ')':
                --profondeur;
                break;
        }
        if (profondeur == 0) {
            // Certes profondeur = 0 au debut, mais on n'appelle cette
            // fonction que si e commence et finit par (). Donc on est sur
            // qu'apres avoir passe le premier caractere on aura profondeur
            // egal 1.
            return false;
        }
    }
    return true;
}

