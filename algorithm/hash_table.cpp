#include <iostream>
#include <climits>

class Hash {
    public:
        int operator() (int k) { return k;}
};

class HashTable {
    public:
        struct HashNode {
            int k = INT_MIN;
            int val = 0;
            HashNode* next = nullptr;
        };
        const int INIT_SIZE = 10000;

        HashTable() { init(); }
        bool init();
        int * find(int k);
        bool insert(int k, int val);
        bool erase(int k);

    private:
        int size = 0;
        HashNode ** table = nullptr;
        Hash *hash = nullptr;
};

typedef HashTable::HashNode* HashTableNodePtr;

bool HashTable::init() {
    table = new HashTableNodePtr[INIT_SIZE];
    size = INIT_SIZE;
    hash = new Hash;
    return true;
}

int * HashTable::find(int k) {
    int hash_code = (*hash)(k) % size;
    auto node = table[hash_code];
    for (;;) {
        if (nullptr == node) {
            return nullptr;
        }
        if (node->k == k) {
            return &(node->val);
        }
        node = node->next;
    }
    return nullptr;
}

bool HashTable::insert(int k, int val) {
    int hash_code = (*hash)(k) % size;
    HashNode* node = table[hash_code];
    if (node == nullptr) {
        auto node = new HashNode();
        node->k = k;
        node->val = val;
        table[hash_code] = node;
        return true;
    }
    while (node->next != nullptr) {
        node = node->next;
    }
    node->next = new HashNode();
    node->next->k = k;
    node->next->val = val;
    return true;
}

bool HashTable::erase(int k) {
    int hash_code = (*hash)(k) % size;
    HashNode* node = table[hash_code];
    if (nullptr == node) {
        return false;
    }
    if (node->k == k) {
        delete node;
        table[hash_code] = nullptr;
        return true;
    }

    for (;;) {
        if (nullptr == node->next) {
            return false;
        }
        if (node->next->k == k) {
            delete node->next;
            node->next = nullptr;
            return true;
        }
    }
    return false;
}

int main() {
    HashTable ht;
    ht.insert(1234, 5678);
    auto p1 = ht.find(1234);
    if (nullptr != p1 ) {
        std::cout << "find val:" << *p1 << std::endl;
    }
    ht.erase(1234);
    auto p2 = ht.find(1234);
    if (nullptr == p2 ) {
        std::cout << "success to erase val:" << std::endl;
    } else {
        std::cout << "failed to erase val" << std::endl;
    }
    return 0;
}
