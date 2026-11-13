#include <algorithm>
#include <array>
#include <climits>
#include <cstdlib>
#include <cstdint>
#include <deque>
#include <iostream>
#include <limits>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>


using namespace std;

bool isValid(const string& s) {
    stack<char> opened;
    for (char ch : s) {
        if (ch == '(' || ch == '[' || ch == '{') {
            opened.push(ch);
        }
        else {
            if (opened.empty()) return false;
            char prev = opened.top();
            opened.pop();
            if ((ch == ')' && prev != '(') ||
                (ch == ']' && prev != '[') ||
                (ch == '}' && prev != '{')) return false;
        }
    }
    return opened.empty();
}

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    explicit TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

vector<int> inorderTraversal(TreeNode* root) {
    vector<int> order;
    stack<TreeNode*> nodes;
    TreeNode* current = root;
    while (current != nullptr || !nodes.empty()) {
        while (current != nullptr) {
            nodes.push(current);
            current = current->left;
        }
        current = nodes.top();
        nodes.pop();
        order.push_back(current->val);
        current = current->right;
    }
    return order;
}

void deleteTree(TreeNode* root) {
    if (!root) return;
    stack<TreeNode*> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        TreeNode* node = nodes.top();
        nodes.pop();
        if (node->left) nodes.push(node->left);
        if (node->right) nodes.push(node->right);
        delete node;
    }
}

class MinStack {
    stack<pair<int, int>> data; 
public:
    void push(int val) {
        int minimum = data.empty() ? val : min(val, data.top().second);
        data.push({ val, minimum });
    }
    void pop() { data.pop(); }     
    int top() const { return data.top().first; }
    int getMin() const { return data.top().second; }
    bool empty() const { return data.empty(); }
    size_t size() const { return data.size(); }
};

class MyQueue {
    stack<int> input, output;
    void moveIfNeeded() {
        if (output.empty()) {
            while (!input.empty()) {
                output.push(input.top());
                input.pop();
            }
        }
    }
public:
    void push(int x) { input.push(x); }
    int pop() {
        moveIfNeeded();
        int x = output.top(); output.pop();
        return x;
    }
    int peek() {
        moveIfNeeded();
        return output.top();
    }
    bool empty() const { return input.empty() && output.empty(); }
};

string decodeString(const string& s) {
    stack<pair<string, int>> frames;
    string current;
    int count = 0;
    constexpr size_t MAX_OUTPUT = 100000;
    for (char ch : s) {
        if (ch >= '0' && ch <= '9') {
            if (count > 1000) throw invalid_argument("Завелике число повторень.");
            count = count * 10 + (ch - '0');
        }
        else if (ch == '[') {
            if (count < 1 || count > 300) throw invalid_argument("Кількість повторень має бути від 1 до 300.");
            frames.push({ move(current), count });
            current.clear();
            count = 0;
        }
        else if (ch == ']') {
            if (frames.empty()) throw invalid_argument("Невідповідні квадратні дужки.");
            pair<string, int> frame = move(frames.top());
            string prefix = move(frame.first);
            int repeats = frame.second;
            frames.pop();
            if (current.size() > MAX_OUTPUT / static_cast<size_t>(repeats) ||
                prefix.size() > MAX_OUTPUT - current.size() * static_cast<size_t>(repeats)) {
                throw length_error("Розкодований рядок перевищує 100000 символів.");
            }
            string decoded = move(prefix);
            decoded.reserve(decoded.size() + current.size() * repeats);
            for (int i = 0; i < repeats; ++i) decoded += current;
            current = move(decoded);
        }
        else if (ch >= 'a' && ch <= 'z') {
            current.push_back(ch);
            if (current.size() > MAX_OUTPUT) throw length_error("Занадто довгий рядок.");
        }
        else {
            throw invalid_argument("Дозволені лише a-z, цифри та квадратні дужки.");
        }
    }
    if (!frames.empty() || count != 0) throw invalid_argument("Некоректне кодування.");
    return current;
}

int evalRPN(const vector<string>& tokens) {
    stack<long long> nums;
    for (const string& t : tokens) {
        if (t.size() == 1 && (t == "+" || t == "-" || t == "*" || t == "/")) {
            if (nums.size() < 2) throw invalid_argument("Неправильна кількість операндів.");
            long long b = nums.top(); nums.pop();
            long long a = nums.top(); nums.pop();
            long long result = 0;
            if (t == "+") result = a + b;
            if (t == "-") result = a - b;
            if (t == "*") result = a * b;
            if (t == "/") {
                if (b == 0) throw invalid_argument("Ділення на нуль.");
                result = a / b; 
            }
            if (result < INT_MIN || result > INT_MAX) throw out_of_range("Результат поза межами int.");
            nums.push(result);
        }
        else {
            size_t parsed = 0;
            long long value;
            try { value = stoll(t, &parsed); }
            catch (...) { throw invalid_argument("Некоректний токен: " + t); }
            if (parsed != t.size() || value < INT_MIN || value > INT_MAX)
                throw invalid_argument("Некоректне число: " + t);
            nums.push(value);
        }
    }
    if (nums.size() != 1) throw invalid_argument("Вираз не є коректним.");
    return static_cast<int>(nums.top());
}

int longestValidParentheses(const string& s) {
    stack<int> indexes;
    indexes.push(-1);
    int best = 0;
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        if (s[i] == '(') indexes.push(i);
        else {
            indexes.pop();
            if (indexes.empty()) indexes.push(i);
            else best = max(best, i - indexes.top());
        }
    }
    return best;
}

long long readNumber(const string& prompt, long long lower, long long upper) {
    while (true) {
        cout << prompt;
        long long value;
        if (cin >> value && value >= lower && value <= upper) return value;
        if (cin.eof()) exit(0);
        cout << "Помилка: введіть ціле число від " << lower << " до " << upper << ".\n";
        cin.clear();
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    }
}

int readInt(const string& prompt, int lower, int upper) {
    return static_cast<int>(readNumber(prompt, lower, upper));
}

string readToken(const string& prompt) {
    cout << prompt;
    string s;
    if (!(cin >> s)) exit(0);
    return s;
}

string readChars(const string& prompt, const string& allowed, int minLength, int maxLength,
    bool allowEmptyAsDash = false) {
    while (true) {
        string s = readToken(prompt);
        if (allowEmptyAsDash && s == "-") s.clear();
        if (s.size() >= static_cast<size_t>(minLength) &&
            s.size() <= static_cast<size_t>(maxLength) &&
            s.find_first_not_of(allowed) == string::npos) return s;
        cout << "Некоректний рядок. Довжина " << minLength << ".." << maxLength
            << ", дозволені символи: " << allowed << ".\n";
    }
}

vector<int> readVector(const string& lengthPrompt, int minLength, int maxLength,
    int minValue, int maxValue) {
    int n = readInt(lengthPrompt, minLength, maxLength);
    vector<int> values(n);
    cout << "Введіть " << n << " цілих чисел (від " << minValue << " до " << maxValue
        << ") через пробіл або Enter:\n";
    for (int i = 0; i < n; ++i) {
        string prompt = n <= 25 ? "Елемент " + to_string(i) + ": " : "";
        values[i] = readInt(prompt, minValue, maxValue);
    }
    return values;
}

void printVector(const vector<int>& a) {
    cout << "[";
    for (size_t i = 0; i < a.size(); ++i) {
        if (i) cout << ", ";
        cout << a[i];
    }
    cout << "]";
}

TreeNode* readTree() {
    cout << "Формат рівнів дерева: [1, null, 2, 3] означає, що у 1 немає лівої дитини,\n"
        "права дитина дорівнює 2, а у 2 ліва дитина дорівнює 3.\n";
    int count = readInt("Кількість токенів у представленні дерева (0..201): ", 0, 201);
    if (count == 0) return nullptr;
    vector<string> tokens(count);
    for (int i = 0; i < count; ++i) {
        while (true) {
            string s = readToken("Токен " + to_string(i) + " (число від -100 до 100 або null): ");
            if (s == "null") { tokens[i] = s; break; }
            try {
                size_t pos = 0;
                int x = stoi(s, &pos);
                if (pos == s.size() && x >= -100 && x <= 100) { tokens[i] = s; break; }
            }
            catch (...) {}
            cout << "Некоректний токен.\n";
        }
    }
    if (tokens[0] == "null") {
        cout << "Порожнє дерево.\n";
        return nullptr;
    }
    TreeNode* root = new TreeNode(stoi(tokens[0]));
    queue<TreeNode*> nodes;
    nodes.push(root);
    int i = 1;
    int actualNodes = 1;
    while (!nodes.empty() && i < count) {
        TreeNode* parent = nodes.front();
        nodes.pop();
        if (i < count && tokens[i] != "null") {
            parent->left = new TreeNode(stoi(tokens[i]));
            nodes.push(parent->left);
            ++actualNodes;
        }
        ++i;
        if (i < count && tokens[i] != "null") {
            parent->right = new TreeNode(stoi(tokens[i]));
            nodes.push(parent->right);
            ++actualNodes;
        }
        ++i;
    }
    if (actualNodes > 100) {
        cout << "Дозволено не більше 100 вузлів.\n";
        deleteTree(root);
        return nullptr;
    }
    return root;
}

void runMinStack() {
    MinStack st;
    while (true) {
        cout << "\nМінімальний стек: 1-push, 2-pop, 3-top, 4-getMin, 5-розмір, 0-назад\n";
        int action = readInt("Операція: ", 0, 5);
        if (action == 0) return;
        if (action == 1) {
            int value = readInt("Значення: ", INT_MIN, INT_MAX);
            st.push(value);
            cout << "Значення додано.\n";
        }
        else if (action == 5) {
            cout << "Кількість елементів: " << st.size() << "\n";
        }
        else if (st.empty()) {
            cout << "Стек порожній.\n";
        }
        else if (action == 2) {
            st.pop();
            cout << "Верхній елемент видалено.\n";
        }
        else if (action == 3) cout << "Верхній елемент: " << st.top() << "\n";
        else cout << "Мінімальний елемент: " << st.getMin() << "\n";
    }
}

void runQueueFromStacks() {
    MyQueue q;
    while (true) {
        cout << "\nЧерга з двох стеків: 1-push, 2-pop, 3-peek, 4-empty, 0-назад\n";
        int action = readInt("Операція: ", 0, 4);
        if (action == 0) return;
        if (action == 1) {
            q.push(readInt("Число (1..9): ", 1, 9));
            cout << "Додано до кінця черги.\n";
        }
        else if (action == 4) cout << "Черга порожня: " << boolalpha << q.empty() << "\n";
        else if (q.empty()) cout << "Черга порожня.\n";
        else if (action == 2) cout << "Вилучено: " << q.pop() << "\n";
        else cout << "На початку: " << q.peek() << "\n";
    }
}

void stackMenu() {
    while (true) {
        cout << "\nАналітичне завдання 4 - Стек\n"
            "1. Валідні дужки\n"
            "2. Inorder обхід бінарного дерева\n"
            "3. Мінімальний стек (MinStack)\n"
            "4. Реалізація черги двома стеками\n"
            "5. Декодування рядка\n"
            "6. Зворотна польська нотація\n"
            "7. Найдовший правильний підрядок дужок\n"
            "0. Назад\n";
        int choice = readInt("Обери завдання: ", 0, 7);
        if (choice == 0) return;
        try {
            if (choice == 1) {
                string s = readChars("Введіть дужки: ", "()[]{}", 1, 10000);
                cout << "Дужки правильні: " << boolalpha << isValid(s) << "\n";
            }
            else if (choice == 2) {
                TreeNode* root = readTree();
                cout << "Обхід inorder: ";
                printVector(inorderTraversal(root));
                cout << "\n";
                deleteTree(root);
            }
            else if (choice == 3) runMinStack();
            else if (choice == 4) runQueueFromStacks();
            else if (choice == 5) {
                string s = readChars("Закодований рядок: ", "abcdefghijklmnopqrstuvwxyz0123456789[]", 1, 30);
                cout << "Результат: " << decodeString(s) << "\n";
            }
            else if (choice == 6) {
                int n = readInt("Кількість токенів (1..10000): ", 1, 10000);
                vector<string> tokens(n);
                cout << "Введіть токени через пробіл, наприклад: 2 1 + 3 *\n";
                for (int i = 0; i < n; ++i)
                    tokens[i] = readToken(n <= 25 ? "Токен " + to_string(i) + ": " : "");
                cout << "Значення виразу: " << evalRPN(tokens) << "\n";
            }
            else {
                string s = readChars("Введіть '(' і ')' (для порожнього рядка введіть -): ",
                    "()", 0, 30000, true);
                cout << "Довжина найдовшого правильного підрядка: "
                    << longestValidParentheses(s) << "\n";
            }
        }
        catch (const exception& e) {
            cout << "Помилка: " << e.what() << "\n";
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << boolalpha;
    stackMenu();
    return 0;
}