#include <fstream>
#include <string>
#include <iostream>
#include "IO.hpp"
#include <sstream>
using namespace std;
class ReadENV{
    public:
        string getenv(){
            string env_path = ".env";
            IO io(env_path);
            fstream f_stream = io.getFileStream();
            stringstream buffer;
            buffer<<f_stream.rdbuf();
            string content = buffer.str();
            return content;
        }
};
