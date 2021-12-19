#include "queue.h"
#include <cassert>

// Macro to check if an exception was thrown
#define assert_throws(x) {                      \
  bool threw{false};                            \
  try { x; }                                    \
  catch (...) { threw = true; }                 \
  assert(threw);                                \
}

int main()
{
  // create empty queue of int, with a maximum of 5 elements
  Queue<int, 5> q1{};

  // const-reference to q1, used to check that all const functions
  // work correctly
  Queue<int, 5> const& q2{q1};

  assert(q1.empty());
  assert(!q1.full());

  // calling front on empty queues should throw an exception
  assert_throws(q1.front());
  assert_throws(q2.front());

  for (int i{10}; i > 0; --i)
  {
    q1.enqueue(i);
    assert(!q1.empty());
    assert(!q1.full());

    assert(q1.front() == i);
    assert(q2.front() == i);

    assert(q1.dequeue() == i);
    assert(q1.empty());
  }

  for (int i{1}; i <= 4; ++i)
  {
    q1.enqueue(i);
    assert(!q1.empty());
    assert(!q1.full());
  }

  q1.enqueue(5);
  assert(!q1.empty());
  assert(q1.full());

  assert_throws(q1.enqueue(6));

  Queue<int, 10> q3 {q1.copy_and_expand<5>()};

  assert(!q3.empty());
  assert(!q3.full());

  for (int i{1}; i <= 4; ++i)
  {
    assert(q1.front() == i);
    assert(q2.front() == i);

    assert(q1.dequeue() == i);
    assert(!q1.empty());
    assert(!q1.full());
  }

  assert(q1.front() == 5);
  assert(q2.front() == 5);

  assert(q1.dequeue() == 5);

  assert_throws(q1.front());
  assert_throws(q2.front());

  for (int i{1}; i <= 4; ++i)
  {
    assert(q3.front() == i);
    assert(q3.dequeue() == i);

    assert(!q3.empty());
    assert(!q3.full());
  }

  assert(q3.front() == 5);
  assert(q3.dequeue() == 5);
  assert(q3.empty());
  assert(!q3.full());

}
