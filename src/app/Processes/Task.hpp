#ifndef TASK_HPP
#define TASK_HPP
#include <string>
#include <iostream>
#include <sstream>
#include "../fileHandling/IO.hpp"
using namespace std;

enum class Action{
    ENCRYPT,
    DECRYPT
};

struct Task{
    string filePath;
    fstream f_stream;
    Action action;
    // Task(fstream &&stream, Action act, string filePath){
    //     filePath = filePath;
    //     action = act;
    //     f_stream = move(stream);
    // }
    Task(std::fstream&& stream, Action act, std::string filePath) : f_stream(std::move(stream)), action(act), filePath(filePath) {}
    string toString(){
        ostringstream oss;
        oss<<filePath<<","<<(action==Action::ENCRYPT ? "ENCRYPT":"DECRYPT");
        return oss.str();
    }
    static Task fromString(const std::string &taskData){
        istringstream iss(taskData);
        string filePath;
        string actionStr;
        if(getline(iss, filePath, ',') && getline(iss, actionStr)){
            Action action = (actionStr== "ENCRYPT"?Action::ENCRYPT:Action::DECRYPT);
            IO io(filePath);
            fstream f_stream = move(io.getFileStream());
            if(f_stream.is_open()){
                return Task(move(f_stream), action, filePath);
            }
            else{
                throw runtime_error("Failed to open file: " + filePath);
            }
        }
        else{
            throw runtime_error("Invalid task data format");
        }
    }
};

#endif