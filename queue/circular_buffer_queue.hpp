#include <cstdlib>

template<class Element_t>
class CircularBufferQueue
{
public:


    CircularBufferQueue( const size_t& max_element_count ) :
        circular_buffer( new Element_t[max_element_count]),
        max_element_count(max_element_count)
    {
        front_it = circular_buffer;
        back_it = circular_buffer;
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
        forwardIterator(back_it);

        return true;
    }

    bool pop( Element_t& popped_element )
    {
        if(size() == 0)
        {
            return false;
        }
        popped_element = front();
        forwardIterator(front_it);

        return true;
    }

    Element_t front() const
    {
        return *front_it;
    }

    size_t size()
    {
        size_t ret_val = 0;

        if ( back_it < front_it )
        {
//          ret_val = (back_it - circular_buffer) + ( (circular_buffer + max_element_count ) - front_it ) +1;
            ret_val = back_it - front_it + max_element_count + 1;
        }
        else if (back_it > front_it )
        {
            ret_val = back_it - front_it;
        }

        return ret_val;
    }

private:

    Element_t* circular_buffer;

    Element_t* front_it;
    Element_t* back_it;

    size_t max_element_count;

    void forwardIterator( Element_t*& it ) 
    {
        it++;

        if (it > (circular_buffer + max_element_count) )
        {
            it = circular_buffer;
        }   
    }

};
