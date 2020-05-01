#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
void fun()
{
	chrono::steady_clock::time_point start = chrono::steady_clock::now();
	auto tim = chrono::system_clock::to_time_t(chrono::system_clock::now());

	cout<<"Hello\nIts "<<ctime(&tim)<<"\n";
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	chrono::steady_clock::duration d = end - start;
	if ( d == chrono::steady_clock::duration::zero())
		cout<<"Thats fast\n";
	else
		cout<<"Thread:"<<chrono::duration_cast<chrono::nanoseconds>(d).count()<<"\n";
	
	throw;
}
int main()
{
	chrono::steady_clock::time_point start = chrono::steady_clock::now();
	thread t1(fun);
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	chrono::steady_clock::duration d = end - start;
	if ( d == chrono::steady_clock::duration::zero())
		cout<<"Instant!!!\n";
	else
		cout<<"Main:"<<chrono::duration_cast<chrono::nanoseconds>(d).count()<<"\n";
	if ( t1.joinable())
		t1.join();

	return 0;
}
