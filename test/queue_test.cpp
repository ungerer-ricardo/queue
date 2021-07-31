//
//  queue_test.cpp
//  queue
//
//  Created by Ricardo Ungerer on 31/07/2021.
//
#include <gtest/gtest.h>

#include <thread>
#include <queue.hpp>


TEST(QueueTest, TestNewQueueEmpty)
{
    Queue<int> q(3);
    EXPECT_TRUE( q.empty() );
    
    q.push(1);
    EXPECT_FALSE( q.empty() );
    
    int el = q.pop();
    
    EXPECT_TRUE( q.empty() );
    EXPECT_EQ( el, 1 );
}

TEST(QueueTest, TestFIFO)
{
    Queue<int> q(3);
    
    q.push(1);
    q.push(2);
    q.push(3);
    
    int el = q.pop();
    EXPECT_EQ( el, 1);
    
    el = q.pop();
    EXPECT_EQ( el, 2);
    
    el = q.pop();
    EXPECT_EQ( el , 3);
    
}

TEST(QueueTest, TestNewQueueFull)
{
    Queue<int> q(1);
    EXPECT_FALSE( q.full() );
    
    q.push( 1 );
    EXPECT_TRUE( q.full() );
}

TEST(QueueTest, TestTwoThreadsInsertingAndPopping)
{
    Queue<int> q(3);
    
    auto pusherFunctor = [](Queue<int>& q)
    {
        q.push(1);
        q.push(2);
        q.push(3);
        
        EXPECT_TRUE(q.full() );
        q.push(4);
    };

    auto poperFunctor = [](Queue<int>& q)
    {
        int el = q.pop();
        EXPECT_EQ(el, 1);
        
        el = q.pop();
        EXPECT_EQ(el, 2);
        
        el = q.pop();
        EXPECT_EQ(el, 3);
        
        el = q.pop();
        EXPECT_EQ(el, 4);
    };
    
    std::thread t1( pusherFunctor, std::ref(q) );
    std::thread t2( poperFunctor, std::ref(q) );
    
    t1.join();
    t2.join();
    
    EXPECT_TRUE(q.empty());
}
