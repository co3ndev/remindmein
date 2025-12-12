#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
#include <cstdlib>
#include <thread>

void remindAtTime(int hour, int minute, const std::string& message) {
    std::cout << "Reminder set for " << std::setfill('0') 
              << std::setw(2) << hour << ":" 
              << std::setw(2) << minute << " - " << message << std::endl;
    
    std::thread([hour, minute, message]() {
        while (true) {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            auto tm = *std::localtime(&time);
            
            if (tm.tm_hour == hour && tm.tm_min == minute) {
                std::cout << "\n🔔 REMINDER: " << message << std::endl;
                system(("notify-send -u critical 'Reminder' '" + message + "'").c_str());
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }).detach();
}

void remindCountdown(int minutes, const std::string& message) {
    std::cout << "Countdown reminder set for " << minutes << " minutes - " 
              << message << std::endl;
    
    std::thread([minutes, message]() {
        std::this_thread::sleep_for(std::chrono::minutes(minutes));
        std::cout << "\n🔔 REMINDER: " << message << std::endl;
        system(("notify-send -u critical 'Reminder' '" + message + "'").c_str());
    }).detach();
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " time HH MM \"message\"\n"
                  << "  " << argv[0] << " countdown MINUTES \"message\"\n";
        return 1;
    }
    
    std::string mode = argv[1];
    
    if (mode == "time" && argc == 5) {
        int hour = std::stoi(argv[2]);
        int minute = std::stoi(argv[3]);
        std::string message = argv[4];
        remindAtTime(hour, minute, message);
    } else if (mode == "countdown" && argc == 4) {
        int minutes = std::stoi(argv[2]);
        std::string message = argv[3];
        remindCountdown(minutes, message);
    } else {
        std::cerr << "Invalid arguments.\n";
        return 1;
    }
    
    return 0;
}
