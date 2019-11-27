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
     User& get_Active_User() const;
    std::vector<Watchable *> get_content() const;
    Session(const Session &other);
    Session& operator=(const Session& other);
    Session(Session &&other);
    Session& operator=(Session &&other);
    std:: unordered_map<std::string,User*> getMap();
    void insertMap(std::string,User*);
    bool isInMap(std::string s);
    int getIdToWatch();
    void set_Active_user(User*); //check about classification
    std::vector<BaseAction*>& get_ActionsLog() ; //what value is returned?
    void copy(const Session &other);
    void set_next_id(int id);
    static std::vector<std::string>  input_to_vector(std::string);
    std::vector<std::string> get_vector_for_actions();
    void steal(Session &other);



private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    void clear();
    //new field:
    std::vector<std::string> vector_for_actions;



};
#endif
