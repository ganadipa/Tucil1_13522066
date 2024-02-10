#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string> 
#include <set>

#include "headers.hpp"
using namespace std;

int buffer_size, width, height, num_sequence, duration, num_token, max_sequence;    
vector<vector<Token>> matrix, sequences;
vector<int> reward;
vector<Token> possible_token;

Buffer current, answer;
int num_op = 0;

void show_answer() {
    // output
    cout << answer.points << " points.\n";

    if (answer.points > 0) {

        for (auto &p: answer.visited) {
            cout << matrix[p.second][p.first] << ' ';
        }
        cout << '\n';

        for (auto &p: answer.visited) {
            cout << p.first + 1 << ", " << p.second + 1 << '\n';
        }

    } else cout << "Solusi memiliki panjang 0.\n";
    cout << "\n\n";
    cout << duration << " ms\n";
}

void save_answer() {
    cout << "Nama file: ";
    string name; cin >> name;

    

    bool rewrite = false;
    string path = "test/output/" + name;
    while (is_file_exist(path) && !rewrite) {
        cout << "\nWarning: Sudah ada file bernama " << name << ". Overwrite saja? Ketik nama baru jika tidak.\n";
        cout << "[Y/<nama-file>] ";
        string ans; cin >> ans;

        if (ans == "Y" || ans == "y") rewrite = true;
        else {
            name = ans;
            path = "test/output/" + name;
        }
    }

    ofstream output(path);
    if (output.is_open()) {
        auto origin = cout.rdbuf();
        cout.rdbuf(output.rdbuf());

        show_answer();
        cout.rdbuf(origin);
    } else {
        throw runtime_error("Terjadi suatu kesalahan saat file ingin disimpan.");
    }

    cout << "\n\nBerhasil menyimpan jawaban pada file " << name << " yang terletak di folder test/output.\n";
}

long long evaluate_points(vector<pair<int, int>>& visited) {
    long long result = 0;

    int length = (int) visited.size();
    vector<int> pointer(num_sequence);
    for (int i = 0; i < length; i++) {
        Token current = matrix[visited[i].second][visited[i].first];
        
        for (int j = 0; j < num_sequence; j++) {

            if (pointer[j] != -1) {
                if (current == sequences[j][pointer[j]]) pointer[j]++;
                else pointer[j] = 0;
            }

            if (pointer[j] == (int) sequences[j].size()) {
                result += reward[j];
                pointer[j] = -1;
            }
        }
    }

    return result;
}

void brute_force(int depth, bool search_horizontally, int which) {
    ++num_op;  
    if (depth == 0) return;

    if (search_horizontally) {
        for (int col = 0; col < width; col++) {
            
            // check whether it is possible to visit here.
            bool possible = true;
            int length = (int) current.visited.size(), i = 0;
            while (possible && i < length) {
                if (current.visited[i].first == col && current.visited[i].second == which) possible = false;
                i++;
            }

            if (!possible) continue;

            // If it is possible, try and evaluate the points gained.
            current.visited.push_back({col, which});
            

            // If points is greater than the computed answer, update.
            current.points = evaluate_points(current.visited);

            if (current.points >= answer.points) {
                if (current.visited.size() < answer.visited.size() || current.points > answer.points) {
                    answer = current;
                };
            }

            // Try to go more deep with the current configuration.
            brute_force(depth - 1, !search_horizontally, col);


            // Try another configuration.
            current.visited.pop_back();
        }
    } else {
        // This is just a mirror case of the above procedure.

        for (int row = 0; row < height; row++) {
            
            // check whether it is possible to visit here.
            bool possible = true;
            int length = (int) current.visited.size(), i = 0;
            while (possible && i < length) {
                if (current.visited[i].first == which && current.visited[i].second == row) possible = false;
                i++;
            }

            if (!possible) continue;

            // We can go here, so try and evaluate the points gained.
            current.visited.push_back({which, row});
            
            current.points = evaluate_points(current.visited);

            if (current.points >= answer.points) {
                if (current.visited.size() < answer.visited.size() || current.points > answer.points) {
                    answer = current;
                };
            }
            

            brute_force(depth - 1, !search_horizontally, row);
            current.visited.pop_back();
        }
    }
}


void program_one() {
    // Inisialisasi, membaca input dari file
    cout << "Masukkan nama file input yang berada di folder test/input: ";
    string fname; cin >> fname;
    fname = "test/input/" + fname;

    // Pastiin ada filenya
    ifstream file(fname);
    if(!file.is_open()) throw runtime_error("File tidak ditemukan, contoh pemakaian: input.txt");

    // Input dari file di mask jadi cin.
    auto origin = cin.rdbuf();
    cin.rdbuf(file.rdbuf());

    // Insert input to global variable
    buffer_size = int_input("", false);
    width = int_input("", false);
    height = int_input("", false);

    matrix.resize(height);
    for (int row = 0; row < height; row++) {
        matrix[row].resize(width);
        for (int col = 0; col < width; col++) {
            matrix[row][col] = token_input();
        }
    }

    num_sequence = int_input("", false);
    // Insert sequence and its reward
    sequences.resize(num_sequence);
    reward.resize(num_sequence);

    string line;
    cin.ignore(1000, '\n');
    for (int i = 0; i < num_sequence; i++) {
        getline(cin, line);
        line = trim(line, " \t");

        sequences[i] = (to_array(line));
        reward[i] = int_input("", false);

        cin.ignore(1000, '\n');
    }

    // bruteforcing and measure the duration
    auto start = chrono::high_resolution_clock::now();

    brute_force(buffer_size, true, 0);
    auto end = chrono::high_resolution_clock::now();
    duration = static_cast<int>(chrono::duration_cast<chrono::milliseconds>(end - start).count());

    // Go back to use CLI as its input.
    cin.rdbuf(origin);
}

void program_two() {
    cout << "\nJumlah token unik: ";
    num_token = int_input("", false);
    cin.ignore(1, '\n');

    // Memastikan token-token yang dimasukkan unik.
    bool unique = true;
    while (unique) {
        cout << "\nToken-token unik: ";
        string line;
        getline(cin, line);
        possible_token = to_array(line);
        if (possible_token.size() > num_token) {
            cout << "Panjang tidak konsisten dengan apa yang dimasukkan sebelumnya. Ulangi.\n";
            continue;
        }

        set<Token> s;
        for (int i = 0; i < (int) possible_token.size(); i++) {
            if (s.find(possible_token[i]) == s.end()) {
                s.insert(possible_token[i]);
            } else {
                unique =  false;
                break;
            }
        }

        if (unique) break;
        else {
            unique = true;
            cout << "\nToken - token yang anda masukkan tidak unik satu sama lain!\n";
            continue;
        }
    }
    

    cout << "Ukuran buffer: ";
    buffer_size = int_input("", false);

    cout << "Tinggi matriks (banyaknya baris): ";
    height = int_input("", false);

    cout << "Lebar matriks (banyaknya kolom): ";
    width = int_input("", false);

    bool valid = false;
    while (!valid) {
        cout << "\nJumlah sekuens: ";
        num_sequence = int_input("", false);

        cout << "Panjang maksimum sekuens: ";
        max_sequence = int_input("", false);

        long long possible_sequences = 0;
        for (int i = 2; i <= max_sequence; i++) {
            possible_sequences += binexp(num_token, i);
        }

        cout << possible_sequences << endl;

        if (possible_sequences < num_sequence) {
            cout << "Jika saya hitung, jumlah sekuens yang dapat digenerate hanya sebanyak " << possible_sequences << '\n';
            cout << "Jadi input anda tidak valid. Ulangi masukkan.\n";
        } else valid = true;
    }
    

    // Generate random matrix, sequence, and also random reward for each sequence;
    bool generate_another = false;
    while (!generate_another) {
        // 1. Generate random matrix
        matrix.clear();
        matrix.resize(height);
        for (int row = 0; row < height; row++) {
            matrix[row].resize(width);
            for (int col = 0; col < width; col++) {
                int random_token_index = random_range(0, num_token - 1);
                matrix[row][col] = possible_token[random_token_index];
            }
        }

        // 2. Generate random sequences (must be unique)
        sequences.clear();
        sequences.resize(num_sequence);
        set<vector<Token>> unique;
        for (int i = 0; i < num_sequence; i++) {
            int length = random_range(2, max_sequence);
            vector<Token> current;
            bool valid = false;

            while (!valid) {
                current = {};
                for (int j = 0; j < length; j++) {
                    int random_token_index = random_range(0, num_token - 1);
                    Token token = possible_token[random_token_index];
                    current.push_back(token);
                }


                if (unique.find(current) == unique.end()) {
                    valid = true;
                }
            }

            sequences[i] = current;
            unique.insert(current);
        }

        // 3. Generate random reward
        reward.clear();
        reward.resize(num_sequence);
        for (int i = 0; i < num_sequence; i++) {
            reward[i] = random_range(15, 40);
        }

        // Output generated.
        cout << "\nMatriks: \n";
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                cout << matrix[row][col] << ' ';
            }
            cout << endl;
        }

        cout << "\nPoint dan sequence-nya: \n";
        for (int i = 0; i < num_sequence; i++) {
            cout << reward[i] << ": ";

            for (auto &token: sequences[i]) {
                cout << token << ' ';
            }
            cout << '\n';
        }

        cout << "\nApakah ingin menggunakan konfigurasi matriks dan \nsequence yang telah digenerate di atas?\n";
        cout << "[Y/n] ";
        generate_another = bool_input(); 
    }

    auto start = chrono::high_resolution_clock::now();
    brute_force(buffer_size, true, 0);
    auto end = chrono::high_resolution_clock::now();
    duration = static_cast<int>(chrono::duration_cast<chrono::milliseconds>(end - start).count());
}