#ifndef __COMMENT_HPP__
#define __COMMENT_HPP__

#include <iostream>
#include <vector>
#include "../server/server.hpp"
typedef std::vector <std::string> Record;

class Comment
{
public:
    Comment(Record comment_info);
    int get_time();
    void show_info();
    std::string get_username();
private:
    std::string username;
    int time;
    std::string text;
};

#endif