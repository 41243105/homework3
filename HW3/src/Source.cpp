#include <iostream>    
#include <cmath>      
#include <sstream>    
#include <string>      
#include <ctime>       

using namespace std;

// �w�q�`�I���O �Ω��쵲��C���C�@��
class Node {
public:
    double coef;  // �t��
    int exp;      // ����
    Node* next;   // ���V�U�@�Ӹ`�I�����w

    // �`�I�c�y��� �Y�Ƭ�0 ���Ƭ�0 ���V�U�@�Ӹ`�I��nullptr
    Node(double c = 0, int e = 0, Node* n = nullptr) : coef(c), exp(e), next(n) {}
};

// �w�q�h�������O
class Polynomial {
private:
    Node* head; // ���V�Y�`�I�����w �h�������_�l��m

    // �M���h���������Ҧ�����
    void clear() {
        if (!head) return; // �p�Ghead���Ū�^
        Node* current = head->next;  // �q�Ĥ@�Ӧ��ĸ`�I�}�l
        while (current != head) {  // current = head�N���� �N�����Ҧ��`�I 
            Node* temp = current;//temp �O�s��e�`�I 
            current = current->next;//current ���V�U�@�Ӹ`�I
            delete temp;  // �R����e�`�I
        }
        delete head;  // �R���Y�`�I
        head = nullptr; // ���mhead
    }

public:
    // �c�y��� ��l���Y�`�I �ë��V�ۤv
    Polynomial() {
        head = new Node();
        head->next = head; // ���ε��c
    }

    //�M���Ҧ��`�I
    ~Polynomial() {
        clear();
    }

    // �h�������J�M�X�� 
    void insertTerm(double coef, int exp) {
        if (coef == 0) return;  // �Y�Y�Ƭ�0��^ 
        Node* prev = head;  // ���V�e�@�Ӹ`�I
        Node* current = head->next;   //���V�Y�`�I���U�@�Ӹ`�I

        //  current ������j�󴡤J���s���ت�����
        while (current != head && current->exp > exp) {
            prev = current;//prev ���V��e���`�I 
            current = current->next;//current ���V�U�@�Ӹ`�I 
        }

        if (current != head && current->exp == exp) {
            // �p�G����ۦP�N�X��
            current->coef += coef;  //�Y�Ƭۥ[
            if (current->coef == 0) {   // �p�G�X�֫�Y�Ƭ�0
                prev->next = current->next;//���V�U�@�� 
                delete current;//�R������ 
            }
        }
        else {
            // �s�`�I
            Node* newNode = new Node(coef, exp, current);
            prev->next = newNode;
        }
    }

    // �ƻs�h����
    Polynomial(const Polynomial& other) {
        head = new Node();//�s���Y�`�I 
        head->next = head;//�s�Y�`�I�� next ���V�ۤv 
        Node* current = other.head->next; // current���V��h���� other ���Ĥ@�Ӧ��ĸ`�I
        while (current != other.head) { // ���L��h�������C�Ӷ�
            insertTerm(current->coef, current->exp);   // �N��e�`�I�� coef exp���J�s�h����
            current = current->next;//current ���ʨ��U�@�Ӹ`�I�~��B�z 
        }
    }

    //�ˬd�h���O�O�_�@�P
    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;  // �p�G *this �M other �O�P�@�ӹ�H ��^��e��H
        clear();  // �M����e�h����
        head = new Node();//�s���Y�`�I 
        head->next = head;  // �����Y�`�I
        Node* current = other.head->next;  // current���V��h���� other ���Ĥ@�Ӧ��ĸ`�I
        while (current != other.head) {// ���L��h�������C�Ӷ�
            insertTerm(current->coef, current->exp);  // ���J�C�@��
            current = current->next;//current ���ʨ��U�@�Ӹ`�I�~��B�z 
        }
        return *this;//��^��e��H 
    }



    // ��J�h����
    friend istream& operator>>(istream& is, Polynomial& poly) {
        string input;
        getline(is, input);  // Ū������J
        istringstream ss(input);  // �N��J�ର�r��y
        char sign = '+';  // �O�����ثe���Ÿ�
        while (ss) {
            double coef = 1.0, exp = 0;  // �Y�Ƭ�1 ���謰0
            if (ss.peek() == '+' || ss.peek() == '-') {
                sign = ss.get();  // Ū���Ÿ�
            }
            if (isdigit(ss.peek())) {
                ss >> coef;  // Ū���Y��
            }
            if (sign == '-') coef = -coef;  // �p�G�Ÿ��O'-'�Y�ƥ['-' 

            if (ss.peek() == 'x') {  //�p�G���r��'x'
                ss.get();  // ���L 'x'
                if (ss.peek() == '^') {//�p�G���r��'^'
                    ss.get();  // ���L '^'
                    ss >> exp;  // Ū������
                }
                else {
                    exp = 1;  // �p�G��'X'���S���� ����]��1
                }
            }
            poly.insertTerm(coef, exp);   // �ǤJ insertTerm�X�� 
        }
        return is;//�^�Ǧh���� 
    }

    // ��X�h����
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        Node* current = poly.head->next;  // �q�Ĥ@�Ӧ��ĸ`�I�}�l
        bool first = true;  // �лx���Ĥ@��
        while (current != poly.head) { // ���L�Ҧ���
            if (current->coef > 0 && !first) os << " + ";  // �p�G�Y�Ƥj��0�B���O�Ĥ@���[�W '+'
            if (current->coef < 0) os << (first ? "-" : " - ");  // �p�G�Y�Ƥp��0 �N�ݦ��S��'-' �p�Gtrue��X'-' ���O���ܿ�X' - '
            os << abs(current->coef); // ��X�Y�ƥ[����� 
            if (current->exp > 0) os << "x";   // �p�G����j��0��X 'x'
            if (current->exp > 1) os << "^" << current->exp;  // �p�G����j��1 ��X'^'�A��J����
            current = current->next;  // ���ʨ�U�@��
            first = false;  // �]�mfirst��false
        }
        if (first) os << "0";  /// �p�G�h�������ſ�X 0
        return os;//�^�Ǧh���� 
    }

    // �h�����[�k
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;  // �x�s���G
        Node* a = head->next;
        Node* b = other.head->next;
        while (a != head || b != other.head) {  
            if (a != head && (b == other.head || a->exp > b->exp)) {//�p�G��e a ����������j�� b ����������
                result.insertTerm(a->coef, a->exp);  //�����N a �����Y�ƩM����[�� result ��
                a = a->next;// a ����U�@�� 
            }
            else if (b != other.head && (a == head || a->exp < b->exp)) {//�p�Gb����������j�� a ����������
                result.insertTerm(b->coef, b->exp); //�����N b �����Y�ƩM����[�� result ��
                b = b->next;// b ����U�@�� 
            }
            else {
                result.insertTerm(a->coef + b->coef, a->exp);  // �X�֬ۦP���誺��
                a = a->next;// a ����U�@�� 
                b = b->next;// b ����U�@�� 
            }
        }
        return result;//�^�ǵ��G 
    }

    // �h������k
    Polynomial operator-(const Polynomial& other) const {
        Polynomial result;// �x�s���G
        Node* a = head->next;
        Node* b = other.head->next;
        while (a != head || b != other.head) {
            if (a != head && (b == other.head || a->exp > b->exp)) {//�p�G��e a ����������j�� b ����������
                result.insertTerm(a->coef, a->exp);//�����N a �����Y�ƩM����[�� result ��  
                a = a->next;// a ����U�@�� 
            }
            else if (b != other.head && (a == head || a->exp < b->exp)) {//�p�Gb����������j�� a ���������� 
                result.insertTerm(-b->coef, b->exp); //  b �����Y�ƨ��t�M����[�� result ��  �Y�ƻݭn���t�]���O�b��h b ������ 
                b = b->next;// b ����U�@�� 
            }
            else {//�p�G a �M b ��������ۦP  a�Mb���ۦP���誺���n�Y�Ƭ۴� ���G�[�� result ��
                result.insertTerm(a->coef - b->coef, a->exp);
                a = a->next;// a ����U�@�� 
                b = b->next;// b ����U�@�� 
            }
        }
        return result;//�^�ǵ��G 
    }

    // �h�������k
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;// �x�s���G
        Node* a = head->next;
        while (a != head) {  // ���La���C�@��
            Node* b = other.head->next;
            while (b != other.head) {  // ���Lb���C�@��
                result.insertTerm(a->coef * b->coef, a->exp + b->exp);  // �Y�Ƭۭ� ���Ƭۥ[ ���G�[�� result �� 
                b = b->next;// b ����U�@�� 
            }
            a = a->next;// a ����U�@�� 
        }
        return result;//�^�ǵ��G 
    }

    // �p��h�������� �a�J��J��x��
    double evaluate(int x) const {
        double result = 0.0f;// �x�s���G
        Node* current = head->next;
        while (current != head) {  // ���L�C�@��
            result += current->coef * pow(x, current->exp); // �ھ�x���ȭp��C�����G�A�֥[
            current = current->next;//���ʨ�U�@���~��p�� 
        }
        return result;//�^�ǵ��G 
    }
};

int main() {
    clock_t start, finish; // �Ω�p�ɪ��ܼ�
    Polynomial a, b;  // �ŧi��Ӧh����

    cout << "a=";
    cin >> a;  // ��J�h����a

    cout << "b=";
    cin >> b;  // ��J�h����b

    cout << "------------------------------------------------------------------------------------------" << endl;

    // �[�k�ާ@�íp��ɶ�
    start = clock();
    Polynomial sum = a + b;
    finish = clock();
    cout << "a + b = " << sum << endl;
    cout << "sum() �ݮ�: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    // ��k�ާ@�íp��ɶ�
    start = clock();
    Polynomial diff = a - b;
    finish = clock();
    cout << "a - b = " << diff << endl;
    cout << "diff() �ݮ�: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    // ���k�ާ@�íp��ɶ�
    start = clock();
    Polynomial prod = a * b;
    finish = clock();
    cout << "a * b = " << prod << endl;
    cout << "prod() �ݮ�: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    cout << "------------------------------------------------------------------------------------------" << endl;

    int x;
    cout << "��J x ���Ȩӭp��h���� a(x)�Bb(x)�G";
    cin >> x;

    // �p��a(x)�íp��ɶ�
    start = clock();
    cout << "a(" << x << ") = " << a.evaluate(x) << endl;
    finish = clock();
    cout << "a(x) �ݮ�: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    // �p��b(x)�íp��ɶ�
    start = clock();
    cout << "b(" << x << ") = " << b.evaluate(x) << endl;
    finish = clock();
    cout << "b(x) �ݮ�: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;

    return 0;
}
/*
5x^9-3x^8+2x^7+x^6-4x^5+6x^4-8x^3+7x^2-9x+10
-2x^10+4x^9-6x^8+8x^7-10x^6+12x^5-14x^4+16x^3-18x^2+20x-22
2
*/

