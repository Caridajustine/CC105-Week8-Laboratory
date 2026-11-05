
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
#include <limits>
using namespace std;

// ==========================================
// PART 1: TEMPLATE STACK USING DYNAMIC ARRAY
// ==========================================

template <typename T>
class Stack {
private:
    T* arr;
    int capacity;
    int topIndex;

public:
    Stack() {
        capacity = 2;
        arr = new T[capacity];
        topIndex = -1;
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    ~Stack() {
        delete[] arr;
    }

    void push(T value) {
        if (topIndex + 1 == capacity) {
            capacity *= 2;

            T* newArr = new T[capacity];

            for (int i = 0; i <= topIndex; i++) {
                newArr[i] = arr[i];
            }

            delete[] arr;
            arr = newArr;
        }

        arr[++topIndex] = value;
    }

    bool pop(T& value) {
        if (isEmpty()) {
            return false;
        }

        value = arr[topIndex--];
        return true;
    }

    bool peek(T& value) const {
        if (isEmpty()) {
            return false;
        }

        value = arr[topIndex];
        return true;
    }

    bool isEmpty() const {
        return topIndex == -1;
    }

    int size() const {
        return topIndex + 1;
    }
};

// ==========================================
// PART 2: BRACKET CHECKER
// ==========================================

bool isOpening(char c) {
    return c == '(' || c == '[' || c == '{';
}

bool isClosing(char c) {
    return c == ')' || c == ']' || c == '}';
}

bool isMatching(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

bool checkBrackets(const string& expression, string& error) {
    Stack<char> brackets;

    for (char c : expression) {
        if (isOpening(c)) {
            brackets.push(c);
        }
        else if (isClosing(c)) {
            char top;

            if (!brackets.pop(top)) {
                error = "Unmatched closing bracket";
                return false;
            }

            if (!isMatching(top, c)) {
                error = "Mismatched brackets";
                return false;
            }
        }
    }

    if (!brackets.isEmpty()) {
        error = "Unclosed opening bracket";
        return false;
    }

    return true;
}

// ==========================================
// PART 3: INFIX TO POSTFIX CONVERSION
// ==========================================

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op) {
    if (op == '*' || op == '/') {
        return 2;
    }

    if (op == '+' || op == '-') {
        return 1;
    }

    return 0;
}

bool infixToPostfix(
    const string& expression,
    vector<string>& postfix,
    string& error
) {
    Stack<char> operators;
    bool expectOperand = true;
    int i = 0;

    while (i < (int)expression.length()) {
        char c = expression[i];

        if (isspace(static_cast<unsigned char>(c))) {
            i++;
            continue;
        }

        // Read multi-digit integers
        if (isdigit(static_cast<unsigned char>(c))) {
            if (!expectOperand) {
                error = "Missing operator";
                return false;
            }

            string number;

            while (i < (int)expression.length() &&
                   isdigit(static_cast<unsigned char>(expression[i]))) {
                number += expression[i];
                i++;
            }

            postfix.push_back(number);
            expectOperand = false;
            continue;
        }

        // Reject decimal numbers
        if (c == '.') {
            error = "Decimal numbers are not supported";
            return false;
        }

        // Opening brackets
        if (isOpening(c)) {
            if (!expectOperand) {
                error = "Implicit multiplication rejected";
                return false;
            }

            operators.push(c);
            expectOperand = true;
        }

        // Closing brackets
        else if (isClosing(c)) {
            if (expectOperand) {
                error = "Empty group or missing operand";
                return false;
            }

            char top;

            while (operators.peek(top) && !isOpening(top)) {
                operators.pop(top);
                postfix.push_back(string(1, top));
            }

            if (!operators.pop(top)) {
                error = "Mismatched brackets";
                return false;
            }

            expectOperand = false;
        }

        // Arithmetic operators
        else if (isOperator(c)) {
            if (expectOperand) {
                error = "Invalid operator sequence or unary sign";
                return false;
            }

            char top;

            while (operators.peek(top) &&
                   isOperator(top) &&
                   precedence(top) >= precedence(c)) {
                operators.pop(top);
                postfix.push_back(string(1, top));
            }

            operators.push(c);
            expectOperand = true;
        }

        else {
            error = "Unsupported character";
            return false;
        }

        i++;
    }

    if (postfix.empty()) {
        error = "Empty expression";
        return false;
    }

    if (expectOperand) {
        error = "Expression ends with an operator or missing operand";
        return false;
    }

    char top;

    while (operators.pop(top)) {
        if (isOpening(top)) {
            error = "Unclosed opening bracket";
            return false;
        }

        postfix.push_back(string(1, top));
    }

    return true;
}

// ==========================================
// PART 4: POSTFIX EVALUATION
// ==========================================

bool evaluatePostfix(
    const vector<string>& postfix,
    double& result,
    string& error
) {
    Stack<double> values;

    for (const string& token : postfix) {

        // Check if token is a number
        if (!token.empty() &&
            isdigit(static_cast<unsigned char>(token[0]))) {

            try {
                size_t position;
                double number = stod(token, &position);

                if (position != token.length() || !isfinite(number)) {
                    error = "Invalid number";
                    return false;
                }

                values.push(number);
            }
            catch (...) {
                error = "Number conversion error";
                return false;
            }
        }

        // Process operators
        else if (token.length() == 1 &&
                 isOperator(token[0])) {

            double right, left;

            if (!values.pop(right) || !values.pop(left)) {
                error = "Insufficient operands";
                return false;
            }

            double answer = 0;
            char op = token[0];

            if (op == '+') {
                answer = left + right;
            }
            else if (op == '-') {
                answer = left - right;
            }
            else if (op == '*') {
                answer = left * right;
            }
            else if (op == '/') {
                if (right == 0) {
                    error = "Division by zero";
                    return false;
                }

                answer = left / right;
            }

            if (!isfinite(answer)) {
                error = "Result is too large or invalid";
                return false;
            }

            values.push(answer);
        }
        else {
            error = "Invalid postfix token";
            return false;
        }
    }

    if (values.size() != 1) {
        error = "Invalid expression: extra or missing operands";
        return false;
    }

    values.pop(result);
    return true;
}

// ==========================================
// DISPLAY POSTFIX
// ==========================================

void displayPostfix(const vector<string>& postfix) {
    for (const string& token : postfix) {
        cout << token << " ";
    }

    cout << endl;
}

// ==========================================
// MENU OPTION 1: CHECK BRACKETS
// ==========================================

void bracketMenu() {
    string expression;
    string error;

    cout << "\nEnter an expression: ";
    getline(cin, expression);

    if (checkBrackets(expression, error)) {
        cout << "Bracket Status: Balanced" << endl;
    }
    else {
        cout << "Bracket Status: " << error << endl;
    }
}

// ==========================================
// MENU OPTION 2: CONVERT AND EVALUATE
// ==========================================

void expressionMenu() {
    string expression;
    string error;
    vector<string> postfix;
    double result;

    cout << "\nEnter an arithmetic expression: ";
    getline(cin, expression);

    cout << "\nInput: " << expression << endl;

    if (!checkBrackets(expression, error)) {
        cout << "Bracket Status: " << error << endl;
        return;
    }

    cout << "Bracket Status: Balanced" << endl;

    if (!infixToPostfix(expression, postfix, error)) {
        cout << "Error: " << error << endl;
        return;
    }

    cout << "Postfix: ";
    displayPostfix(postfix);

    if (!evaluatePostfix(postfix, result, error)) {
        cout << "Error: " << error << endl;
        return;
    }

    cout << "Result: " << result << endl;
    cout << "Status: Valid expression" << endl;
}

// ==========================================
// MENU OPTION 3: STACK DEMONSTRATION
// ==========================================

void stackDemo() {
    Stack<int> numbers;
    int value;

    cout << "\n===== STACK DEMONSTRATION =====" << endl;

    cout << "Initial empty: "
         << boolalpha << numbers.isEmpty() << endl;

    cout << "Pop empty stack: "
         << numbers.pop(value) << endl;

    cout << "\nPushing 10, 20, and 30..." << endl;

    numbers.push(10);
    numbers.push(20);
    numbers.push(30);

    cout << "Stack size: " << numbers.size() << endl;

    if (numbers.peek(value)) {
        cout << "Top element: " << value << endl;
    }

    cout << "\nPopping elements:" << endl;

    while (numbers.pop(value)) {
        cout << "Popped: " << value << endl;
    }

    cout << "Final empty: " << numbers.isEmpty() << endl;
    cout << "Final size: " << numbers.size() << endl;
}

// ==========================================
// PART 5: MAIN MENU
// ==========================================

int main() {
    int choice;

    while (true) {
        cout << "\n====================================" << endl;
        cout << "       CC105 WEEK 8 LABORATORY" << endl;
        cout << "====================================" << endl;
        cout << "1. Check Brackets" << endl;
        cout << "2. Convert and Evaluate Expression" << endl;
        cout << "3. Stack Demonstration" << endl;
        cout << "0. Exit" << endl;
        cout << "====================================" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "\nInvalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            bracketMenu();
        }
        else if (choice == 2) {
            expressionMenu();
        }
        else if (choice == 3) {
            stackDemo();
        }
        else if (choice == 0) {
            cout << "\nThank you for using the program!" << endl;
            break;
        }
        else {
            cout << "\nInvalid choice. Please select 0, 1, 2, or 3." << endl;
        }
    }

    return 0;
}
