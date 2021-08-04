#include <circular_buffer_queue.hpp>

#include <gtest/gtest.h>

#include <logger.h>

logger::ConsoleLogger global_logger;

TEST(CircularBufferQueueTest, pushOnEmptyQueueUntilFull)
{
    CircularBufferQueue<int> q(3);
    CircularBufferQueue<int>::iterator it = q.begin();

    EXPECT_TRUE(q.push(1));
    EXPECT_EQ(q.front(), 1);
    EXPECT_EQ(*it, 1);

    EXPECT_TRUE(q.push(2));
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*(it+1), 2);

    EXPECT_TRUE(q.push(3));
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*(it+1), 2);
    EXPECT_EQ(*(it+2), 3);

    EXPECT_FALSE(q.push(4));
}

TEST(CircularBufferQueueTest, popOnEmptyQueue )
{
    CircularBufferQueue<int> q(3);
    int el;

    EXPECT_FALSE( q.pop(el) );
}

TEST(CircularBufferQueueTest, popElementFromQueue )
{
    CircularBufferQueue<int> q(3);

    int el;

    q.push(1);
    q.push(2);

    EXPECT_TRUE( q.pop(el) );
    EXPECT_EQ( el, 1);

    EXPECT_TRUE( q.pop(el) );
    EXPECT_EQ( el, 2);

    EXPECT_FALSE( q.pop(el) );
}

TEST(CircularBufferQueueTest, pushPopElementSizeTest)
{
    CircularBufferQueue<int> q(3);
    CircularBufferQueue<int>::iterator it = q.begin();
    int el;

    q.push(1); //1 - - 
    q.push(2); //1 2 -
    q.push(3); //1 2 3
    EXPECT_EQ( q.size(), 3);
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*(it+1), 2);
    EXPECT_EQ(*(it+2), 3);

    q.pop(el); //- 2 3
    q.push(4); //4 2 3
    EXPECT_EQ(*it, 4);
    EXPECT_EQ(*(it+1), 2);
    EXPECT_EQ(*(it+2), 3);

    EXPECT_TRUE( q.pop(el) ); //4 - 3
    EXPECT_EQ( el, 2); 
    EXPECT_EQ( q.size(), 2); 

    EXPECT_TRUE( q.push(5) ); //4 5 3
    EXPECT_EQ( q.size(), 3 );
    EXPECT_EQ(*it, 4);
    EXPECT_EQ(*(it+1), 5);
    EXPECT_EQ(*(it+2), 3);

    q.pop(el); //4 5 -
    EXPECT_EQ( el, 3 );

    q.pop(el); //- 5 -
    EXPECT_EQ(el, 4);

    q.pop(el); // - - -
    EXPECT_EQ(el, 5);
    EXPECT_EQ(q.size(), 0);

    EXPECT_TRUE( q.push(6) ); //4 5 3
    EXPECT_EQ(*it, 4);
    EXPECT_EQ(*(it+1), 5);
    EXPECT_EQ(*(it+2), 6);

    EXPECT_TRUE( q.push(7) ); //4 5 3
    EXPECT_EQ(*it, 7);
    EXPECT_EQ(*(it+1), 5);
    EXPECT_EQ(*(it+2), 6);
}

TEST(CircularBufferQueueTest, NoSizedQueue)
{
    CircularBufferQueue<int> q(0);

    EXPECT_FALSE( q.push(1) );

    int el;
    EXPECT_FALSE( q.pop(el));
}

