#include "Document.h"

//=================>PUBLIC FUNCTIONS<========================

void Document::fill_input_vec_from_cin()
{
    //input data is transfered by a redirection of a text file to this process - so we read it from cin
    string line;
    while (getline(cin, line))
    {
        if (line.find("\r") == string::npos) //this way it would work both on windows and linux
        {
            insert_to_input_vec(line + "\r");
        }
        else
        {
            insert_to_input_vec(line);
        }
    }
    cin.clear();
    //starting the current line from the beginning of the now empty output vector
    _curr_line = get_output_iter_begin();
};

void Document::parse_input_text()
{
    regex non_write_commands("\\d+n|%p|\\d+d|^/[A-Za-z0-9]+|^\\?[A-Za-z0-9]+|^s/[A-Za-z0-9]+/[A-Za-z0-9]+|[0-9]+,[0-9]+j|Q");
    smatch match;
    //parsing the whole input text - line by line
    for (auto it = get_input_iter_begin(); it != get_input_iter_end(); ++it)
    {
        //checking if it is a write command
        if (it->compare("a\r") == 0 || it->compare("i\r") == 0 || it->compare("c\r") == 0)
        {
            parse_write_command(it);
        }
        //or it is non - write commend
        else if (regex_search(*it, match, non_write_commands))
        {
            parse_non_write_command(it);
        }
    }
};

//=================>PRIVATE FUNCTIONS<========================

void Document::goto_print_tab_on_lineNum(const string &line_num)
{
    int line_num_n = stoi(line_num) - 1;                 //getting number of line as an int
    string extra = line_num + "\t";                      //should print tab
    cout << extra + _output_text.at(line_num_n) << endl; //printing
    _curr_line = get_output_iter_begin() + +line_num_n;  //moving the pointed to line
};

void Document::print_all()
{
    for (auto it = get_output_const_iter_begin(); it != get_output_iter_end(); ++it)
    {
        cout << *it << endl;
    }
    _curr_line = get_output_iter_end() - 1;
};

void Document::push_text_after(const int num_of_lines_added, string &text)
{
    int how_much_moved = 0;
    if (_curr_line == get_output_iter_begin())
    {
        while (text.find("\r") != string::npos) //parsing string to get line by line
        {
            string to_push = text.substr(0, text.find("\r") + 1);
            _curr_line = _output_text.emplace(get_output_iter_begin() + how_much_moved, to_push);
            how_much_moved++;
            text.replace(0, to_push.size(), "");
        }
    }
    else
    {
        if (distance(get_output_iter_end(), _curr_line + num_of_lines_added) <= 0) //making sure the vector is big enough
        {
            _output_text.resize(_output_text.size() + num_of_lines_added);
        }
        while (text.find("\r") != string::npos) //parsing string to get line by line
        {
            string to_push = text.substr(0, text.find("\r") + 1);
            _curr_line = _output_text.emplace(_curr_line + 1, to_push);
            text.replace(0, to_push.size(), "");
        }
    }
};

void Document::push_text_before(const int line_num, string &text)
{
    int how_much_moved = 0;
    while (text.find("\r") != string::npos) //parsing string to get line by line
    {
        string to_push = text.substr(0, text.find("\r") + 1);
        _curr_line = _output_text.emplace(_curr_line - how_much_moved, to_push);
        text.replace(0, to_push.size(), "");
        how_much_moved--;
    }
    --_curr_line; //validating we point to the current line
};

void Document::push_text_instead(const int line_num, string &text)
{
    _output_text.erase(_curr_line);
    int how_much_moved = 0;
    while (text.find("\r") != string::npos) //parsing string to get line by line
    {
        string to_push = text.substr(0, text.find("\r") + 1);
        _curr_line = _output_text.emplace(_curr_line - how_much_moved, to_push);
        text.replace(0, to_push.size(), "");
        how_much_moved--;
    }
    --_curr_line; //validating we point to the current line
};

void Document::delete_line(const int line_to_delete)
{
    _output_text.erase(get_output_iter_begin() + line_to_delete);
    _curr_line = get_output_iter_begin() + line_to_delete; //validating we point to the current line
};

void Document::search_fow(const string &word)
{
    for (auto it = _curr_line; it != get_output_iter_end(); ++it) //searching for the word - going foward from current line
    {
        if (it->find(word) != string::npos)
        {
            _curr_line = it;
            cout << *_curr_line << endl; //line found should be printed
            return;
        }
    }
    _curr_line = get_output_iter_end() - 1; //if we haven't found thw word - go to the last line
    cout << *_curr_line << endl;            //line found should be printed
};

void Document::search_back(const string &word)
{
    for (auto it = get_output_iter_begin(); it != _curr_line + 1; ++it) //searching for the word - going backwards from current line
    {
        if (it->find(word) != string::npos)
        {
            _curr_line = it;
            cout << *_curr_line << endl; //line found should be printed
            return;
        }
    }
    _curr_line = get_output_iter_begin(); //if we haven't found thw word - go to the last line
    cout << *_curr_line << endl;
};

void Document::swap(const string &old_word, const string &new_word)
{
    while (_curr_line->find(old_word) != string::npos) //as long as there are "old words" haven been replaced - keep replacing
    {

        _curr_line->replace(_curr_line->find(old_word), old_word.size(), new_word);
    }
};

void Document::connect(const int line_num1, const int line_num2)
{
    if (_output_text[line_num1].find("\r") != string::npos) //if there is a "new line" - delete it to connect the lines
    {
        _output_text[line_num1].replace(_output_text[line_num1].find("\r"), 2, "");
    }
    _output_text[line_num1] = _output_text[line_num1].append(_output_text[line_num2]); //append strings
    _output_text.erase(get_output_iter_begin() + line_num2);
    _curr_line = get_output_iter_begin() + line_num1; //validate current lines location
};

void Document::exit_editor() const
{
    exit(0);
};

void Document::parse_write_command(vector<string>::iterator it)
{
    int num_of_lines = 0; //counter for num of lines to be added - to make sure we wont pass vectors size
    string all_lines_to_write = "";
    if (it->compare("a\r") == 0)
    {
        while (it != get_input_iter_end() && it->compare(".\r") != 0)
        {
            ++it;
            if (it->compare(".\r") != 0)
            {
                all_lines_to_write += *it;
                num_of_lines++;
            }
        }
        push_text_after(num_of_lines, all_lines_to_write);
    }

    if (it->compare("i\r") == 0)
    {
        while (it != get_input_iter_end() && it->compare(".\r") != 0)
        {
            ++it;
            if (it->compare(".\r") != 0)
            {
                all_lines_to_write += *it;
                num_of_lines++;
            }
        }
        push_text_before(num_of_lines, all_lines_to_write);
    }

    if (it->compare("c\r") == 0)
    {
        while (it != get_input_iter_end() && it->compare(".\r") != 0)
        {
            ++it;
            if (it->compare(".\r") != 0)
            {
                all_lines_to_write += *it;
                num_of_lines++;
            }
        }
        push_text_instead(num_of_lines, all_lines_to_write);
    }
};

void Document::parse_non_write_command(vector<string>::iterator it)
{
    //regex to find the relevant command in input text
    regex n("\\d+n");
    regex p("%p");
    regex d("\\d+d");
    regex fow_search("^/[A-Za-z0-9]+");
    regex back_search("^\\?[A-Za-z0-9]+");
    regex swap("^s/[A-Za-z0-9]+/[A-Za-z0-9]+");
    regex j("[0-9]+,[0-9]+j");
    regex q("Q");
    smatch match;

    if (regex_search(*it, match, n))
    {
        string line_num = it->substr(0, it->find("n")); //taking only the number itself
        goto_print_tab_on_lineNum(line_num);
    }

    if (regex_search(*it, match, p))
    {
        print_all();
    }

    if (regex_search(*it, match, d))
    {
        string num_of_line_to_delete = it->substr(0, it->find("d")); //taking only the number itself
        delete_line(stoi(num_of_line_to_delete) - 1);
    }

    if (regex_search(*it, match, fow_search))
    {
        string word = it->substr(1); //the word to search - parsed from the command
        search_fow(word);
    }

    if (regex_search(*it, match, back_search))
    {
        string word = it->substr(1); //the word to search - parsed from the command
        search_back(word);
    }

    if (regex_search(*it, match, swap))
    {
        //parsing the command string to get only the words needed to swap between
        it->replace(it->find("s/"), 2, "");
        string old_word = it->substr(0, it->find("/"));
        it->replace(it->find(old_word), old_word.size() + 1, "");
        string new_word = it->substr(0, it->find("/"));
        Document::swap(old_word, new_word);
    }

    if (regex_search(*it, match, j))
    {
        //parsing only the numbers from the command string
        string first_line = it->substr(0, it->find(","));
        string sec_line = it->substr(it->find(",") + 1, it->find("j") - 2);
        connect(stoi(first_line) - 1, stoi(sec_line) - 1);
    }

    if (regex_search(*it, match, q))
    {
        exit_editor();
    }
};