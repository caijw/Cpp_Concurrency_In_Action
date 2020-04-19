#include <thread>
#include <string>
#include <iostream>

void open_document_and_display_gui(std::string const& filename)
{
    std::cout << "open_document_and_display_gui" << std::endl;
}

bool done_editing()
{
    std::cout << "done_editing" << std::endl;
    return true;
    // return false;
}

enum command_type{
    open_new_document
};


struct user_command
{
    command_type type;

    user_command():
        type(open_new_document)
    {}
};

user_command get_user_input()
{
    std::cout << "get_user_input" << std::endl;
    return user_command();
}

std::string get_filename_from_user()
{
    std::cout << "get_filename_from_user" << std::endl;
    return "foo.doc";
}

void process_user_input(user_command const& cmd)
{
    std::cout << "process_user_input" << std::endl;
}

void edit_document(std::string const& filename)
{
    std::cout << "edit_document" << std::endl;
    open_document_and_display_gui(filename);
    while(!done_editing())
    {
        user_command cmd=get_user_input();
        if(cmd.type==open_new_document)
        {
            std::string const new_name=get_filename_from_user();
            std::thread t(edit_document,new_name);
            t.detach();
        }
        else
        {
            process_user_input(cmd);
        }
    }
}

int main()
{
    edit_document("bar.doc");
}
