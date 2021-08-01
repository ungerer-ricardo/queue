//
//  queue.hpp
//  queue
//
//  Created by Ricardo Ungerer on 30/07/2021.
//

#ifndef queue_hpp
#define queue_hpp

#include <queue>
#include <mutex>
#include <chrono>
#include <limits>

typedef std::chrono::milliseconds millisecs;

template<class Element_t>
class Queue
{
public:
    
    typedef enum {
        ST_RUNNING,
        ST_CLOSING
    } QueueState;

    Queue ( const size_t& max_queue_size, 
            const unsigned& timeout_in_milli = std::numeric_limits<unsigned>::max() ) :
        max_queue_size(max_queue_size),
        timeout_in_milli( timeout_in_milli ),
        my_state(ST_RUNNING)
    {}
    
    virtual ~Queue()
    {
        std::unique_lock<std::mutex> lock( queue_mutex );

        my_state = ST_CLOSING;

        push_condition.notify_all();
        pop_condition.notify_all();
    }
    
    bool push (const Element_t& element )
    {
        if ( my_state != ST_RUNNING ) 
        {
            return false;
        }

        std::unique_lock<std::mutex> lock ( queue_mutex );

        if ( full() && 
             push_condition.wait_for(lock, millisecs( timeout_in_milli ) ) == std::cv_status::timeout )
        {
            return false;
        }

        //Rechecking as the queue could have been closed while waiting
        if (my_state!=ST_RUNNING) 
        {
            return false;
        }
        
        queue.push(element);
        pop_condition.notify_one();
        return true;
    }
    
    bool pop( Element_t& popped_val )
    {
        if( my_state != ST_RUNNING )
        {
            return false;
        }

        std::unique_lock<std::mutex> lock( queue_mutex );
    
        if ( empty() && 
             pop_condition.wait_for( lock, millisecs( timeout_in_milli) ) == std::cv_status::timeout ) 
        {
            return false;
        }

        if (my_state!=ST_RUNNING)
        {
            return false;
        }

        popped_val = queue.front();
        queue.pop();
        push_condition.notify_one();
        return true;
    }
    
    size_t count() const
    {
        return this->queue.count();
    }
    
    size_t max_size() const
    {
        return this->max_queue_size;
    }
    
    bool empty() const
    {
        return queue.empty();
    }
    
    bool full() const
    {
        return queue.size() >= max_queue_size;
    }
    
private:

    std::queue<Element_t> queue;
    const size_t max_queue_size;

    const unsigned timeout_in_milli;

    QueueState my_state;

    std::mutex queue_mutex;
    std::condition_variable push_condition;
    std::condition_variable pop_condition;
};

#endif /* queue_hpp */
