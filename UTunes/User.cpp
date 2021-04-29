#include "User.hpp"
using namespace std;

User::User(Record user_info)
{
    email = user_info[0];
    username = user_info[1];
    password = user_info[2];
}

string User::get_email()
{
    return email;
}

string User::get_username()
{
    return username;
}

bool User::validate(string pw)
{
    return (password == pw);
}

void User::add_to_favorites(Song* song)
{
    favorite_songs.push_back(song);
    sort(favorite_songs.begin(), favorite_songs.end(), [](Song* a, Song* b) -> bool {
        return a->get_id() < b->get_id();
    });
}

bool User::likes(Song* song)
{
    for (Song* favorite_song : favorite_songs)
        if (favorite_song == song)
            return true;
    return false;
}

void User::add_to_playlists(Playlist* new_playlist)
{
    playlists.push_back(new_playlist);
}

Playlist* User::find_playlist_by_id(int id)
{
    for (Playlist* playlist : playlists)
        if (playlist->get_id() == id)
            return playlist;
    return nullptr;
}

void User::add_song_to_playlist(Playlist* playlist, Song* song)
{
    playlist->add_song(song);
}

vector <vector <string> > User::get_liked_songs()
{
    vector <vector <string> > liked_songs;
    sort(favorite_songs.begin(), favorite_songs.end(), [](Song* a, Song* b) -> bool {
        return a->get_id() < b->get_id();
    });
    for (Song* song : favorite_songs)
        liked_songs.push_back(song->get_info(false));
    return liked_songs;
}

vector <Playlist*> User::get_playlists(bool restricted)
{
    if (restricted)
    {
        vector <Playlist*> public_playlists;
        for (Playlist* playlist : playlists)
            if (playlist->is_public())
                public_playlists.push_back(playlist);
        return public_playlists;
    }
    else
        return playlists;
}

void User::show_playlists(bool restricted)
{
    bool empty = true;
    for (Playlist* playlist : playlists)
    {
        if (restricted)
        {
            if (playlist->is_public())
            {
                playlist->show_info();
                empty = false;
            }
        }
        else
        {
            playlist->show_info();
            empty = false;
        }
    }
}

void User::remove_from_favorite_songs(int song_id)
{
    for (int i = 0; i < favorite_songs.size(); i ++)
    {
        if (favorite_songs[i]->get_id() == song_id)
        {
            favorite_songs[i]->dec_likes();
            favorite_songs.erase(favorite_songs.begin() + i);
            return;
        }
    }
    throw runtime_error(BAD_REQUEST);
}

void User::remove_song_from_playlist(int pid, int sid)
{
    Playlist* playlist = find_playlist_by_id(pid);
    if (playlist == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    playlist->remove_song(sid);
}