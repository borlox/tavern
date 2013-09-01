#ifndef _DELEGATES_H_
#define _DELEGATES_H_

#include <functional>

namespace sfg{

class Delegate {
  public:
    // Default constructor
    Delegate() {}

    // Bind to global functions and contextless lambdas
    Delegate(void (__cdecl *func)(void)){
      m_function = func; }

    // Bind to std::function objects(lambdas with context, std::bind)
    Delegate(std::function<void(void)> func ) {
      m_function = func; }

    // Bind to member functions
    template < class Class >
    Delegate(Class *pthis, void(Class::*function_to_bind)() ) {
      m_function = std::bind(function_to_bind, pthis); }

    // Bind to const member functions.
    template < class Class >
    Delegate(const Class *pthis, void (Class::*function_to_bind)() const) {
      m_function = std::bind(function_to_bind, pthis); }

    // Call the stored function
    void operator() () const {
      m_function(); }

    // Copying and assigning std::function objects seems safe, so I'm letting
    // the compilier generate copy constructers and asignment operators for me.

  private:
    std::function<void(void)> m_function;
};

} // namespace

#endif //_DELEGATES_H_
