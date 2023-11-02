#include "satnet.h"

class Tester {

public:
    double sampleTimeMeasurement(SatNet & aNet, int tempArray[], int arraySize){
        double T = 0.0;//stores running times
        clock_t start, stop;//stores the clock ticks while running the program
        start = clock();
        // the algorithm to be measured
        for (int j=0;j<arraySize;j++){
            aNet.findSatellite(tempArray[j]);
        }
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        double measureTime = T/CLOCKS_PER_SEC;//time in seconds
        return measureTime;
    }

    bool constructorNormal() {
        SatNet net;

        return net.m_root == nullptr;
    }

    bool insertNormal() {
        SatNet net;

        net.insert(11000);
        net.insert(10050);

        return (net.m_root->m_left->m_id == 10050);
    }

    bool insertEdge() {
        SatNet net;

        net.insert(11000);
        net.insert(10000);

        return (net.m_root->m_left->m_id == 10000);
    }

    bool insertError() {
        SatNet net;

        net.insert(11000);
        net.insert(500);

        return (net.m_root->m_left == nullptr && net.m_root->m_right == nullptr);
    }

    bool clearNormal() {
        SatNet net;

        net.insert(11000);
        net.insert(10000);

        net.clear();

        return (net.m_root == nullptr);
    }

    bool clearEdge() {
        SatNet net;
        net.clear();
        return (net.m_root == nullptr);
    }

    bool removeNormal() {
        SatNet net;

        net.insert(11000);
        net.insert(11500);

        net.remove(11500);

        return (net.m_root->m_right == nullptr);
    }

    bool removeError() {
        SatNet net;

        net.insert(11000);
        net.insert(11500);

        net.remove(12000);

        return (net.m_root->m_right != nullptr);
    }

    bool setStateNormal() {
        SatNet net;

        net.insert(11000);
        net.insert(11500);

        net.setState(11500, DECAYING);

        return (net.m_root->m_right->m_state == DECAYING);
    }

    bool setStateError() {
        SatNet net;

        net.insert(11000);
        net.insert(11500);

        net.setState(12000, DECAYING);

        return (net.m_root->m_state != DECAYING && net.m_root->m_right->m_state != DECAYING);
    }

    bool removeDeorbitedNormal() {
        SatNet net;

        net.insert(11000);
        net.insert(11500);
        net.insert(10500);

        net.setState(10500, DEORBITED);
        net.setState(11500, DEORBITED);

        net.removeDeorbited();

        return (net.m_root->m_right == nullptr && net.m_root->m_left == nullptr);
    }

    bool removeDeorbitedError() {
        SatNet net;

        net.insert(11000);
        net.insert(11500);
        net.insert(10500);
        
        net.removeDeorbited();

        return (net.m_root && net.m_root->m_left && net.m_root->m_right);
    }

    bool findSatelliteNormal() {
        SatNet net;

        net.insert(11000);
        net.insert(11500);
        net.insert(10500);
        net.insert(13000);
        net.insert(15470);

        return net.findSatellite(13000);
    }

    bool findSatelliteError() {
        SatNet net;

        net.insert(11000);
        net.insert(11500);
        net.insert(10500);
        net.insert(13000);
        net.insert(15470);

        if (net.findSatellite(17000)) {
            return false;
        }
        else {
            return true;
        }
    }

    bool countSatellitesNormal() {
        SatNet net;

        net.insert(11000);
        net.insert(11500);
        net.insert(10500);

        net.m_root->setInclin(I53);
        net.m_root->m_left->setInclin(I53);

        return (net.countSatellites(I53) == 2);
    }

    bool balancedAfterInsertions() {
        SatNet net;

        for (int i = 0; i < 300; i++) {
            net.insert(i + 11000);
        }

        return net.isBalanced(net.m_root);
    }

    bool balancedAfterDeletions() {
        SatNet net;

        for (int i = 0; i < 300; i++) {
            net.insert(i + 11000);
        }

        for (int i = 0; i < 150; i++) {
            net.remove(i + 11000);
        }

        return net.isBalanced(net.m_root);
    }

    bool BSTPropertyAfterInsertions() {
        SatNet net;

        for (int i = 0; i < 300; i++) {
            net.insert(i + 11000);
        }

        return net.isBST();
    }

    bool BSTPropertyAfterDeletions() {
        SatNet net;

        for (int i = 0; i < 300; i++) {
            net.insert(i + 11000);
        }

        for (int i = 0; i < 150; i++) {
            net.remove(i + 11000);
        }

        return net.isBST();
    }

    bool assignmentOperatorNormal() {
        SatNet net1;

        for (int i = 0; i < 300; i++) {
            net1.insert(i + 11000);
        }

        SatNet net2;
        net2 = net1;

        return trueAssignment(net1.m_root, net2.m_root);
    }

    bool assignmentOperatorEdge() {
        SatNet net1;
        SatNet net2;

        for (int i = 0; i < 300; i++) {
            net1.insert(i + 11000);
            net2.insert(i + 11000);
        }

        net2 = net1;

        return trueAssignment(net1.m_root, net2.m_root);
    }

    bool assignmentOperatorError() {
        SatNet net1;
        SatNet net2;

        net2 = net1;

        return trueAssignment(net1.m_root, net2.m_root);
    }

    bool runtimeCheckInsert() {
        SatNet aNet;
        int arraySize = 1000;
        int tempArray[1000];
        for (int i = 0; i < arraySize; i++) {
            tempArray[i] = (i + 10000);
        }
        double T = 0.0; //stores running times
        clock_t start, stop; //stores the clock ticks while running the program
        start = clock();
        // the algorithm to be measured
        for (int j=0;j<arraySize;j++){
            aNet.insert(tempArray[j]);
        }
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        double T1 = T/CLOCKS_PER_SEC;//time in seconds

        SatNet bNet;
        int arraySize2 = 2000;
        int tempArray2[2000];
        for (int i = 0; i < arraySize2; i++) {
            tempArray2[i] = (i + 10000);
        }
        double Tx = 0.0; //stores running times
        clock_t start2, stop2; //stores the clock ticks while running the program
        start2 = clock();
        // the algorithm to be measured
        for (int j=0;j<arraySize2;j++){
            aNet.insert(tempArray2[j]);
        }
        stop2 = clock();
        Tx = stop2 - start2;//number of clock ticks the algorithm took
        double T2 = Tx/CLOCKS_PER_SEC;//time in seconds

        return (((1.1 - 0.4) < (T2 / (2*T1))) && ((T2 / (2*T1)) < (1.1 + 0.4)));
    }

    bool runtimeCheckRemove() {
        SatNet aNet;
        int arraySize = 1000;
        int tempArray[1000];
        for (int i = 0; i < arraySize; i++) {
            tempArray[i] = (i + 10000);
        }
        double T = 0.0; //stores running times
        clock_t start, stop; //stores the clock ticks while running the program
        for (int j=0;j<arraySize;j++){
            aNet.insert(tempArray[j]);
        }

        start = clock();
        for (int j=0;j<arraySize;j++){
            aNet.remove(tempArray[j]);
        }
        stop = clock();
        T = stop - start;
        double T1 = T/CLOCKS_PER_SEC;//time in seconds


        SatNet bNet;
        int arraySize2 = 2000;
        int tempArray2[2000];
        for (int i = 0; i < arraySize2; i++) {
            tempArray2[i] = (i + 10000);
        }
        double Tx = 0.0; //stores running times
        clock_t start2, stop2; //stores the clock ticks while running the program
        for (int j=0;j<arraySize2;j++){
            bNet.insert(tempArray2[j]);
        }

        start2 = clock();
        for (int j=0;j<arraySize2;j++){
            bNet.remove(tempArray2[j]);
        }
        stop2 = clock();
        Tx = stop2 - start2;
        double T2 = Tx/CLOCKS_PER_SEC;//time in seconds

        return (((1.1 - 0.4) < (T2 / (2*T1))) && ((T2 / (2*T1)) < (1.1 + 0.4)));
    }


    // helper functions
    private:
    
    bool trueAssignment(Sat* root1, Sat* root2) {
        // both empty
        if (root1 == nullptr && root2 == nullptr) return true;

        // only 1 is empty
        else if (root1 == nullptr || root2 == nullptr) return false;

        if (root1->m_id != root2->m_id) return false;
        if (root1->m_altitude != root2->m_altitude) return false;
        if (root1->m_inclin != root2->m_inclin) return false;
        if (root1->m_state != root2->m_state) return false;

        return trueAssignment(root1->m_left, root2->m_left) && trueAssignment(root1->m_right, root2->m_right);
    }
};

int main() {
    Tester test;

    // tests constructor normal case
    if (test.constructorNormal()) {
        cout << "Constructor normal case passed" << endl;
    }
    else {
        cout << "Constructor normal case failed" << endl;
    }
    cout << endl;

    // tests insert cases
    if (test.insertNormal()) {
        cout << "Normal insert case passed" << endl;
    }
    else {
        cout << "Normal insert case failed" << endl;
    }

    if (test.insertEdge()) {
        cout << "Edge insert case passed" << endl;
    }
    else {
        cout << "Edge insert case failed passed" << endl;
    }

    if (test.insertError()) {
        cout << "Error insert case passed" << endl;
    }
    else {
        cout << "Error insert case failed" << endl;
    }
    cout << endl;

    // tests clear cases
    if (test.clearNormal()) {
        cout << "Normal Clear case passed" << endl;
    }
    else {
        cout << "Normal Clear case failed" << endl;
    }

    if (test.clearEdge()) {
        cout << "Edge clear case passed" << endl;
    }
    else {
        cout << "Edge clear case failed" << endl;
    }
    cout << endl;

    // tests remove cases
    if (test.removeNormal()) {
        cout << "Normal remove case passed" << endl;
    }
    else {
        cout << "Normal remove case failed" << endl;
    }

    if (test.removeError()) {
        cout << "Error remove case passed" << endl;
    }
    else {
        cout << "Error remove case failed" << endl;
    }
    cout << endl;

    // tests setState cases
    if (test.setStateNormal()) {
        cout << "Normal setState case passed" << endl;
    }
    else {
        cout << "Normal setState case failed" << endl;
    }

    if (test.setStateError()) {
        cout << "Error setState case passed" << endl;
    }
    else {
        cout << "Error setState case failed" << endl;
    }
    cout << endl;

    // tests remove deorbited cases
    if (test.removeDeorbitedNormal()) {
        cout << "Normal removeDeorbited case passed" << endl;
    }
    else {
        cout << "Normal removeDeorbited case failed" << endl;
    }

    if (test.removeDeorbitedError()) {
        cout << "Error removeDeorbited case passed" << endl;
    }
    else {
        cout << "Error removeDeorbited case failed" << endl;
    }
    cout << endl;

    // tests find satellite cases
    if (test.findSatelliteNormal()) {
        cout << "Normal findSatellite case passed" << endl;
    }
    else {
        cout << "Normal findSatellite case failed" << endl;
    }

    if (test.findSatelliteError()) {
        cout << "Error findSatellite case passed" << endl;
    }
    else {
        cout << "Error findSatellite case failed" << endl;
    }
    cout << endl;

    // tests count satellites case
    if (test.countSatellitesNormal()) {
        cout << "Normal countSatellites case passed" << endl;
    }
    else {
        cout << "Normal countSatellites case failed" << endl;
    }
    cout << endl;

    // tests if balanced after insertions and deletions
    if (test.balancedAfterInsertions()) {
        cout << "Insertion balance test passed" << endl;
    }
    else {
        cout << "Insertion balance test failed" << endl;
    }
    
    if (test.balancedAfterDeletions()) {
        cout << "Deletion balance test passed" << endl;
    }
    else {
        cout << "Deletion balance test failed" << endl;
    }
    cout << endl;

    // tests if still bst after insertions and deletions
    if (test.BSTPropertyAfterInsertions()) {
        cout << "BST property after insertions passed" << endl;
    }
    else {
        cout << "BST property after insertions failed" << endl;
    }

    if (test.BSTPropertyAfterDeletions()) {
        cout << "BST property after deletions passed" << endl;
    }
    else {
        cout << "BST property after deletions failed" << endl;
    }
    cout << endl;

    // tests assignment operator cases
    if (test.assignmentOperatorNormal()) {
        cout << "Normal assignment operator case passed" << endl;
    }
    else {
        cout << "Normal assignment operator case failed" << endl;
    }

    if (test.assignmentOperatorEdge()) {
        cout << "Edge assignment operator case passed" << endl;
    }
    else {
        cout << "Edge assignment operator case failed" << endl;
    }

    if (test.assignmentOperatorError()) {
        cout << "Error assignment operator case passed" << endl;
    }
    else {
        cout << "Error assignment operator case failed" << endl;
    }
    cout << endl;

    // tests runtimes
    if (test.runtimeCheckInsert()) {
        cout << "The insert runtime is O(log n) and passed" << endl;
    }
    else {
        cout << "The insert runtime is not O(log n) and failed" << endl;
    }
     if (test.runtimeCheckRemove()) {
        cout << "The remove runtime is O(log n) and passed" << endl;
     }
     else {
        cout << "The remove runtime is not O(log n) and failed" << endl;
     }

    return 0;
}