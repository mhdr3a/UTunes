#include "UTunes.hpp"
using namespace std;

UTunes::UTunes(const vector <Record>& records, const vector <Record>& liked_records)
{
    for (Record record : records)
    {
        Song* new_song = new Song(record);
        songs.push_back(new_song);
    }
    for (Record record : liked_records)
    {
        sign_up(record[1], record[0], record[2], "", true);
        add_to_favorites(record[3], "", find_user_by_username(record[0]));
    }
    sort(songs.begin(), songs.end(), [](Song* a, Song* b) -> bool {
        return a->get_id() < b->get_id();
    });
    users_songs = new Matrix(users.size(), songs.size());
    for (int i = 0; i < users.size(); i ++)
        for (int j = 0; j < songs.size(); j ++)
            if (users[i]->likes(songs[j]))
                users_songs->set(i, j, 1);
            else
                users_songs->set(i, j, 0);
    users_users = new Matrix(users.size(), users.size());
    for (int i = 0; i < users.size(); i ++)
    {
        for (int j = i + 1; j < users.size(); j ++)
        {
            double s = users_songs->similarity(i, j);
            users_users->set(i, j, s);
            users_users->set(j, i, s);
        }
    }
    playlist_id = 1;
}

UTunes::UTunes(const UTunes& ut)
{
    songs = {};
    for (Song* song : ut.songs)
        songs.push_back(song);
    users = {};
    for (User* user : ut.users)
        users.push_back(user);
    users_songs = ut.users_songs;
    users_users = ut.users_users;
    playlist_id = ut.playlist_id;
}

UTunes::~UTunes()
{
    for (User* user : users)
        delete user;
    for (Song* song : songs)
        delete song;
    delete users_songs;
    delete users_users;
}

UTunes& UTunes::operator=(const UTunes& ut)
{
    if (this == &ut)
        return *this;
    for (User* user : users)
        delete user;
    for (Song* song : songs)
        delete song;
    delete users_songs;
    songs = {};
    for (Song* song : ut.songs)
        songs.push_back(song);
    users = {};
    for (User* user : ut.users)
        users.push_back(user);
    users_songs = ut.users_songs;
    users_users = ut.users_users;
    playlist_id = ut.playlist_id;
    return *this;
}

bool UTunes::duplicate(User* new_user)
{
    for (User* user : users)
        if (user->get_email() == new_user->get_email()
            || user->get_username() == new_user->get_username())
            return true;
    return false;
}

int UTunes::sign_up(string email, string username, string password, string session_id, bool init)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (init == false && logged_in_user != nullptr)
        throw runtime_error(PERMISSION_DENIED);
    User* new_user = new User({email, username, password});
    if (duplicate(new_user))
    {
        delete new_user;
        if (init)
            return 1;
        throw runtime_error(BAD_REQUEST);
    }
    users.push_back(new_user);
    sort(users.begin(), users.end(), [](User* a, User*b) -> bool {
        return a->get_username() < b->get_username();
    });
    if (init == false)
    {
        int user_index = find_user_index_by_username(username);
        users_songs->add_row(user_index);
        users_users->add_row(user_index);
        users_users->add_col(user_index);
    }
    return 0;
}

User* UTunes::find_by_email(string email)
{
    for (User* user : users)
        if (user->get_email() == email)
            return user;
    return nullptr;
}

int UTunes::log_in(string username, string password, string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user != nullptr)
        throw runtime_error(PERMISSION_DENIED);
    User* user = find_user_by_username(username);
    if (user == nullptr || user->validate(password) == false)
        throw runtime_error(BAD_REQUEST);
    return 0;
}

int UTunes::log_out(string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    return 0;
}

Song* UTunes::find_song_by_id(int id)
{
    for (Song* song : songs)
        if (song->get_id() == id)
            return song;
    return nullptr;
}

int UTunes::add_to_favorites(string id, string session_id, User* user)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (user == nullptr && logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    Song* song = find_song_by_id(stoi(id));
    if (song == nullptr)
    {
        if (user != nullptr)
            return 1;
        throw runtime_error(NOT_FOUND);
    }
    if (user != nullptr)
    {
        if (user->likes(song))
            return 1;
        user->add_to_favorites(song);
    }
    else
    {
        if (logged_in_user->likes(song))
            throw runtime_error(BAD_REQUEST);
        logged_in_user->add_to_favorites(song);
        int user_index = find_user_index_by_username(logged_in_user->get_username());
        int song_index = song->get_id() - 1;
        users_songs->set(user_index, song_index, 1);
        update_users_users(user_index);
    }
    song->inc_likes();
    return 0;
}

int UTunes::add_to_playlists(string name, string privacy, string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    if (name.empty() || privacy.empty())
        throw runtime_error(BAD_REQUEST);
    Playlist* new_playlist = new Playlist({name, privacy, to_string(playlist_id)});
    playlist_id ++;
    logged_in_user->add_to_playlists(new_playlist);
    return (playlist_id - 1);
}

Playlist* UTunes::find_playlist_by_id(int id)
{
    Playlist* playlist;
    for (User* user : users)
    {
        playlist = user->find_playlist_by_id(id);
        if (playlist != nullptr)
            return playlist;
    }
    return nullptr;
}

int UTunes::add_song_to_playlist(string pid, string sid, string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    Playlist* playlist = find_playlist_by_id(stoi(pid));
    if (playlist == nullptr)
        throw runtime_error(NOT_FOUND);
    playlist = logged_in_user->find_playlist_by_id(stoi(pid));
    if (playlist == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    Song* song = find_song_by_id(stoi(sid));
    if (song == nullptr)
        throw runtime_error(NOT_FOUND);
    logged_in_user->add_song_to_playlist(playlist, song);
    song->inc_playlists();
    return 0;
}

int UTunes::add_to_comments(string song_id, string time, string text, string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw Server::Exception(PERMISSION_DENIED);
    Song* song = find_song_by_id(stoi(song_id));
    if (song == nullptr)
        throw Server::Exception(NOT_FOUND);
    song->add_to_comments(logged_in_user->get_username(), time, text);
    return 0;
}

vector <vector <string> > UTunes::get_song_info(string session_id, string song_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    vector <vector <string> > info;
    if (song_id == NOT_SPECIFIED)
        for (Song* song : songs)
            info.push_back(song->get_info(false));
    else
    {
        Song* song = find_song_by_id(stoi(song_id));
        if (song == nullptr)
            throw runtime_error(NOT_FOUND);
        info.push_back(song->get_info(true));
    }
    return info;
}

vector <vector <string> > UTunes::get_liked_songs(string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    return logged_in_user->get_liked_songs();
}

vector <Playlist*> UTunes::get_available_playlists(string session_id, bool restricted)
{
    User* logged_in_user = find_user_by_username(session_id);
    vector <Playlist*> available_playlists;
    for (User* user : users)
        if (user == logged_in_user)
            for (Playlist* playlist : user->get_playlists())
                available_playlists.push_back(playlist);
        else
            for (Playlist* playlist : user->get_playlists(restricted))
                available_playlists.push_back(playlist);
    sort(available_playlists.begin(), available_playlists.end(), [](Playlist* a, Playlist* b) -> bool {
        return a->get_id() < b->get_id();
    });
    return available_playlists;
}

User* UTunes::find_user_by_username(string username)
{
    for (User* user : users)
        if (user->get_username() == username)
            return user;
    return nullptr;
}

int UTunes::find_user_index_by_username(string username)
{
    for (int i = 0; i < users.size(); i ++)
        if (users[i]->get_username() == username)
            return i;
    return -1;
}

vector <vector <string> > UTunes::get_playlists(string session_id, string pid, bool restriced)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    vector <vector <string> > playlists;
    for (Playlist* playlist : logged_in_user->get_playlists())
    {
        string id = to_string(playlist->get_id());
        if (pid != NOT_SPECIFIED)
            if (pid != id)
                continue;
        string name = playlist->get_name();
        string privacy = playlist->is_public() ? PUBLIC : PRIVATE;
        playlists.push_back({id, name, privacy});
    }
    if (restriced == false)
    {
        for (User* user : users)
        {
            for (Playlist* playlist : user->get_playlists())
            {
                string id = to_string(playlist->get_id());
                if (pid != NOT_SPECIFIED)
                    if (pid != id)
                        continue;
                string name = playlist->get_name();
                string privacy = playlist->is_public() ? PUBLIC : PRIVATE;
                playlists.push_back({id, name, privacy});
            }
        }
    }
    return playlists;
}

vector <vector <string> > UTunes::get_playlist_songs(string pid, string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    vector <Playlist*> all_playlists = get_available_playlists(session_id, false);
    Playlist* playlist = nullptr;
    for (Playlist* pl : all_playlists)
    {
        if (pl->get_id() == stoi(pid))
        {
            playlist = pl;
            break;
        }
    }
    if (playlist == nullptr)
        throw runtime_error(NOT_FOUND);
    bool available = false;
    vector <Playlist*> available_playlists = get_available_playlists(session_id);
    for (Playlist* pl : available_playlists)
    {
        if (pl == playlist)
        {
            available = true;
            break;
        }
    }
    if (available == false)
        throw runtime_error(PERMISSION_DENIED);
    return playlist->get_songs();
}

void UTunes::show_usernames(string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw Server::Exception(PERMISSION_DENIED);
    if (users.size() == 1)
        throw Server::Exception(EMPTY);
    for (User* user : users)
        if (user != logged_in_user)
            cout << user->get_username() << '\n';
}

void UTunes::show_comments(string song_id, string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw Server::Exception(PERMISSION_DENIED);
    Song* song = find_song_by_id(stoi(song_id));
    if (song == nullptr)
        throw Server::Exception(NOT_FOUND);
    song->show_comments();
}

int UTunes::remove_from_favorite_songs(string song_id, string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    logged_in_user->remove_from_favorite_songs(stoi(song_id));
    int user_index = find_user_index_by_username(logged_in_user->get_username());
    int song_index = stoi(song_id) - 1;
    if (song_index >= users_songs->get_col())
        throw runtime_error(NOT_FOUND);
    users_songs->set(user_index, song_index, 0);
    update_users_users(user_index);
    return 0;
}

int UTunes::remove_song_from_playlist(string pid, string sid, string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    Playlist* playlist = find_playlist_by_id(stoi(pid));
    Song* song = find_song_by_id(stoi(sid));
    if (playlist == nullptr || song == nullptr)
        throw runtime_error(NOT_FOUND);
    logged_in_user->remove_song_from_playlist(stoi(pid), stoi(sid));
    return 0;
}

void UTunes::update_users_users(int user_index)
{
    for (int j = 0; j < users.size(); j ++)
    {
        if (j != user_index)
        {
            double s = users_songs->similarity(user_index, j);
            users_users->set(user_index, j, s);
            users_users->set(j, user_index, s);
        }
    }
}

void UTunes::show_similar_users(string count, string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw Server::Exception(PERMISSION_DENIED);
    if (stoi(count) <= 0)
        throw Server::Exception(BAD_REQUEST);
    int user_index = find_user_index_by_username(logged_in_user->get_username());
    vector < pair <string, double> > similarities;
    for (int j = 0; j < users.size(); j ++)
        if (j != user_index)
            similarities.push_back(pair <string, double> (users[j]->get_username(), 100 * users_users->get(user_index, j)));
    sort(similarities.begin(), similarities.end(), [](pair <string, double>& a, pair <string, double>& b) -> bool {
        if (a.second == b.second)
            return a.first < b.first;
        else
            return a.second > b.second;
    });
    for (int i = 0; i < min(stoi(count), (int) (users.size() - 1)); i ++)
        cout << fixed << setprecision(PRECISION)
             << similarities[i].second << "% " << similarities[i].first << '\n';
}

double UTunes::calc_confidence(int user_index, int song_index)
{
    double s = 0;
    for (int j = 0; j < users.size(); j ++)
        if (j != user_index)
            s += users_songs->get(j, song_index) * users_users->get(user_index, j);
    return 100 * s / (users.size() - 1);
}

vector < pair <int, double> > UTunes::get_confidences(int user_index, string session_id, string sid)
{
    User* logged_in_user = find_user_by_username(session_id);
    vector < pair <int, double> > confidences;
    for (Song* song : songs)
        if (logged_in_user->likes(song) == false && song->get_id() != stoi(sid))
            confidences.push_back(pair <int, double> (song->get_id(), calc_confidence(user_index, song->get_id() - 1)));
    sort(confidences.begin(), confidences.end(), [](pair <int, double>& a, pair <int, double>& b) -> bool {
        return a.second > b.second;
    });
    return confidences;
}

vector <vector <string> > UTunes::get_recommended_songs_info(string count, string session_id, string sid)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw runtime_error(PERMISSION_DENIED);
    if (stoi(count) <= 0)
        throw runtime_error(BAD_REQUEST);
    vector <vector <string> > recommended_songs_info;
    int user_index = find_user_index_by_username(logged_in_user->get_username());
    vector < pair <int, double> > confidences = get_confidences(user_index, session_id, sid);
    for (int i = 0; i < min(stoi(count), (int) confidences.size()); i ++)
    {
        Song* song = find_song_by_id(confidences[i].first);
        recommended_songs_info.push_back(song->get_info(false));
    }
    return recommended_songs_info;
}

bool UTunes::validate(string session_id)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        return false;
    return true;
}

bool UTunes::user_likes_this(string session_id, string sid)
{
    User* logged_in_user = find_user_by_username(session_id);
    if (logged_in_user == nullptr)
        throw Server::Exception(PERMISSION_DENIED);
    Song* song = find_song_by_id(stoi(sid));
    if (song == nullptr)
        throw Server::Exception(NOT_FOUND);
    return logged_in_user->likes(song);
}