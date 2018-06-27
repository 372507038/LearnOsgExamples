#include <iostream>
#include <string>
#include <sstream>


#include <OpenThreads/Thread>
#include <OpenThreads/Atomic>
#include <OpenThreads/Condition>
#include <OpenThreads/ScopedLock>
#include <iostream>
#include <string>

// from :https://blog.csdn.net/snail_hunan/article/details/69815539

OpenThreads::Atomic g_ticketCounts(150);
OpenThreads::Mutex g_ticketSellMutex;
OpenThreads::Condition g_condition;

class TicketSellThread : public OpenThreads::Thread
{
public:
	TicketSellThread(const std::string& threadName) : _name(threadName){}
	virtual void run()
	{
		if (_name != std::string("A") ) //����A��ƱԱ����ʼ��������Ʊ��
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> mutlock(g_ticketSellMutex);
			g_condition.wait(&g_ticketSellMutex);
		}
		for (int i = 0; i < 50; i++)
		{
			//��A����20��Ʊ��������ƱԱ�ſ�����Ʊ
			if ( (i == 20) && (_name == std::string("A") ) )
			{
				g_condition.broadcast();
			}
			std::cout << _name << " sell " <<  --g_ticketCounts << std::endl;
		}
	}
private:
	std::string _name;
};


int main(int argc, char** argv)
{
	TicketSellThread ticketSellThreadA("A");
	ticketSellThreadA.start();

	TicketSellThread ticketSellThreadB("B");
	ticketSellThreadB.start();

	TicketSellThread ticketSellThreadC("C");
	ticketSellThreadC.start();


	while(ticketSellThreadA.isRunning())
		OpenThreads::Thread::YieldCurrentThread();

	while(ticketSellThreadB.isRunning())
		OpenThreads::Thread::YieldCurrentThread();

	while(ticketSellThreadC.isRunning())
		OpenThreads::Thread::YieldCurrentThread();

	return 0;
}