#include <thread>
#include <string>
#include <mutex>
#include <iostream>
#include <fstream>

using namespace std;

std::mutex mu;

class LogFile
{
	std::mutex m_mutex;
	std::ofstream f;
public:
	LogFile()
	{
		f.open("log.txt");
	}//Need destructor to close file

	void shared_print(string id, int value)
	{
		//std::lock_guard<std::mutex> guard(mu);
		std::lock_guard<mutex> locker(m_mutex);
		f << "From" << id << ": " << value << endl;
		//mu.lock();
		//cout << msg << id << endl;
		//mu.unlock();
	}//Never return f to the outside world

	ofstream& getStream()
	{
		return f;
	}//Never pass f as an augument to user provided function

	void processf(void fun(ofstream&))
	{
		fun(f);
	}
};


void function_1(LogFile& log)
{
	for (int i = 0; i > -100; i--) 
	{
		log.shared_print(string("From t1: "), i);
	}
}

int main()
{
	LogFile log;
	std::thread t1(function_1, std::ref(log));

	for (int i = 0; i < 100; i++) 
	{
		log.shared_print(string("From main: "), i);
	}

	t1.join();

	return 0;
}