#include <iostream>
#include <stack>
#include <cctype>
#include <string>
#include <stdexcept>
using namespace std;

int evaluate(int op1, int op2, char op) {
    switch (op) {
        case '+': return op1 + op2;
        case '-': return op1 - op2;
        case '*': return op1 * op2;
        case '/':
            if (op2 == 0) {
                throw runtime_error("Division by zero");
            }
            return op1 / op2;
        default: throw invalid_argument("Invalid operator");
    }
}

int priority(char x) {
    if (x == '(') return 0;
    if (x == '+' || x == '-') return 1;
    if (x == '*' || x == '/') return 2;
    return -1;
}

int evaluateExpression(const string &exp) {
    stack<int> values;
    stack<char> ops;

    for (size_t i = 0; i < exp.length(); i++) {
        if (isspace(exp[i])) {
            continue;
        }
        if (isdigit(exp[i])) {
            int num = 0;
            while (i < exp.length() && isdigit(exp[i])) {
                num = num * 10 + (exp[i] - '0');
                i++;
            }
            i--;
            values.push(num);
        } else if (exp[i] == '(') {
            ops.push(exp[i]);
        } else if (exp[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                int op2 = values.top(); values.pop();
                int op1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(evaluate(op1, op2, op));
            }
            if (ops.empty() || ops.top() != '(') {
                throw runtime_error("Mismatched parentheses");
            }
            ops.pop();
        } else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
            while (!ops.empty() && priority(ops.top()) >= priority(exp[i])) {
                int op2 = values.top(); values.pop();
                int op1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(evaluate(op1, op2, op));
            }
            ops.push(exp[i]);
        } else {
            throw invalid_argument("Invalid character in expression");
        }
    }

    while (!ops.empty()) {
        if (ops.top() == '(') {
            throw runtime_error("Mismatched parentheses");
        }
        int op2 = values.top(); values.pop();
        int op1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(evaluate(op1, op2, op));
    }

    if (values.size() != 1) {
        throw runtime_error("Error in expression evaluation");
    }
    return values.top();
}

int main() {
    string exp;
    cout << "Enter the expression: ";
    getline(cin, exp);
    try {
        cout << "The result is: " << evaluateExpression(exp) << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}
