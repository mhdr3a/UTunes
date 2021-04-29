#include "Comment.hpp"
using namespace std;

Comment::Comment(Record comment_info)
{
    username = comment_info[0];
    time = stoi(comment_info[1]);
    text = comment_info[2];
}

int Comment::get_time()
{
    return time;
}

void Comment::show_info()
{
    cout << time << ' ' << username << ": ";
    cout << text << '\n';
}

string Comment::get_username()
{
    return username;
}