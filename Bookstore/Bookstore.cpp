#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

const int MAX_ORDERS = 50;
const int NUM_BOOKS = 6;
const int BULK_QTY = 3;
const double BULK_DISCOUNT = 0.10;
const int LOYALTY_ORDERS = 3;
const double LOYALTY_DISCOUNT = 0.05;

string customerNames[MAX_ORDERS];
string phoneNumbers[MAX_ORDERS];
int    previousOrders[MAX_ORDERS];
int    orderLines[MAX_ORDERS][NUM_BOOKS];
int    orderCount = 0;

string bookNames[NUM_BOOKS] = { "Book A","Book B","Book C","Book D","Book E","Book F" };
double bookPrices[NUM_BOOKS] = { 120.00,85.50,99.99,150.00,75.00,45.50 };

bool isValidPhone(string p) {
    if (p.length() != 10) return false;
    for (char c : p) if (!isdigit(c)) return false;
    return true;
}

int findCustomer(string name) {
    for (int i = 0; i < MAX_ORDERS; i++)
        if (customerNames[i] == name) return i;
    return -1;
}

double calculateOrderTotal(int idx) {
    double subtotal = 0;
    for (int b = 0; b < NUM_BOOKS; b++) {
        int qty = orderLines[idx][b];
        if (qty > 0) {
            double line = qty * bookPrices[b];
            if (qty >= BULK_QTY) line *= (1 - BULK_DISCOUNT);
            subtotal += line;
        }
    }
    if (subtotal >= 100 && previousOrders[idx] > LOYALTY_ORDERS)
        subtotal *= (1 - LOYALTY_DISCOUNT);
    return subtotal;
}

void addOrder() {
    if (orderCount >= MAX_ORDERS) { cout << "Storage full.\n"; return; }
    string name, phone;
    cout << "Customer name: "; getline(cin, name);
    if (name.empty()) { cout << "Name required.\n"; return; }
    while (true) {
        cout << "Phone (10 digits): "; getline(cin, phone);
        if (isValidPhone(phone)) break;
        cout << "Invalid phone. Must be 10 digits.\n";
    }
    int idx = findCustomer(name);
    if (idx == -1) {
        idx = orderCount++;
        customerNames[idx] = name;
        phoneNumbers[idx] = phone;
        previousOrders[idx] = 0;
    }
    cout << "\nAvailable books:\n";
    for (int i = 0; i < NUM_BOOKS; i++)
        cout << "  " << i << ") " << bookNames[i] << " - R" << bookPrices[i] << "\n";

    int numLines;
    do { cout << "How many different books (1-5)? "; cin >> numLines; } while (numLines < 1 || numLines > 5);
    cin.ignore();

    for (int i = 0; i < NUM_BOOKS; i++) orderLines[idx][i] = 0;
    for (int l = 0; l < numLines; l++) {
        int bIdx, qty;
        do { cout << "Book index (0-5): "; cin >> bIdx; } while (bIdx < 0 || bIdx >= NUM_BOOKS);
        do { cout << "Quantity (1-10): "; cin >> qty; } while (qty < 1 || qty > 10);
        orderLines[idx][bIdx] += qty;
    }
    cin.ignore();
    previousOrders[idx]++;
    cout << "Order placed. Total: R"
        << fixed << setprecision(2) << calculateOrderTotal(idx) << "\n";
}

void listOrders() {
    if (orderCount == 0) { cout << "No orders yet.\n"; return; }
    cout << left << setw(15) << "Name" << setw(15) << "Phone" << "Total\n";
    cout << string(45, '-') << "\n";
    for (int i = 0; i < orderCount; i++) {
        cout << left << setw(15) << customerNames[i]
            << setw(15) << phoneNumbers[i]
            << "R" << fixed << setprecision(2) << calculateOrderTotal(i) << "\n";
    }
}

void searchCustomer() {
    string name;
    cout << "Name: "; getline(cin, name);
    int idx = findCustomer(name);
    if (idx == -1) { cout << "Not found.\n"; return; }
    cout << "Name:  " << customerNames[idx] << "\n";
    cout << "Phone: " << phoneNumbers[idx] << "\n";
    cout << "Prev:  " << previousOrders[idx] << " order(s)\n";
    cout << "Total: R" << fixed << setprecision(2) << calculateOrderTotal(idx) << "\n";
}

void mostPopularBook() {
    int bestIdx = 0, bestQty = 0;
    for (int b = 0; b < NUM_BOOKS; b++) {
        int total = 0;
        for (int o = 0; o < orderCount; o++) total += orderLines[o][b];
        if (total > bestQty) { bestQty = total; bestIdx = b; }
    }
    cout << "Most popular: " << bookNames[bestIdx]
        << " (" << bestQty << " copies sold)\n";
}

void sessionRevenue() {
    double total = 0;
    for (int i = 0; i < orderCount; i++) total += calculateOrderTotal(i);
    cout << "Session revenue: R" << fixed << setprecision(2) << total << "\n";
}

void displayMenu() {
    cout << "\n=== Pages & Prose ===\n";
    cout << "  1) Add order\n";
    cout << "  2) List orders\n";
    cout << "  3) Search customer\n";
    cout << "  4) Most popular book\n";
    cout << "  5) Session revenue\n";
    cout << "  6) Quit\n";
}

int main() {
    int choice;
    do {
        displayMenu();
        do { cout << "Choice: "; cin >> choice; } while (choice < 1 || choice > 6);
        cin.ignore();
        switch (choice) {
        case 1: addOrder();          break;
        case 2: listOrders();        break;
        case 3: searchCustomer();    break;
        case 4: mostPopularBook();   break;
        case 5: sessionRevenue();    break;
        case 6: cout << "Goodbye.\n"; break;
        }
    } while (choice != 6);
    return 0;
}