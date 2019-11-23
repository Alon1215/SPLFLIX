#ifndef SESSION_H_
#define SESSION_H_

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
    bool isInMap(std::string);
    std::string getPrefAlgo();
    std::string getUserName();
    std::string getNameOfClone();
    int getIdToWatch();
    void set_Active_user(User*); //check about classification



private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;
    void clear();
    void copy(const Session &other);
    std::string prefAlgo;
    std::string userName;
    std::string nameOfClone; //missing function()
    int idToWatch;

};
#endif
