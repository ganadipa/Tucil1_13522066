#include <iostream>
#include <vector>
#include <random>
#include <fstream>

#include "headers.hpp"
using namespace std;

long long binexp(int a, int b) {
    if (b == 0) return 1;
    long long square_root = binexp(a, b/2);
    if (b&1) {
        return square_root*square_root*a;
    } else {
        return square_root*square_root;
    }

}

Token token_validator(string token) {
    if (token.size() != 2) throw runtime_error("Terdeteksi sebuah non-token pada masukkan yang seharusnya token.");

    for (int i = 0; i < 2; i++) {
        if (token[i] - 'A' <= 25 && token[i] - 'A' >= 0) continue;
        else if (token[i] - '0' <= 9 && token[i] - '0' >= 0) continue;
        else throw runtime_error("Terdeteksi sebuah non-token pada masukkan yang seharusnya token.");
    }

    return token;
}

vector<Token> to_array(string line) {

    vector<Token> result;
    int length = (int) line.size();
    Token current = "";
    for (int i = 0; i < length; i++) {
        if (i % 3 == 2) {
            if (line[i] != ' ') throw runtime_error("Terdeteksi sebuah non-token pada masukkan yang seharusnya token.");
            else {
                current = token_validator(current);
                result.push_back(current);
                current = "";
                continue;
            }
        } 
        
        current += line[i];
    }
    current = token_validator(current);
    result.push_back(current);

    return result;
}

Token token_input() {
    string token;
    cin >> token;
    token = token_validator(token);
    return token;
}

int int_input(string message, bool force) {
    int user_input;

    while (true) {
        try {
            cout << message;
            cin >> user_input;

            if (cin.fail()) {
                if (!force) throw runtime_error("Terdeteksi sebuah non-angka, input diharapkan angka.");
                else throw runtime_error("Input bukan berupa angka. Masukkanlah angka.");
            }

        } catch (const runtime_error& e) {
            
            if (force){
                cerr << "Message: " << e.what() << endl;
                continue;
            } else {
                throw runtime_error(e.what());
            }
        }

        break;
    }

    return user_input;
}

bool bool_input() {
    string ans;
    cin >> ans;

    if (ans == "Y" || ans == "y") return true;
    else if (ans == "n" || ans == "N") return false;
    else throw runtime_error("Input di luar jawaban yang disediakan.");
}

int random_range(int start, int end) {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> distribution(start, end);
    return distribution(gen);
}

bool is_file_exist(string &fname) {
    ifstream file(fname);
    return file.good();
}

string trim(const string& str,
                 const string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}