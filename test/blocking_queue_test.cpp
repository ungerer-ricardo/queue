//
//  queue_test.cpp
//  queue
//
//  Created by Ricardo Ungerer on 31/07/2021.
//
#include <gtest/gtest.h>

#include <thread>
#include <blocking_queue.hpp>


TEST(QueueTest, TestNewQueueEmpty)
{
    BlockingQueue<int> q(3);
    int el = 0;

    EXPECT_TRUE( q.empty() );
    
    EXPECT_TRUE( q.push(1) );
    EXPECT_FALSE( q.empty() );
    
    EXPECT_TRUE( q.pop( el ) );
    
    EXPECT_TRUE( q.empty() );
    EXPECT_EQ( el, 1 );
}

TEST(QueueTest, TestFIFO)
{
    BlockingQueue<int> q(3);
    int el = 0;

    EXPECT_TRUE( q.push(1) );
    EXPECT_TRUE( q.push(2) );
    EXPECT_TRUE( q.push(3) );
    
    EXPECT_TRUE( q.pop( el ) );
    EXPECT_EQ( el, 1);
    
    EXPECT_TRUE( q.pop( el ) );
    EXPECT_EQ( el, 2);
    
    EXPECT_TRUE( q.pop( el ) );
    EXPECT_EQ( el , 3);
    
}

TEST(QueueTest, TestNewQueueFull)
{
    BlockingQueue<int> q(1);
    EXPECT_FALSE( q.full() );
    
    EXPECT_TRUE( q.push( 1 ) );
    EXPECT_TRUE( q.full() );
}

TEST(QueueTest, TestTwoThreadsInsertingAndPopping)
{
    BlockingQueue<int> q(3);
    
    auto pusherFunctor = [](BlockingQueue<int>& q)
    {
        EXPECT_TRUE( q.push(1) );
        EXPECT_TRUE( q.push(2) );
        EXPECT_TRUE( q.push(3) );
        EXPECT_TRUE( q.push(4) );
    };

    auto poperFunctor = [](BlockingQueue<int>& q)
    {
        int el;

        EXPECT_TRUE( q.pop( el) );
        EXPECT_EQ(el, 1);
        
        EXPECT_TRUE( q.pop( el ) );
        EXPECT_EQ(el, 2);
        
        EXPECT_TRUE( q.pop( el ) );
        EXPECT_EQ(el, 3);
        
        EXPECT_TRUE( q.pop( el ) );
        EXPECT_EQ(el, 4);
    };
    
    std::thread t1( pusherFunctor, std::ref(q) );
    std::thread t2( poperFunctor, std::ref(q) );
    
    t1.join();
    t2.join();
    
    EXPECT_TRUE(q.empty());
}

TEST(QueueTest, TestTimeoutEmptyQueue )
{
    BlockingQueue<int> q(3,100);
    int el;

    EXPECT_FALSE( q.pop( el ) );
}

TEST( QueueTest, TestTimeoutFullQueue )
{
    BlockingQueue<int>  q(1, 100);

    EXPECT_TRUE( q.push(1) );
    EXPECT_FALSE( q.push(1) );
}

TEST( QueueTest, TestClosingState ) 
{
    BlockingQueue<int>* q = new BlockingQueue<int>(3);

    auto callingPopWhileDestroyingFunctor = [](BlockingQueue<int>* q)
    {
        int el;
        EXPECT_FALSE( q->pop( el) );
    };

    std::thread t1( callingPopWhileDestroyingFunctor, q );
    delete q;

    t1.join();
}
