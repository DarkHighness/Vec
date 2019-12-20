#include <functional>
#include <iostream>

template <typename Type, unsigned N>
struct Vec {
  using type = Type;
  using size_type = unsigned;

  Type head;
  Vec<Type,N - 1> tail;

public:
  Vec(const Type head, Vec <Type,N - 1> tail) : head(head), tail(tail) {}

  friend std::ostream &operator<<(std::ostream &os, const Vec &vec) {
    os  << vec.head << " " << vec.tail;
    return os;
  }
};

template <typename Type>
struct Vec<Type,0>{
  using type = Type;
  using size_type = unsigned;

  friend std::ostream &operator<<(std::ostream &os, const Vec<Type, 0> &vec) {
    return os;
  }
};

template <typename Type, unsigned N>
auto cons(Type x, Vec<Type,N> xs) -> Vec<Type,N + 1> {
  return Vec<Type,N + 1>(x,xs);
}

template <typename Type>
auto nil() -> Vec<Type,0> {
  return Vec<Type,0>();
}

template <typename Type>
auto mkVec(Type value) -> Vec<Type,1>{
  return cons(value,nil<Type>());
}

template <typename Type, typename ... Args>
auto mkVec(Type value, Args ... args) -> Vec<Type,
    sizeof...(args) + 1>{
  return cons(value,mkVec(args...));
}

template <typename Type, unsigned N>
auto length(Vec<Type,N>) -> unsigned {
  return N;
}

template <typename Type, unsigned N>
auto head(Vec<Type,N> vec, typename std::enable_if<N >= 0>::type* = nullptr) -> Type {
  return vec.head;
}

template <typename Type, unsigned N>
auto tail(Vec<Type,N> vec, typename std::enable_if<N >= 0>::type* = nullptr) -> Type {
  return tail(vec.tail);
}

template <typename Type>
auto tail(Vec<Type,1> vec) -> Type {
  return vec.head;
}

template <typename Type, unsigned M, unsigned N>
auto append(Vec<Type,M> lhs, Vec<Type,N> rhs) -> Vec<Type,M + N> {
  return cons(lhs.head, append(lhs.tail,rhs));
}

template <typename Type, unsigned N>
auto append(Vec<Type,0>, Vec<Type,N> rhs) -> Vec<Type,N> {
  return rhs;
}

template <typename Type, typename Result>
auto fmap(Vec<Type,0>,std::function<Result(Type)>) -> Vec<Result,0> {
  return Vec<Result,0>();
}

template <typename Type, typename Result, unsigned N>
auto fmap(Vec<Type,N> vec, std::function<Result(Type)> f) -> Vec<Result,N> {
  return cons(f(vec.head),fmap(vec.tail,f));
}

template <typename Type, typename Result, unsigned N>
auto foldr(Vec<Type,N> vec, std::function<Result(Type,Result)> f, Result z) -> Result{
  return f(vec.head, foldr(vec.tail,f,z));
}

template <typename Type, typename Result>
auto foldr(Vec<Type,0>, std::function<Result(Type,Result)>, Result z) -> Result {
  return z;
}

template <typename Type, typename Result, unsigned N>
auto foldl(Vec<Type,N> vec, std::function<Result(Result,Type)> f, Result z) -> Result {
  return foldl(vec.tail,f,f(z, vec.head));
}

template <typename Type, typename Result>
auto foldl(Vec<Type,0>, std::function<Result(Result,Type)>, Result z) -> Result {
  return z;
}

template <typename Type, unsigned N>
auto any(Vec<Type,N> vec, std::function<bool(Type)> predicate) -> bool {
  return predicate(vec.head) || any(vec.tail,predicate);
}

template <typename Type>
auto any(Vec<Type,0>, std::function<bool(Type)>) -> bool {
  return false;
}

template <typename Type, unsigned N>
auto all(Vec<Type,N> vec, std::function<bool(Type)> predicate) -> bool {
  return predicate(vec.head) && any(vec.tail,predicate);
}

template <typename Type>
auto all(Vec<Type,0>, std::function<bool(Type)>) -> bool {
  return true;
}

template <typename Type, unsigned N>
auto elem(Vec<Type,N> vec, Type v) -> bool {
  std::function<bool(Type)> predicate = [v](Type x) -> bool { return x == v; };
  return any(vec, predicate);
}

//template <typename Type, unsigned N>
//auto filter(Vec<Type,N> vec, std::function<bool(Type)> predicate)
//  -> decltype(predicate(vec.head) ? cons(vec.head, filter(vec.tail, predicate)) : filter(vec.tail,predicate)) {
//    return predicate(vec.head) ? cons(vec.head, filter(vec.tail, predicate)) : filter(vec.tail,predicate);
//}
//
//template <typename Type>
//auto filter(Vec<Type,0>, std::function<bool(Type)>) -> auto{
//  return nil<Type>();
//}

int main() {
  auto x = mkVec(1,2);
  auto y = mkVec(3,4);

  std::cout << length(x) << std::endl;
  std::cout << head(x) << std::endl;
  std::cout << tail(x) << std::endl;

  auto z = append(x,y);
  std::cout << length(z) << std::endl;

  std::cout << z << std::endl;
  std::function<int(int)> f = [](int x) -> int { return x + 1; };

  auto k = fmap(z,f);
  std::cout << k << std::endl;

  std::function<int(int,int)> sum = [](int x,int y) -> int { return x + y; };

  auto s = foldr(x,sum,0);

  std::cout << s << std::endl;

  auto i = elem(x,3);
  std::cout << i << std::endl;

//  std::function<bool (int)> even = [](int x) -> bool { return x % 2 == 0; };
//
//  auto t = filter(x, even);
//
//  std::cout << t << std::endl;

  return 0;
}
