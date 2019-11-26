#ifndef SESSION_H_
#define SESSION_H_
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "json.hpp"
class User;
class Watchable;
class Movie;
class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    User& get_Active_User();
    const std::vector<Watchable *> get_content() const;
    Session(const Session &other);
    Session& operator=(const Session& other);
    Session(Session &&other);
    Session& operator=(Session &&other);
    std:: unordered_map<std::string,User*> getMap();
    void insertMap(std::string,User*);
    bool isInMap(std::string s);
    std::string getPrefAlgo();
    std::string getUserName();
    int getIdToWatch();
    void set_Active_user(User*); //check about classification
    std::vector<BaseAction*>& get_ActionsLog() ; //what value is returned?
    void copy(const Session &other);
    void set_next_id(int id);



private:
    std::vector<Watchable*> unwatched; //vector containing unwatched content only
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    void clear();
    std::string command; //first input word
    std::string second; //second input word
    std::string third; // third '' "    "



};
#endif
