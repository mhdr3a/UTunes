#include "handlers.hpp"
#include "my_server.hpp"
#include <iostream>
#include <fstream>

using namespace std;

Record split(string str, char delim)
{
  stringstream ss(str);
  string token;
  Record result;
  while (getline(ss, token, delim))
    if (token.empty() == false)
      result.push_back(token);
  return result;
}

vector <Record> read_csv(char* filepath)
{
  if (filepath)
  {
    string fp(filepath);
    ifstream csv(fp);
    string line;
    vector <Record> data;
    getline(csv, line);
    Record values;
    while (getline(csv, line))
    {
        values = split(line, ',');
        data.push_back(values);
    }
    csv.close();
    return data;
  }
}

int main(int argc, char **argv)
{
  try {
    vector <Record> songs = read_csv(argv[1]);
    vector <Record> liked_songs = read_csv(argv[2]);
    UTunes* utunes = new UTunes(songs, liked_songs);
    MyServer server(3000);
    server.setNotFoundErrPage("static/404.html");
    server.get("/signup", new ShowPage("static/signupcss.html"));
    server.post("/signup", new SignupHandler(utunes));
    server.get("/login", new ShowPage("static/logincss.html"));
    server.post("/login", new LoginHandler(utunes));
    server.get("/logout", new LogoutHandler(utunes));
    server.get("/cover.jpg", new ShowImage("static/cover.jpg"));
    server.get("/likes.png", new ShowImage("static/likes.png"));
    server.get("/playlist.png", new ShowImage("static/playlist.png"));
    server.get("/like.png", new ShowImage("static/like.png"));
    server.get("/like", new LikeHandler(utunes));
    server.get("/dislike.png", new ShowImage("static/dislike.png"));
    server.get("/dislike", new DislikeHandler(utunes));
    server.get("/style.css", new ShowPage("static/style.css"));
    server.get("/", new HomeHandler(utunes));
    server.get("/favorite-songs", new FavoriteSongsHandler(utunes));
    server.get("/song", new SongHandler(utunes));
    server.get("/playlists", new PlaylistsHandler(utunes));
    server.get("/create-playlist", new CreatePlaylistHandler(utunes));
    server.get("/playlist", new PlaylistHandler(utunes));
    server.get("/add-song-to-playlist", new AddSongToPlaylistHandler(utunes));
    server.get("/remove.png", new ShowImage("static/remove.png"));
    server.get("/remove-song-from-playlist", new RemoveSongFromPlaylistHandler(utunes));
    server.run();
  } catch (Server::Exception e) {
    cerr << e.getMessage() << endl;
  }
}
