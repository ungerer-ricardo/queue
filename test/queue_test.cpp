//
//  queue_test.cpp
//  queue
//
//  Created by Ricardo Ungerer on 31/07/2021.
//
#include <gtest/gtest.h>

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

TEST(QueueTest, TestNewQueueIsBlockedWhenPopEmpty)
{

}

TEST(QueueTest, TestNewQueueIsBlockedWhenFull)
{
    
}

