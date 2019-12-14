#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include<fstream>
#include <stdlib.h>
#include <assert.h>
#include "dirent.h"


using namespace std;

// A Tree node
struct Node
{
	char c;		// Kí tự của node
	int freq;	// Số lần xuất hiện của kí tự	
	Node *left, *right;	// node con bên trái và bên phải
};

// Tạo Note mới
Node* getNode(char c, int freq, Node* left, Node* right)
{
	Node* node = new Node();
	node->c = c;
	node->freq = freq;
	node->left = left;
	node->right = right;
	return node;
}

// Độ ưu tiên lấy node có freq nhỏ hơn
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		return l->freq > r->freq;
	}
};

// Nén huffman
// Hàm đệ quy. Duyêt qua node bên trái thì dãy bit xét là 0
//             Duyệt qua node bên phải thì dãy bit xét là 1 
void encode(Node* root, string str, unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr)  // Điều kiện dừng 
		return;

	// Nếu là Node lá 
	if (!root->left && !root->right)
	{
		huffmanCode[root->c] = str;
	}
	// Đệ quy duyệt qua trái
	encode(root->left, str + "0", huffmanCode);
	// Đệ quy duyệt qua phải
	encode(root->right, str + "1", huffmanCode);
}

// Giải nén
void decode(Node* root, int &index, string str)
{
	if (root == nullptr) {
		return;
	}

	// Tìm node lá
	if (!root->left && !root->right)
	{
		cout << root->c;
		return;
	}

	index++;
	// node lá bên trái
	if (str[index] == '0')	decode(root->left, index, str);
	else	decode(root->right, index, str);
}

// Tạo cây Huffman 
void buildHuffmanTree(const string &input)
{
	ifstream fi;
	fi.open(input, ios::in);
	string text;

	if (!fi.is_open())
	{
		cout << "File is not exists or Can not open file \n";
		return;
	}
	else
	{
		getline(fi, text);
	}
	fi.close();
	ofstream fo;
	fo.open("FileNen.txt", ios::out);


	// Một map( dữ liệu liên kết) freg là chứa tần suất xuất hiện của một kí tự. (position/data) freg. vd: A - 10
	unordered_map<char, int> freq;

	// BƯỚC 1: THỐNG KÊ TẤN SỐ XUẤT HIỆN CÁC KÝ TỰ.
	// Duyệt các kí tự trong text
	for (char c : text)
	{
		// nếu có thì tăng tần suất của kí tự đó lên
		freq[c]++;
	}

	// BƯỚC 2: TẠO CÂY HUFFMAN TREE
	// Tạo một priority_queue để lưu các node còn sống ( có nghĩa là chưa duyệt đến) của cây huffman với độ ưu tiên là cmp
	priority_queue<Node*, vector<Node*>, comp> pq;

	// Tạo một node lá cho mỗi kí tự và thêm nó vào priority_queue. Ví dụ thêm node A-2 . Có nghĩa là node kí tự A có tần suất là 2
	for (auto pair : freq)
	{
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	// Trong khi hàng đợi có hơn 1 node 
	while (pq.size() != 1)
	{
		// Lấy ra hai node đầu tiên ( tức là hai node có tần suất nhỏ nhất) vì trong priority_queue mặc định lấy ra top( pt đầu tiên)
		// độ ưu tiên là less( phần từ nhỏ nhất)
		Node *left = pq.top();
		pq.pop(); // Giảm số lương số phần tử trong pq xuống đi 1
		Node *right = pq.top();
		pq.pop();

		// Tạo ra một node mới có hai node con vừa rồi và có tần suất là tổng của hai node con cộng lại.
		// Thêm node mới vào priority queue
		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}

	// BƯỚC 3: PHÁT SINH BẢNG MÃ BIT TỪ CÂY HUFFMAN
	// Lấy nút gốc là phần từ top của priority queue
	Node* root = pq.top();

	// lưu bảng mã bit của các kí tự trong cây.
	unordered_map<char, string> huffmanCode;
	encode(root, "", huffmanCode);

	fo << "Table bit code Huffman :\n";
	for (auto pair : huffmanCode) {
		fo << pair.first << " " << pair.second << '\n';
	}

	fo << "\nString :\n" << text << '\n';

	// str là chuỗi dãy bit sau khi đã mã hóa.
	string str = "";
	// Nén huffman. Duyệt các kí tự của text, đối chiếu với bảng bit code Huffman để thay các kí tự bằng bằng dãy bit.
	for (char ch : text) {
		str += huffmanCode[ch];
	}

	fo << "\nEncoded string is :\n" << str << '\n';
	cout << "\n- String have been encoded and stored in file FileNen.txt\n";
	cout << "\n- Decoded string is: ";

	// Giải nén. Từ gốc truy ngược lại đến đến các node lá để 
	int index = -1;
	while (index < (int)str.size() - 2)
	{
		decode(root, index, str);
	}
	cout << "\n";
	fo.close();
}


int main (int argc, char *argv[]) 
{
	int select;
	do
	{
		cout << "\n------------------ Program Huffman -------------------------------\n";
		cout << "Delepment:	Nguyen Truong Anh Tuan - 1712197\n";
		cout << "Contact  :	1712197@student.hcmus.edu.vn\n";
		cout << "\n------------------------------------------------------------------\n";
		cout << " *Chuc nang 1:      Encode 1 File txt.\n";
		cout << " *Chuc nang 2:      Encode 1 Directory.\n";
		cout << endl;
	
		cout << "Enter Select with (0 < Select < 3) - Select: ";		cin >> select;
		switch (select)
		{
		case 1:
		{
			// Nhập tên file txt
			string s;
			cout << "Enter name File txt to encode: ";
			cin.ignore();
			getline(std::cin, s);
			std::fflush(stdin);
			buildHuffmanTree(s);
			break;

		}
		case 2:
		{
			string name;
			cout << "Enter name Drictory: "; 
			cin.ignore();
			getline(std::cin, name);
			DIR *dir;
			dirent *ent;
			cout << "\nList File txt in Drictory: \n";
			if ((dir = opendir(name.c_str())) != NULL) {
				// print all the files and directories within directory 
				while ((ent = readdir(dir)) != NULL) {
					string t;
					t = ent->d_name;
					//buildHuffmanTree(t);
					cout << ent->d_name << endl;				
					string text;
					ifstream fi;
					fi.open(ent->d_name, ios::in);
					if (!fi.is_open())
					{
						cout << "File is not exists or Can not open file \n";
						break;
					}
					else
					{
						getline(fi, text);
					}
				
					cout << text;
					fi.close();
				}
				closedir(dir);
			}
			else {
				// could not open directory 
				perror("");				
			}
			break;
		}
		default:
			cout << "Error input: Enter 0 < select < 3. Please.....!!!\n";
			break;
		}
	} while (select != 3);
	
	system("pause");
	return 0;
}
