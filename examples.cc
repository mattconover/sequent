#include "sequent.hh"

using namespace logic;
using namespace logic::sentence_letters;

static_assert((P |= P));            // Law of identity
static_assert((List() |= ~(P&~P))); // Law of non-contradiction
static_assert((List() |= P|~P));    // Law of excluded middle

static_assert((List(P, Q<=P) |= Q));               // Modus ponens
static_assert((List(P, Q) |= P&Q));                // Conjunction introduction
static_assert((P&Q |= P));                         // Conjunction elimination
static_assert((P |= P|Q));                         // Disjunction introduction
static_assert((List(Q<=P, Q<=R, P|R) |= Q));       // Disjunction elimination
static_assert((List(Q<=P, P<=Q) |= P==Q));         // Biconditional introduction
static_assert((P==Q |= Q<=P));                     // Biconditional elimination
static_assert((List(Q<=P, ~Q<=P) |= ~P));          // Negation introduction
static_assert((List(P, ~P) |= falsum));            // Negation elimination
static_assert((List(Q<=P, R<=Q) |= R<=P));         // Hypothetical syllogism
static_assert((List(P|Q, ~P) |= Q));               // Disjunctive syllogism
static_assert((List(Q<=P, S<=R, P|R) |= Q|S));     // Constructive dilemma
static_assert((List(Q<=P, S<=R, ~Q|~S) |= ~P|~R)); // Destructive dilemma
static_assert((List(~Q, Q<=P) |= ~P));             // Modus tollens
static_assert((Q<=P |= (P&Q)<=P));                 // Absorption

static_assert((P|(Q|R) |= (P|Q)|R));         // Associative laws
static_assert(((P|Q)|R |= P|(Q|R)));         //
static_assert((P&(Q&R) |= (P&Q)&R));         //
static_assert(((P&Q)&R |= P&(Q&R)));         //
static_assert((P|Q |= Q|P));                 // Commutative laws
static_assert((P&Q |= Q&P));                 //
static_assert((P&(Q|R)     |= (P&Q)|(P&R))); // Distributive laws
static_assert(((P&Q)|(P&R) |= P&(Q|R)));     //
static_assert((P|(Q&R)     |= (P|Q)&(P|R))); //
static_assert(((P|Q)&(P|R) |= P|(Q&R)));     //
static_assert((P   |= ~~P));                 // Double negation
static_assert((~~P |= P));                   //
static_assert((~(P&Q) |= ~P|~Q));            // DeMorgan's laws
static_assert((~P|~Q  |= ~(P&Q)));           //
static_assert((~(P|Q) |= ~P&~Q));            //
static_assert((~P&~Q  |= ~(P|Q)));           //
static_assert((Q<=P |= ~P<=~Q));             // Transposition
static_assert((Q<=P |= ~P|Q));               // Material implication
static_assert((~P|Q |= Q<=P));               //
static_assert((R<=(P&Q)  |= (R<=Q)<=P));     // Exportation law
static_assert(((R<=Q)<=P |= R<=(P&Q)));      //
static_assert((P|P |= P));                   // Idempotent laws
static_assert((P&P |= P));                   //

static_assert((!(List(Q<=P, Q) |= P)));   // Affirming the consequent fallacy
static_assert((!(List(Q<=P, ~P) |= ~Q))); // Denying the antecedent fallacy
static_assert((!(List(P|Q, P) |= ~Q)));   // Affirming a disjunct fallacy
static_assert((!(List(~(P&Q), Q) |= P))); // Denying a conjunct fallacy
static_assert((!(Q<=P |= P<=Q)));         // Illicit commutativity fallacy

// All the tests are compile-time :P
int main() {return 0;}
