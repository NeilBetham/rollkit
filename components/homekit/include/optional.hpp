#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <utility>

template <typename T>
class optional {
public:
  typedef T value_type;

  optional() {}
  optional(T const &val) : _has_value(true), _value(val) {}
  optional(T &&val) : _has_value(true) { _value = std::move(val); }

  template <class U>
  optional(optional<U> const &rhs) : _has_value(rhs._has_value), _value(rhs.value()) {}

  template <class U>
  optional& operator=(optional<U> const &rhs) {
      _has_value = rhs._has_value;
      _value = rhs.value();
      return *this;
  }

  template <class U>
  optional& operator=(optional<U> &&rhs) {
      _has_value = rhs._has_value;
      _value = std::move(rhs.value());
      return *this;
  }

  const T& operator*() const { return value(); }
  T& operator*() { return value(); }
  const T* operator->() const { return &_value; }
  T* operator->() { return &_value; }

  operator bool() const { return _has_value; }
  bool has_value() const { return _has_value; }

  const T& value() const {
      return _value;
  }

  T& value() {
      return _value;
  }

  template <class U>
  T value_or(U const &v) const {
      return _has_value ? value() : static_cast<T>(v);
  }

  void reset() { _has_value = false; }

private:
  bool _has_value = false;
  T _value;
};


template <typename T, typename U>
inline bool operator==(optional<T> const &x, optional<U> const &y) {
    return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
}

template <typename T, typename U>
inline bool operator!=(optional<T> const &x, optional<U> const &y) {
    return !(x == y);
}

template <typename T, typename U>
inline bool operator==(optional<T> const &x, U const &v) {
    return bool(x) ? *x == v : false;
}

template <typename T, typename U>
inline bool operator==(U const &v, optional<T> const &x) {
    return bool(x) ? v == *x : false;
}

template <typename T, typename U>
inline bool operator!=(optional<T> const &x, U const &v) {
    return bool(x) ? *x != v : true;
}

template <typename T, typename U>
inline bool operator!=(U const &v, optional<T> const &x) {
    return bool(x) ? v != *x : true;
}

template <typename T>
inline optional<T> make_optional(T const &v) {
    return optional<T>(v);
}


#endif // OPTIONAL_HPP
