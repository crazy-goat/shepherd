//
// Created by piotr on 26.02.17.
//

#include "LoadBalancer.h"

namespace crazygoat::shepherd {

    void LoadBalancer::handleAccept(const boost::system::error_code &error) {
        if (!error) {
            auto worker = this->watchDog->getFreeWorker();
            session->setWorker(worker);
            session->start(
                    "127.0.0.1",
                    worker->getPort()
            );

            if (!acceptConnections()) {
                std::cerr << "Failure during call to accept." << std::endl;
            }
        } else {
            std::cerr << "Very bad error: " << error.message() << std::endl;
        }
    }

    bool LoadBalancer::acceptConnections() {
        try {
            session = boost::shared_ptr<Session>(new Session(ios));
            acceptor->getAcceptor()->async_accept(
                    session->getDownstreamSocket(),
                    boost::bind(
                            &LoadBalancer::handleAccept,
                            this,
                            boost::asio::placeholders::error
                    )
            );
        }
        catch (std::exception &e) {
            std::cerr << "acceptor exception: " << e.what() << std::endl;
            return false;
        }

        return true;
    }

    LoadBalancer::LoadBalancer(const std::shared_ptr<ConfigLoader> &config) :
        config(config)
    {
        this->watchDog = std::make_shared<WatchDog>(this->ios, this->config);
        this->acceptor = std::make_shared<TcpAcceptor>(this->ios, this->config);
    }

    const std::shared_ptr<WatchDog> &LoadBalancer::getWatchDog() const {
        return watchDog;
    }

    void LoadBalancer::run() {
        this->ios.run();
    }
}
