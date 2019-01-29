#pragma once

#include <cmath>
#include <stdexcept>

#define PARENT(index) ((index-1)/2)
#define LEFT_CHILD(index) (2*index+1)
#define RIGHT_CHILD(index) (2*index+2)

/// \brief Templated priority element for \link PriorityQueue \endlink
template <class T>
struct Element {
  T value; ///< Templated value (arbitrary)
  float priority; ///< Priority for sorting
};

///
enum PriorityType {MAX_PRIORITY, MIN_PRIORITY};

/// \brief Templated min/max priority queue implemented using a heap of \link Element\endlink objects
template <class T>
class PriorityQueue {
public:
  /// \brief Constructor. Reserves memory for heap
  /// \param max_length maximum length of the queue
  /// \param priority_type specicies either min or max priority queue
  PriorityQueue(unsigned int max_length, PriorityType priority_type) {
    m_heap = new Element<T>[max_length];
    m_max_length = max_length;
    m_length = 0;
    m_priority = priority_type;

    for(unsigned int i=0; i<max_length; i++) {
      m_heap[i].value = T();
      m_heap[i].priority = NAN;
    }
  }

  /// \brief Constructor. Reserves memory for heap and initializes it using given values
  /// \param length length of the value and priority arrays
  /// \param values array of values
  /// \param priorities array of priorities for sorting
  /// \param priority_type specicies either min or max priority queue
  PriorityQueue(unsigned int length, T *values, float *priorities, PriorityType priority_type) {
    m_heap = new Element<T>[length];
    m_max_length = length;
    m_length = length;
    m_priority = priority_type;

    for(unsigned int i=0; i<length; i++) {
      m_heap[i].value = values[i];
      m_heap[i].priority = priorities[i];
    };

    for(int i=length/2; i>=0; i--)
      sink_element(i);
  };

  /// \brief Destructor. Releases memory reserved for heap
  ~PriorityQueue() {
    delete[] m_heap;
  };

  /// \brief Adds new \link Element\endlink to queue
  /// \param value arbitary value
  /// \param priority priority for sorting
  /// \return new length of queue
  int push(T value, float priority) {
    if(m_length >= m_max_length)
      return -1;

    m_heap[m_length].value = value;
    m_heap[m_length].priority = priority;
    float_element(m_length);
    m_length++;
    return m_length;
  };

  /// \brief Removes top \link Element\endlink of the queue
  /// \return \link Element\endlink with highest priority
  Element<T> pop() {
    Element<T> top = peek();
    m_heap[0].value = m_heap[m_length-1].value;
    m_heap[0].priority = m_heap[m_length-1].priority;
    m_length--;
    sink_element(0);
    return top;
  };

  /// \brief Checks but does not remove the top \link Element\endlink of the queue
  /// \return \link Element\endlink with the highest priority
  Element<T> peek() {
    if(m_length == 0)
      throw std::out_of_range("Queue is empty!");

    Element<T> top;
    top.value = m_heap[0].value;
    top.priority = m_heap[0].priority;
    return top;
  };

  /// \brief Checks the length of the queue
  /// \return number of \link Element\endlink in the queue
  unsigned int length() {
    return m_length;
  };

  /// \brief Checks emptiness of queue
  /// \return boolean indicating whether the queue is empty
  bool empty() {
    return m_length == 0;
  }

private:
  /// \brief Helper function to move an \link Element\endlink down the heap until the heap condition is fulfilled
  /// \param index the starting index of the \link Element\endlink
  void sink_element(unsigned int index) {
    if(index > m_length)
      return;

    Element<T> parent = m_heap[index];
    unsigned int left_index = LEFT_CHILD(index);
    Element<T> left_child = m_heap[left_index];
    unsigned int right_index = RIGHT_CHILD(index);
    Element<T> right_child = m_heap[right_index];
    unsigned int switch_index = index;

    if(left_index < m_length &&
       (((m_priority == MAX_PRIORITY) && (left_child.priority > parent.priority)) ||
        ((m_priority == MIN_PRIORITY) && (left_child.priority < parent.priority))))
      switch_index = left_index;

    if(right_index < m_length &&
       (((m_priority == MAX_PRIORITY) && (right_child.priority > m_heap[switch_index].priority)) ||
        ((m_priority == MIN_PRIORITY) && (right_child.priority < m_heap[switch_index].priority))))
      switch_index = right_index;

    if(switch_index != index) {
      swap_elements(index, switch_index);
      sink_element(switch_index);
    }
  };

  /// \brief Helper function to move an \link Element\endlink up the heap until the heap condition is fulfilled
  /// \param index starting index of the \link Element\endlink
  void float_element(unsigned int index) {
    while(index > 0 &&
          (((m_priority == MAX_PRIORITY) && (m_heap[PARENT(index)].priority < m_heap[index].priority)) ||
           ((m_priority == MIN_PRIORITY) && (m_heap[PARENT(index)].priority > m_heap[index].priority)))) {
      swap_elements(index, PARENT(index));
      index = PARENT(index);
    }
  };

  /// \brief Helper function to swap the position of two \link Element\endlink objects on the heap
  /// \param index of the first \link Element\endlink
  /// \param index of the second \link Element\endlink
  void swap_elements(unsigned int index_a, unsigned int index_b) {
    T tmp_value = m_heap[index_a].value;
    float tmp_priority = m_heap[index_a].priority;
    m_heap[index_a].value = m_heap[index_b].value;
    m_heap[index_a].priority = m_heap[index_b].priority;
    m_heap[index_b].value = tmp_value;
    m_heap[index_b].priority = tmp_priority;
  };

  Element<T> *m_heap; ///< Space for \link Element\endlink on the heap
  unsigned int m_max_length; ///< Maximum length of the queue
  unsigned int m_length; ///< Current length of the queue
  PriorityType m_priority; ///< Type of priority queue (min/max)
};
