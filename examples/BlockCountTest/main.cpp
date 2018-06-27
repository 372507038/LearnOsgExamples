#include <OpenThreads/Thread>
#include <OpenThreads/Atomic>
#include <OpenThreads/Block>
#include <iostream>
#include <string>

// from: https://blog.csdn.net/snail_hunan/article/details/69848465

OpenThreads::Atomic g_ticketCounts(150);
OpenThreads::BlockCount g_blockCount(2);

class TicketSellThread : public OpenThreads::Thread
{
public:
	TicketSellThread(const std::string& threadName) : _name(threadName){}
	virtual void run()
	{
		//A�������Լ�
		if ( _name == std::string("A") )    
		{
			g_blockCount.reset();
			g_blockCount.block();
		}
		for (int i = 0; i < 50; i++)
		{
			std::cout << _name << " sell " <<  --g_ticketCounts << std::endl;
		}
		//B��C����Լ���ѭ��������block���һ�Ρ�
		//��ΪBlockCount����Ϊ2�����Ե�B��C����ɺ�A�������С�
		if (_name != std::string("A"))
		{
			g_blockCount.completed();
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