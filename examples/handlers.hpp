#ifndef _MY_HANDLERS_
#define _MY_HANDLERS_

#include "../UTunes/UTunes.hpp"
#include <iostream>
#include <sstream>

const std::string RECOMMENDED_SONGS_MAX = "4";

class LoginHandler : public RequestHandler {
public:
  LoginHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class SignupHandler : public RequestHandler {
public:
  SignupHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class LogoutHandler : public RequestHandler {
public:
  LogoutHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class HomeHandler : public RequestHandler {
public:
  HomeHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class FavoriteSongsHandler : public RequestHandler {
public:
  FavoriteSongsHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class SongHandler : public RequestHandler {
public:
  SongHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class LikeHandler : public RequestHandler {
public:
  LikeHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class DislikeHandler : public RequestHandler {
public:
  DislikeHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class PlaylistsHandler : public RequestHandler {
public:
  PlaylistsHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class CreatePlaylistHandler : public RequestHandler {
public:
  CreatePlaylistHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class PlaylistHandler : public RequestHandler {
public:
  PlaylistHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class AddSongToPlaylistHandler : public RequestHandler {
public:
  AddSongToPlaylistHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

class RemoveSongFromPlaylistHandler : public RequestHandler {
public:
  RemoveSongFromPlaylistHandler(UTunes* ut) : utunes(ut) {};
  Response *callback(Request *);
private:
  UTunes* utunes;
};

#endif
