#include <iostream>
using namespace std;

int numRes = 6;
int numProc = 7;
int** resourceGraph;

/*
The first is a display function.  
It should simply dump the 2d array to the screen with a double for loop.  
The other is the deadlockCheck function.  
This is the function that will perform the cycle check and return 1 if there was a cycle and 0 if there wasn't.
*/

int deadlockCheck() {

    bool* visitedProc = (bool*)malloc(numProc*sizeof(bool));
    bool* visitedRes = (bool*)malloc(numRes*sizeof(bool));
    for (int i = 0; i < numProc; ++i) {
        
        for (int j = 0; j < numRes; ++j) {
            if (resourceGraph[i][j] == 1) {
                visitedProc[i] = true;
            }
            if (resourceGraph[i][j] == -1) {
                visitedRes[j] = true;
            }
            if (visitedProc && visitedRes) {
                return 1; // Cycle detected
            }
        }
    }

    return 0; // No cycle detected
}




void deadlockTester() {

    // First make a resource graph that has a cycle
    // Use the one from the slides
    // Init the resource Graph
    // It has 7 processes (A-G) and 6 resources (R-W)
    //   but we will call the processes 0-6 and the resources 0-5
    // Arcs out of process node are +1, into process node are -1
    resourceGraph = new int*[numProc];
    for (int i=0; i<numProc; i++) {
        resourceGraph[i] = new int[numRes];
        for (int j=0; j<numRes; j++) {
            resourceGraph[i][j] = 0;
            //print each item in the array
            cout << resourceGraph[i][j] << " "; 
        }
        cout << resourceGraph[i] << "\n";
    }

    // Hard code the graph from the slides
    resourceGraph[0][0] = -1;  // R->A
    resourceGraph[0][1] = +1;  // A->S

    resourceGraph[1][2] = +1;  // B->T

    resourceGraph[2][1] = +1;  // C->S

    resourceGraph[3][1] = +1;  // D->S
    resourceGraph[3][2] = +1;  // D->T
    resourceGraph[3][3] = -1;  // U->D

    resourceGraph[4][2] = -1;  // T->E
    resourceGraph[4][4] = +1;  // E->V

    resourceGraph[5][1] = +1;  // F->S
    resourceGraph[5][5] = -1;  // W->F

    resourceGraph[6][3] = +1;  // G->U
    resourceGraph[6][4] = -1;  // V->G


    // Check for cycle
    int deadlock = deadlockCheck();
    if (deadlock == 1) {
        cout << "Deadlock detected\n";
    } else {
        cout << "No deadlock detected\n";
    }
}



int main() {
    deadlockTester();
    exit(0);
    return 0;
}