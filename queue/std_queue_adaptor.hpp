#ifndef STD_QUEUE_ADAPTOR_H_
#define STD_QUEUE_ADAPTOR_H_

#include <queue>
#include <cstdlib>

template<class Element_t>
class StdQueueAdaptor
{
public:
    
    StdQueueAdaptor( const size_t& max_element_count) :
        max_element_count(max_element_count),
        current_element_count(0)
    {}

    bool push( const Element_t& new_element )
    {
        if ( current_element_count >= max_element_count)
        {
            return false;
        }
        queue.push(new_element);
        return true;
    }

    bool pop( Element_t& popped_element )
    {
        if (empty())
        {
            return false;
        }
        popped_element = front();
        queue.pop();
        return true;
    }

    Element_t front() const 
    {
        return queue.front();
    }

    size_t size() const
    {
        return queue.size();
    }

    bool empty() const
    {
        return queue.empty();
    }

    size_t max_size() const
    {
        return max_element_count;
    }

private:

    std::queue<Element_t> queue;

    const size_t max_element_count;
    size_t current_element_count;
};

#endif