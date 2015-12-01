#pragma once

#include <stdexcept>

namespace savage {
/**
 * Used for recoverable problems, such as bad user data.
 */
class savage_exception : public std::runtime_error {
public:
  savage_exception( const std::string& m )
    : std::runtime_error( m ) {}
};

/**
 * Used for problems that cannot be recovered from, such as bad built-in data or bad control flow.
 */
class savage_error : public std::runtime_error {
public:
  savage_error( const std::string& m )
    : std::runtime_error( m ) {}
};
} // namespace savage
