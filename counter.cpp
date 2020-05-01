#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
using namespace std::chrono;
typedef unsigned long long ull;
void show()
{
    cout<<"I am running inside a thread"<<endl;
    cout<<"thread ID is "<<this_thread::get_id()<<endl;
}

void oddSum(ull start, ull end, ull sum)
{
    for(ull i = start; i <= end; i++)
    {
        if((i & 1)==1)
            sum+=i;
    }
    cout<<"Odd sum:"<<sum<<endl;
}

void evenSum(ull start, ull end, ull sum)
{
    for(ull i = start; i <= end; i++)
    {
        if((i & 1)==0)
            sum+=i;
    }
    cout<<"Even Sum:"<<sum<<endl;
}

int main()
{


    ull oddTot=0, evenTot=0;
    ull start=0, end=1900000000;
    auto startTime = high_resolution_clock::now();
    thread t2(oddSum,start,end,oddTot);
    thread t3(evenSum,start,end,evenTot);
    t2.join();
    t3.join();
//    oddSum(start,end,oddTot);
//    evenSum(start,end,evenTot);
    auto endTime=high_resolution_clock::now();
    auto duration=duration_cast<microseconds>(endTime-startTime);
    cout<<"total time spent in execution: "<<duration.count()/1000000<<" oddTot: "<<oddTot<<" evenTot: "<<evenTot<<endl;
    cout<<"Total supported concurrent threads: "<<std::thread::hardware_concurrency();


    return 0;
}
