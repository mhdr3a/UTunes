#ifndef __SONG_HPP__
#define __SONG_HPP__

#include "Comment.hpp"
#include <algorithm>
#include <iomanip>

const std::string DETAIL_FLAG = "#";
const std::string LIKES = "likes";
const std::string COMMENTS = "comments";
const std::string PLAYLISTS = "playlists";
const std::string EMPTY = "Empty";
const int PRECISION = 2;

class Song
{
public:
    Song(Record song_info);
    Song(const Song&);
    ~Song();
    Song& operator=(const Song&);
    int get_id();
    void inc_likes();
    void dec_likes();
    void add_to_comments(std::string song_id, std::string time, std::string text);
    void inc_playlists();
    void dec_playlists();
    void show_info(bool detail, double confidence = -1);
    std::vector <std::string> get_info(bool detail);
    void show_comments();
    bool is_composed_by(std::string artist_name);
    bool is_composed_within(int min_year, int max_year);
    bool num_of_likes_is_within(int min_likes, int max_likes);
private:
    int id;
    std::string title;
    std::string artist;
    int release_year;
    std::string link;
    std::vector <Comment*> comments;
    int num_of_likes;
    int num_of_playlists;
};

#endif