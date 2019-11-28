#include <iostream>
#include "include/Session.h"

/*int main() {
    Session *s=new Session("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config1.json");
    s->start();
    Session *s2= s;
    delete s;
    s2->start();
    delete s2;
} */

int main(int argc, char** argv){
    if(argc!=2)
    {
        std::cout << "usage splflix input_file" << std::endl;
        return 0;
    }
    Session* s = new Session("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config3 (copy).json");
    s->start();
    Session s2("/users/studs/bsc/2020/mosesofe/Desktop/SPL/config3 (copy).json");
    s2 = std::move(s2.forTestOnly());
    s2=*s;
    delete(s);
    s2.start();

    return 0;
}
