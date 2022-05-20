#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <queue>
#include <bitset>
#include <streambuf>
#include <bit>
#include <algorithm>
#include <cmath>

#define str_to_value(type, value)                \
    ({                                           \
        type value_n;                            \
                                                 \
        if (typeid(int) == typeid(type))         \
        {                                        \
            value_n = std::stoi(value);          \
        }                                        \
                                                 \
        if (typeid(float) == typeid(type))       \
        {                                        \
            value_n = std::stof(value);          \
        }                                        \
                                                 \
        if (typeid(double) == typeid(type))      \
        {                                        \
            value_n = std::stod(value);          \
        }                                        \
        value_n;                                 \
                                                 \
        if (typeid(long double) == typeid(type)) \
        {                                        \
            value_n = std::stold(value);         \
        }                                        \
        value_n;                                 \
                                                 \
        if (typeid(long) == typeid(type))        \
        {                                        \
            value_n = std::stol(value);          \
        }                                        \
        value_n;                                 \
                                                 \
        if (typeid(long long) == typeid(type))   \
        {                                        \
            value_n = std::stoll(value);         \
        }                                        \
        value_n;                                 \
    })

// Autor: Lopez Martinez Sergio Demis

// La macro str_to_value nos ayuda a convertir un tipo de dato string a uno numerico del tipo seleccionado
// esto nos permite poder usar la clase con implementaciones propias de un tipo de dato con los operadores
// aritmeticos sobrecargados y que a su vez tenga setido una expresion algebraica, es decir que el tipo de
// dato que implementemos debe de poder hacer uso de los operadores aritmericos (+,-,*,/,^) y similarmente
// debe de poder usar los operadores booleanos (==, < ,>,>=,<=,!=)

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
    Expression ex("");
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

    return ex;
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
    std::string name;                 // nombre de la function
    std::vector<char> variable_names; // vector para el nombre de las variables
    std::string expresion;            // representacion normal  de la funcion
    std::string sufix_expr;           // representacion sufijo

    std::string sufix_tranform(std::string xpr);
    Function<T> &prefix_tranform();
    bool is_operator(char op);
    inline bool is_parentheses(char c);
    int precedence(char c);
    int association_order(char c);

    Function<T> &fix_expression();

    T eval_op(T A, char op, T B);

public:
    Function(std::string name, std::string func, std::initializer_list<char> variables);
    T operator()(std::initializer_list<T> tuple); // retunr the function in n-tuple of T values
    T operator()(std::vector<T> tuple);           // retunr the function in n-tuple of T values
    T operator()(T *tuple);                       // retunr the function in n-tuple of T values
    template <typename... args>
    T operator()(args... ts); // using parameter pack expansion we can evaluate the object like f(x1,x2,...,xn)
    void show_varibles();

    T evaluate_num_expression(std::string expr); // Evalua expresiones que constan unicamente de numeros
    ~Function();
};

template <typename T>
Function<T>::Function(std::string name, std::string func, std::initializer_list<char> variables)
{
    this->name = name;
    this->expresion = func;

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
std::string Function<T>::sufix_tranform(std::string xpr)
{

    std::string exprs = xpr, current_op = "";

    std::string fixed_expr = "";

    std::stack<char> Pila;
    // seaparamos los operandos con '|' para poder usar numeros flotantes o cualquier otra representacion de un numero

    for (size_t i = 0; i < exprs.size(); i++)
    { // recorremos la expresion infijo

        if (this->is_operator(exprs[i]))
        { // Si es operador

            if ((exprs[i] == '-' || exprs[i] == '+'))
            { // estas son excepciones en las cuales el operador '-' o '+' funge como la representacion de un numero negativo o positivo

                if (exprs[i + 1] && (exprs[i + 1] - '0' >= 0 && exprs[i + 1] - '0' <= 9) && i == 0)
                { // si el operador se encuentra al inicio de la expresion (-1 + 5)
                    fixed_expr += '|';
                    fixed_expr += exprs[i];
                    continue;
                }

                if (exprs[i - 1] && (this->is_operator(exprs[i - 1])))
                {
                    fixed_expr += '|';
                    fixed_expr += exprs[i];
                    continue;
                }

                if (exprs[i - 1] && exprs[i - 1] == '(')
                {
                    fixed_expr += '|';
                    fixed_expr += exprs[i];
                    continue;
                }
            }

            while (!Pila.empty() && this->precedence(Pila.top()) > this->precedence(exprs[i]))
            { // Mientras el operador en el top sea de mayor precedencia lo escribimos y lo sacamos de la pila
                fixed_expr += '|';
                fixed_expr += Pila.top();
                fixed_expr += '|';
                Pila.pop();
            }

            if (Pila.empty() || this->is_parentheses(Pila.top()) || this->precedence(Pila.top()) < this->precedence(exprs[i]))
            {
                Pila.push(exprs[i]);
            }
            else if (this->precedence(Pila.top()) == this->precedence(exprs[i]))
            {
                if (this->association_order(exprs[i]) > 0)
                { // De izquierda a derecha
                    fixed_expr += '|';
                    fixed_expr += Pila.top();
                    fixed_expr += '|';
                    Pila.pop();
                    Pila.push(exprs[i]);
                }
                else
                {
                    Pila.push(exprs[i]);
                }
            }
        }
        else if (this->is_parentheses(exprs[i]))
        {
            if (exprs[i] == '(')
            { // Si el operador en turno es un parentesis que abre entra en la pila
                Pila.push(exprs[i]);
            }
            else
            { // Si el parentesis cierra sacamos todos los operadores de la pila hasta encontrar el que abre
                while (!Pila.empty() && Pila.top() != '(')
                {
                    fixed_expr += '|';
                    fixed_expr += Pila.top();
                    fixed_expr += '|';
                    Pila.pop();
                }
                Pila.pop(); // Descartamos los parentesis
            }
        }
        else if ((exprs[i] - '0' >= 0 && exprs[i] - '0' <= 9) || exprs[i] == '.')
        {
            if (i == 0)
            {
                fixed_expr += '|';
            }

            fixed_expr += exprs[i];

            if (exprs[i + 1] && (this->is_operator(exprs[i + 1]) || exprs[i + 1] == ')'))
            {
                fixed_expr += '|';
            }
        }
    }

    // std::cout << current_op << "---" << std::endl;

    while (!Pila.empty())
    { // sacamos el resto de operadores de la pila
        fixed_expr += '|';
        fixed_expr += Pila.top();
        fixed_expr += '|';
        Pila.pop();
    }

    return fixed_expr;
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

        if (B < 0)
            throw std::invalid_argument("Negative square root :(!");
        return std::pow(A, B);
    }
    else if (op == '*')
    {
        return A * B;
    }
    if (op == '/')
    {
        if (B == 0)
            throw std::invalid_argument("Zero division :(!");
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
T Function<T>::evaluate_num_expression(std::string expr)
{ // evalution sufix
    // std::cout << ">:v\n";
    int k = 0;
    T result;

    std::stack<std::string> Pila;
    std::string c = "";

    while (k < expr.size())
    {
        // std::cout << "w>:v\n";
        if (expr[k] != '|')
        {
            c += expr[k];
        }
        else
        {
            if (c != "")
            {
                // std::cout << c << std::endl;
                if (c.size() == 1 && this->is_operator(c[0]))
                {
                    try
                    {
                        T A = str_to_value(T, Pila.top());
                        Pila.pop();
                        T B = str_to_value(T, Pila.top());
                        Pila.pop();
                        T curr = this->eval_op(B, c[0], A);
                        Pila.push(std::to_string(curr));

                        // std::cout << B << c[0] << A << std::endl;
                    }
                    catch (const std::exception &e)
                    {
                        std::cerr << e.what() << '\n';
                    }
                }
                else
                {
                    Pila.push(c);
                }
                c = "";
            }
        }

        k++;
    }

    result = std::stold(Pila.top());

    return result;
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

        if ((this->expresion[i] - '0' >= 0 && this->expresion[i] - '0' <= 9) || this->is_operator(this->expresion[i]) || this->is_parentheses(this->expresion[i]) || this->expresion[i] == '.')
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
                    newrep += ("(" + std::to_string(iter) + ")");
                    break;
                }
                k++;
            }
        }
    }

    std::cout << newrep << std::endl;
    newrep = this->sufix_tranform(newrep);

    // std::cout << newrep << std::endl;
    return this->evaluate_num_expression(newrep);
}

template <typename T>
T Function<T>::operator()(std::vector<T> tuple)
{

    std::string newrep = "";
    for (size_t i = 0; i < this->expresion.size(); i++)
    {

        if ((this->expresion[i] - '0' >= 0 && this->expresion[i] - '0' <= 9) || this->is_operator(this->expresion[i]) || this->is_parentheses(this->expresion[i]) || this->expresion[i] == '.')
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

            if (tuple[p])
                newrep += ("(" + std::to_string(tuple[p]) + ")");
        }
    }

    std::cout << newrep << std::endl;
    newrep = this->sufix_tranform(newrep);

    // std::cout << newrep << std::endl;
    return this->evaluate_num_expression(newrep);
}

template <typename T>
T Function<T>::operator()(T *tuple)
{

    std::string newrep = "";
    for (size_t i = 0; i < this->expresion.size(); i++)
    {

        if ((this->expresion[i] - '0' >= 0 && this->expresion[i] - '0' <= 9) || this->is_operator(this->expresion[i]) || this->is_parentheses(this->expresion[i]) || this->expresion[i] == '.')
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

            if (tuple[p])
                newrep += ("(" + std::to_string(tuple[p]) + ")");
        }
    }

    std::cout << newrep << std::endl;
    newrep = this->sufix_tranform(newrep);

    // std::cout << newrep << std::endl;
    return this->evaluate_num_expression(newrep);
}

template <typename T>
template <typename... args>
T Function<T>::operator()(args... ts)
{

    T res[sizeof...(args)] = {ts...};

    std::string newrep = "";
    for (size_t i = 0; i < this->expresion.size(); i++)
    {

        if ((this->expresion[i] - '0' >= 0 && this->expresion[i] - '0' <= 9) || this->is_operator(this->expresion[i]) || this->is_parentheses(this->expresion[i]) || this->expresion[i] == '.')
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

            if (res[p])
                newrep += ("(" + std::to_string(res[p]) + ")");
        }
    }

    std::cout << newrep << std::endl;
    newrep = this->sufix_tranform(newrep);

    // std::cout << newrep << std::endl;
    return this->evaluate_num_expression(newrep);
}

template <typename T>
Function<T>::~Function()
{
}

int main(int argc, char const *argv[])
{
    Function<long double> X("f(x,y,z,w)", "(2*x*y^4+4*y^2*z-2*z^8*w)/(x^4*y^3-1)", {'x', 'y', 'z', 'w'}), f("g(x)", "(1/(x))", {'x'});
    long double t[2] = {5, 10};
    std::cout << str_to_value(int, "5684") << std::endl;

    /*std::stof();
    std::stol();
    std::stoll();
    std::stold();
    std::stod();*/

    double k = 0.01;
    // X({-1, -1.5});
    std::cout << f(1.0654) << std::endl;

    return 0;
}
