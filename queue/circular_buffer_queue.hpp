#pragma once

#include <iostream>
#include <cstdlib>

template<class Element_t>
class CircularBufferQueue
{
public:
    typedef Element_t* iterator;

    CircularBufferQueue( const size_t& max_element_count ) :
        max_element_count{max_element_count},
        circular_buffer{ new Element_t[max_element_count] },
        current_element_count{0}
    {
        front_it = circular_buffer;
        back_it = circular_buffer;
    }

    CircularBufferQueue( const CircularBufferQueue& q ) :
        max_element_count{q.max_element_count},
        circular_buffer{ new Element_t[q.max_element_count] },
        current_element_count {q.current_element_count}
    {
        for ( unsigned i {0}; i < max_element_count; ++i )
        {
            circular_buffer[i] = q.circular_buffer[i];
        }

        front_it = circular_buffer + (q.front_it - q.circular_buffer);
        back_it = circular_buffer + (q.back_it - q.circular_buffer);
    }

    CircularBufferQueue& operator= ( const CircularBufferQueue& q )
    {
        const_cast<size_t&>(max_element_count) = q.max_element_count;
        current_element_count = q.current_element_count;
        front_it = circular_buffer + (q.front_it - q.circular_buffer);
        back_it = circular_buffer + (q.back_it - q.circular_buffer);

        delete[] circular_buffer;
        circular_buffer = new Element_t[ max_element_count];
        
        for ( unsigned i {0}; i < max_element_count; ++i )
        {
            circular_buffer[i] = q.circular_buffer[i];
        }
        
        return *this;
    }
    
    virtual ~CircularBufferQueue() 
    {
        delete[] circular_buffer;
    }

    bool push ( const Element_t& new_element )
    {
        if (size() >= max_element_count)
        {
            return false;
        }   

        *back_it = new_element;
        ++current_element_count;
        forwardIterator(back_it);

        return true;
    }

    bool pop( Element_t& popped_element )
    {
        if(empty())
        {
            return false;
        }
        
        popped_element = front();
        --current_element_count;
        forwardIterator(front_it);

        return true;
    }

    Element_t front() const
    {
        return *front_it;
    }

    size_t size() const
    {
        return current_element_count;
    }

    bool empty() const
    {
        return (size() == 0);
    }

    size_t max_size() const
    {
        return max_element_count;
    }

    iterator begin() const
    {
        return circular_buffer;
    }

    iterator end() const
    {
        return circular_buffer+max_element_count;
    }

private:

    //TODO: Change this to use std::array
    Element_t* circular_buffer;

    iterator front_it;
    iterator back_it;

    const size_t max_element_count;
    size_t current_element_count;

    void forwardIterator( iterator& it ) 
    {
        it++;

        if (it >= (circular_buffer + max_element_count) )
        {
            it = circular_buffer;
        }
    }

};

