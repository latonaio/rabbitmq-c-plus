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
    std::ifstream f("example.json");
    json data = json::parse(f);

    std::cout << "start publish" << std::endl;

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

    std::string messageBody = data.dump();

    channel->BasicPublish("", rabbitmqQueueFrom, AmqpClient::BasicMessage::Create(messageBody));

    std::cout << "finish publish" << std::endl;

    return 0;
}
