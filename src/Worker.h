//
// Created by piotr on 11.11.17.
//

#ifndef SHEPHERD_WORKER_H
#define SHEPHERD_WORKER_H

#include <iostream>
#include <boost/process.hpp>

namespace crazygoat::shepherd {

    class Worker {
    protected:
        std::string command, params;
        unsigned short port;
    public:
        unsigned short getPort() const;

    protected:
        std::shared_ptr<boost::process::child> process;
        std::vector<std::string> replacePort(std::string subject, const std::string& search,  const std::string& replace);
        bool isFree;
    public:
        bool isIsFree() const;

        void setIsFree(bool isFree);

    public:
        Worker(std::string command, std::string params, int port);
        void spawn();
        bool isWorking();
    };
}

#endif //SHEPHERD_WORKER_H