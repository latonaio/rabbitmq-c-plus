#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <iostream>
#include <chrono>
#include <thread>

using json = nlohmann::json;

using namespace AmqpClient;

int main() {
    std::cout << "start consume" << std::endl;

    const char* rabbitmqHostIp = std::getenv("RABBITMQ_HOST_IP");
    const char* rabbitmqUser = std::getenv("RABBITMQ_USER");
    const char* rabbitmqPassword = std::getenv("RABBITMQ_PASSWORD");
    const char* rabbitmqPortStr = std::getenv("RABBITMQ_PORT");
    const char* rabbitmqVirtualHost = std::getenv("RABBITMQ_VIRTUAL_HOST");
    const char* rabbitmqQueueFrom = std::getenv("RABBITMQ_QUEUE_FROM");

    Channel::ptr_t channel;

    int rabbitmqPort = std::stoi(rabbitmqPortStr);

    channel = Channel::Create(
            rabbitmqHostIp, rabbitmqPort, rabbitmqUser, rabbitmqPassword, rabbitmqVirtualHost
    );

    channel->BindQueue(rabbitmqQueueFrom, "amq.fanout","");
    channel->BasicConsume(rabbitmqQueueFrom, rabbitmqQueueFrom);

    while(true)
    {
        std::string a;
        Envelope::ptr_t envelope;
        bool flag = channel->BasicConsumeMessage(rabbitmqQueueFrom, envelope, 7000);
        if (flag == false)
        {
            //cout << "timeout\n" << endl;
            //break;
            continue;
        }

        a = envelope->Message()->Body();

        std::cout << a << std::endl;
    }

    std::cout << "OK"<< std::endl;
}
