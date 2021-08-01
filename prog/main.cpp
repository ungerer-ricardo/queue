//
//  main.cpp
//  queue
//
//  Created by Ricardo Ungerer on 30/07/2021.
//

#include <iostream>

#include <thread>

#include <logger.h>

#include "queue.hpp"

logger::ConsoleLogger global_logger;

void producer_func( Queue<int>& q )
{
	for ( int i = 0; i < 10000; i++ ) {
		if (! q.push( i ) ) {
			global_logger() << "timeout inserting ";
		}
		else {
			global_logger() << "inserted element " << i;
		}
	}
}

void consumer_func( Queue<int>& q )
{
	for ( int i = 0; i < 20000; i++ ) {
		int front; 
		if(! q.pop( front ) )
		{
			global_logger() << "timeout popping";
		}
		else
		{
			global_logger() << "Popped element " << front;
		}

	}
}

int main(int argc, const char * argv[]) {
    // insert code here...
    Queue<int> q(100, 1000);

    std::thread producer1( producer_func, std::ref(q));
    std::thread producer2( producer_func, std::ref(q));
    std::thread consumer( consumer_func, std::ref(q));

    producer1.join();
    producer2.join();
    consumer.join();
    return 0;
}
