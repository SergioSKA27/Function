#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <stack>
#include <queue>
#include <bitset>
#include <streambuf>
#include <bit>
// Autor: Lopez Martinez Sergio Demis

// This struct will help us to create the sintax tree
// to evaluate the functions
struct Node
{
    std::string value;
    Node *father;
    Node *Rson;
    Node *Lson;
};

class Function
{
private:
    std::string name;                        // name of the function
    std::vector<std::string> variable_names; // vector for the name of the variables
    std::string expresion;                   // normal representation of the function
    std::string sufix_expr;                  // sufix representation
    std::string prefix_expr;                 // prefix representation;

    Function &sufix_tranform();
    Function &prefix_tranform();
    bool is_operator(char op);
    inline bool is_parentheses(char c);
    int precedence(char c);
    int association_order(char c);

    Function &fix_expression();

public:
    Function(std::string name, std::string func, std::initializer_list<std::string> variables);
    std::string infijo_to_prefijo(std::string expr);
    std::string infijo_to_sufijo(std::string expr);
    void show_varibles();
    ~Function();
};

Function::Function(std::string name, std::string func, std::initializer_list<std::string> variables)
{
    this->name = name;
    this->expresion = func;

    for (auto e : variables)
    {
        this->variable_names.push_back(e);
    }
}

Function &Function::sufix_tranform()
{
    return *this;
}

bool Function::is_operator(char op)
{
    return (op == '+' || op == '-' || op == '*' || op == '/' || op == '^');
}

inline bool Function::is_parentheses(char c)
{
    return (c == '(' || c == ')');
}

int Function::precedence(char c)
{
    switch (c)
    {
    case '+':
    case '-':
        return 0;

    case '*':
    case '/':
        return 1;

    case '^':
        return 2;

    default:
        return -1;
    }
}

int Function::association_order(char c)
{
    switch (c)
    {
    case '^':
        return -1;
    case '*':
    case '/':
    case '-':
    case '+':
        return 1;

    default:
        return 0;
    }
}

std::string Function::infijo_to_sufijo(std::string expr)
{
    std::string result = "";
    std::stack<char> Pl;

    for (int i = 0; i < expr.size(); i++)
    {
        if (this->is_operator(expr[i]))
        {
            // Si es operador se compara contra la pila
            while (!Pl.empty() && this->precedence(Pl.top()) > this->precedence(expr[i]))
            { // Si el top es de mayor precedencia que el operador actual lo escribimos
                result = result + Pl.top();
                Pl.pop();
            }

            if (Pl.empty() || this->precedence(Pl.top()) < this->precedence(expr[i]) || this->is_parentheses(Pl.top()))
            { // si la pila esta vacia o el operador en turno de es de menor precedencia que el top de la pila
                Pl.push(expr[i]);
            }
            else
            { // El operador en turno y el top de la pila tienen la misma precedencia

                if (this->association_order(expr[i]) > 0) // izquierda a derecha
                {
                    result = result + Pl.top();
                    Pl.pop();
                    Pl.push(expr[i]);
                }
                else
                {
                    Pl.push(expr[i]);
                }
            }
        }
        else
        {
            if (this->is_parentheses(expr[i]))
            { // Si es parentesis revisamos si abre o cierra
                if (expr[i] == '(')
                {
                    Pl.push(expr[i]); // si es un parentesis que abre lo ponemos en la pila
                }
                else
                { // si el parentesis cierra sacamos los operadores de la pila hasta encontrar el que abre
                    while (Pl.top() != '(' && !Pl.empty())
                    {
                        result = result + Pl.top();
                        Pl.pop();
                    }
                    Pl.pop(); // eliminamos el parentesis que abre
                }
            }
            else
            {
                // Si es operando lo escribimos directamente
                result = result + expr[i];
            }
        }
    }

    while (!Pl.empty())
    {
        result = result + Pl.top();
        Pl.pop();
    }

    return result;
}

std::string Function::infijo_to_prefijo(std::string expr)
{
    std::string result = "";
    std::stack<char> Pl;

    for (int i = expr.size() - 1; i >= 0; i--)
    {
        if (this->is_operator(expr[i]))
        {
            // Si es operador se compara contra la pila
            while (!Pl.empty() && this->precedence(Pl.top()) > this->precedence(expr[i]))
            { // Si el top es de mayor precedencia que el operador actual lo escribimos
                result = Pl.top() + result;
                Pl.pop();
            }

            if (Pl.empty() || this->precedence(Pl.top()) < this->precedence(expr[i]) || this->is_parentheses(Pl.top()))
            { // si la pila esta vacia o el operador en turno de es de menor precedencia que el top de la pila
                Pl.push(expr[i]);
            }
            else
            { // El operador en turno y el top de la pila tienen la misma precedencia

                if (this->association_order(expr[i]) < 0) // derecha a izquierda
                {
                    result = Pl.top() + result;
                    Pl.pop();
                    Pl.push(expr[i]);
                }
                else
                {
                    Pl.push(expr[i]);
                }
            }
        }
        else
        {
            if (this->is_parentheses(expr[i]))
            { // Si es parentesis revisamos si abre o cierra
                if (expr[i] == ')')
                {
                    Pl.push(expr[i]); // si es un parentesis que abre lo ponemos en la pila
                }
                else
                { // si el parentesis cierra sacamos los operadores de la pila hasta encontrar el que abre
                    while (Pl.top() != ')' && !Pl.empty())
                    {
                        result = Pl.top() + result;
                        Pl.pop();
                    }
                    Pl.pop(); // eliminamos el parentesis que abre
                }
            }
            else
            {
                // Si es operando lo escribimos directamente
                result = expr[i] + result;
            }
        }
    }

    while (!Pl.empty())
    {
        result = Pl.top() + result;
        Pl.pop();
    }

    return result;
}

Function &Function::fix_expression()
{

        return *this;
}

void Function::show_varibles()
{
    for (auto i : this->variable_names)
        std::cout << i << ",";
    std::cout << std::endl;
}

Function::~Function()
{
}

int main(int argc, char const *argv[])
{
    Function X("f(x)", "3x-7", {"x"});
    std::cout << X.infijo_to_prefijo("7^2+6*(5+3)-9") << std::endl;
    X.show_varibles();
    return 0;
}
