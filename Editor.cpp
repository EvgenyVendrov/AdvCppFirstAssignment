#include "Editor.h"


void Editor::loop()
{
    _doc.fill_input_vec_from_cin();//filling the document with the input text from the redirect
    _doc.parse_input_text();//parsing this text and executing the commands found
}