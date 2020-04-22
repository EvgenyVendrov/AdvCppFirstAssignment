#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <stdlib.h>

using namespace std;

class Document
{
    //data members
    vector<string> _input_text;
    vector<string> _output_text;
    vector<string>::iterator _curr_line;
    //private functions
    void insert_to_input_vec(const string &to_insert) { _input_text.push_back(to_insert); };
    void goto_print_tab_on_lineNum(const string &line_num);
    void print_all();
    void push_text_after(const int num_of_lines, string &text);
    void push_text_before(const int line_num, string &text);
    void push_text_instead(const int line_num, string &text);
    void delete_line(const int line_to_delete);
    void search_fow(const string &word);
    void search_back(const string &word);
    void swap(const string &old_word, const string &new_word);
    void connect(const int line_num1, const int line_num2);
    void exit_editor() const;
    void parse_write_command(vector<string>::iterator it);
    void parse_non_write_command(vector<string>::iterator it);
    vector<string>::iterator get_input_iter_begin() { return _input_text.begin(); };
    vector<string>::iterator get_input_iter_end() { return _input_text.end(); };
    vector<string>::iterator get_output_iter_begin() { return _output_text.begin(); };
    vector<string>::iterator get_output_iter_end() { return _output_text.end(); };
    vector<string>::const_iterator get_output_const_iter_begin() const { return _output_text.cbegin(); };

public:
    void fill_input_vec_from_cin();
    void parse_input_text();
};
