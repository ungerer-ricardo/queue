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

template<class Element_t>
class Queue
{
public:
    
    Queue ( const size_t& max_queue_size ) :
        max_queue_size(max_queue_size)
    {}
    
    virtual ~Queue()
    {}
    
    void push (const Element_t& element )
    {
        std::unique_lock<std::mutex> lock ( queue_mutex );
        
        while ( full() )
        {
            push_condition.wait(lock);
        }
        
        queue.push(element);
        
        pop_condition.notify_one();
    }
    
    Element_t pop()
    {
        std::unique_lock<std::mutex> lock( queue_mutex );
    
        while ( empty() )
        {
            pop_condition.wait(lock);
        }
        
        Element_t ret_val = queue.front();
        queue.pop();
        
        push_condition.notify_one();
        
        return ret_val;
    }
    
    size_t count() const
    {
        return this->queue.count();
    }
    
    size_t size() const
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

    std::mutex queue_mutex;
    std::condition_variable push_condition;
    std::condition_variable pop_condition;
};

#endif /* queue_hpp */
