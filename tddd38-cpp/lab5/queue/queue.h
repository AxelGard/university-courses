#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdexcept>
#include <utility>

class queue_error : std::runtime_error
{
public:
  using std::runtime_error::runtime_error;
};

template <typename T, unsigned N>
class Queue
{
public:

  static_assert(N > 0, "Queue must have space for at least 1 value");

  void enqueue(T value);
  T dequeue();

  bool empty() const;
  bool full() const;

  void clear();

  T& front();
  T const& front() const;

  template <int M>
  Queue<T, N + M> copy_and_expand() const;

  template <typename U, unsigned M>
  friend class Queue;

private:

  T data[N]{};
  unsigned head{};
  unsigned tail{};

};

#include "queue.tcc"
#endif//QUEUE_H_
