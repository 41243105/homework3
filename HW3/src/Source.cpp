#include <iostream>    
#include <cmath>      
#include <sstream>    
#include <string>      
#include <ctime>       

using namespace std;

// 定義節點類別 用於鏈結串列的每一項
class Node {
public:
    double coef;  // 系數
    int exp;      // 指數
    Node* next;   // 指向下一個節點的指針

    // 節點構造函數 係數為0 指數為0 指向下一個節點為nullptr
    Node(double c = 0, int e = 0, Node* n = nullptr) : coef(c), exp(e), next(n) {}
};

// 定義多項式類別
class Polynomial {
private:
    Node* head; // 指向頭節點的指針 多項式的起始位置

    // 清除多項式中的所有項目
    void clear() {
        if (!head) return; // 如果head為空返回
        Node* current = head->next;  // 從第一個有效節點開始
        while (current != head) {  // current = head就結束 代表走完所有節點 
            Node* temp = current;//temp 保存當前節點 
            current = current->next;//current 指向下一個節點
            delete temp;  // 刪除當前節點
        }
        delete head;  // 刪除頭節點
        head = nullptr; // 重置head
    }

public:
    // 構造函數 初始化頭節點 並指向自己
    Polynomial() {
        head = new Node();
        head->next = head; // 環形結構
    }

    //清除所有節點
    ~Polynomial() {
        clear();
    }

    // 多項式插入和合併 
    void insertTerm(double coef, int exp) {
        if (coef == 0) return;  // 若係數為0返回 
        Node* prev = head;  // 指向前一個節點
        Node* current = head->next;   //指向頭節點的下一個節點

        //  current 的次方大於插入的新項目的次方
        while (current != head && current->exp > exp) {
            prev = current;//prev 指向當前的節點 
            current = current->next;//current 指向下一個節點 
        }

        if (current != head && current->exp == exp) {
            // 如果次方相同就合併
            current->coef += coef;  //係數相加
            if (current->coef == 0) {   // 如果合併後係數為0
                prev->next = current->next;//指向下一項 
                delete current;//刪除此項 
            }
        }
        else {
            // 新節點
            Node* newNode = new Node(coef, exp, current);
            prev->next = newNode;
        }
    }

    // 複製多項式
    Polynomial(const Polynomial& other) {
        head = new Node();//新的頭節點 
        head->next = head;//新頭節點的 next 指向自己 
        Node* current = other.head->next; // current指向原多項式 other 的第一個有效節點
        while (current != other.head) { // 走過原多項式的每個項
            insertTerm(current->coef, current->exp);   // 將當前節點的 coef exp插入新多項式
            current = current->next;//current 移動到原下一個節點繼續處理 
        }
    }

    //檢查多項是是否一致
    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;  // 如果 *this 和 other 是同一個對象 返回當前對象
        clear();  // 清除當前多項式
        head = new Node();//新的頭節點 
        head->next = head;  // 重建頭節點
        Node* current = other.head->next;  // current指向原多項式 other 的第一個有效節點
        while (current != other.head) {// 走過原多項式的每個項
            insertTerm(current->coef, current->exp);  // 插入每一項
            current = current->next;//current 移動到原下一個節點繼續處理 
        }
        return *this;//返回當前對象 
    }



    // 輸入多項式
    friend istream& operator>>(istream& is, Polynomial& poly) {
        string input;
        getline(is, input);  // 讀取整行輸入
        istringstream ss(input);  // 將輸入轉為字串流
        char sign = '+';  // 記錄項目前的符號
        while (ss) {
            double coef = 1.0, exp = 0;  // 係數為1 次方為0
            if (ss.peek() == '+' || ss.peek() == '-') {
                sign = ss.get();  // 讀取符號
            }
            if (isdigit(ss.peek())) {
                ss >> coef;  // 讀取係數
            }
            if (sign == '-') coef = -coef;  // 如果符號是'-'係數加'-' 

            if (ss.peek() == 'x') {  //如果有字元'x'
                ss.get();  // 跳過 'x'
                if (ss.peek() == '^') {//如果有字元'^'
                    ss.get();  // 跳過 '^'
                    ss >> exp;  // 讀取次方
                }
                else {
                    exp = 1;  // 如果有'X'但沒次方 次方設為1
                }
            }
            poly.insertTerm(coef, exp);   // 傳入 insertTerm合併 
        }
        return is;//回傳多項式 
    }

    // 輸出多項式
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        Node* current = poly.head->next;  // 從第一個有效節點開始
        bool first = true;  // 標誌為第一項
        while (current != poly.head) { // 走過所有項
            if (current->coef > 0 && !first) os << " + ";  // 如果係數大於0且不是第一項加上 '+'
            if (current->coef < 0) os << (first ? "-" : " - ");  // 如果係數小於0 就看有沒有'-' 如果true輸出'-' 不是的話輸出' - '
            os << abs(current->coef); // 輸出係數加絕對值 
            if (current->exp > 0) os << "x";   // 如果次方大於0輸出 'x'
            if (current->exp > 1) os << "^" << current->exp;  // 如果次方大於1 輸出'^'再輸入次方
            current = current->next;  // 移動到下一項
            first = false;  // 設置first為false
        }
        if (first) os << "0";  /// 如果多項式為空輸出 0
        return os;//回傳多項式 
    }

    // 多項式加法
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;  // 儲存結果
        Node* a = head->next;
        Node* b = other.head->next;
        while (a != head || b != other.head) {  
            if (a != head && (b == other.head || a->exp > b->exp)) {//如果當前 a 中的項次方大於 b 中的項次方
                result.insertTerm(a->coef, a->exp);  //直接將 a 中的係數和次方加到 result 中
                a = a->next;// a 移到下一項 
            }
            else if (b != other.head && (a == head || a->exp < b->exp)) {//如果b中的項次方大於 a 中的項次方
                result.insertTerm(b->coef, b->exp); //直接將 b 中的係數和次方加到 result 中
                b = b->next;// b 移到下一項 
            }
            else {
                result.insertTerm(a->coef + b->coef, a->exp);  // 合併相同次方的項
                a = a->next;// a 移到下一項 
                b = b->next;// b 移到下一項 
            }
        }
        return result;//回傳結果 
    }

    // 多項式減法
    Polynomial operator-(const Polynomial& other) const {
        Polynomial result;// 儲存結果
        Node* a = head->next;
        Node* b = other.head->next;
        while (a != head || b != other.head) {
            if (a != head && (b == other.head || a->exp > b->exp)) {//如果當前 a 中的項次方大於 b 中的項次方
                result.insertTerm(a->coef, a->exp);//直接將 a 中的係數和次方加到 result 中  
                a = a->next;// a 移到下一項 
            }
            else if (b != other.head && (a == head || a->exp < b->exp)) {//如果b中的項次方大於 a 中的項次方 
                result.insertTerm(-b->coef, b->exp); //  b 中的係數取負和次方加到 result 中  係數需要取負因為是在減去 b 中的項 
                b = b->next;// b 移到下一項 
            }
            else {//如果 a 和 b 中的次方相同  a和b中相同次方的項要係數相減 結果加到 result 中
                result.insertTerm(a->coef - b->coef, a->exp);
                a = a->next;// a 移到下一項 
                b = b->next;// b 移到下一項 
            }
        }
        return result;//回傳結果 
    }

    // 多項式乘法
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;// 儲存結果
        Node* a = head->next;
        while (a != head) {  // 走過a的每一項
            Node* b = other.head->next;
            while (b != other.head) {  // 走過b的每一項
                result.insertTerm(a->coef * b->coef, a->exp + b->exp);  // 係數相乘 指數相加 結果加到 result 中 
                b = b->next;// b 移到下一項 
            }
            a = a->next;// a 移到下一項 
        }
        return result;//回傳結果 
    }

    // 計算多項式的值 帶入輸入的x值
    double evaluate(int x) const {
        double result = 0.0f;// 儲存結果
        Node* current = head->next;
        while (current != head) {  // 走過每一項
            result += current->coef * pow(x, current->exp); // 根據x的值計算每項結果再累加
            current = current->next;//移動到下一項繼續計算 
        }
        return result;//回傳結果 
    }
};

int main() {
    clock_t start, finish; // 用於計時的變數
    Polynomial a, b;  // 宣告兩個多項式

    cout << "a=";
    cin >> a;  // 輸入多項式a

    cout << "b=";
    cin >> b;  // 輸入多項式b

    cout << "------------------------------------------------------------------------------------------" << endl;

    // 加法操作並計算時間
    start = clock();
    Polynomial sum = a + b;
    finish = clock();
    cout << "a + b = " << sum << endl;
    cout << "sum() 需時: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    // 減法操作並計算時間
    start = clock();
    Polynomial diff = a - b;
    finish = clock();
    cout << "a - b = " << diff << endl;
    cout << "diff() 需時: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    // 乘法操作並計算時間
    start = clock();
    Polynomial prod = a * b;
    finish = clock();
    cout << "a * b = " << prod << endl;
    cout << "prod() 需時: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    cout << "------------------------------------------------------------------------------------------" << endl;

    int x;
    cout << "輸入 x 的值來計算多項式 a(x)、b(x)：";
    cin >> x;

    // 計算a(x)並計算時間
    start = clock();
    cout << "a(" << x << ") = " << a.evaluate(x) << endl;
    finish = clock();
    cout << "a(x) 需時: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    // 計算b(x)並計算時間
    start = clock();
    cout << "b(" << x << ") = " << b.evaluate(x) << endl;
    finish = clock();
    cout << "b(x) 需時: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    return 0;
}
/*
5x^9-3x^8+2x^7+x^6-4x^5+6x^4-8x^3+7x^2-9x+10
-2x^10+4x^9-6x^8+8x^7-10x^6+12x^5-14x^4+16x^3-18x^2+20x-22
2
*/

