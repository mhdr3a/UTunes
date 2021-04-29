#include "Song.hpp"
using namespace std;

Song::Song(Record song_info)
{
    id = stoi(song_info[0]);
    title = song_info[1];
    artist = song_info[2];
    release_year = stoi(song_info[3]);
    link = song_info[4];
    num_of_likes = 0;
    num_of_playlists = 0;
}

Song::Song(const Song& s)
{
    id = s.id;
    title = s.title;
    artist = s.artist;
    release_year = s.release_year;
    link = s.link;
    comments = {};
    for (Comment* comment : s.comments)
        comments.push_back(comment);
    num_of_likes = s.num_of_likes;
    num_of_playlists = s.num_of_playlists;
}

Song::~Song()
{
    for (Comment* comment : comments)
        delete comment;
}

Song& Song::operator=(const Song& s)
{
    if (this == &s)
        return *this;
    id = s.id;
    title = s.title;
    artist = s.artist;
    release_year = s.release_year;
    link = s.link;
    for (Comment* comment : comments)
        delete comment;
    comments = {};
    for (Comment* comment : s.comments)
        comments.push_back(comment);
    num_of_likes = s.num_of_likes;
    num_of_playlists = s.num_of_playlists;
    return *this;
}

int Song::get_id()
{
    return id;
}

void Song::inc_likes()
{
    num_of_likes ++;
}

void Song::dec_likes()
{
    num_of_likes --;
}

void Song::add_to_comments(string username, string time, string text)
{
    Comment* comment = new Comment({username, time, text});
    comments.push_back(comment);
    sort(comments.begin(), comments.end(), [](Comment* a, Comment* b) -> bool {
        if (a->get_time() == b->get_time())
            return a->get_username() < b->get_username();
        return a->get_time() < b->get_time();
    });
}

void Song::inc_playlists()
{
    num_of_playlists ++;
}

void Song::dec_playlists()
{
    num_of_playlists --;
}

void Song::show_info(bool detail, double confidence)
{
    if (detail)
    {
        cout << id << '\n' << title << '\n';
        cout << artist << '\n' << release_year << '\n';
        cout << DETAIL_FLAG << LIKES << ": " << num_of_likes << '\n';
        cout << DETAIL_FLAG << COMMENTS << ": " << comments.size() << '\n';
        cout << DETAIL_FLAG << PLAYLISTS << ": " << num_of_playlists << '\n';
    }
    else
    {
        cout << id << ' ';
        if (confidence != -1)
            cout << fixed << setprecision(PRECISION) << confidence << "% ";
        cout << title << ' ' << artist << ' ';
        cout << release_year << '\n';
    }
}

vector <string> Song::get_info(bool detail)
{
    vector <string> info;
    if (detail)
    {
        info.push_back(to_string(id));
        info.push_back(title);
        info.push_back(artist);
        info.push_back(to_string(release_year));
        info.push_back(link);
        info.push_back(to_string(num_of_likes));
        info.push_back(to_string(comments.size()));
        info.push_back(to_string(num_of_playlists));
    }
    else
    {
        info.push_back(to_string(id));
        info.push_back(title);
        info.push_back(artist);
        info.push_back(to_string(release_year));
    }
    return info;
}

void Song::show_comments()
{
    if (comments.empty())
        throw runtime_error(EMPTY);
    for (Comment* comment : comments)
        comment->show_info();
}

bool Song::is_composed_by(string artist_name)
{
    return artist == artist_name;
}

bool Song::is_composed_within(int min_year, int max_year)
{
    return (release_year >= min_year) && (release_year <= max_year);
}

bool Song::num_of_likes_is_within(int min_likes, int max_likes)
{
    return (num_of_likes >= min_likes) && (num_of_likes <= max_likes);
}