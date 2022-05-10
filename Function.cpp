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
#include <algorithm>
// Autor: Lopez Martinez Sergio Demis

// This struct will help us to create the sintax tree
// to evaluate the functions

class Expression
{
private:
public:
    std::string expr;
    Expression(std::string expr);
    bool is_constant() const;
    bool is_variable() const;
    bool has_exponent() const;
    bool is_operator(char op) const;

    Expression operator+(const Expression &expr);
    Expression operator-(const Expression &expr);
    Expression operator*(const Expression &expr);
    Expression operator/(const Expression &expr);
    Expression &operator=(const Expression &expr);
    size_t size() const;

    friend std::ostream &operator<<(std::ostream &o, Expression expr)
    {
        o << expr.expr;
        return o;
    }

    ~Expression();
};

Expression::Expression(std::string expr)
{
}

bool Expression::is_constant() const
{

    bool f = true;

    for (size_t i = 0; i < this->expr.size(); i++)
    {
        if (this->expr[i] - '0' >= 0 && this->expr[i] - '0' < 10)
            continue;
        else
        {
            f = false;
            break;
        }
    }

    return f;
}

bool Expression::is_variable() const
{
    return this->is_constant() ? false : true;
}

bool Expression::is_operator(char op) const
{
    return (op == '+' || op == '-' || op == '*' || op == '/' || op == '^');
}

size_t Expression::size() const
{
    return this->expr.size();
}

Expression Expression::operator+(const Expression &expr)
{
    if (expr.is_variable() && this->is_variable())
    {
        int k = 0;
        std::string subxpr = "";
        std::map<std::string, Expression> exprs;

        for (size_t i = 0; i < expr.size(); i++)
        {
            if (this->is_operator(expr.expr[i]) == false)
            {
                subxpr += expr.expr[i];
            }
            else if (expr.expr[i] == '^')
            {
                subxpr += expr.expr[i];
            }
            else
            {
                Expression aux(subxpr);
                exprs.insert({subxpr, aux});
            }
        }
    }
}

Expression::~Expression()
{
}

struct Node
{
    Expression value;
    Node *father;
    Node *Rson;
    Node *Lson;
};

template <typename P>
struct Monoid
{
};

template <typename T>
class Function
{
private:
    std::string name;                 // name of the function
    std::vector<char> variable_names; // vector for the name of the variables
    std::string expresion;            // normal representation of the function
    std::string sufix_expr;           // sufix representation
    std::string prefix_expr;          // prefix representation;

    Function<T> &sufix_tranform();
    Function<T> &prefix_tranform();
    bool is_operator(char op);
    inline bool is_parentheses(char c);
    int precedence(char c);
    int association_order(char c);

    Function<T> &fix_expression();

    T eval_op(T A, char op, T B);
    T evaluate_expression();

public:
    Function(std::string name, std::string func, std::initializer_list<char> variables);
    std::string infijo_to_prefijo(std::string expr);
    std::string infijo_to_sufijo(std::string expr);
    T operator()(const T &value);                 // return the function valued in T value
    T operator()(std::initializer_list<T> tuple); // retunr the function in n-tuple of T values
    template <typename... args>
    T operator()(args... ts); // using parameter pack expansion we can evaluate the object like f(x1,x2,...,xn)
    void show_varibles();
    ~Function();
};

template <typename T>
Function<T>::Function(std::string name, std::string func, std::initializer_list<char> variables)
{
    this->name = name;
    this->expresion = func;
    this->prefix_expr = this->infijo_to_prefijo(func);

    for (auto e : variables)
    {
        this->variable_names.push_back(e);
    }
}

template <typename T>
bool Function<T>::is_operator(char op)
{
    return (op == '+' || op == '-' || op == '*' || op == '/' || op == '^');
}

template <typename T>
inline bool Function<T>::is_parentheses(char c)
{
    return (c == '(' || c == ')');
}

template <typename T>
int Function<T>::precedence(char c)
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

template <typename T>
int Function<T>::association_order(char c)
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

template <typename T>
std::string Function<T>::infijo_to_sufijo(std::string expr)
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

template <typename T>
std::string Function<T>::infijo_to_prefijo(std::string expr)
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

template <typename T>
Function<T> &Function<T>::fix_expression()
{

    return *this;
}

template <typename T>
T Function<T>::eval_op(T A, char op, T B)
{
    if (op == '^')
    {
        return std::pow(A, B);
    }
    else if (op == '*')
    {
        return A * B;
    }
    if (op == '/')
    {
        return A / B;
    }
    if (op == '+')
    {
        return A + B;
    }
    if (op == '-')
    {
        return A - B;
    }

    return A;
}

template <typename T>
void Function<T>::show_varibles()
{
    for (auto i : this->variable_names)
        std::cout << i << " ";
    std::cout << std::endl;
}

template <typename T>
T Function<T>::operator()(std::initializer_list<T> tuple)
{

    std::string newrep = "";
    for (size_t i = 0; i < this->expresion.size(); i++)
    {

        if ((this->expresion[i] - '0' >= 0 && this->expresion[i] - '0' <= 9) || this->is_operator(this->expresion[i]) || this->is_parentheses(this->expresion[i]))
        {
            newrep += this->expresion[i];
        }
        else
        {
            int p = 0, k = 0;
            for (size_t j = 0; j < this->variable_names.size(); j++)
            {
                if (this->expresion[i] == this->variable_names[j])
                {
                    p = j;
                    break;
                }
            }

            for (auto iter : tuple)
            {
                if (k == p)
                {
                    newrep += ('(' + std::to_string(iter) + ')');
                    break;
                }
                k++;
            }
        }
    }

    std::cout << newrep << std::endl;
}

template <typename T>
T Function<T>::operator()(const T &value)
{

    std::string newrep = "";
    for (size_t i = 0; i < this->expresion.size(); i++)
    {

        if ((this->expresion[i] - '0' >= 0 && this->expresion[i] - '0' <= 9) || this->is_operator(this->expresion[i]) || this->is_parentheses(this->expresion[i]))
        {
            newrep += this->expresion[i];
        }
        else
        {
            newrep += ('(' + std::to_string(value) + ')');
        }
    }

    std::cout << newrep << std::endl;
}

template <typename T>
Function<T>::~Function()
{
}

int main(int argc, char const *argv[])
{
    Function<int> X("f(x,y,z)", "3*x-7*y+z", {'x', 'y', 'z'}), f("g(x)", "x+1/x-x^2", {'x'});
    X({1, 2, 3});
    f(2);

    int x = 1.0;
    Expression T("298x");
    if (T.is_variable())
        std::cout << X.infijo_to_prefijo("(7*x^2+4*x^2)*y") << std::endl;
    X.show_varibles();
    return 0;
}
