#include "ics_exceptions.hpp"

namespace ics {

IcsError::IcsError(const std::string& message) : runtime_error("IcsError " + message) {};
IcsError::~IcsError(){};


EmptyError::EmptyError(const std::string& message) : IcsError("EmptyError " + message) {};
EmptyError::~EmptyError() {};


TemplateFunctionError::TemplateFunctionError(const std::string& message) : IcsError("TemplateFunctionError " + message) {};
TemplateFunctionError::~TemplateFunctionError() {};


ConcurrentModificationError::ConcurrentModificationError(const std::string& message) : IcsError("ConcurrentModificationError " + message) {};
ConcurrentModificationError::~ConcurrentModificationError() {};

ComparingDifferentIteratorsError::ComparingDifferentIteratorsError(const std::string& message) : IcsError("ComparingDifferentIteratorsError " + message) {};
ComparingDifferentIteratorsError::~ComparingDifferentIteratorsError() {};

IteratorTypeError::IteratorTypeError(const std::string& message) : IcsError("IteratorTypeError " + message) {};
IteratorTypeError::~IteratorTypeError() {};


CannotEraseError::CannotEraseError(const std::string& message) :  IcsError("CannotEraseError " + message) {};
CannotEraseError::~CannotEraseError() {};


IteratorPositionIllegal::IteratorPositionIllegal(const std::string& message) : IcsError("IteratorPositionIllegal " + message) {};
IteratorPositionIllegal::~IteratorPositionIllegal() {};

KeyError::KeyError(const std::string& message) : IcsError("KeyError " + message) {};
KeyError::~KeyError() {};


EquivalenceError::EquivalenceError(const std::string& message) : IcsError("EquivalenceError " + message) {};
EquivalenceError::~EquivalenceError() {};

GraphError::GraphError(const std::string& message) : IcsError("GraphError " + message) {};
GraphError::~GraphError() {};

}
