#pragma once
#include <vector>
#include <exception>



// A vector acting as a circular buffer that overrides the last element
// if space does not permit adding to an empty locaiton.
template <typename T> class Backlog
{
public:
  // Constructor and destructor
  Backlog(unsigned int size = 20);
  
  // Accessors and methods
  void Add(T &object);
  T &operator[](unsigned int index);
  unsigned int Size();

private:
  // Private mehods
  void movePrev();
  void moveNext();

  // Variables of circular buffer
  std::vector<T> contents_;
  unsigned int nextToReplace_;
  unsigned int size_;
};



  /////////////////////////////
 // Public member variables //
/////////////////////////////
// Instantiates the backlog object with the specified size.
template <typename T>
Backlog<T>::Backlog(unsigned int size)
  : size_(size)
  , nextToReplace_(0)
{
  contents_.resize(size);
}


// Adds the specified item to the log.
template <typename T>
void Backlog<T>::Add(T &obj)
{
  contents_[nextToReplace_] = obj;
  moveNext();
}


// Gets the log item at the specified index from the beginning.
template <typename T>
T &Backlog<T>::operator[](unsigned int index)
{
  if (index >= size_ || index < 0)
    throw "Invalid index accessed!";

  // Figure out the proper index to go to, wrap if necessary.
  // Account for our current position being one past the end with + 1.
  int pos = nextToReplace_ - (index + 1);
  if (pos < 0)
    pos = -pos;

  // Return value at adjusted position.
  return contents_[pos];
}


// Gets the size of the backlog object
template <typename T>
unsigned int Backlog<T>::Size()
{
  return size_;
}


// Move to the previous index in the curcular buffer, content_.
template <typename T>
void Backlog<T>::movePrev()
{
  --nextToReplace_;
  if (nextToReplace_ < 0)
    nextToReplace_ = size_ - 1;
}


// Move to the next index in the circular buffer, content_.
template <typename T>
void Backlog<T>::moveNext()
{
  ++nextToReplace_;
  if (nextToReplace_ >= size_)
    nextToReplace_ = 0;
}
