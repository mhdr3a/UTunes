#ifndef __PLAYLIST_HPP__
#define __PLAYLIST_HPP__

#include "Song.hpp"

enum Privacy {Public, Private};

const std::string BAD_REQUEST = "Bad Request";
const std::string PERMISSION_DENIED = "Permission Denied";
const std::string NOT_FOUND = "Not Found";
const std::string PUBLIC = "public";
const std::string PRIVATE = "private";
const std::string NOT_SPECIFIED = "Not_Specified";

class Playlist
{
public:
    Playlist(Record playlist_info);
    int get_id();
    void add_song(Song* song);
    bool is_public();
    void show_info();
    std::vector <std::vector <std::string> > get_songs();
    void remove_song(int sid);
    std::string get_name();
private:
    bool duplicate(Song* song);
    std::string name;
    int privacy;
    int id;
    std::vector <Song*> songs;
};

#endif