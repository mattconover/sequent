#ifndef SEQUENT_HH_
#define SEQUENT_HH_

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace logic
{

// Well-formed formula

template <class>
struct Wff;

namespace detail
{

// Truth-functors

template <std::size_t I>
struct Letter;

struct Falsum;

template <class>
struct Not;

template <class, class>
struct And;

template <class, class>
struct Or;

template <class U, class V>
using Impl = Or<Not<U>, V>;

template <class U, class V>
using Iff = And<Impl<U, V>, Impl<V, U>>;

}

// List of wff

template <class... Ts>
struct List {constexpr List(Wff<Ts>...) {}};

namespace detail
{

template <class Tup, class T>
struct ContainsType;

template <class... Ts, class T>
struct ContainsType<std::tuple<Ts...>, T> :
  std::disjunction<std::is_same<T, Ts>...> {};

template <class Set, class T, bool = ContainsType<Set, T>{}>
struct AddTypeImpl {using type = Set;};

template <class... Ts, class T>
struct AddTypeImpl<std::tuple<Ts...>, T, false>
{
  using type = std::tuple<T, Ts...>;
};

template <class Set, class T>
using AddType = typename AddTypeImpl<Set, T>::type;

//====================================================================
//                    Semantic Tableau Algorithm
//====================================================================

template <
  class Antecedent, class Consequent, class AntecedentLetters = std::tuple<>,
  class ConsequentLetters = std::tuple<>
>
struct SemanticTableau;

//====================== DECOMPOSE ANTECEDENT ========================

// Sentence letter

template <std::size_t I, class... As, class C, class AL, class CL>
struct SemanticTableau<List<Letter<I>, As...>, C, AL, CL> :
  SemanticTableau<List<As...>, C, AddType<AL, Letter<I>>, CL> {};

// Falsum

template <class... As, class C, class AL, class CL>
struct SemanticTableau<List<Falsum, As...>, C, AL, CL> : std::true_type {};

// Not

template <class U, class... As, class... Cs, class AL, class CL>
struct SemanticTableau<List<Not<U>, As...>, List<Cs...>, AL, CL> :
  SemanticTableau<List<As...>, List<U, Cs...>, AL, CL> {};

// And

template <class U, class V, class... As, class C, class AL, class CL>
struct SemanticTableau<List<And<U, V>, As...>, C, AL, CL> :
  SemanticTableau<List<U, V, As...>, C, AL, CL> {};

// Or

template <class U, class V, class... As, class C, class AL, class CL>
struct SemanticTableau<List<Or<U, V>, As...>, C, AL, CL> :
  std::conjunction<
    SemanticTableau<List<U, As...>, C, AL, CL>,
    SemanticTableau<List<V, As...>, C, AL, CL>
  >
{};

//===================== DECOMPOSE CONSEQUENT =========================

// Sentence letter

template <std::size_t I, class... Cs, class AL, class CL>
struct SemanticTableau<List<>, List<Letter<I>, Cs...>, AL, CL> :
  SemanticTableau<List<>, List<Cs...>, AL, AddType<CL, Letter<I>>>
{};

// Falsum

template <class... Cs, class AL, class CL>
struct SemanticTableau<List<>, List<Falsum, Cs...>, AL, CL> :
  SemanticTableau<List<>, List<Cs...>, AL, CL> {};

// Not

template <class U, class... Cs, class AL, class CL>
struct SemanticTableau<List<>, List<Not<U>, Cs...>, AL, CL> :
  SemanticTableau<List<U>, List<Cs...>, AL, CL> {};

// And

template <class U, class V, class... Cs, class AL, class CL>
struct SemanticTableau<List<>, List<And<U, V>, Cs...>, AL, CL> :
  std::conjunction<
    SemanticTableau<List<>, List<U, Cs...>, AL, CL>,
    SemanticTableau<List<>, List<V, Cs...>, AL, CL>
  >
{};

// Or

template <class U, class V, class... Cs, class AL, class CL>
struct SemanticTableau<List<>, List<Or<U, V>, Cs...>, AL, CL> :
  SemanticTableau<List<>, List<U, V, Cs...>, AL, CL>
{};

//========================= CHECK VALIDITY ===========================

template <class... As, class CL>
struct SemanticTableau<List<>, List<>, std::tuple<As...>, CL> :
  std::disjunction<ContainsType<CL, As>...> {};

//====================================================================

}

namespace detail
{

// Using built-in operators to compose sentences

struct WffOps
{
  template <class U>
  friend constexpr auto operator~(Wff<U>) {return create<Not<U>>();}

  template <class U, class V>
  friend constexpr auto operator&(Wff<U>, Wff<V>) {return create<And<U,V>>();}

  template <class U, class V>
  friend constexpr auto operator|(Wff<U>, Wff<V>) {return create<Or<U,V>>();}

  template <class U, class V>
  friend constexpr auto operator<=(Wff<U>, Wff<V>) {return create<Impl<V,U>>();}

  template <class U, class V>
  friend constexpr auto operator==(Wff<U>, Wff<V>) {return create<Iff<U,V>>();}

private:
  template <class U>
  constexpr static Wff<U> create() {return {};}
};

}

// The only public way to construct Wff is by applying the built-in operators
// to the sentence letter and falsum objects.

template <class U>
struct Wff : detail::WffOps
{
private:
  Wff() = default;
  friend detail::WffOps;
};

template <std::size_t I>
struct Wff<detail::Letter<I>> : detail::WffOps {};

template <>
struct Wff<detail::Falsum> : detail::WffOps {};

// Wff constants

constexpr auto falsum = Wff<detail::Falsum>{};

template <std::size_t I>
constexpr auto Pn = Wff<detail::Letter<26 + I>>{};

namespace sentence_letters
{

constexpr auto A = Wff<detail::Letter<0>>{};
constexpr auto B = Wff<detail::Letter<1>>{};
constexpr auto C = Wff<detail::Letter<2>>{};
constexpr auto D = Wff<detail::Letter<3>>{};
constexpr auto E = Wff<detail::Letter<4>>{};
constexpr auto F = Wff<detail::Letter<5>>{};
constexpr auto G = Wff<detail::Letter<6>>{};
constexpr auto H = Wff<detail::Letter<7>>{};
constexpr auto I = Wff<detail::Letter<8>>{};
constexpr auto J = Wff<detail::Letter<9>>{};
constexpr auto K = Wff<detail::Letter<10>>{};
constexpr auto L = Wff<detail::Letter<11>>{};
constexpr auto M = Wff<detail::Letter<12>>{};
constexpr auto N = Wff<detail::Letter<13>>{};
constexpr auto O = Wff<detail::Letter<14>>{};
constexpr auto P = Wff<detail::Letter<15>>{};
constexpr auto Q = Wff<detail::Letter<16>>{};
constexpr auto R = Wff<detail::Letter<17>>{};
constexpr auto S = Wff<detail::Letter<18>>{};
constexpr auto T = Wff<detail::Letter<19>>{};
constexpr auto U = Wff<detail::Letter<20>>{};
constexpr auto V = Wff<detail::Letter<21>>{};
constexpr auto W = Wff<detail::Letter<22>>{};
constexpr auto X = Wff<detail::Letter<23>>{};
constexpr auto Y = Wff<detail::Letter<24>>{};
constexpr auto Z = Wff<detail::Letter<25>>{};

}

// Sequent

template <class Antecedent, class Consequent>
struct Sequent;

template <class... As, class... Cs>
struct Sequent<List<As...>, List<Cs...>> :
  detail::SemanticTableau<List<As...>, List<Cs...>>
{
  constexpr Sequent(List<As...>, List<Cs...>) {};
};

template <class A, class C>
Sequent(A a, C c) -> Sequent<decltype(List(a)), decltype(List(c))>;

template <class C, class A>
constexpr auto operator|=(Wff<A> a, C c) {return Sequent(a, c);}

template <class C, class... As>
constexpr auto operator|=(List<As...> a, C c) {return Sequent(a, c);}

}

#endif
