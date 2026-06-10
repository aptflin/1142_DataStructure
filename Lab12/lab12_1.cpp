#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

struct Product {
    string name;
    int expiry;     
    int popularity; 

    bool operator<(const Product& other) const {
        if (expiry != other.expiry)
            return expiry < other.expiry;
        return popularity > other.popularity;
    }
};

class MinHeap {
public:
    vector<Product> heap;

    void buildHeap(const vector<Product>& arr) {
        heap = arr;
        for (int i = heap.size() / 2 - 1; i >= 0; --i)
            heapify(i);
    }

    void heapify(int i) {
        int smallest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        
        if (l < heap.size() && heap[l] < heap[smallest])
            smallest = l;
        if (r < heap.size() && heap[r] < heap[smallest])
            smallest = r;

        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapify(smallest);
        }
    }

    Product extractMin() {
        if (heap.empty()) throw runtime_error("Heap is empty");

        Product top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapify(0);
        return top;
    }

    bool isEmpty() {
        return heap.empty();
    }
};

int main() {
    ifstream infile("input1.txt"); 
    if (!infile) {
        cerr << "無法打開檔案 input1.txt" << endl;
        return 1;
    }

    int N;
    if (!(infile >> N)) return 1;
    infile.ignore();

    vector<Product> products;
    string line;
    for (int i = 0; i < N && getline(infile, line); ++i) {
        if (line.empty()) { 
            i--;
            continue; 
        }
        
        size_t lastSpace = line.find_last_not_of(" \t\r\n");
        if (lastSpace != string::npos) {
            line = line.substr(0, lastSpace + 1); // Crop trailing layout noise
        }

        size_t p2 = line.find_last_of(" \t");
        size_t p1 = line.substr(0, p2).find_last_of(" \t");

        if (p1 == string::npos || p2 == string::npos) {
            cerr << "檔案格式不符需求" << endl;
            return 1;
        }

        string name = line.substr(0, p1);
        int expiry = stoi(line.substr(p1 + 1, p2 - p1 - 1));
        int popularity = stoi(line.substr(p2 + 1));

        products.push_back({ name, expiry, popularity });
    }
    infile.close();

    MinHeap heap;
    heap.buildHeap(products);

    vector<string> sortedNames;
    while (!heap.isEmpty()) {
        sortedNames.push_back(heap.extractMin().name);
    }

    cout << "商品排序:" << endl;
    for (const string& name : sortedNames) {
        cout << name << endl;
    }

    return 0;
}
