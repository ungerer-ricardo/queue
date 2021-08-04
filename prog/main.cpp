//
//  main.cpp
//  queue
//
//  Created by Ricardo Ungerer on 30/07/2021.
//

#include <iostream>

#include <thread>

#include <logger.h>

#include "blocking_queue.hpp"
#include "circular_buffer_queue.hpp"

logger::ConsoleLogger global_logger;

const int total=10000000;

void producer_func( BlockingQueue<int>& q , int start )
{
    for ( int i = start; i < start+(total/2); i++ ) {
        if (! q.push( i ) ) {
            global_logger() << "timeout inserting ";
        }
        else {
            global_logger() << "inserted element " << i;
        }
    }

    global_logger() << "Exiting producer...";
}

void consumer_func( BlockingQueue<int>& q )
{
    for ( int i = 0; i < total; i++ ) {
        int front; 
        if(! q.pop( front ) )
        {
            global_logger() << "timeout popping";
        }
        else
        {
            global_logger() << "Popped " << i << " element " << front;
        }
    }

    global_logger() << "Exiting consumer...";
}

int main(int argc, const char * argv[]) {
    // insert code here...
    BlockingQueue<int> q(400, 1000);

    std::thread producer1( producer_func, std::ref(q), 0); 
    std::thread producer2( producer_func, std::ref(q), total/2);
    std::thread consumer( consumer_func, std::ref(q));

    producer1.join();
    producer2.join();
    consumer.join();
    return 0;
}
