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
    
    Queue ( const size_t& max_queue_size, 
            const unsigned& timeout_in_milli = std::numeric_limits<unsigned>::max() ) :
        max_queue_size(max_queue_size),
        timeout_in_milli( timeout_in_milli )
    {}
    
    virtual ~Queue()
    {}
    
    bool push (const Element_t& element )
    {
        std::unique_lock<std::mutex> lock ( queue_mutex );
        
        bool ret_val = true;
        if ( full() && 
             push_condition.wait_for(lock, millisecs( timeout_in_milli ) ) == std::cv_status::timeout )
        {
            ret_val = false;
        }
        else
        {
            queue.push(element);
            pop_condition.notify_one();
        }
        
        return ret_val;
    }
    
    bool pop( Element_t& popped_val )
    {
        std::unique_lock<std::mutex> lock( queue_mutex );
    
        bool ret_val = true;
        if ( empty() && 
             pop_condition.wait_for( lock, millisecs( timeout_in_milli) ) == std::cv_status::timeout ) 
        {
            ret_val = false;
        }
        else 
        {
            popped_val = queue.front();
            queue.pop();
            push_condition.notify_one();
        }
        
        return ret_val;
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

    std::mutex queue_mutex;
    std::condition_variable push_condition;
    std::condition_variable pop_condition;
};

#endif /* queue_hpp */
