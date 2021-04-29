#ifndef __UTUNES_HPP__
#define __UTUNES_HPP__

#include "User.hpp"
#include "Matrix.hpp"

class UTunes
{
public:
    UTunes(const std::vector <Record>& records = {}, const std::vector <Record>& liked_records = {});
    UTunes(const UTunes&);
    ~UTunes();
    UTunes& operator=(const UTunes&);
    int sign_up(std::string email, std::string username, std::string password,
                std::string session_id, bool init = false);
    int log_in(std::string email, std::string password, std::string session_id);
    int log_out(std::string session_id);
    int add_to_favorites(std::string id, std::string session_id, User* user = nullptr);
    int add_to_playlists(std::string name, std::string privacy, std::string session_id);
    int add_song_to_playlist(std::string pid, std::string sid, std::string session_id);
    int add_to_comments(std::string song_id, std::string time, std::string text, std::string session_id);
    std::vector <std::vector <std::string> > get_song_info(std::string session_id, std::string song_id = NOT_SPECIFIED);
    std::vector <std::vector <std::string> > get_liked_songs(std::string session_id);
    std::vector <Playlist*> get_available_playlists(std::string session_id, bool restricted = true);
    User* find_user_by_username(std::string username);
    int find_user_index_by_username(std::string username);
    std::vector <std::vector <std::string> > get_playlists(std::string session_id, std::string pid = NOT_SPECIFIED, bool restriced = true);
    std::vector <std::vector <std::string> > get_playlist_songs(std::string pid, std::string session_id);
    void show_usernames(std::string session_id);
    void show_comments(std::string song_id, std::string session_id);
    int remove_from_favorite_songs(std::string song_id, std::string session_id);
    int remove_song_from_playlist(std::string pid, std::string sid, std::string session_id);
    void update_users_users(int user_index);
    void show_similar_users(std::string count, std::string session_id);
    double calc_confidence(int user_index, int song_index);
    std::vector < std::pair <int, double> > get_confidences(int user_index, std::string session_id, std::string sid);
    std::vector <std::vector <std::string> > get_recommended_songs_info
        (std::string count, std::string session_id, std::string sid);
    bool validate(std::string session_id);
    bool user_likes_this(std::string session_id, std::string sid);
private:
    bool duplicate(User* new_user);
    User* find_by_email(std::string email);
    Song* find_song_by_id(int id);
    Playlist* find_playlist_by_id(int id);
    std::vector <Song*> songs;
    std::vector <User*> users;
    int playlist_id;
    Matrix* users_songs;
    Matrix* users_users;
};

#endif