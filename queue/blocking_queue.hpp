//
//  queue.hpp
//  queue
//
//  Created by Ricardo Ungerer on 30/07/2021.
//
#pragma once

#include <queue>
#include <mutex>
#include <chrono>
#include <limits>
#include <condition_variable>

#include <logger.h>

#include "circular_buffer_queue.hpp"

extern logger::ConsoleLogger global_logger;

typedef std::chrono::milliseconds millisecs;

template<class Element_t, 
         class Queue_t = CircularBufferQueue<Element_t>, 
         class Mutex_t = std::mutex,
         class CondVar_t = std::condition_variable>
class BlockingQueue
{
public:
    
    enum class QueueState {
        RUNNING,
        CLOSING
    };

    BlockingQueue ( const size_t& max_queue_size, 
                    const unsigned& timeout_in_milli = std::numeric_limits<unsigned>::max() ) :
        queue(max_queue_size),
        timeout_in_milli( timeout_in_milli ),
        my_state(QueueState::RUNNING)
    {}
    
    virtual ~BlockingQueue()
    {
        std::unique_lock<Mutex_t> lock( queue_mutex );

        my_state = QueueState::CLOSING;

        push_condition.notify_all();
        pop_condition.notify_all();
    }
    
    bool push (const Element_t& element )
    {
        if ( my_state != QueueState::RUNNING )
        {
            return false;
        }

        std::unique_lock<Mutex_t> lock ( queue_mutex );

        while ( full() )
        {
            if (push_condition.wait_for(lock, millisecs( timeout_in_milli ) ) == std::cv_status::timeout)
            {
                global_logger() << "push timeout on element " << element;
                return false;
            }
        }

        //Rechecking as the queue could have been closed while waiting
        if (my_state!=QueueState::RUNNING)
        {
            global_logger() << "trying to push on a closed queue";
            return false;
        }

        queue.push(element);        
        pop_condition.notify_one();
        return true;
    }
    
    bool pop( Element_t& popped_val )
    {
        if( my_state != QueueState::RUNNING )
        {
            return false;
        }

        std::unique_lock<Mutex_t> lock( queue_mutex );
    
        while ( empty() ) 
        {
            if ( pop_condition.wait_for( lock, millisecs( timeout_in_milli) ) == std::cv_status::timeout )
            {
                global_logger() << "pop timeout.";
                return false;
            }
        }

        if (my_state!=QueueState::RUNNING)
        {
            return false;
        }

        queue.pop( popped_val);
        push_condition.notify_one();
        return true;
    }
    
    size_t max_size() const
    {
        return queue.max_size();
    }
    
    bool empty() const
    {
        return queue.empty();
    }
    
    bool full() const
    {
        return queue.size() >= queue.max_size();
    }
    
private:

    Queue_t queue;
    const unsigned timeout_in_milli;

    QueueState my_state;

    Mutex_t queue_mutex;
    CondVar_t push_condition;
    CondVar_t pop_condition;
};
