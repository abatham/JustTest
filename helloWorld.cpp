#include <iostream>
#include <chrono>

using namespace std;
int main()
{
	chrono::steady_clock::time_point start = chrono::steady_clock::now();
	cout<<"Hello World\n";
	auto timeNow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	cout<<"It's "<<ctime(&timeNow)<<endl;
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	chrono::steady_clock::duration d = end - start ;
	if ( d == chrono::steady_clock::duration::zero())
		cout<<"That was fast YO!!!!\n";
	cout<<"It took about "<<chrono::duration_cast<chrono::nanoseconds>(d).count()<<" Nano-secs to finish\n";
	return 0;
}
