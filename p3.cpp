#include <iostream>
#include <vector>
#include <unordered_set>
#include "chainhash.h"

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// crear una lista desde un vector
ListNode* createList(const vector<int>& values) {
    if (values.empty()) return nullptr;
    
    ListNode* head = new ListNode(values[0]);
    ListNode* current = head;
    
    for (size_t i = 1; i < values.size(); i++) {
        current->next = new ListNode(values[i]);
        current = current->next;
    }
    
    return head;
}

// imprimir una lista
void printList(ListNode* head, int maxNodes = 10) {
    ListNode* current = head;
    int count = 0;
    
    cout << "[";
    while (current && count < maxNodes) {
        cout << current->val;
        if (current->next && count < maxNodes - 1) cout << " -> ";
        current = current->next;
        count++;
    }
    if (current) cout << " -> ...";
    cout << "]";
}

// encontrar un nodo por valor en una lista
ListNode* findNode(ListNode* head, int value) {
    while (head) {
        if (head->val == value) return head;
        head = head->next;
    }
    return nullptr;
}

#include <stack>
// TODO: implementar la conexión de las listas en el nodo con valor intersectVal
// Si intersectVal no se encuentra, las listas permanecen separadas   
void connectLists(ListNode*& listA, ListNode*& listB, int intersectVal) {
    //TODO: implemente aqui
    //Tuve que cambiar a que los punteros se pasen por referencia ya que sino no se podría modificar
    //la dirección de los punteros cabeza, lo que se necesita para el test 3. El nodo de intersección
    //sí o sí tiene que ser la cabeza para una resolución con sentido.
    //En los testcases no se asume el caso de que hayan varios nodos con el valor intersectVal
    //(ya que ningún test lo evalúa). Sino se tendría que hacer una implementación con dos stacks
    //comparando cada puntero uno por uno.

    ListNode* target = findNode(listB, intersectVal);

    if (target == nullptr) return;
    auto temp = listA;
    if (listA->val == intersectVal) {listA = target; return;}
    while (temp->next) {if (temp->next->val == intersectVal) {temp->next = target; return;} temp=temp->next;}
    return;
    }



// TODO: implementar el algoritmo para encontrar la intersección de dos listas
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    // TODO: implemente aqui
    ChainHash<ListNode*, bool> table(13);
    auto temp = headA;
    while (temp != nullptr) {table.set(temp, true); temp=temp->next;}
    temp = headB; cout << endl;
    while (temp != nullptr) {if (table.contains(temp)) return temp; temp=temp->next;}
    return nullptr;
}

void testIntersection(const vector<int>& listA, const vector<int>& listB, 
                     int intersectVal, const string& testName) {
    cout << "\n" << testName << "\n";
    
    ListNode* headA = createList(listA);
    ListNode* headB = createList(listB);

    // crear interseccion 
    if (intersectVal != -1) {
        connectLists(headA, headB, intersectVal);
    }

    cout << "Lista A: ";
    printList(headA);
    cout << "\nLista B: ";
    printList(headB);
    cout << "\n";

    // encontrar interseccion
    ListNode* intersection = getIntersectionNode(headA, headB);

    if (intersection) {
        cout << "Intersección encontrada en nodo con valor: " << intersection->val << "\n";
    } else {
        cout << "No hay intersección\n";
    }

    if (intersectVal == -1) {
        if (intersection == nullptr) {
            cout << "CORRECTO: No se esperaba intersección\n";
        } else {
            cout << "ERROR: Se encontró intersección cuando no debería\n";
        }
    } else {
        if (intersection && intersection->val == intersectVal) {
            cout << "CORRECTO: Intersección encontrada en el nodo esperado\n";
        } else {
            cout << "ERROR: Intersección incorrecta o no encontrada\n";
        }
    }
}

int main() {
    cout << "INTERSECCION DE LISTAS ENLAZADAS\n";
    cout << "Problema: Encontrar el nodo donde dos listas se intersectan\n";    
    
    // Caso 1: Intersección existe
    testIntersection({4, 1, 8, 4, 5}, {5, 6, 1, 8, 4, 5}, 8, 
                    "Caso 1: Con intersección");
    
    // Caso 2: Sin intersección
    testIntersection({2, 6, 4}, {1, 5}, -1, 
                    "Caso 2: Sin intersección");
    
    // Caso 3: Intersección al inicio
    testIntersection({3, 7, 1}, {3, 7, 1}, 3, 
                    "Caso 3: Intersección al inicio");
    
    // Caso 4: Una lista vacía
    testIntersection({1, 2, 3}, {}, -1, 
                    "Caso 4: Lista B vacía");
    
    // Caso 5: Listas de diferente longitud con intersección
    testIntersection({1, 9, 1, 2, 4}, {3, 2, 4}, 2, 
                    "Caso 5: Diferentes longitudes con intersección");
    
    return 0;
}
