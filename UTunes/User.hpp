#ifndef __USER_HPP__
#define __USER_HPP__

#include "Playlist.hpp"

class User
{
public:
    User(Record user_info);
    std::string get_email();
    std::string get_username();
    bool validate(std::string pw);
    void add_to_favorites(Song* song);
    bool likes(Song* song);
    void add_to_playlists(Playlist* new_playlist);
    Playlist* find_playlist_by_id(int id);
    void add_song_to_playlist(Playlist* playlist, Song* song);
    std::vector <std::vector <std::string> > get_liked_songs();
    std::vector <Playlist*> get_playlists(bool restricted = false);
    void show_playlists(bool restricted = false);
    void remove_from_favorite_songs(int song_id);
    void remove_song_from_playlist(int pid, int sid);
private:
    std::string email;
    std::string username;
    std::string password;
    std::vector <Song*> favorite_songs;
    std::vector <Playlist*> playlists;
};

#endif