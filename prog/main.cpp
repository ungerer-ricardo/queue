//
//  main.cpp
//  queue
//
//  Created by Ricardo Ungerer on 30/07/2021.
//
#include <iostream>
#include <thread>

#include <boost/program_options.hpp>

#include <logger.h>

#include "blocking_queue.hpp"
#include "circular_buffer_queue.hpp"
#include "std_queue_adaptor.hpp"

logger::ConsoleLogger global_logger;

typedef struct 
{
    unsigned total;
    unsigned queue_size;
    unsigned timeout;
    unsigned producers;
    unsigned consumers;

    int error = 0;
    bool should_continue = true;
} input_params;

void producer_func( BlockingQueue<int>& q , unsigned total )
{
    for ( int i = 0; i < total; i++ ) {
        if (! q.push( i ) ) {
            global_logger() << "timeout inserting ";
        }
        else {
            global_logger() << "inserted element " << i;
        }
    }

    global_logger() << "Exiting producer...";
}

void consumer_func( BlockingQueue<int>& q, unsigned total )
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

input_params parse_arguments( int argc, const char* argv[] )
{
    namespace po = boost::program_options;

    po::options_description options_def ( "simple queue producer/consumer" );
    options_def.add_options()
        ("help","Produce this message")
        ("total-elements,T", po::value<unsigned>(), "total number of elements to include in the queue" )
        ("queue-size", po::value<unsigned>()->default_value(100), "size of the shared queue")
        ("timeout", po::value<unsigned>()->default_value(100), "Timeout time")
        ("producers,p", po::value<unsigned>()->default_value(1), "Amount of producers threads to run")
        ("consumers,c", po::value<unsigned>()->default_value(1), "Amount of consumer threads to run");

    po::variables_map vm;
    po::store ( po::parse_command_line(argc, argv, options_def), vm);
    po::notify(vm);

    input_params ret_val;

    if ( vm.count("help") )
    {
        std::cout << options_def;
        ret_val.error = 0;
        ret_val.should_continue=false;
        return ret_val;
    }

    if(! vm.count("total-elements")) 
    {
        ret_val.error=1;
        ret_val.should_continue=false;
    }
    else
    {
        ret_val.total = vm["total-elements"].as<unsigned>();
    }

    ret_val.timeout = vm["timeout"].as<unsigned>();
    ret_val.queue_size = vm["queue-size"].as<unsigned>();
    ret_val.producers = vm["producers"].as<unsigned>();
    ret_val.consumers = vm["consumers"].as<unsigned>();

    return ret_val;
}

int main(int argc, const char * argv[]) {

    input_params inputs = parse_arguments(argc, argv);

    if (!inputs.should_continue)
    {
        return inputs.error;
    }

    unsigned elements_per_producer = inputs.total / inputs.producers;
    unsigned elements_per_consumer = inputs.total / inputs.consumers;

    BlockingQueue<int> q(inputs.queue_size, inputs.timeout) ;

    std::thread producers [inputs.producers];
    std::thread consumers [inputs.consumers];

    for( unsigned i =0 ; i< inputs.producers; i++)
    {
        producers[i] = std::thread( producer_func, std::ref(q), elements_per_producer );
    }

    for( unsigned i= 0; i< inputs.consumers; i++ )
    {
        consumers[i] = std::thread( consumer_func, std::ref(q), elements_per_consumer );
    }        

    for( unsigned i = 0 ; i< inputs.producers; i++)
    {
        producers[i].join();
    }

    for( unsigned i= 0; i< inputs.consumers; i++ )
    {
        consumers[i].join();
    }        

    return 0;
}
