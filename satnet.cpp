// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
SatNet::SatNet(){
    m_root = nullptr;
}

SatNet::~SatNet(){
    deleteSubTree(m_root);
}

// deletes subtree from pointer passed in
void SatNet::deleteSubTree(Sat* ptr) {
    if (ptr) {
        deleteSubTree(ptr->m_left);
        deleteSubTree(ptr->m_right);
        delete ptr;
    }
}

// returns max of 2 ints
int SatNet::max(int x, int y) {
    if (x > y) {
        return x;
    }
    else {
        return y;
    }

}

// gets height of a node
int SatNet::height(Sat* ptr) {
    if (ptr == nullptr) {
        return 0;
    }
    return ptr->m_height;
}

// rotates ptr to the right
Sat* SatNet::rightRotate(Sat* ptr) {
    Sat* left = ptr->m_left;
    Sat* second = left->m_right;

    // rotation
    left->m_right = ptr;
    ptr->m_left = second;

    // update heights
    updateHeight(ptr);
    updateHeight(left);
    
    return left;
}

// rotates ptr to the left
Sat* SatNet::leftRotate(Sat* ptr) {
    Sat* right = ptr->m_right;
    Sat* second = right->m_left;

    // rotation
    right->m_left = ptr;
    ptr->m_right = second;

    // update heights
    updateHeight(ptr);
    updateHeight(right);

    return right;
}

void SatNet::balanceNode(Sat*& ptr) {
    int balance = height(ptr->m_left) - height(ptr->m_right);
    // Left cases
    if (balance > 1) {
        // Left Left
        if (ptr->m_id > ptr->m_left->m_id) {
            ptr = rightRotate(ptr);
        }

        // Left Right
        else {
            ptr->m_left = leftRotate(ptr->m_left);
            ptr = rightRotate(ptr);
        }
    }

    // Right cases
    if (balance < -1) {
        // Right Right
        if (ptr->m_id < ptr->m_right->m_id) {
            ptr = leftRotate(ptr);
        }
        // Right Left
        else {
            ptr->m_right = rightRotate(ptr->m_right);
            ptr = leftRotate(ptr);
        }
    }
}

void SatNet::updateHeight(Sat* ptr) {
    if (ptr != nullptr) {
        if (!ptr->m_right && !ptr->m_left) {
            ptr->m_height = 0;
        }
        else {
            ptr->m_height = max(height(ptr->m_left), height(ptr->m_right)) + 1;
        }
    }
}

void SatNet::insert(const Sat& satellite){
    if (satellite.m_id <= MAXID && satellite.m_id >= MINID) {
        insertPrivate(satellite, m_root);
    }
}

// insert that's called inside the public insert
void SatNet::insertPrivate(const Sat& satellite, Sat*& ptr) {

    // tree is empty
    if (ptr == nullptr) {
        ptr = new Sat(satellite.m_id, satellite.m_altitude, satellite.m_inclin, satellite.m_state);
    }
    // if satellite.m_id is less than ptr's m_id it goes to the left
    else if (satellite.m_id < ptr->m_id) {
        insertPrivate(satellite, ptr->m_left);
    }
    // if satellite.m_id is greater than ptr's m_id it goes to the left
    else if (satellite.m_id > ptr->m_id) {
        insertPrivate(satellite, ptr->m_right);
    }

    // updates height
    updateHeight(ptr);

    // balances node
    balanceNode(ptr);
}

void SatNet::clear(){
    deleteSubTree(m_root);
    m_root = nullptr;
}

void SatNet::remove(int id){
    removePrivate(m_root, id);
}

void SatNet::removePrivate(Sat*& ptr, int id) {
    if (!ptr) {
        return;
    }

    // if id is less than desired id, go left
    if (id < ptr->m_id) {
        removePrivate(ptr->m_left, id);
    }

    // if id is greater than desired id, go right
    else if (id > ptr->m_id) {
        removePrivate(ptr->m_right, id);
    }

    // correct node
    else {
        // if no left child, replace node with right child
        if (!ptr->m_left) {
            Sat* temp = ptr;
            ptr = ptr->m_right;
            delete temp;
        }
        // if no right child, replace node with left child
        else if (!ptr->m_right) {
            Sat* temp = ptr;
            ptr = ptr->m_left;
            delete temp;
        }
        // ptr has both childen
        // replace with smallest node in right subtree
        else {
            Sat* temp = findMin(ptr->m_right);
            ptr->m_id = temp->m_id;
            ptr->m_inclin = temp->m_inclin;
            ptr->m_state = temp->m_state;
            ptr->m_altitude = temp->m_altitude;
            
            // ptr = Sat(temp.m_id, temp.m_altitude, temp.m_inclin, temp.m_state);

            removePrivate(ptr->m_right, temp->m_id);
        }
    }

    // updates height and balance if tree not empty
    if (ptr) {
        updateHeight(ptr);
        balanceNode(ptr);
    }
}

// finds smallest value in subtree of given node
Sat* SatNet::findMin(Sat* ptr) {
    while (ptr->m_left) {
        ptr = ptr->m_left;
    }
    return ptr;
}

void SatNet::dumpTree() const {
    dump(m_root);
}

void SatNet::dump(Sat* satellite) const{
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}

void SatNet::listSatellites() const {
    inOrder(m_root);
}

// prints the nodes in order of key
void SatNet::inOrder(Sat* aNode) const {
    if (aNode != nullptr) {
        inOrder(aNode->m_left);
        cout << aNode->m_id << ": " << aNode->getStateStr() << ": " 
        << aNode->getInclinStr() << ": " << aNode->getAltStr() << endl;
        inOrder(aNode->m_right);
    }
}

bool SatNet::setState(int id, STATE state){
    bool found = false;
    setStatePrivate(id, state, m_root, found);
    return found;
}

// sets state of node with given id
void SatNet::setStatePrivate(int id, STATE state, Sat* ptr, bool& found) {
    if (ptr != nullptr) {
        if (ptr->m_id == id) {
            ptr->setState(state);
            found = true;
        }
        else if (id < ptr->m_id) {
            setStatePrivate(id, state, ptr->m_left, found);
        }
        else {
            setStatePrivate(id, state, ptr->m_right, found);
        }
    }
}

void SatNet::removeDeorbited(){
    removeDeorbitedPrivate(m_root);
}

// traverses through and removes any node with a deorbited state
void SatNet::removeDeorbitedPrivate(Sat* ptr) {
    if (ptr != nullptr) {
        removeDeorbitedPrivate(ptr->m_left);
        removeDeorbitedPrivate(ptr->m_right);
        if (ptr->m_state == DEORBITED) {
            remove(ptr->m_id);
        }
    }
}

bool SatNet::findSatellite(int id) const {
    bool found = false;
    findSatellitePrivate(id, m_root, found);
    return found;
}

// recursively traverses and sets found to true if node found
void SatNet::findSatellitePrivate(int id, Sat* ptr, bool& found) const {
    if (ptr != nullptr) {
        if (ptr->m_id == id) {
            found = true;
            return;
        }
        else if (id < ptr->m_id && ptr->m_left != nullptr) {
            findSatellitePrivate(id, ptr->m_left, found);
        }
        else if (id > ptr->m_id && ptr->m_right != nullptr) {
            findSatellitePrivate(id, ptr->m_right, found);
        }
    }
}

const SatNet & SatNet::operator=(const SatNet & rhs){
    if (this == &rhs) {
        return *this;
    }

    clear();

    if (rhs.m_root != nullptr) {
        m_root = copyTree(rhs.m_root);
    }

    return *this;
}

// copies individual nodes recursively
Sat* SatNet::copyTree(const Sat* ptr) {
    if (ptr == nullptr) {
        return  nullptr;
    }

    Sat* temp = new Sat;
    temp->m_id = ptr->m_id;
    temp->m_altitude = ptr->m_altitude;
    temp->m_inclin = ptr->m_inclin;
    temp->m_state = ptr->m_state;
    temp->m_left = copyTree(ptr->m_left);
    temp->m_right = copyTree(ptr->m_right);
    temp->m_height = ptr->m_height;

    return temp;
}


// counts the amount of satellites with the given degree
int SatNet::countSatellites(INCLIN degree) const{
    int count = 0;
    // passes in count as a reference to modify the variable
    inOrderTraverse(degree, m_root, count);
    return count;
}

// traverses the tree and adds one to count if incline matches
void SatNet::inOrderTraverse(INCLIN degree, Sat* ptr, int& count) const {
    if (ptr != nullptr) {
        inOrderTraverse(degree, ptr->m_left, count);
        if (ptr->m_inclin == degree) count++;
        inOrderTraverse(degree, ptr->m_right, count);
    }
}

bool SatNet::isBalanced(Sat* ptr) {
    if (ptr == nullptr) {
        return true;
    }

    int balance;

    if (ptr == nullptr) {
        balance = 0;
    }
    else {
        balance = height(ptr->m_left) - height(ptr->m_right);
    }

    return (abs(balance) <= 1 && isBalanced(ptr->m_left) && isBalanced(ptr->m_right));
}

bool SatNet::isBST() {
    bool balanced = true;
    isBSTPrivate(m_root, balanced);
    return balanced;
}

void SatNet::isBSTPrivate(Sat* ptr, bool& balanced) {
    if (ptr == nullptr) {
        return;
    }

    // recursively traverses through and makes sure left and right pointers are good
    isBSTPrivate(ptr->m_left, balanced);

    if (ptr->m_left != nullptr) {
        if (ptr->m_left->m_id > ptr->m_id) balanced = false;
    }
    if (ptr->m_right != nullptr) {
        if (ptr->m_right->m_id < ptr->m_id) balanced = false;
    }

    isBSTPrivate(ptr->m_right, balanced);
}
