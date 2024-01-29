// Jacob Martin
#ifndef DOUBLE_HASHING_H
#define DOUBLE_HASHING_H

#include <vector>
#include <algorithm>
#include <functional>

namespace {
// Internal method to test if a positive number is prime.
bool IsPrime_Double(size_t n) 
{
  if( n == 2 || n == 3 )
    return true;
  if( n == 1 || n % 2 == 0 )
    return false;
  for( int i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  return true;
}


// Internal method to return a prime number at least as large as n.
int NextPrime_Double(size_t n) 
{
  if (n % 2 == 0)
    ++n;  
  while (!IsPrime_Double(n)) n += 2;  
    return n;
}
}  // namespace

// Quadratic probing implementation.
template <typename HashedObj>
class HashTableDouble {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  //Constructor for hashtable object
  explicit HashTableDouble(size_t size = 101) : array_(NextPrime_Double(size))
    { MakeEmpty(); }
  
  //checks if the value(x) is in the hashtable
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }
  
  //Empties hashtable
  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  //finds position and returns true if value(x) was succesfully inserted into hashtable
  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
  
  //finds position and returns true if value(x) was succesfully inserted into hashtable(using move)
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }
  
  //Finds and Removes value(x) from hashtable
  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }
  
  //Number 1(A)
  //return size of hash table (T)
  int getSize() const
  {
    return array_.size();
  }
  //return total number of elements in the table (N)
  int getCurrentSize() const
  {
    return current_size_;
  }
  //return total number of collisions (C)
  int getCollisions() const
  {
    return collisions_;
  }
  // resets the total number of collisions
  void resetNumOfCollisions()
  {
    collisions_ = 0;
  }

 private: 
  //type of item in vector<HashEntry> array_      
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    
  //variables
  std::vector<HashEntry> array_;
  size_t current_size_;
  mutable int collisions_ = 0;

  //checks if the index trying to be inserted into is (1)already occupied, (2)avalible for insertion
  //or (3)lazy deleted
  bool IsActive(size_t current_pos) const
  { 
    return array_[current_pos].info_ == ACTIVE; 
  }

  //finds the array index that the value(x) should be inserted in
  size_t FindPos(const HashedObj & x) const 
  { 
    static std::hash<HashedObj> hf2;
    size_t offset = 89 - (hf2(x) % 89);
    size_t current_pos = InternalHash(x);
    while (array_[current_pos].info_ != EMPTY && array_[current_pos].element_ != x) 
    {
      current_pos += offset;  // Compute ith probe. <----------
      collisions_++;//increase collision count
      if (current_pos >= array_.size())
        current_pos -= array_.size();
    }
    return current_pos;
  }

  //doubles table size and reharshes all the values into the new table
  void Rehash() 
  {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime_Double(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }
  
  //Mods the tablesize by the value being entered,
  //thus computing the index that it should be inserted into
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }

};

#endif  // DOUBLE_HASHING_H