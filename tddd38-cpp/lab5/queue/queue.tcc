template <typename T, unsigned N>
void Queue<T, N>::enqueue(T value)
{
  if (full())
  {
    throw queue_error{"Cannot enqueue into full queue"};
  }

  data[(tail++) % N] = value;
}

template <typename T, unsigned N>
T Queue<T, N>::dequeue()
{
  if (empty())
  {
    throw queue_error{"Cannot dequeue from empty queue"};
  }

  T value {std::move(data[(head++) % N])};
  return value;
}

template <typename T, unsigned N>
bool Queue<T, N>::empty() const
{
  return head == tail;
}

template <typename T, unsigned N>
bool Queue<T, N>::full() const
{
  return tail - head >= N;
}

template <typename T, unsigned N>
void Queue<T, N>::clear()
{
  head = tail = 0;
}

template <typename T, unsigned N>
T& Queue<T, N>::front()
{
  return const_cast<T&>(static_cast<Queue<T, N> const*>(this)->front());
}

template <typename T, unsigned N>
T const& Queue<T, N>::front() const
{
  if (empty())
  {
    throw queue_error{"Queue is empty"};
  }

  return data[head % N];
}

template <typename T, unsigned N>
template <int M>
Queue<T, N + M> Queue<T, N>::copy_and_expand() const
{
  Queue<T, N + M> result{};
  unsigned size {tail - head};

  for (unsigned i{0}; i < size; ++i)
  {
    result.data[i] = data[(i + head) % N];
  }
  result.tail = size;
  return result;
}
  
