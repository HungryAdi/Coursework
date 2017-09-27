#ifndef ICS_EXCEPTIONS_HPP_
#define ICS_EXCEPTIONS_HPP_

#include <string>
#include <stdexcept>

namespace ics {

class IcsError : public std::runtime_error {
public:
  IcsError(const std::string& message);
  virtual ~IcsError();
protected:
};


class EmptyError : public IcsError {
public:
  EmptyError(const std::string& message);
  virtual ~EmptyError();
};


class TemplateFunctionError : public IcsError {
public:
  TemplateFunctionError(const std::string& message);
  virtual ~TemplateFunctionError();
};


class ConcurrentModificationError : public IcsError {
public:
  ConcurrentModificationError(const std::string& message);
  virtual ~ConcurrentModificationError();
};


class ComparingDifferentIteratorsError : public IcsError {
public:
  ComparingDifferentIteratorsError(const std::string& message);
  virtual ~ComparingDifferentIteratorsError();
};


class IteratorTypeError : public IcsError {
public:
  IteratorTypeError(const std::string& message);
  virtual ~IteratorTypeError();
};


class CannotEraseError : public IcsError {
public:
  CannotEraseError(const std::string& message);
  virtual ~CannotEraseError();
};


class IteratorPositionIllegal : public IcsError {
public:
  IteratorPositionIllegal(const std::string& message);
  virtual ~IteratorPositionIllegal();
};

class KeyError : public IcsError {
public:
  KeyError(const std::string& message);
  virtual ~KeyError();
};


class EquivalenceError : public IcsError {
public:
  EquivalenceError(const std::string& message);
  virtual ~EquivalenceError();
};

class GraphError : public IcsError {
public:
  GraphError(const std::string& message);
  virtual ~GraphError();
};


}

#endif /* ICS_EXCEPTIONS_HPP_ */
