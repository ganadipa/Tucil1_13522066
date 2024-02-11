#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string> 

#include "headers.hpp"
using namespace std;


void opening() {
    cout << "\n\n dP\"\"b8 Yb  dP 88\"\"Yb 888888 88\"\"Yb 88\"\"Yb 88   88 88b 88 88  dP   oP\"Yb.  dP\"Yb  888888P 888888P \n";
    cout << "dP   `\"  YbdP  88__dP 88__   88__dP 88__dP 88   88 88Yb88 88odP    \"' dP' dP   Yb     dP      dP  \n";
    cout << "Yb        8P   88\"\"Yb 88\"\"   88\"Yb  88\"\"\"  Y8   8P 88 Y88 88\"Yb      dP'  Yb   dP    dP      dP   \n";
    cout << " YboodP  dP    88oodP 888888 88  Yb 88     `YbodP' 88  Y8 88  Yb   .d8888  YbodP    dP      dP   \n\n";
}

int main() {
    opening();
    cout << "Tipe input:\n  1. Input spesifikasi 1 (file)\n  2. Input spesifikasi 2 (CLI)\n";

    int type = int_input("Masukkan angka: ", false);

    try {
        switch (type) {
            case 1: 
                program_one();
                break;
            case 2:
                program_two();
                cout << "\n\nMatriks yang digunakan: \n";
                for (auto &row: matrix) {
                    for (auto &col: row) {
                        cout << col << ' ';
                    } cout << '\n';
                }

                cout << "\n\nReward sekuens dan sekuens yang digunakan: \n";
                for (int i = 0; i < num_sequence; i++) {
                    cout << reward[i] << ": ";
                    for (auto &t: sequences[i]) cout << t << ' ';
                    cout << '\n';
                }

                break;
            default:
                throw runtime_error("Angka yang anda masukkan di luar yang diinginkan.\n");

        }
    } catch (const runtime_error& e) {
        cout << "\n\nError: " << e.what() << "\nProgram berhenti.\n";
        return 0;
    }

    cout << "\n\n";
    show_answer();
    cout << "\n\n";

    cout << "\nApakah ingin menyimpan solusi? [Y/n] ";
    bool save = bool_input();
    if (save) save_answer();

    return 0;
}