#include <string>
#include <stdio.h>
#include <windows.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>

struct timer {
	typedef std::chrono::steady_clock clock;
	typedef std::chrono::seconds seconds;

	void reset() {
		start = clock::now();
	}

	unsigned long long seconds_elapsed() const {
		return std::chrono::duration_cast<seconds>(clock::now() - start).count();
	}

	private: clock::time_point start = clock::now();
}t;

int add(int &i){
    i += 60;
    return i;
}

std::string returnDate(){
    time_t t = time(0);
	struct tm* now = localtime(&t);
    std::stringstream ss;

	ss << (now->tm_year + 1900) << "/" << (now->tm_mon + 1) << "/" << now->tm_mday << " " << now->tm_hour << "-" << now->tm_min << "-" << now->tm_sec << "\n";

	return ss.str();
}

void checkIfGameIsRunning(){
    int i = 60;
    bool windowFound = false;
    std::string windowName;

    std::cout << "Enter the name of the window > "; std::getline(std::cin, windowName);

    again:if(FindWindowA(NULL, windowName.c_str())){
        windowFound = true;
        printf("We found the window!\n");

        while(FindWindowA(NULL, windowName.c_str())){
            if(t.seconds_elapsed() == i){
                printf("Application %s has been running for %i seconds.\n", windowName.c_str(), i);
                add(i);
                //MessageBoxA(NULL, windowName+" Has been running for 60 seconds", "Information", MB_ICONINFORMATION);
            }
        }

        printf("Window closed!\n");
        if(windowFound){
            std::fstream file;
            file.open("Info.txt", std::fstream::app);

            if(file.is_open()){
                float mins = (t.seconds_elapsed()/60);
                float hours = (mins/60);
                file << "You played " << windowName << " for " << t.seconds_elapsed() << " seconds " << " (" << mins << " mins)" << " (" << hours << " hours)" <<  " the " << returnDate() << std::endl;
                file.close();
            }
            else if(!file.is_open()){
                printf("Could not access or create the file.\n");
                return;
            }
        }
    }
    else{
        char x;
        printf("\nCould not find the window\nTry again?\ny = yes\nn = re-enter name\n> ");
        std::cin >> x;
        if(x == 'y'){
            goto again;
        }
    }
    return;
}

int main() {
    checkIfGameIsRunning();
}
