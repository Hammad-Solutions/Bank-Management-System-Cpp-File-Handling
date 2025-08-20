/*
   Code Written by: Muhammad Hammad
   Project: Simple Bank Management System
   Features:
   - Create Account (with CNIC check to prevent duplicates)
   - Deposit, Withdraw, Transfer
   - Transaction History
   - Save/Load from File
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Account {
    int accNo;
    string name;
    string cnic;
    string pin;
    double balance;
    string history;
};

Account accounts[100];
int accountCount = 0;
int nextAccNo = 100001;

void saveData() {
    ofstream file("bankdata.txt");
    for (int i = 0; i < accountCount; i++) {
        file << accounts[i].accNo << "|"
             << accounts[i].name << "|"
             << accounts[i].cnic << "|"
             << accounts[i].pin << "|"
             << accounts[i].balance << "|"
             << accounts[i].history << endl;
    }
    file.close();
}

void loadData() {
    ifstream file("bankdata.txt");
    if (!file) return;
    accountCount = 0;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        Account acc;
        size_t p1 = line.find("|");
        size_t p2 = line.find("|", p1 + 1);
        size_t p3 = line.find("|", p2 + 1);
        size_t p4 = line.find("|", p3 + 1);
        size_t p5 = line.find("|", p4 + 1);

        acc.accNo = stoi(line.substr(0, p1));
        acc.name = line.substr(p1 + 1, p2 - p1 - 1);
        acc.cnic = line.substr(p2 + 1, p3 - p2 - 1);
        acc.pin = line.substr(p3 + 1, p4 - p3 - 1);
        acc.balance = stod(line.substr(p4 + 1, p5 - p4 - 1));
        acc.history = line.substr(p5 + 1);

        accounts[accountCount++] = acc;
        if (acc.accNo >= nextAccNo) nextAccNo = acc.accNo + 1;
    }
    file.close();
}

bool cnicExists(const string &cnic) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].cnic == cnic) return true;
    }
    return false;
}

void createAccount() {
    Account acc;
    acc.accNo = nextAccNo++;
    cout << "Full Name: ";
    cin.ignore();
    getline(cin, acc.name);

    cout << "CNIC (xxxxx-xxxxxxx-x): ";
    getline(cin, acc.cnic);

    if (cnicExists(acc.cnic)) {
        cout << "Account already exists with this CNIC!" << endl;
        return;
    }

    cout << "Set 4-digit PIN: ";
    cin >> acc.pin;

    acc.balance = 0;
    acc.history = "Account created with balance 0;";
    accounts[accountCount++] = acc;
    cout << "Account created! Your Account No is: " << acc.accNo << endl;
}

int findAccount(int accNo) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accNo == accNo) return i;
    }
    return -1;
}

void deposit(Account &acc) {
    double amt;
    cout << "Enter amount to deposit: ";
    cin >> amt;
    if (amt <= 0) return;
    acc.balance += amt;
    acc.history += "Deposit " + to_string((int)amt) + " | Balance: " + to_string((int)acc.balance) + ";";
    cout << "Deposited successfully! New Balance: " << acc.balance << endl;
}

void withdraw(Account &acc) {
    double amt;
    cout << "Enter amount to withdraw: ";
    cin >> amt;
    if (amt > 0 && amt <= acc.balance) {
        acc.balance -= amt;
        acc.history += "Withdraw " + to_string((int)amt) + " | Balance: " + to_string((int)acc.balance) + ";";
        cout << "Withdrawn successfully! New Balance: " << acc.balance << endl;
    } else {
        cout << "Invalid amount!" << endl;
    }
}

void transfer(Account &acc) {
    int toAccNo;
    cout << "Enter account number to transfer: ";
    cin >> toAccNo;
    int idx = findAccount(toAccNo);
    if (idx == -1 || toAccNo == acc.accNo) {
        cout << "Invalid account number!" << endl;
        return;
    }
    cout << "Receiver: " << accounts[idx].name << endl;
    cout << "Do you confirm transfer? (y/n): ";
    char c;
    cin >> c;
    if (c != 'y' && c != 'Y') {
        cout << "Transaction cancelled." << endl;
        return;
    }
    double amt;
    cout << "Enter amount: ";
    cin >> amt;
    if (amt > 0 && amt <= acc.balance) {
        acc.balance -= amt;
        accounts[idx].balance += amt;
        acc.history += "Transfer to " + to_string(toAccNo) + " (" + accounts[idx].name + ") " + to_string((int)amt) + " | Balance: " + to_string((int)acc.balance) + ";";
        accounts[idx].history += "Transfer from " + to_string(acc.accNo) + " (" + acc.name + ") " + to_string((int)amt) + " | Balance: " + to_string((int)accounts[idx].balance) + ";";
        cout << "Transferred successfully!" << endl;
    } else {
        cout << "Invalid amount!" << endl;
    }
}

void showHistory(Account &acc) {
    cout << "--- Transaction History ---" << endl;
    string temp = acc.history;
    size_t pos = 0;
    while ((pos = temp.find(";")) != string::npos) {
        cout << temp.substr(0, pos) << endl;
        temp.erase(0, pos + 1);
    }
}

void accountMenu(Account &acc) {
    int choice;
    do {
        cout << "\n--- Account Menu (" << acc.name << ") ---\n";
        cout << "1. Deposit\n2. Withdraw\n3. Transfer\n4. History\n0. Logout\nSelect: ";
        cin >> choice;
        switch (choice) {
            case 1: deposit(acc); break;
            case 2: withdraw(acc); break;
            case 3: transfer(acc); break;
            case 4: showHistory(acc); break;
        }
    } while (choice != 0);
    saveData();
}

void login() {
    int accNo;
    string pin;
    cout << "Account No: ";
    cin >> accNo;
    cout << "PIN: ";
    cin >> pin;
    int idx = findAccount(accNo);
    if (idx != -1 && accounts[idx].pin == pin) {
        accountMenu(accounts[idx]);
    } else {
        cout << "Invalid login!" << endl;
    }
}

int main() {
    loadData();
    int choice;
    do {
        cout << "\n======= BANK SYSTEM =======\n";
        cout << "1. Create Account\n2. Login\n0. Exit\nSelect: ";
        cin >> choice;
        switch (choice) {
            case 1: createAccount(); break;
            case 2: login(); break;
        }
    } while (choice != 0);
    saveData();
    return 0;
}
