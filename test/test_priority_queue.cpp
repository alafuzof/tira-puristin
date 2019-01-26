#include "gtest/gtest.h"
#include "priority_queue.h"

class CharQueueTest : public ::testing::Test {
public:
  CharQueueTest() {
    min_queue = new PriorityQueue<char>(256, MIN_PRIORITY);
    max_queue = new PriorityQueue<char>(256, MAX_PRIORITY);
  };
  ~CharQueueTest() {
    delete min_queue;
    delete max_queue;
  };
  PriorityQueue<char> *min_queue;
  PriorityQueue<char> *max_queue;
};

TEST_F(CharQueueTest, LengthStartsAsZero) {
  EXPECT_EQ(0, min_queue->length());
  EXPECT_EQ(0, max_queue->length());
}

TEST_F(CharQueueTest, EmptyQueueIsEmpty) {
  EXPECT_EQ(true, min_queue->empty());
  EXPECT_EQ(true, max_queue->empty());
}

TEST_F(CharQueueTest, PeekingEmptyQueueRaisesException) {
  ASSERT_THROW(min_queue->peek(), std::out_of_range);
  ASSERT_THROW(max_queue->peek(), std::out_of_range);
}

TEST_F(CharQueueTest, PoppingEmptyQueueRaisesExceptionAndDoesNotDecreaseLength) {
  ASSERT_THROW(min_queue->pop(), std::out_of_range);
  ASSERT_THROW(max_queue->pop(), std::out_of_range);
  EXPECT_EQ(0, min_queue->length());
  EXPECT_EQ(0, max_queue->length());
}

TEST_F(CharQueueTest, PushingValueIncreasesLengthByOne) {
  min_queue->push('a', 1.0);
  max_queue->push('a', 1.0);
  EXPECT_EQ(1, min_queue->length());
  EXPECT_EQ(1, max_queue->length());
}

TEST_F(CharQueueTest, NonEmptyQueueIsNotEmpty) {
  min_queue->push('a', 1.0);
  max_queue->push('a', 1.0);
  EXPECT_EQ(false, min_queue->empty());
  EXPECT_EQ(false, max_queue->empty());
}

TEST_F(CharQueueTest, PeekingNonEmptyQueueDoesNotChangeLength) {
  min_queue->push('a', 1.0);
  max_queue->push('a', 1.0);
  unsigned int min_len_a = min_queue->length();
  unsigned int max_len_a = max_queue->length();
  min_queue->peek();
  max_queue->peek();
  unsigned int min_len_b = min_queue->length();
  unsigned int max_len_b = max_queue->length();
  EXPECT_EQ(min_len_a, min_len_b);
  EXPECT_EQ(max_len_a, max_len_b);
}

TEST_F(CharQueueTest, PoppingReturnsMaxElementForMaxQueue) {
  max_queue->push('a', 3.0);
  max_queue->push('b', 1.0);
  max_queue->push('c', 7.0);
  max_queue->push('d', 2.0);
  Element<char> top = max_queue->pop();
  EXPECT_EQ('c', top.value);
  top = max_queue->pop();
  EXPECT_EQ('a', top.value);
  top = max_queue->pop();
  EXPECT_EQ('d', top.value);
  top = max_queue->pop();
  EXPECT_EQ('b', top.value);
}

TEST_F(CharQueueTest, PoppingReturnsMinElementsFromMinQueue) {
  min_queue->push('a', 3.0);
  min_queue->push('b', 1.0);
  min_queue->push('c', 7.0);
  min_queue->push('d', 2.0);
  Element<char> top = min_queue->pop();
  EXPECT_EQ('b', top.value);
  top = min_queue->pop();
  EXPECT_EQ('d', top.value);
  top = min_queue->pop();
  EXPECT_EQ('a', top.value);
  top = min_queue->pop();
  EXPECT_EQ('c', top.value);
}
