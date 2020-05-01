#include <iostream>
#include <chrono>

int main()
{
	std::ratio<2,3> r;
	std::cout<<r.num<<"\n";
	std::cout<<r.den<<"\n";

	std::cout<< std::chrono::system_clock::period::num<<"\\"<<std::chrono::system_clock::period::den<<"\n";
	std::cout<<std::chrono::steady_clock::period::num<<"\\"<<std::chrono::steady_clock::period::den<<"\n";

	std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now();
//	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	std::cout<<tp.time_since_epoch().count()<<std::endl;
	auto timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::cout<<ctime(&timeNow)<<std::endl;

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	for (int i =0 ; i<10; i++)
		std::cout<<"Printed\n";
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::chrono::steady_clock::duration elaps = end - start ;
	if ( elaps == std::chrono::steady_clock::duration::zero() )
		std::cout<<"Duration was zero\n";
	std::cout<<std::chrono::duration_cast<std::chrono::nanoseconds>(elaps).count()<<std::endl;
	return 0;
}
