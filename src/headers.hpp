#include <iostream>
#include <vector>
using namespace std;

typedef string Token;

struct Buffer {
    vector<pair<int, int>> visited;
    long long points;
};

#ifndef HEADERS_HPP
#define HEADERS_HPP

extern int buffer_size, width, height, num_sequence, duration, num_token, max_sequence, total_reward;    
extern vector<vector<Token>> matrix, sequences;
extern vector<int> reward;
extern vector<Token> possible_token;

extern Buffer current, answer;
extern int num_op;
extern bool done;

int int_input(string message, bool force);

Token token_input();

vector<Token> to_array(string line);

int random_range(int start, int end);

bool bool_input();

bool is_file_exist(string &fname);

void show_answer();

void save_answer();

long long evaluate_points(vector<pair<int, int>>& visited);

void brute_force(int depth, bool search_horizontally, int which);

void program_one();

void program_two();

string trim(const string& str,
                 const string& whitespace);

long long binexp(int a, int b);

#endif