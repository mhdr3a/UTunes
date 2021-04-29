#include "handlers.hpp"
#include <map>

using namespace std;

const string HOME = "Home";
const string FAVORITE_SONGS = "Favorite Songs";

ostringstream BuildErrorPage(string error_msg)
{
  ostringstream body;
  int error_code = error_msg == PERMISSION_DENIED ? 403 : error_msg == BAD_REQUEST ? 400 : error_msg == NOT_FOUND ? 404 : 500;
  body << "<!DOCTYPE html>";
  body << "<html>";
  body << "<head>";
  body << "<title>UTunes</title>";
  body << "<meta charset='UTF-8'>";
  body << "<link rel='stylesheet' href='/style.css' />";
  body << "</head>";
  body << "<body>";
  body << "<nav class='navbar'>";
  body << "<div class='brand'>";
  body << "<p>UTunes</p>";
  body << "</div>";
  body << "<ul class='error menu'>";
  body << "<li>";
  body << "<a href='/'>Home</a>";
  body << "</li>";
  body << "</ul>";
  body << "</nav>";
  body << "<div class='error-template'>";
  body << "<h1 style='margin-top: 10px;'>UTunes</h1>";
  body << "<h2>Err: <em>" << error_code << "</em></h2>";
  body << "<p>" << error_msg << "</p>";
  body << "</div>";
  body << "</body>";
  body << "</html>";
  return body;
}

ostringstream BuildTemplate(string active_item = "")
{
  ostringstream body;
  body << "<!DOCTYPE html>";
  body << "<html>";
  body << "<head>";
  body << "<title>UTunes</title>";
  body << "<meta charset='UTF-8' />";
  body << "<link rel='stylesheet' href='/style.css' />";
  body << "</head>";
  body << "<body>";
  body << "<nav class='navbar'>";
  body << "<div class='brand'>";
  body << "<p>UTunes</p>";
  body << "</div>";
  body << "<ul class='menu'>";
  if (active_item == HOME)
    body << "<li class='active'>";
  else
    body << "<li>";
  body << "<a href='/'>Home</a>";
  body << "</li>";
  if (active_item == FAVORITE_SONGS)
    body << "<li class='active'>";
  else
    body << "<li>";
  body << "<a href='/favorite-songs'>Favorite Songs</a>";
  body << "</li>";
  if (active_item == PLAYLISTS)
    body << "<li class='active'>";
  else
    body << "<li>";
  body << "<a href='/playlists'>Playlists</a>";
  body << "</li>";
  body << "<li>";
  body << "<a href='/logout'>Log out</a>";
  body << "</li>";
  body << "</ul>";
  body << "</nav>";
  body << "<script>";
  body << "function FormSubmit(id)";
  body << "{";
  body << "document.getElementById(id).submit();";
  body << "}";
  body << "</script>";
  return body;
}

Response *LoginHandler::callback(Request *req) {
  string username = req->getBodyParam("username");
  string password = req->getBodyParam("password");
  string session_id = req->getSessionId();
  Response* res = new Response();
  try
  {
    utunes->log_in(username, password, session_id);
    res = Response::redirect("/");
    res->setSessionId(username);
  }
  catch (const exception& e)
  {
    ostringstream body = BuildErrorPage(e.what());
    res->setHeader("Content-Type", "text/html");
    res->setBody(body.str());
  }
  return res;
}

Response *SignupHandler::callback(Request *req) {
  string username = req->getBodyParam("username");
  string email = req->getBodyParam("email");
  string password = req->getBodyParam("password");
  string session_id = req->getSessionId();
  Response* res = new Response();
  try
  {
    utunes->sign_up(email, username, password, session_id);
    res = Response::redirect("/");
    res->setSessionId(username);
  }
  catch (const exception& e)
  {
    ostringstream body = BuildErrorPage(e.what());
    res->setHeader("Content-Type", "text/html");
    res->setBody(body.str());
  }
  return res;
}

Response *LogoutHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response* res = new Response();
  try
  {
    utunes->log_out(session_id);
    res = Response::redirect("/login");
    res->setSessionId("");
  }
  catch (const exception& e)
  {
    ostringstream body = BuildErrorPage(e.what());
    res->setHeader("Content-Type", "text/html");
    res->setBody(body.str());
  }
  return res;
}

Response *HomeHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    vector <vector <string> > song_info = utunes->get_song_info(session_id);
    res->setHeader("Content-Type", "text/html");
    ostringstream body = BuildTemplate(HOME);
    body << "<div class='songs'>";
    body << "<div class='section-title'>";
    body << "<p>Songs</p>";
    body << "</div>";
    body << "<table>";
    body << "<tbody>";
    for (int i = 0; i < song_info.size(); i ++)
    {
      string id = song_info[i][0];
      string title = song_info[i][1];
      string artist = song_info[i][2];
      string release_year = song_info[i][3];
      body << "<tr onclick='FormSubmit(\"Form_" << id << "\")'>";
      body << "<td>" << id << "</td>";
      body << "<td>" << title << "</td>";
      body << "<td>" << artist << "</td>";
      body << "<td>" << release_year << "</td>";
      body << "<form id='Form_" << id << "' action='/song' method='get'>";
      body << "<input type='hidden' name='id' value='" << id << "' />";
      body << "</form>";
      body << "</tr>";
    }
    body << "</tbody>";
    body << "</table>";
    body << "</div>";
    body << "</body>";
    body << "</html>";
    res->setBody(body.str());
  }
  else
    res = Response::redirect("/login");
  return res;
}

Response *FavoriteSongsHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    vector <vector <string> > liked_songs = utunes->get_liked_songs(session_id);
    res->setHeader("Content-Type", "text/html");
    ostringstream body = BuildTemplate(FAVORITE_SONGS);
    body << "<div class='songs'>";
    body << "<div class='section-title'>";
    body << "<p>Songs</p>";
    body << "</div>";
    body << "<table>";
    body << "<tbody>";
    for (int i = 0; i < liked_songs.size(); i ++)
    {
      string id = liked_songs[i][0];
      string title = liked_songs[i][1];
      string artist = liked_songs[i][2];
      string release_year = liked_songs[i][3];
      body << "<tr>";
      body << "<td onclick='FormSubmit(\"FormSong_" << id << "\")'>" << id << "</td>";
      body << "<td onclick='FormSubmit(\"FormSong_" << id << "\")'>" << title << "</td>";
      body << "<td onclick='FormSubmit(\"FormSong_" << id << "\")'>" << artist << "</td>";
      body << "<td onclick='FormSubmit(\"FormSong_" << id << "\")'>" << release_year << "</td>";
      body << "<td onclick='FormSubmit(\"FormDislike_" << id << "\")'>";
      body << "<img style='float:left;' width='50' height='50' src='/dislike.png' />";
      body << "<p style='float:left; margin:0; line-height:50px; height:50px;'>Dislike</p>";
      body << "</td>";
      body << "<form id='FormDislike_" << id << "' action='/dislike' method='get'>";
      body << "<input type='hidden' name='id' value='" << id << "' />";
      body << "<input type='hidden' name='return' value='" << 1 << "' />";
      body << "</form>";
      body << "<form id='FormSong_" << id << "' action='/song' method='get'>";
      body << "<input type='hidden' name='id' value='" << id << "' />";
      body << "</form>";
      body << "</tr>";
    }
    body << "</tbody>";
    body << "</table>";
    body << "</div>";
    body << "</body>";
    body << "</html>";
    res->setBody(body.str());
  }
  else
    res = Response::redirect("/login");
  return res;
}

Response *SongHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    try
    {
      string id = req->getQueryParam("id");
      vector <vector <string> > song_info = utunes->get_song_info(session_id, id);
      vector <vector <string> > recommended_songs_info
        = utunes->get_recommended_songs_info(RECOMMENDED_SONGS_MAX, session_id, id);
      res->setHeader("Content-Type", "text/html");
      ostringstream body = BuildTemplate();
      body << "<div class='song-info-container songs'>";
      body << "<div class='song-cover-container'>";
      body << "<img src='/cover.jpg' />";
      body << "</div>";
      body << "<div class='song-info'>";
      body << "<div class='recommended-songs-container'>";
      body << "<div class='section-title'>";
      body << "<p>Recommended Songs</p>";
      body << "</div>";
      body << "<table>";
      body << "<thead>";
      body << "<tr>";
      body << "<th>id</th>";
      body << "<th>title</th>";
      body << "<th>artist</th>";
      body << "<th>release year</th>";
      body << "</tr>";
      body << "</thead>";
      body << "<tbody>";
      for (int i = 0; i < recommended_songs_info.size(); i ++)
      {
        string id = recommended_songs_info[i][0];
        string title = recommended_songs_info[i][1];
        string artist = recommended_songs_info[i][2];
        string release_year = recommended_songs_info[i][3];
        body << "<tr onclick='FormSubmit(\"Form_" << id << "\")'>";
        body << "<td>" << id << "</td>";
        body << "<td>" << title << "</td>";
        body << "<td>" << artist << "</td>";
        body << "<td>" << release_year << "</td>";
        body << "<form id='Form_" << id << "' action='/song' method='get'>";
        body << "<input type='hidden' name='id' value='" << id << "' />";
        body << "</form>";
        body << "</tr>";
      }
      string title = song_info[0][1];
      string artist = song_info[0][2];
      string release_year = song_info[0][3];
      string link = song_info[0][4];
      string likes = song_info[0][5];
      string playlists = song_info[0][7];
      body << "</tbody>";
      body << "</table>";
      body << "</div>";
      body << "<div class='h2-container'>";
      body << "<h2>" << title << " - " << artist << " (" << release_year << ")</h2>";
      body << "</div>";
      body << "<div class='audio-container'>";
      body << "<audio controls>";
      body << "<source src='" << link << "' type='audio/mp3'>";
      body << "Your browser does not support the audio element.";
      body << "</audio>";
      body << "</div>";
      body << "<div class='utils-container'>";
      body << "<div class='util'>";
      body << "<img src='/likes.png' />";
      body << "<p>" << likes << "</p>";
      body << "</div>";
      body << "<div class='util'>";
      body << "<img src='/playlist.png' />";
      body << "<p>" << playlists << "</p>";
      body << "</div>";
      body << "<div onclick='FormSubmit(\"Form_" << id << "\")' class='util like-this-song'>";
      if (utunes->user_likes_this(session_id, id))
      {
        body << "<img src='/dislike.png' />";
        body << "<p>Dislike</p>";
        body << "<form id='Form_" << id << "' action='/dislike' method='get'>";
        body << "<input type='hidden' name='return' value='" << 0 << "' />";
        body << "<input type='hidden' name='id' value='" << id << "' />";
        body << "</form>";
      }
      else
      {
        body << "<img src='/like.png' />";
        body << "<p>Like</p>";
        body << "<form id='Form_" << id << "' action='/like' method='get'>";
        body << "<input type='hidden' name='id' value='" << id << "' />";
        body << "</form>";
      }
      body << "</div>";
      body << "</div>";
      body << "</div>";
      body << "</div>";
      body << "</body>";
      body << "</html>";
      res->setBody(body.str());
    }
    catch (const exception& e)
    {
      ostringstream body = BuildErrorPage(e.what());
      res->setHeader("Content-Type", "text/html");
      res->setBody(body.str());
    } 
  }
  else
    res = Response::redirect("/login");
  return res;
}

Response *LikeHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    try
    {
      string song_id = req->getQueryParam("id");
      utunes->add_to_favorites(song_id, session_id);
      res = Response::redirect("/song?id=" + song_id);
    }
    catch (const exception& e)
    {
      ostringstream body = BuildErrorPage(e.what());
      res->setHeader("Content-Type", "text/html");
      res->setBody(body.str());
    }
  }
  else
    res = Response::redirect("/login");
  return res;
}

Response *DislikeHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    try
    {
      string song_id = req->getQueryParam("id");
      utunes->remove_from_favorite_songs(song_id, session_id);
      if (stoi(req->getQueryParam("return")))
        res = Response::redirect("/favorite-songs");
      else
        res = Response::redirect("/song?id=" + song_id);
    }
    catch (const exception& e)
    {
      ostringstream body = BuildErrorPage(e.what());
      res->setHeader("Content-Type", "text/html");
      res->setBody(body.str());
    }
  }
  else
    res = Response::redirect("/login");
  return res;
}

Response *PlaylistsHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    vector <vector <string> > playlists = utunes->get_playlists(session_id);
    res->setHeader("Content-Type", "text/html");
    ostringstream body = BuildTemplate(PLAYLISTS);
    body << "<div class='songs'>";
    body << "<div class='section-title'>";
    body << "<p>Playlists</p>";
    body << "</div>";
    body << "<table>";
    if (playlists.size())
    {
      body << "<thead>";
      body << "<tr style='background-color: #EEE;'>";
      body << "<th>id</th>";
      body << "<th>name</th>";
      body << "<th>privacy</th>";
      body << "<th></th>";
      body << "</tr>";
      body << "</thead>";
    }
    body << "<tbody>";
    for (int i = 0; i < playlists.size(); i ++)
    {
      string id = playlists[i][0];
      string name = playlists[i][1];
      string privacy = playlists[i][2];
      body << "<tr onclick='FormSubmit(\"Form_" << id << "\")'>";
      body << "<td><div class='inside-td'>" << id << "</div></td>";
      body << "<td><div class='inside-td'>" << name << "</div></td>";
      body << "<td><div class='inside-td'>" << privacy << "</div></td>";
      body << "<td></td>";
      body << "<form id='Form_" << id << "' action='/playlist' method='get'>";
      body << "<input type='hidden' name='id' value='" << id << "' />";
      body << "</form>";
      body << "</tr>";
    }
    body << "<tr class='non-link'>";
    body << "<form id='NewPlaylist' action='/create-playlist' method='get'>";
    body << "<td>";
    body << "<div class='inside-td'>";
    body << "<label for='name'>Name:</label>";
    body << "<input type='text' id='name' name='name' />";
    body << "</div>";
    body << "</td>";
    body << "<td>";
    body << "<div class='inside-td'>";
    body << "<label for='public'>Public</label>";
    body << "<input type='radio' id='public' name='privacy' value='public' checked />";
    body << "</div>";
    body << "</td>";
    body << "<td>";
    body << "<div class='inside-td'>";
    body << "<label for='private'>Private</label>";
    body << "<input type='radio' id='private' name='privacy' value='private' />";
    body << "</div>";
    body << "</td>";
    body << "<td>";
    body << "<div class='inside-td'>";
    body << "<input type='submit' value='Create' />";
    body << "</div>";
    body << "</td>";
    body << "</form>";
    body << "</tr>";
    body << "</tbody>";
    body << "</table>";
    body << "</div>";
    body << "</body>";
    body << "</html>";
    res->setBody(body.str());
  }
  else
    res = Response::redirect("/login");
  return res;
}

Response *PlaylistHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    try
    {
      string pid = req->getQueryParam("id");
      vector <vector <string> > playlists = utunes->get_playlists(session_id, pid, false);
      vector <vector <string> > songs = utunes->get_playlist_songs(pid, session_id);
      string name = playlists[0][1];
      string privacy = playlists[0][2];
      res->setHeader("Content-Type", "text/html");
      ostringstream body = BuildTemplate();
      body << "<div class='songs'>";
      body << "<div class='section-title'>";
      body << "<p>Songs (" << name << " - " << privacy << ")</p>";
      body << "</div>";
      body << "<table>";
      body << "<tbody>";
      for (int i = 0; i < songs.size(); i ++)
      {
        string sid = songs[i][0];
        string title = songs[i][1];
        string artist = songs[i][2];
        string release_year = songs[i][3];
        body << "<tr>";
        body << "<td onclick='FormSubmit(\"FormSong_" << sid << "\")'>" << sid << "</td>";
        body << "<td onclick='FormSubmit(\"FormSong_" << sid << "\")'>" << title << "</td>";
        body << "<td onclick='FormSubmit(\"FormSong_" << sid << "\")'>" << artist << "</td>";
        body << "<td onclick='FormSubmit(\"FormSong_" << sid << "\")'>" << release_year << "</td>";
        body << "<td onclick='FormSubmit(\"FormRemove_" << sid << "\")'>";
        body << "<img style='float:left;' width='40' height='40' src='/remove.png' />";
        body << "<p style='float:left; margin:0; margin-left: 5px; line-height:40px; height:40px;'>Remove</p>";
        body << "</td>";
        body << "<form id='FormRemove_" << sid << "' action='/remove-song-from-playlist' method='get'>";
        body << "<input type='hidden' name='pid' value='" << pid << "' />";
        body << "<input type='hidden' name='sid' value='" << sid << "' />";
        body << "</form>";
        body << "<form id='FormSong_" << sid << "' action='/song' method='get'>";
        body << "<input type='hidden' name='id' value='" << sid << "' />";
        body << "</form>";
        body << "</tr>";
      }
      body << "<tr class='non-link'>";
      body << "<form id='AddSong' action='/add-song-to-playlist' method='get'>";
      body << "<input type='hidden' name='pid' value='" << pid << "' />";
      body << "<td></td>";
      body << "<td>";
      body << "<div class='inside-td'>";
      body << "<label for='sid'>Song id:</label>";
      body << "<input type='text' id='sid' name='sid' />";
      body << "</div>";
      body << "</td>";
      body << "<td></td>";
      body << "<td>";
      body << "<div class='inside-td'>";
      body << "<input type='submit' value='Add' />";
      body << "</div>";
      body << "</td>";
      body << "<td></td>";
      body << "</form>";
      body << "</tr>";
      body << "</tbody>";
      body << "</table>";
      body << "</div>";
      body << "</body>";
      body << "</html>";
      res->setBody(body.str());
    }
    catch (const exception& e)
    {
      ostringstream body = BuildErrorPage(e.what());
      res->setHeader("Content-Type", "text/html");
      res->setBody(body.str());
    }
  }
  else
    res = Response::redirect("/login");
  return res;
}

Response *CreatePlaylistHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    try
    {
      string name = req->getQueryParam("name");
      string privacy = req->getQueryParam("privacy");
      utunes->add_to_playlists(name, privacy, session_id);
      res = Response::redirect("/playlists");
    }
    catch (const exception& e)
    {
      ostringstream body = BuildErrorPage(e.what());
      res->setHeader("Content-Type", "text/html");
      res->setBody(body.str());
    }
  }
  else
    res = Response::redirect("/login");
  return res;
}

Response *AddSongToPlaylistHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    try
    {
      string pid = req->getQueryParam("pid");
      string sid = req->getQueryParam("sid");
      utunes->add_song_to_playlist(pid, sid, session_id);
      res = Response::redirect("/playlist?id=" + pid);
    }
    catch (const exception& e)
    {
      ostringstream body = BuildErrorPage(e.what());
      res->setHeader("Content-Type", "text/html");
      res->setBody(body.str());
    }
  }
  else
    res = Response::redirect("/login");
  return res;
}

Response *RemoveSongFromPlaylistHandler::callback(Request *req) {
  string session_id = req->getSessionId();
  Response *res = new Response;
  if (utunes->validate(session_id))
  {
    try
    {
      string pid = req->getQueryParam("pid");
      string sid = req->getQueryParam("sid");
      utunes->remove_song_from_playlist(pid, sid, session_id);
      res = Response::redirect("/playlist?id=" + pid);
    }
    catch (const exception& e)
    {
      ostringstream body = BuildErrorPage(e.what());
      res->setHeader("Content-Type", "text/html");
      res->setBody(body.str());
    }
  }
  else
    res = Response::redirect("/login");
  return res;
}