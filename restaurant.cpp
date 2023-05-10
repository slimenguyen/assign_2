#include "main.h"

struct Slot {
    string name;
    int result;
    int ID;
    int num;
    int time;
};
vector<Slot> slot_1;
vector<Slot> slot_2;

class Heap {
   public:
    vector<Slot> heap;
    void heapify_up(vector<Slot>& heap, int index) {
        if (index == 0) return;
        int parent_index = (index - 1) / 2;
        if (heap[index].num < heap[parent_index].num) {
            swap(heap[index], heap[parent_index]);
            heapify_up(heap, parent_index);
        } else if (heap[index].num == heap[parent_index].num) {
            if (heap[index].time < heap[parent_index].time) {
                swap(heap[index], heap[parent_index]);
                heapify_up(heap, parent_index);
            }
        }
    }

    void heapify_down(vector<Slot>& heap, int i) {
        int left_child_index = 2 * i + 1;
        int right_child_index = 2 * i + 2;
        int smallest = i;
        signed int n = heap.size();
        if (left_child_index < n &&
            ((heap[left_child_index].num < heap[smallest].num) ||
             ((heap[left_child_index].num == heap[smallest].num) &&
              (heap[left_child_index].time < heap[smallest].time)))) {
            smallest = left_child_index;
        }

        if (right_child_index < n &&
            ((heap[right_child_index].num < heap[smallest].num) ||
             ((heap[right_child_index].num == heap[smallest].num) &&
              (heap[right_child_index].time < heap[smallest].time)))) {
            smallest = right_child_index;
        }

        if (smallest != i) {
            swap(heap[i], heap[smallest]);
            heapify_down(heap, smallest);
        }
    }

    void insert(vector<Slot>& heap, string name, int result, int ID, int num,
                int time) {
        Slot slot;
        slot.name = name;
        slot.result = result;
        slot.ID = ID;
        slot.num = num;
        slot.time = time;
        heap.push_back(slot);
        heapify_up(heap, heap.size() - 1);
    }

    void delete_value_heap_ID(vector<Slot>& heap, int ID) {
        signed int n = heap.size();
        if (heap.empty()) return;
        int index = -1;
        for (int i = 0; i < n; i++) {
            if (heap[i].ID == ID) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            return;
        }
        swap(heap[index], heap[heap.size() - 1]);
        heap.pop_back();
        heapify_up(heap, index);
        heapify_down(heap, index);
    }

    void delete_value_heap(vector<Slot>& heap, string name) {
        signed int n = heap.size();
        if (heap.empty()) return;
        int index = -1;
        for (int i = 0; i < n; i++) {
            if (heap[i].name == name) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            return;
        }
        swap(heap[index], heap[heap.size() - 1]);
        heap.pop_back();
        heapify_up(heap, index);
        heapify_down(heap, index);
    }

    void display(vector<Slot>& heap, int i) {
        signed int n = heap.size();
        if (i < n) {
            cout << heap[i].ID << "-" << heap[i].num << endl;
            display(heap, 2 * i + 1);
            display(heap, 2 * i + 2);
        }
    }
};

Heap order_dish;

class HashTable {
   private:
    struct NodeHash {
        string name;
        int result;
        int ID;
        int num;
    };

    NodeHash* table;
    int size;

   public:
    HashTable() {
        table = new NodeHash[MAXSIZE / 2];
        size = MAXSIZE / 2;
        for (int i = 0; i < size; i++) {
            table[i].ID = -1;
            table[i].result = -1;
            table[i].num = -1;
            table[i].name = "";
        }
    }

    int hashFunction(int result) { return result % size; }

    void insert(int result, int ID, int num, string name) {
        int index = hashFunction(result);
        int startIndex = index;
        if (table[index].ID == -1) {
            table[index].ID = ID;
            table[index].result = result;
            table[index].num = num;
            table[index].name = name;
        } else {
            while (table[index].ID != -1) {
                index = (index + 1) % size;
                if (index == startIndex) {
                    return;
                }
            }
            table[index].ID = ID;
            table[index].result = result;
            table[index].num = num;
            table[index].name = name;
        }
    }

    void deleteNodeHash(int ID) {
        int index = 0;
        int startIndex = index;
        if (table[index].ID == ID) {
            table[index].ID = -1;
            table[index].result = -1;
            table[index].num = -1;
            table[index].name = "";
            return;
        }
        index++;
        while (index != startIndex) {
            if (table[index].ID == ID) {
                table[index].ID = -1;
                table[index].result = -1;
                table[index].num = -1;
                table[index].name = "";
                return;
            }
            index = (index + 1) % size;
        }
    }

    int countSpace() {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (table[i].ID != -1) {
                count++;
            }
        }
        return count;
    }

    bool search(int ID) {
        int index = 0;
        int startIndex = index;
        if (table[index].ID == ID) {
            return true;
        }
        index++;
        while (index != startIndex) {
            if (table[index].ID == ID) {
                return true;
            }
            index = (index + 1) % size;
        }
        return false;
    }

    void updateNum(int ID, int num) {
        int index = 0;
        int startIndex = index;
        if (table[index].ID == ID) {
            table[index].num = num;
            return;
        }
        index++;
        while (index != startIndex) {
            if (table[index].ID == ID) {
                table[index].num = num;
                return;
            }
            index = (index + 1) % size;
        }
    }

    void clear(HashTable& zone1) {
        int delete_ID;
        string delete_name;
        bool checkZone1 = false;
        signed int n = slot_1.size();
        signed int m = slot_2.size();
        for (int i = 0; i < n; i++) {
            checkZone1 = zone1.search(slot_1[i].ID);
            if (checkZone1 == true) {
                for (int j = 0; j < size; j++) {
                    if (table[j].ID == slot_1[i].ID) {
                        delete_ID = table[j].ID;
                        delete_name = table[j].name;
                        table[j].ID = -1;
                        table[j].result = -1;
                        table[j].num = -1;
                        table[j].name = "";
                        break;
                    }
                }
                slot_1.erase(slot_1.begin() + i);
                order_dish.delete_value_heap(order_dish.heap, delete_name);
                i--;
                for (int k = 0; k < m; k++) {
                    if (slot_2[k].ID == delete_ID) {
                        slot_2.erase(slot_2.begin() + k);
                    }
                }
            }
        }
    }

    void display() {
        for (int i = 0; i < size; i++) {
            if (table[i].ID != -1) {
                cout << table[i].ID << "-" << table[i].result << "-"
                     << table[i].num << endl;
            }
        }
    }

    ~HashTable() { delete[] table; }
};

// Build AVL Tree
struct Node {
    int result;
    int ID;
    int height;
    int num;
    Node* left;
    Node* right;
};
class AVLTree {
   public:
    Node* root;

    AVLTree() { root = nullptr; }

    int height(Node* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }
    int getBalance(Node* node) {
        if (node == nullptr) return 0;
        return height(node->left) - height(node->right);
    }

    int countNode(Node* node) {
        if (node == nullptr) return 0;
        return countNode(node->left) + countNode(node->right) + 1;
    }

    Node* insert(Node* node, int result, int ID, int num) {
        if (node == nullptr) {
            Node* newNode = new Node();
            newNode->result = result;
            newNode->ID = ID;
            newNode->num = num;
            newNode->left = nullptr;
            newNode->right = nullptr;
            newNode->height = 1;
            return newNode;
        }

        if (result < node->result) {
            node->left = insert(node->left, result, ID, num);
        } else if (result > node->result) {
            node->right = insert(node->right, result, ID, num);
        } else {
            node->right = insert(node->right, result, ID, num);
        }

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && result < node->left->result) {
            return rotateRight(node);
        }

        if (balance < -1 && result > node->right->result) {
            return rotateLeft(node);
        }

        if (balance > 1 && result > node->left->result) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && result < node->right->result) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        if (balance > 1 && result == node->left->result) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        return node;
    }
    bool traversal(Node* node, int ID) {
        if (node == nullptr) return false;
        if (node->ID == ID) return true;
        return traversal(node->left, ID) || traversal(node->right, ID);
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    Node* deleteNode(Node* node, int result, int ID) {
        if (node == nullptr) return node;
        if (result < node->result) {
            node->left = deleteNode(node->left, result, ID);
        } else if (result > node->result) {
            node->right = deleteNode(node->right, result, ID);
        } else {
            if ((node->left == nullptr) || node->right == nullptr) {
                Node* temp = node->left ? node->left : node->right;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                Node* temp = minValueNode(node->right);
                node->result = temp->result;
                node->right = deleteNode(node->right, temp->result, temp->ID);
            }
        }
        if (node == nullptr) return node;
        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);
        // Left left case
        if (balance > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        // Left right case
        if (balance > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // Right right case
        if (balance < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        // Right left case
        if (balance < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    void getResult(Node* node, int ID, int& result) {
        if (node == nullptr) return;
        if (node->ID == ID) result = node->result;
        getResult(node->left, ID, result);
        getResult(node->right, ID, result);
    }

    void updateNum(Node* node, int ID, int num) {
        if (node == nullptr) return;
        if (node->ID == ID) {
            node->num = num;
            return;
        }
        updateNum(node->left, ID, num);
        updateNum(node->right, ID, num);
    }

    void deleteOrderDish(Node* root) {
        signed int n = order_dish.heap.size();
        for (int i = 0; i < n; i++) {
            if (traversal(root, order_dish.heap[i].ID) == true) {
                order_dish.delete_value_heap(order_dish.heap,
                                             order_dish.heap[i].name);
                i--;
            }
        }
    }

    void helperClearSlot(Node* root) {
        signed int n = slot_1.size();
        signed int m = slot_2.size();
        for (int i = 0; i < n; i++) {
            if (traversal(root, slot_1[i].ID) == true) {
                deleteOrderDish(root);
                slot_1.erase(slot_1.begin() + i);
                i--;
            }
        }
        for (int j = 0; j < m; j++) {
            if (traversal(root, slot_2[j].ID) == true) {
                slot_2.erase(slot_2.begin() + j);
                j--;
            }
        }
    }

    void clear(Node* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    void display(Node* root) {
        if (!root) {
            return;
        }
        queue<Node*> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                Node* curr = q.front();
                q.pop();
                cout << curr->ID << "-" << curr->result << "-" << curr->num
                     << endl;
                if (curr->left) {
                    q.push(curr->left);
                }
                if (curr->right) {
                    q.push(curr->right);
                }
            }
        }
    }
};

map<char, string> codes;

vector<pair<char, int>> FrequencyDect(string s) {
    map<char, int> mp;
    for (size_t i = 0; i < s.size(); i++) {
        mp[s[i]]++;
    }
    vector<pair<char, int>> v;
    for (map<char, int>::iterator it = mp.begin(); it != mp.end(); it++) {
        v.push_back(make_pair(it->first, it->second));
    }
    return v;
}

// A Huffman tree node
struct MinHeapNode {
    // One of the input characters
    char data;

    // Frequency of the character
    unsigned freq;

    // Left and right child
    MinHeapNode *left, *right;

    MinHeapNode(char data, unsigned freq)

    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

void storeCodes(struct MinHeapNode* root, string str) {
    if (root == NULL) return;
    if (root->data != '$') codes[root->data] = str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void HuffmanCodes(char data[], int freq[], int size) {
    struct MinHeapNode *left, *right, *top;

    // Create a min heap & inserts all characters of data[]

    vector<MinHeapNode*> buildMinHeap;

    for (int i = 0; i < size; i++) {
        buildMinHeap.push_back(new MinHeapNode(data[i], freq[i]));
    }

    for (int i = 0; i < size - 1; i++) {
        for (int j = size - 1; j > i; j--) {
            if (buildMinHeap[j]->freq < buildMinHeap[j - 1]->freq) {
                swap(buildMinHeap[j], buildMinHeap[j - 1]);
            }
        }
    }

    // Iterate while size of heap doesn't become 1
    while (buildMinHeap.size() != 1) {
        left = buildMinHeap[0];
        buildMinHeap.erase(buildMinHeap.begin());

        right = buildMinHeap[0];
        buildMinHeap.erase(buildMinHeap.begin());

        top = new MinHeapNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        buildMinHeap.push_back(top);

        int n = buildMinHeap.size();

        for (int i = 0; i < n - 1; i++) {
            for (int j = n - 1; j > i; j--) {
                if (buildMinHeap[j]->freq < buildMinHeap[j - 1]->freq) {
                    swap(buildMinHeap[j], buildMinHeap[j - 1]);
                }
            }
        }
        storeCodes(buildMinHeap[0], "");
        // printCodes(buildMinHeap[0], "");
    }
}

int binaryToDecimal(string n) {
    int decimal = 0;
    int power = 0;
    for (int i = n.size() - 1; i >= 0; i--) {
        if (n[i] == '1') {
            decimal += pow(2, power);
        }
        power++;
    }
    return decimal;
}

void FIFO(vector<Slot>& slot_1, vector<Slot>& slot_2, string name, int res,
          HashTable& zone1, AVLTree& zone2, Node*& node) {
    int delete_ID = slot_1[0].ID;
    int delete_res = slot_1[0].result;
    string delete_name = slot_1[0].name;
    signed int m = slot_2.size();
    slot_1.erase(slot_1.begin());
    if (zone1.search(delete_ID) == true) {
        zone1.deleteNodeHash(delete_ID);
        zone1.insert(res, delete_ID, 1, name);
    } else if (zone2.traversal(node, delete_ID) == true) {
        node = zone2.deleteNode(node, delete_res, delete_ID);
        node = zone2.insert(node, res, delete_ID, 1);
    }
    int time_insert;
    time_insert = slot_1.back().time;
    time_insert++;
    slot_1.push_back({name, res, delete_ID, 1, time_insert});
    order_dish.delete_value_heap(order_dish.heap, delete_name);
    order_dish.insert(order_dish.heap, name, res, delete_ID, 1, time_insert);
    // xoa ben cua slot 2
    for (int i = 0; i < m; i++) {
        if (slot_2[i].ID == delete_ID) {
            slot_2.erase(slot_2.begin() + i);
            slot_2.push_back({name, res, delete_ID, 1});
            break;
        }
    }
}

void LRCO(vector<Slot>& slot_2, vector<Slot>& slot_1, string name, int res,
          int ID, HashTable& zone1, AVLTree& zone2, Node*& node) {
    int delete_ID = slot_2[0].ID;
    int delete_res = slot_2[0].result;
    string delete_name = slot_2[0].name;
    signed int n = slot_1.size();
    slot_2.erase(slot_2.begin());
    if (zone1.search(delete_ID) == true) {
        zone1.deleteNodeHash(delete_ID);
        zone1.insert(res, delete_ID, 1, name);
    } else if (zone2.traversal(node, delete_ID) == true) {
        node = zone2.deleteNode(node, delete_res, delete_ID);
        node = zone2.insert(node, res, delete_ID, 1);
    }
    slot_2.push_back({name, res, delete_ID, 1});
    // xoa ben cua slot 1
    int time_insert;
    time_insert = slot_1.back().time;
    time_insert++;
    order_dish.delete_value_heap(order_dish.heap, delete_name);
    order_dish.insert(order_dish.heap, name, res, delete_ID, 1, time_insert);
    for (int i = 0; i < n; i++) {
        if (slot_1[i].ID == delete_ID) {
            slot_1.erase(slot_1.begin() + i);
            slot_1.push_back({name, res, delete_ID, 1, time_insert});
            break;
        }
    }
}

void LFCO(Heap& order_dish, vector<Slot>& slot_1, string name, int res, int ID,
          HashTable& zone1, AVLTree& zone2, Node*& node) {
    int min_order_dish = order_dish.heap[0].num;
    int min_order_time = order_dish.heap[0].time;
    string name_delete;
    int delete_ID;
    int delete_res;
    int save_i;
    int save_j;
    signed int n = slot_1.size();
    signed int m = slot_2.size();
    for (int i = 0; i < n; i++) {
        if (slot_1[i].num == min_order_dish &&
            slot_1[i].time == min_order_time) {
            delete_ID = slot_1[i].ID;
            delete_res = slot_1[i].result;
            save_i = i;
            break;
        }
    }
    for (int j = 0; j < m; j++) {
        if (slot_2[j].num == min_order_dish &&
            slot_2[j].time == min_order_time) {
            save_j = j;
            break;
        }
    }
    order_dish.delete_value_heap(order_dish.heap, name_delete);
    int time_insert;
    time_insert = slot_1.back().time;
    time_insert++;
    slot_1.erase(slot_1.begin() + save_i);
    slot_1.push_back({name, res, delete_ID, 1, time_insert});
    slot_2.erase(slot_2.begin() + save_j);
    slot_2.push_back({name, res, delete_ID, 1});
    order_dish.insert(order_dish.heap, name, res, delete_ID, 1, time_insert);
    if (zone1.search(delete_ID) == true) {
        zone1.deleteNodeHash(delete_ID);
        zone1.insert(res, ID, 1, name);
    } else if (zone2.traversal(node, delete_ID) == true) {
        zone2.deleteNode(node, delete_res, delete_ID);
        zone2.insert(node, res, ID, 1);
    }
}

void order(string name, Node*& root, HashTable& zone1, AVLTree& zone2) {
    string s = name;
    vector<pair<char, int>> freq = FrequencyDect(s);
    int size = freq.size();
    char* arr = new char[size];
    int* frequency = new int[size];
    for (int i = 0; i < size; i++) {
        arr[i] = freq[i].first;
        frequency[i] = freq[i].second;
    }
    HuffmanCodes(arr, frequency, size);
    string encodedString;
    signed int tmp = s.size();
    for (int i = 0; i < tmp; i++) {
        char i_char = s[i];
        encodedString += codes[i_char];
    }
    string last15Bits = encodedString;
    if (encodedString.size() > 15)
        last15Bits = encodedString.substr(encodedString.size() - 15);
    int res = binaryToDecimal(last15Bits);

    int ID = (res % MAXSIZE) + 1;
    int count = 0;
    signed int n = slot_1.size();
    signed int m = slot_2.size();
    // Xu li chon dat ban va chon khu vuc
    string name_insert_1 = "";
    string name_insert_2 = "";
    for (int i = 0; i < m; i++) {
        if (slot_2[i].name == name) {
            name_insert_2 = name;
        }
    }

    for (int i = 0; i < n; i++) {
        count++;
        if (slot_1[i].name == name) {
            name_insert_1 = name;
        }
    }

    if (count == MAXSIZE && name_insert_1 != name && name_insert_2 != name) {
        int opt = res % 3;
        if (opt == 0) {
            FIFO(slot_1, slot_2, name, res, zone1, zone2, root);
            return;
        } else if (opt == 1) {
            LRCO(slot_2, slot_1, name, res, ID, zone1, zone2, root);
            return;
        } else if (opt == 2) {
            LFCO(order_dish, slot_1, name, res, ID, zone1, zone2, root);
            return;
        }
    } else if (count == MAXSIZE && name_insert_1 == name &&
               name_insert_2 == name) {
        for (int i = 0; i < n; i++) {
            if (slot_1[i].name == name) {
                slot_1[i].num++;
                int update_num = slot_1[i].num;
                int time_insert = slot_1[i].time;
                zone1.updateNum(slot_1[i].ID, update_num);
                zone2.updateNum(root, slot_1[i].ID, update_num);
                order_dish.delete_value_heap(order_dish.heap, name);
                order_dish.insert(order_dish.heap, name, res, slot_1[i].ID,
                                  update_num, time_insert);
            }
        }
        for (int i = 0; i < m; i++) {
            if (slot_2[i].name == name) {
                int delete_ID = slot_2[i].ID;
                slot_2.erase(slot_2.begin() + i);
                slot_2.push_back({name, res, delete_ID, 1});
                return;
            }
        }
    } else if (count != MAXSIZE && name_insert_1 == name &&
               name_insert_2 == name) {
        for (int i = 0; i < n; i++) {
            if (slot_1[i].name == name) {
                slot_1[i].num++;
                int update_num = slot_1[i].num;
                int time_insert = slot_1[i].time;
                zone1.updateNum(slot_1[i].ID, update_num);
                zone2.updateNum(root, slot_1[i].ID, update_num);
                order_dish.delete_value_heap(order_dish.heap, name);
                order_dish.insert(order_dish.heap, name, res, slot_1[i].ID,
                                  update_num, time_insert);
            }
        }
        for (int i = 0; i < m; i++) {
            if (slot_2[i].name == name) {
                int delete_ID = slot_2[i].ID;
                slot_2.erase(slot_2.begin() + i);
                slot_2.push_back({name, res, delete_ID, 1});
                return;
            }
        }
    }

    if (res % 2 != 0) {
        for (int i = 0; i < 2 * MAXSIZE; i++) {
            bool checkZone1 = zone1.search(ID);
            bool checkZone2 = zone2.traversal(root, ID);

            if (checkZone1 == false && checkZone2 == false) {
                int count = zone1.countSpace();
                if (count == MAXSIZE / 2) {
                    root = zone2.insert(root, res, ID, 1);
                    int time_insert;
                    for (int i = 0; i < n; i++) {
                        count++;
                    }
                    if (count == 0) {
                        time_insert = 1;
                        slot_1.push_back({name, res, ID, 1, time_insert});
                        order_dish.insert(order_dish.heap, name, res, ID, 1,
                                          time_insert);
                        slot_2.push_back({name, res, ID, 1});
                    } else {
                        time_insert = slot_1.back().time;
                        time_insert++;
                        slot_1.push_back({name, res, ID, 1, time_insert});
                        order_dish.insert(order_dish.heap, name, res, ID, 1,
                                          time_insert);
                        slot_2.push_back({name, res, ID, 1});
                    }
                    // 	for(int i = 0; i < slot_2.size(); i++){
                    // 	cout << slot_2[i].name << " " << slot_2[i].result << " "
                    // << slot_2[i].ID << " " << slot_2[i].num << endl;
                    // }
                    break;
                }
                zone1.insert(res, ID, 1, name);
                int time_insert;
                for (int i = 0; i < n; i++) {
                    count++;
                }
                if (count == 0) {
                    time_insert = 1;
                    slot_1.push_back({name, res, ID, 1, time_insert});
                    order_dish.insert(order_dish.heap, name, res, ID, 1,
                                      time_insert);
                    slot_2.push_back({name, res, ID, 1});
                } else {
                    time_insert = slot_1.back().time;
                    time_insert++;
                    slot_1.push_back({name, res, ID, 1, time_insert});
                    order_dish.insert(order_dish.heap, name, res, ID, 1,
                                      time_insert);
                    slot_2.push_back({name, res, ID, 1});
                }
                // for(int i = 0; i < slot_2.size(); i++){
                // 	cout << slot_2[i].name << " " << slot_2[i].result << " "
                // << slot_2[i].ID << " " << slot_2[i].num << endl;
                // }
                break;
            } else {
                ID++;
                if (ID > MAXSIZE) ID = 1;
            }
        }
    } else {
        for (int i = 0; i < 2 * MAXSIZE; i++) {
            bool checkZone1 = zone1.search(ID);
            bool checkZone2 = zone2.traversal(root, ID);
            if (checkZone1 == false && checkZone2 == false) {
                int count = zone2.countNode(root);
                if (count == MAXSIZE / 2) {
                    zone1.insert(res, ID, 1, name);
                    int time_insert;
                    for (int i = 0; i < n; i++) {
                        count++;
                    }
                    if (count == 0) {
                        time_insert = 1;
                        slot_1.push_back({name, res, ID, 1, time_insert});
                        order_dish.insert(order_dish.heap, name, res, ID, 1,
                                          time_insert);
                        slot_2.push_back({name, res, ID, 1});
                    } else {
                        time_insert = slot_1.back().time;
                        time_insert++;
                        slot_1.push_back({name, res, ID, 1, time_insert});
                        order_dish.insert(order_dish.heap, name, res, ID, 1,
                                          time_insert);
                        slot_2.push_back({name, res, ID, 1});
                    }
                    // 	for(int i = 0; i < slot_2.size(); i++){
                    // 	cout << slot_2[i].name << " " << slot_2[i].result << " "
                    // << slot_2[i].ID << " " << slot_2[i].num << endl;
                    // }
                    break;
                }
                root = zone2.insert(root, res, ID, 1);
                int time_insert;
                for (int i = 0; i < n; i++) {
                    count++;
                }
                if (count == 0) {
                    time_insert = 1;
                    slot_1.push_back({name, res, ID, 1, time_insert});
                    order_dish.insert(order_dish.heap, name, res, ID, 1,
                                      time_insert);
                    slot_2.push_back({name, res, ID, 1});
                } else {
                    time_insert = slot_1.back().time;
                    time_insert++;
                    slot_1.push_back({name, res, ID, 1, time_insert});
                    order_dish.insert(order_dish.heap, name, res, ID, 1,
                                      time_insert);
                    slot_2.push_back({name, res, ID, 1});
                }
                // for(int i = 0; i < slot_2.size(); i++){
                // 	cout << slot_2[i].name << " " << slot_2[i].result << " "
                // << slot_2[i].ID << " " << slot_2[i].num << endl;
                // }
                break;
            } else {
                ID++;
                if (ID > MAXSIZE) ID = 1;
            }
        }
    }
}

void clear(Node*& root, HashTable& zone1, AVLTree& zone2, int num,
           Heap& order_dish) {
    string name_delete_order_dish;
    signed int n = slot_1.size();
    signed int m = slot_2.size();
    if (num < 1) {
        zone1.clear(zone1);
    } else if (num > MAXSIZE) {
        zone2.helperClearSlot(root);
        zone2.clear(root);
        root = nullptr;
    } else {
        bool checkZone1 = zone1.search(num);
        bool checkZone2 = zone2.traversal(root, num);
        if (checkZone1 == true) {
            order_dish.delete_value_heap_ID(order_dish.heap, num);
            zone1.deleteNodeHash(num);
            for (int i = 0; i < n; i++) {
                if (slot_1[i].ID == num) {
                    slot_1.erase(slot_1.begin() + i);
                    break;
                }
            }
            for (int i = 0; i < m; i++) {
                if (slot_2[i].ID == num) {
                    slot_2.erase(slot_2.begin() + i);
                    break;
                }
            }
        } else if (checkZone2 == true) {
            int res = 0;
            zone2.getResult(root, num, res);
            order_dish.delete_value_heap_ID(order_dish.heap, num);
            root = zone2.deleteNode(root, res, num);
            for (int i = 0; i < n; i++) {
                if (slot_1[i].ID == num) {
                    slot_1.erase(slot_1.begin() + i);
                    break;
                }
            }
            for (int i = 0; i < m; i++) {
                if (slot_2[i].ID == num) {
                    slot_2.erase(slot_2.begin() + i);
                    break;
                }
            }
        } else {
            return;
        }
    }
}

void printHT(HashTable& zone1) { zone1.display(); }

void printAVL(Node*& root, AVLTree& zone2) { zone2.display(root); }

void printMH(Heap& order_dish, int i) {
    order_dish.display(order_dish.heap, i);
}

void process(string line, Node*& root, HashTable& zone1, AVLTree& zone2) {
    string delimiter = " ";
    size_t pos = 0;
    string opcode;
    string get1 = "";
    for (int i = 1; i <= 2 && (pos = line.find(delimiter)) != string::npos;
         i++) {
        if (i == 1) {
            opcode = line.substr(0, pos);
            line.erase(0, pos + delimiter.length());
        } else if (i == 2) {
            get1 = line.substr(0, pos);
            line.erase(0, pos + delimiter.length());
        }
    }
    if (opcode == "") {
        opcode = line;
    }

    else if (get1 == "") {
        get1 = line;
    }
    if (opcode == "REG") {
        string name = get1;
        order(name, root, zone1, zone2);
    } else if (opcode == "CLE") {
        int num = stoi(get1);
        clear(root, zone1, zone2, num, order_dish);
    } else if (opcode == "PrintHT") {
        if (get1 == "") {
            printHT(zone1);
        }
    } else if (opcode == "PrintAVL") {
        if (get1 == "") {
            printAVL(root, zone2);
        }
    } else if (opcode == "PrintMH") {
        if (get1 == "") {
            int i = 0;
            printMH(order_dish, i);
        }
    }
}

void simulate(string filename) {
    ifstream file;
    file.open(filename);
    string line;
    Node* root = nullptr;
    HashTable zone1;
    AVLTree zone2;
    while (getline(file, line)) {
        process(line, root, zone1, zone2);
    }
}