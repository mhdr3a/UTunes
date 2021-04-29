#include "Playlist.hpp"
using namespace std;

Playlist::Playlist(Record playlist_info)
{
    name = playlist_info[0];
    privacy = playlist_info[1] == PUBLIC ? Public : Private;
    id = stoi(playlist_info[2]);
}

int Playlist::get_id()
{
    return id;
}

string Playlist::get_name()
{
    return name;
}

bool Playlist::duplicate(Song* song)
{
    for (Song* psong : songs)
        if (psong == song)
            return true;
    return false;
}

void Playlist::add_song(Song* song)
{
    if (duplicate(song) == false)
        songs.push_back(song);
    sort(songs.begin(), songs.end(), [](Song* a, Song* b) -> bool {
        return a->get_id() < b->get_id();
    });
}

bool Playlist::is_public()
{
    return privacy == Public;
}

void Playlist::show_info()
{
    cout << id << ' ' << name << ' ';
    if (privacy == Public)
        cout << PUBLIC;
    else
        cout << PRIVATE;
    cout << '\n';
}

vector <vector <string> > Playlist::get_songs()
{
    vector <vector <string> > songs_info;
    for (Song* song : songs)
        songs_info.push_back(song->get_info(false));
    return songs_info;
}

void Playlist::remove_song(int sid)
{
    for (int i = 0; i < songs.size(); i ++)
    {
        if (songs[i]->get_id() == sid)
        {
            songs[i]->dec_playlists();
            songs.erase(songs.begin() + i);
            return;
        }
    }
    throw runtime_error(BAD_REQUEST);
}