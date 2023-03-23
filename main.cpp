#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http; // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio; // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

int main()
{
    try
    {
        // Create an IO context with one thread
        net::io_context ioc{1};

        // Create a TCP acceptor and bind it to port 8083 on localhost
        tcp::acceptor acceptor{ioc, {tcp::v4(), 8765}};

        // Wait for incoming connections and handle them asynchronously
        while (true)
        {
            // Wait for a new client to connect
            tcp::socket socket{ioc};
            acceptor.accept(socket);

            // Create a new WebSocket stream from the connected socket
            websocket::stream<tcp::socket> ws{std::move(socket)};

            // Accept the WebSocket handshake from the client
            ws.accept();
            while (true){
                try
                {
                    // Generate random latitude and longitude
                    double latitude = -90.0 + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (90.0 + 90.0)));
                    double longitude = -180.0 + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (180.0 + 180.0)));
                    // Get the current timestamp
                    std::time_t timestamp = std::time(nullptr);
                    
                    // Construct a JSON message with the data
                    std::ostringstream oss;
                    oss << "{";
                    oss << "\"latitude\": " << latitude << ",";
                    oss << "\"longitude\": " << longitude << ",";
                    oss << "\"timestamp\": " << timestamp;
                    oss << "}";
                    
                    // Send the message to the server
                    ws.write(net::buffer(oss.str()));

                    // Wait for one second before sending the next message
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                catch(beast::system_error const& se)
                {
                    if (se.code()!=websocket::error::closed)
                    {
                        std::cout << se.code().message() << std::endl;
                        break;
                    }
                }
            }

            // Close the WebSocket connection
            //ws.close(websocket::close_code::normal);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

