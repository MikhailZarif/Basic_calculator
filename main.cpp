#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>

int main() {
    int error_flag = 0;
    double answer;
    std::stack<char> incoming;
    std::string formula, postfix_formula;
    std::vector<char> operations{'+', '-', '*', '/', '(', ')'};
    std::vector<char> numbers{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    std::vector<std::string> special_symbols{" ", "PI", "pi", "E", "e"};//Допустимые символы
    std::map<char, int> priority;
    priority['+'] = 2;
    priority['-'] = 2;
    priority['*'] = 1;
    priority['/'] = 1;
    std::cout << "Welcome to Basic Console Calculator!" << std::endl;
    std::cout
            << "Here You can divide, multiply, sum and subtract."
            << "This calculator can operate with any type of real numbers," << std::endl
            << "including constants PI and E."
            << std::endl;
    std::cout << "Here are some tips for You:" << std::endl;
    std::cout << " - Use brackets for unary minus expressions: " << "Example '-(3+4*PI)'" << std::endl;
    std::cout << " - Don't commit math errors: 3/0" << std::endl;
    std::cout << " - Numbers PI and E are typed in as 'PI' or 'pi' and 'E' or 'e'" << std::endl;
    std::cout << "Enjoy!" << std::endl;
    std::cout << "Enter formula" << std::endl;
    getline(std::cin, formula);
    std::string::iterator it;
    for (it = formula.begin(); it != formula.end() - 1; ++it) {
        if ((std::find(operations.begin(), operations.end(), *it) == operations.end()) &&
            (std::find(numbers.begin(), numbers.end(), *it) == numbers.end())) {
            std::string a = {}, b = {};
            b.push_back(*it);
            a.push_back(*it);
            a.push_back(*(++it));
            if ((std::find(special_symbols.begin(), special_symbols.end(), a) == special_symbols.end()) &&
                (std::find(special_symbols.begin(), special_symbols.end(), b) == special_symbols.end())) {
                std::cerr << "Some symbols are incorrect!" << std::endl << "Program collapsed" << std::endl;
                error_flag = 1;
            }
        }
    }//Проверка на правильность символов
    for (it = formula.begin(); it != formula.end(); ++it) {
        if (*it == ' ') {
            formula.replace(it - formula.begin(), 1, "");
        }
    }//Удаление пробелов
    for (it = formula.begin(); it != formula.end() - 1; ++it) {
        if ((*it == '-') && (*(++it) == '(')) {
            --it;
            formula.insert(it, '+');
            *(++it) = '(';
            *(++it) = '0';
            formula.insert((++it) - formula.begin(), "-(");
            it = std::find(it, formula.end(), ')');
            formula.insert(it, ')');
        }
    }//Приведение унарного минуса
    it = formula.begin();
    if (error_flag == 0) {
        while (it != formula.end()) {
            if (find(operations.begin(), operations.end(), *it) != operations.end()) {
                postfix_formula.push_back(' ');
                if (*it == '(') {
                    incoming.push(*it);
                }
                if (*it == ')') {
                    while (incoming.top() != '(') {
                        postfix_formula.push_back(incoming.top());
                        incoming.pop();
                    }
                    incoming.pop();
                }
                if ((*it == '+') || (*it == '-') || (*it == '*') || (*it == '/')) {
                    int pr = priority[*it];
                    if (pr == 2) {
                        if (!incoming.empty()) {
                            while (incoming.top() != '(') {
                                postfix_formula.push_back(incoming.top());
                                incoming.pop();
                                if (incoming.empty())
                                    break;
                            }
                        }
                        incoming.push(*it);
                    } else {
                        if (!incoming.empty()) {
                            while ((incoming.top() != '(') && (priority[incoming.top()] == 1)) {
                                postfix_formula.push_back(incoming.top());
                                incoming.pop();
                                if (incoming.empty())
                                    break;
                            }
                        }
                        incoming.push(*it);
                    }
                }
            } else {
                if (*it != ' ') {
                    postfix_formula.push_back(*it);
                }
            }
            ++it;
        }
        int n = incoming.size();
        for (int i = 0; i < n; i++) {
            postfix_formula.push_back(incoming.top());
            incoming.pop();
        } //Алгоритм Дейкстры, на выходе postfix_formula
        it = postfix_formula.begin();
        std::stack<double> output;
        while (it != postfix_formula.end()) {
            if (find(operations.begin(), operations.end(), *it) != operations.end()) {
                double operand_a, operand_b, result;
                operand_a = output.top();
                output.pop();
                operand_b = output.top();
                output.pop();
                switch (*it) {
                    case '+' :
                        result = operand_a + operand_b;
                        break;
                    case '-' :
                        result = operand_b - operand_a;
                        break;
                    case '*' :
                        result = operand_a * operand_b;
                        break;
                    case '/' :
                        if (operand_a == 0) {
                            std::cerr << "You cannot divide by 0" << std::endl << "Program collapsed"
                                      << std::endl;
                            error_flag = 1;
                            break;
                        }
                        result = operand_b / operand_a;
                }
                output.push(result);
                ++it;
            } else {
                double op = 0;
                int razryad;
                while ((find(operations.begin(), operations.end(), *it) == operations.end()) && (*it != ' ')) {
                    if ((*it != '.') && (*it != 'P') && (*it != 'p') && (*it != 'e') && (*it != 'E')) {
                        while ((find(operations.begin(), operations.end(), *it) == operations.end()) && (*it != ' ') &&
                               (*it != '.')) {
                            razryad = *it - 48;
                            op = 10 * op + razryad;
                            ++it;
                        }
                    }
                    if (*it == '.') {
                        ++it;
                        int k = -1;
                        while ((find(operations.begin(), operations.end(), *it) == operations.end()) && (*it != ' ')) {
                            razryad = *it - 48;
                            op += pow(10.0, k) * razryad;
                            k -= 1;
                            ++it;
                        }
                    }
                    if ((*it == 'p') || (*it == 'P')) {
                        op = M_PI;
                        ++it;
                        ++it;
                    }
                    if ((*it == 'e') || (*it == 'E')) {
                        op = M_E;
                        ++it;
                    }
                    output.push(op);
                }
                if (*it == ' ') {
                    ++it;
                }
            }
        }//вычисляем значение выражения в постфиксной форме
        answer = output.top();
    }//Калькуляция
    if (error_flag == 0) {
        std::cout << "Please, set precision for your answer (how many symbols do you need)" << std::endl;
        int n;
        std::cin >> n;
        std::cout << "Answer is " << std::setprecision(n) << answer << std::endl << "Thank you!";
    }
}
