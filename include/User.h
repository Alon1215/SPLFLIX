#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual void watch_handle_algo( Watchable *watched) = 0;
    virtual User* duplicate( std::string name)=0;
    virtual ~User();
    void addToHistory(Watchable* toAdd);


protected:
    std::vector<Watchable*> history;
    std::vector<Watchable*>get_unwatched(Session& s);

private:
     std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    LengthRecommenderUser (const LengthRecommenderUser &other);
    virtual User* duplicate(std::string name);
    void fix_avg(int length);
    virtual void watch_handle_algo( Watchable *watched);
    virtual ~LengthRecommenderUser();

private:
    float avg;

};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* duplicate( std::string name);
    virtual void watch_handle_algo( Watchable *watched);
    virtual ~RerunRecommenderUser();
private:
    int lastrec; //index for the last recommended content in history
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    std::string get_next_tag( std::vector<std::pair <std::string, int>>& tags);
    virtual void watch_handle_algo( Watchable *watched);
    virtual User* duplicate( std::string name);
    virtual ~GenreRecommenderUser();

private:
};

#endif
