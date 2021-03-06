#include <iostream>
#include <chrono>

#include "basic.h"
#include "add.h"
#include "symbol.h"
#include "dict.h"
#include "integer.h"
#include "mul.h"
#include "pow.h"
#include "functions.h"
#include "complex.h"
#include "constants.h"

using CSymPy::Basic;
using CSymPy::Add;
using CSymPy::Mul;
using CSymPy::Pow;
using CSymPy::Log;
using CSymPy::Symbol;
using CSymPy::umap_basic_num;
using CSymPy::map_vec_int;
using CSymPy::Integer;
using CSymPy::integer;
using CSymPy::multinomial_coefficients;
using CSymPy::one;
using CSymPy::zero;
using CSymPy::sin;
using CSymPy::RCP;
using CSymPy::rcp;
using CSymPy::sqrt;
using CSymPy::pow;
using CSymPy::add;
using CSymPy::mul;
using CSymPy::div;
using CSymPy::sub;
using CSymPy::exp;
using CSymPy::E;
using CSymPy::Rational;
using CSymPy::Complex;
using CSymPy::Number;
using CSymPy::I;
using CSymPy::rcp_dynamic_cast;
using CSymPy::print_stack_on_segfault;

void test_add()
{
    RCP<const Basic> x = rcp(new Symbol("x"));
    RCP<const Basic> y = rcp(new Symbol("y"));
    RCP<const Basic> z = rcp(new Symbol("z"));
    RCP<const Basic> i2 = rcp(new Integer(2));
    RCP<const Basic> i3 = rcp(new Integer(3));
    RCP<const Basic> i4 = rcp(new Integer(4));

    RCP<const Basic> r1 = add(x, x);
    RCP<const Basic> r2 = mul(i2, x);
    RCP<const Basic> r3 = mul(i3, x);
    assert(eq(r1, r2));
    assert(neq(r1, r3));

    r3 = mul(i2, y);
    assert(neq(r1, r3));
    assert(neq(r2, r3));

    r1 = add(mul(y, x), mul(mul(i2, x), y));
    r2 = mul(mul(i3, x), y);
    assert(eq(r1, r2));

    r1 = add(add(x, x), x);
    r2 = mul(i3, x);
    assert(eq(r1, r2));

    r1 = add(add(x, x), x);
    r2 = mul(x, i3);
    assert(eq(r1, r2));

    r1 = add(x, one);
    r2 = add(one, x);
    assert(eq(r1, r2));

    r1 = add(pow(x, y), z);
    r2 = add(z, pow(x, y));
    assert(eq(r1, r2));

    r1 = add(x, I);
    r2 = add(I, x);
    assert(eq(r1, r2));

    r1 = mul(x, I);
    r2 = mul(mul(I, i2), x);
    r3 = mul(mul(I, i3), x);
    r2 = add(r1, r2);
    assert(eq(r3, r2));


}

void test_mul()
{
    RCP<const Basic> x = rcp(new Symbol("x"));
    RCP<const Basic> y = rcp(new Symbol("y"));
    RCP<const Basic> z = rcp(new Symbol("z"));
    RCP<const Basic> im2 = rcp(new Integer(-2));
    RCP<const Basic> i2 = rcp(new Integer(2));
    RCP<const Basic> i3 = rcp(new Integer(3));
    RCP<const Basic> i4 = rcp(new Integer(4));
    RCP<const Basic> i6 = rcp(new Integer(6));

    RCP<const Basic> r1, r2, mhalf;
    r1 = mul(pow(x, y), z);
    r2 = mul(z, pow(x, y));
    assert(eq(r1, r2));

    r1 = mul(mul(x, y), mul(y, x));
    r2 = mul(pow(x, i2), pow(y, i2));
    assert(eq(r1, r2));

    r1 = mul(pow(x, add(y, z)), z);
    r2 = mul(z, pow(x, add(z, y)));
    assert(eq(r1, r2));

    r1 = mul(pow(x, y), pow(x, z));
    r2 = pow(x, add(y, z));
    assert(eq(r1, r2));

    r1 = mul(mul(pow(x, y), pow(x, z)), pow(x, x));
    r2 = pow(x, add(add(x, y), z));
    assert(eq(r1, r2));

    r1 = mul(mul(mul(pow(x, y), pow(x, z)), pow(x, x)), y);
    r2 = mul(pow(x, add(add(x, y), z)), y);
    assert(eq(r1, r2));

    r1 = mul(mul(i2, pow(y, mul(im2, pow(x, i2)))),
             mul(i3, pow(y, mul(i2, pow(x, i2)))));
    r2 = i6;
    assert(eq(r1, r2));

    r1 = mul(mul(mul(mul(div(i3, i2), pow(cos(pow(x, i2)), im2)), x),
           sin(pow(x, i2))), cos(div(mul(i3, x), i4)));
    r2 = mul(mul(mul(mul(div(i3, i2), pow(cos(pow(x, i2)), im2)), x),
           sin(pow(x, i2))), cos(div(mul(i3, x), i4)));
    std::cout << *r1 << std::endl;
    std::cout << *r2 << std::endl;
    assert(eq(r1, r2));

    mhalf = div(integer(-1), i2);
    r1 = mul(integer(12), pow(integer(196), mhalf));
    r2 = mul(integer(294), pow(integer(196), mhalf));
    assert(eq(integer(18), mul(r1, r2)));

    r1 = mul(mul(integer(12), pow(integer(196), mhalf)), pow(i3, mhalf));
    r2 = mul(mul(integer(294), pow(integer(196), mhalf)), pow(i3, mhalf));
    assert(eq(i6, mul(r1, r2)));

    r1 = mul(add(x, mul(y, I)), sub(x, mul(y, I)));
    r2 = mul(sub(x, mul(y, I)), add(x, mul(y, I)));
    std::cout << *r1 << std::endl;
    std::cout << *r2 << std::endl;
    assert(eq(r1, r2));

    r1 = mul(mul(x, I), mul(y, I));
    r2 = mul(integer(-1), mul(x, y));
    assert(eq(r1, r2));

    RCP<const Number> rc1, rc2, c1, c2;
    rc1 = Rational::from_two_ints(integer(2), integer(1));
    rc2 = Rational::from_two_ints(integer(3), integer(1));
    c1 = Complex::from_two_nums(*rc1, *rc2);
    rc1 = Rational::from_two_ints(integer(-5), integer(1));
    rc2 = Rational::from_two_ints(integer(12), integer(1));
    c2 = Complex::from_two_nums(*rc1, *rc2);

    r1 = mul(x, c1);
    r2 = mul(x, c1);
    r1 = mul(r1, r2);
    r2 = mul(pow(x, i2), c2);
    assert(eq(r1, r2));
}

void test_sub()
{
    RCP<const Basic> x = rcp(new Symbol("x"));
    RCP<const Basic> y = rcp(new Symbol("y"));
    RCP<const Basic> z = rcp(new Symbol("z"));
    RCP<const Basic> im1 = rcp(new Integer(-1));
    RCP<const Basic> i2 = rcp(new Integer(2));
    RCP<const Basic> i3 = rcp(new Integer(3));
    RCP<const Basic> i4 = rcp(new Integer(4));

    RCP<const Basic> r1, r2;

    r1 = sub(i3, i2);
    r2 = one;
    assert(eq(r1, r2));

    r1 = sub(x, x);
    r2 = zero;
    assert(eq(r1, r2));

    r1 = sub(mul(i2, x), x);
    r2 = x;
    assert(eq(r1, r2));

    r1 = add(mul(mul(i2, x), y), mul(x, y));
    r2 = mul(i3, mul(x, y));
    assert(eq(r1, r2));

    r1 = add(mul(mul(i2, x), y), mul(x, y));
    r2 = mul(mul(x, y), i3);
    assert(eq(r1, r2));

    r1 = sub(mul(mul(i2, x), y), mul(x, y));
    r2 = mul(x, y);
    assert(eq(r1, r2));

    r1 = sub(add(x, one), x);
    r2 = one;
    assert(eq(r1, r2));

    r1 = add(add(x, one), add(x, i2));
    r2 = add(mul(i2, x), i3);
    assert(eq(r1, r2));

    r1 = sub(add(x, one), add(x, i2));
    r1 = expand(r1);
    r2 = im1;
    assert(eq(r1, r2));

    RCP<const Number> rc1, rc2, rc3, c1, c2;
    rc1 = Rational::from_two_ints(integer(1), integer(2));
    rc2 = Rational::from_two_ints(integer(3), integer(4));
    rc3 = Rational::from_two_ints(integer(-5), integer(6));

    c1 = Complex::from_two_nums(*rc1, *rc2);
    c2 = Complex::from_two_nums(*rc1, *rc3);

    r1 = mul(x, c1);
    r2 = mul(x, c2);
    r1 = sub(r1, r2);
    r2 = mul(div(mul(I, integer(19)), integer(12)), x);
    assert(eq(r1, r2));
}

void test_div()
{
    RCP<const Basic> x = rcp(new Symbol("x"));
    RCP<const Basic> y = rcp(new Symbol("y"));
    RCP<const Basic> z = rcp(new Symbol("z"));
    RCP<const Basic> im1 = rcp(new Integer(-1));
    RCP<const Basic> i2 = rcp(new Integer(2));
    RCP<const Basic> i3 = rcp(new Integer(3));
    RCP<const Basic> i4 = rcp(new Integer(4));

    assert(integer(2)->is_positive());
    assert(integer(0)->is_zero());
    assert(integer(1)->is_one());
    assert(!(integer(-1)->is_positive()));
    assert(integer(-1)->is_negative());

    RCP<const Basic> r1, r2;

    r1 = div(i4, integer(1));
    r2 = mul(integer(1), i4);
    std::cout << "r1: " << *r1 << std::endl;
    std::cout << "r2: " << *r2 << std::endl;
    assert(eq(r1, r2));

    r1 = pow(i3, i2);
    r2 = integer(9);
    assert(eq(r1, r2));

    r1 = div(i4, i2);
    r2 = i2;
    assert(eq(r1, r2));

    r1 = div(x, x);
    r2 = one;
    assert(eq(r1, r2));

    r1 = div(mul(i2, x), x);
    r2 = i2;
    assert(eq(r1, r2));

    r1 = div(pow(x, i2), x);
    r2 = x;
    assert(eq(r1, r2));

    r1 = div(mul(mul(i2, x), y), mul(x, y));
    r2 = i2;
    std::cout << "r1: " << *r1 << std::endl;
    std::cout << "r2: " << *r2 << std::endl;
    assert(eq(r1, r2));

    r1 = div(mul(mul(y, x), i2), mul(x, y));
    r2 = i2;
    assert(eq(r1, r2));

    r1 = div(mul(x, i2), x);
    r2 = i2;
    assert(eq(r1, r2));

    r1 = div(mul(x, i4), mul(x, i2));
    r2 = i2;
    assert(eq(r1, r2));

    r1 = div(i2, div(i3, mul(i2, im1)));
    r2 = mul(im1, div(i4, i3));
    assert(eq(r1, r2));

    r1 = div(i4, mul(im1, i2));
    r2 = mul(im1, i2);
    assert(eq(r1, r2));

    r1 = div(i4, im1);
    r2 = mul(im1, i4);
    assert(eq(r1, r2));

    r1 = div(integer(5), div(integer(1), integer(3)));
    assert(eq(r1, integer(15)));

    RCP<const Number> rc1, rc2, rc3, c1, c2;
    rc1 = Rational::from_two_ints(integer(1), integer(2));
    rc2 = Rational::from_two_ints(integer(3), integer(4));
    rc3 = Rational::from_two_ints(integer(12), integer(13));

    c1 = Complex::from_two_nums(*rc1, *rc2);
    c2 = Complex::from_two_nums(*rc1, *rc1);

    r1 = div(x, c1);
    r2 = mul(sub(div(integer(8), integer(13)), mul(I, rc3)), x);
    assert(eq(r1, r2));

    r1 = mul(c2, div(x, c1));
    rc3 = Rational::from_two_ints(integer(2), integer(13));
    r2 = mul(sub(div(integer(10), integer(13)), mul(I, rc3)), x);
    assert(eq(r1, r2));

}

void test_pow()
{
    RCP<const Symbol> x = rcp(new Symbol("x"));
    RCP<const Basic> y = rcp(new Symbol("y"));
    RCP<const Basic> z = rcp(new Symbol("z"));
    RCP<const Basic> im1 = rcp(new Integer(-1));
    RCP<const Basic> i2 = rcp(new Integer(2));
    RCP<const Basic> i3 = rcp(new Integer(3));
    RCP<const Basic> im3 = rcp(new Integer(-3));
    RCP<const Basic> i4 = rcp(new Integer(4));
    RCP<const Basic> i6 = rcp(new Integer(6));
    RCP<const Basic> i9 = rcp(new Integer(9));
    RCP<const Basic> i27 = rcp(new Integer(27));

    RCP<const Basic> r1;
    RCP<const Basic> r2;

    r1 = mul(x, x);
    r2 = pow(x, i2);
    assert(eq(r1, r2));

    r1 = mul(mul(x, x), x);
    r2 = pow(x, i3);
    assert(eq(r1, r2));

    r1 = mul(mul(mul(x, x), x), x);
    r2 = pow(x, i4);
    assert(eq(r1, r2));

    r1 = mul(mul(add(x, y), add(x, y)), add(x, y));
    r2 = pow(add(x, y), i3);
    assert(eq(r1, r2));

    r1 = mul(mul(add(x, y), add(y, x)), add(x, y));
    r2 = pow(add(x, y), i3);
    assert(eq(r1, r2));

    r1 = sub(pow(x, y), pow(x, y));
    r2 = zero;
    assert(eq(r1, r2));

    /* Test (x*y)^2 -> x^2*y^2 type of simplifications */

    r1 = pow(mul(x, y), i2);
    r2 = mul(pow(x, i2), pow(y, i2));
    assert(eq(r1, r2));

    r1 = pow(mul(i2, mul(x, y)), i2);
    r2 = mul(i4, mul(pow(x, i2), pow(y, i2)));
    assert(eq(r1, r2));

    r1 = pow(mul(i3, mul(x, y)), i2);
    r2 = mul(i9, mul(pow(x, i2), pow(y, i2)));
    assert(eq(r1, r2));

    r1 = pow(mul(i3, mul(x, y)), im1);
    r2 = mul(div(one, i3), mul(pow(x, im1), pow(y, im1)));
    assert(eq(r1, r2));

    r1 = pow(mul(i3, mul(pow(x, i2), pow(y, i3))), i2);
    r2 = mul(i9, mul(pow(x, i4), pow(y, i6)));
    assert(eq(r1, r2));

    r1 = pow(mul(i3, mul(pow(x, i2), pow(y, im1))), i3);
    r2 = mul(i27, mul(pow(x, i6), pow(y, im3)));
    assert(eq(r1, r2));

    /*    */
    r1 = sqrt(x);
    r1 = r1->diff(x)->diff(x);
    r2 = mul(div(im1, i4), pow(x, div(im3, i2)));
    assert(eq(r1, r2));

    // Just test that it works:
    r2 = sin(r1)->diff(x)->diff(x);

    r1 = div(one, sqrt(i2));
    r2 = mul(pow(i2, pow(i2, im1)), pow(i2, im1));
    assert(eq(r1, r2));

    r1 = div(one, sqrt(i2));
    r2 = div(sqrt(i2), i2);
    assert(eq(r1, r2));

    r1 = exp(pow(x, i3));
    r1 = r1->diff(x);
    r2 = mul(mul(i3, exp(pow(x, i3))), pow(x, i2));
    assert(eq(r1, r2));

    r1 = pow(x, x);
    r1 = r1->diff(x);
    r2 = mul(pow(x, x), add(log(x), one));
    assert(eq(r1, r2));

    r1 = pow(x, y);
    r1 = r1->diff(x);
    r2 = mul(pow(x, sub(y, one)), y);
    assert(eq(r1, r2));

    r1 = pow(y, x);
    r1 = r1->diff(x);
    r2 = mul(pow(y, x), log(y));
    assert(eq(r1, r2));

    r1 = pow(div(i4, i6), i2);
    assert(eq(r1, div(integer(4), integer(9))));

    r1 = pow(i2, div(im1, i2));
    r2 = div(sqrt(i2), i2);
    assert(eq(r1, r2));

    r1 = pow(div(i3, i2), div(integer(7), i2));
    r2 = mul(div(integer(27), integer(16)), mul(pow(i2, div(integer(1), i2)),
        pow(i3, div(integer(1), i2))));
    assert(eq(r1, r2));

    r1 = pow(div(i2, i3), div(integer(7), i2));
    r2 = mul(div(integer(8), integer(81)), mul(pow(i2, div(integer(1), i2)),
        pow(i3, div(integer(1), i2))));
    assert(eq(r1, r2));

    r1 = pow(i6, div(integer(7), i2));
    r2 = mul(integer(216), pow(i6, div(integer(1), i2)));
    assert(eq(r1, r2));

    r1 = pow(div(i3, i2), div(integer(-7), i2));
    r2 = mul(div(integer(8), integer(81)), mul(pow(i2, div(integer(1), i2)),
        pow(i3, div(integer(1), i2))));
    assert(eq(r1, r2));

    r1 = pow(i6, div(integer(-7), i2));
    r2 = mul(div(one, integer(1296)), pow(i6, div(integer(1), i2)));
    assert(eq(r1, r2));

    r1 = mul(pow(i3, div(i27, i4)), pow(i2, div(integer(-13), i6)));
    r2 = mul(mul(div(integer(729), integer(8)), pow(i3, div(i3, i4))),
        pow(i2, div(integer(5), i6)));
    assert(eq(r1, r2));

    r1 = div(integer(12), pow(integer(196), div(integer(1), integer(2))));
    r2 = mul(div(i3, integer(49)), sqrt(integer(196)));
    assert(eq(r1, r2));

    r1 = pow(div(sqrt(integer(12)), sqrt(integer(6))), integer(2));
    r2 = integer(2);
    assert(eq(r1, r2));

    r1 = pow(pow(x, y), z);
    r2 = pow(x, mul(y, z));
    assert(neq(r1, r2));

    r1 = pow(mul(x, y), z);
    r2 = mul(pow(x, z), pow(y, z));
    assert(neq(r1, r2));

    RCP<const Number> rc1, rc2, rc3, c1, c2;
    rc1 = Rational::from_two_ints(integer(1), integer(2));
    rc2 = Rational::from_two_ints(integer(3), integer(4));
    rc3 = Rational::from_two_ints(integer(12), integer(13));

    c1 = Complex::from_two_nums(*rc1, *rc2);
    c2 = Complex::from_two_nums(*rc1, *rc1);

    r1 = pow(x, c1);
    r2 = mul(pow(x, div(one, i2)), pow(x, mul(I, div(i3, i4))));
    assert(eq(r1, r2));

    r1 = pow(c1, x);
    r2 = pow(c1, y);
    r1 = mul(r1, r2);
    r2 = pow(c1, add(x, y));
    assert(eq(r1, r2));

    r1 = pow(I, integer(3));
    r2 = mul(im1, I);
    assert(eq(r1, r2));

    r1 = pow(mul(I, i2), i2);
    r2 = mul(im1, i4);
    assert(eq(r1, r2));

    r1 = pow(mul(I, im3), integer(5));
    r2 = mul(integer(243), mul(I, im1));
    assert(eq(r1, r2));

    r1 = pow(mul(I, im3), integer(4));
    r2 = integer(81);
    assert(eq(r1, r2));

    r1 = pow(im1, div(one, i2));
    r2 = I;
    assert(eq(r1, r2));

    r1 = pow(im1, div(i6, i4));
    r2 = mul(im1, I);
    assert(eq(r1, r2));

    r1 = pow(im1, div(integer(9), i6));
    r2 = mul(im1, I);
    assert(eq(r1, r2));

    r1 = pow(im3, div(integer(9), i6));
    r2 = mul(mul(im3, I), pow(i3, div(one, i2)));
    assert(eq(r1, r2));

    r1 = pow(im3, div(i4, i3));
    r2 = pow(r1, i3);
    assert(eq(r2, integer(81)));
}

 void test_log()
 {
    RCP<const Basic> i2 = rcp(new Integer(2));
    RCP<const Basic> i3 = rcp(new Integer(3));

    RCP<const Basic> r1;
    RCP<const Basic> r2;

    r1 = log(E);
    r2 = one;
    assert(eq(r1, r2));

    r1 = log(one);
    r2 = zero;
    assert(eq(r1, r2));

    r1 = log(div(i2, i3));
    r2 = sub(log(i2), log(i3));
    assert(eq(r1, r2));

    r1 = log(E, i2);
    r2 = div(one, log(i2));
    assert(eq(r1, r2));
 }

void test_multinomial()
{
    map_vec_int r;
    auto t1 = std::chrono::high_resolution_clock::now();
    multinomial_coefficients(4, 20, r);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
        << "ms" << std::endl;
}

void test_expand1()
{
    RCP<const Basic> x = rcp(new Symbol("x"));
    RCP<const Basic> y = rcp(new Symbol("y"));
    RCP<const Basic> z = rcp(new Symbol("z"));
    RCP<const Basic> w = rcp(new Symbol("w"));
    RCP<const Basic> i2 = rcp(new Integer(2));
    RCP<const Basic> i3 = rcp(new Integer(3));
    RCP<const Basic> i4 = rcp(new Integer(10));

    RCP<const Basic> r1;
    RCP<const Basic> r2;

    r1 = pow(add(add(add(x, y), z), w), i4);

    std::cout << *r1 << std::endl;

    auto t1 = std::chrono::high_resolution_clock::now();
    r2 = expand(r1);
    auto t2 = std::chrono::high_resolution_clock::now();
    //std::cout << *r2 << std::endl;
    std::cout
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
        << "ms" << std::endl;
    std::cout << "number of terms: "
        << rcp_dynamic_cast<const Add>(r2)->dict_.size() << std::endl;
}

void test_expand2()
{
    RCP<const Basic> x = rcp(new Symbol("x"));
    RCP<const Basic> y = rcp(new Symbol("y"));
    RCP<const Basic> z = rcp(new Symbol("z"));
    RCP<const Basic> w = rcp(new Symbol("w"));
    RCP<const Basic> im1 = rcp(new Integer(-1));
    RCP<const Basic> im2 = rcp(new Integer(-2));
    RCP<const Basic> i2 = rcp(new Integer(2));
    RCP<const Basic> i3 = rcp(new Integer(3));
    RCP<const Basic> i4 = rcp(new Integer(4));
    RCP<const Basic> i5 = rcp(new Integer(5));
    RCP<const Basic> i6 = rcp(new Integer(6));
    RCP<const Basic> i9 = rcp(new Integer(9));
    RCP<const Basic> i10 = rcp(new Integer(10));
    RCP<const Basic> i12 = rcp(new Integer(12));
    RCP<const Basic> i16 = rcp(new Integer(16));
    RCP<const Basic> i24 = rcp(new Integer(24));
    RCP<const Basic> i25 = rcp(new Integer(25));
    RCP<const Basic> i30 = rcp(new Integer(30));

    RCP<const Basic> r1;
    RCP<const Basic> r2;

    r1 = mul(w, add(add(x, y), z)); // w*(x+y+z)
    std::cout << *r1 << std::endl;

    r2 = expand(r1);
    std::cout << *r2 << std::endl;

    assert(eq(r2, add(add(mul(w, x), mul(w, y)), mul(w, z))));
    assert(neq(r2, add(add(mul(w, x), mul(w, w)), mul(w, z))));

    r1 = mul(add(x, y), add(z, w)); // (x+y)*(z+w)
    std::cout << *r1 << std::endl;

    r2 = expand(r1);
    std::cout << *r2 << std::endl;

    assert(eq(r2, add(add(add(mul(x, z), mul(y, z)), mul(x, w)), mul(y, w))));
    assert(neq(r2, add(add(add(mul(y, z), mul(y, z)), mul(x, w)), mul(y, w))));

    r1 = pow(add(x, y), im1);       // 1/(x+y)
    std::cout << *r1 << std::endl;

    r2 = expand(r1);
    std::cout << *r2 << std::endl;

    assert(eq(r2, r1));

    r1 = pow(add(x, y), im2);       // 1/(x+y)^2
    std::cout << *r1 << std::endl;

    r2 = expand(r1);
    std::cout << *r2 <<std::endl;

    assert(eq(r2, pow(add(add(pow(x, i2), mul(mul(i2, x), y)), pow(y, i2)), im1)));
    assert(neq(r2, r1));

    r1 = mul(im1, add(x, i2));
    r1 = expand(r1);
    r2 = add(mul(im1, x), im2);
    assert(eq(r1, r2));

    r1 = pow(add(x, one), i2);
    r1 = expand(r1);
    r2 = add(add(pow(x, i2), mul(i2, x)), one);
    assert(eq(r1, r2));

    r1 = pow(add(x, i2), i2);
    r1 = expand(r1);
    r2 = add(add(pow(x, i2), mul(i4, x)), i4);
    assert(eq(r1, r2));

    r1 = pow(add(x, i3), i2);
    r1 = expand(r1);
    r2 = add(add(pow(x, i2), mul(i6, x)), i9);
    assert(eq(r1, r2));

    r1 = pow(add(mul(i3, x), i5), i2);
    r1 = expand(r1);
    r2 = add(add(mul(i9, pow(x, i2)), mul(i30, x)), i25);
    assert(eq(r1, r2));

    r1 = pow(add(mul(i2, pow(x, i2)), mul(i3, y)), i2);
    r1 = expand(r1);
    r2 = add(add(mul(i4, pow(x, i4)), mul(i12, mul(pow(x, i2), y))),
            mul(i9, pow(y, i2)));
    assert(eq(r1, r2));

    r1 = pow(add(add(pow(x, i3), pow(x, i2)), x), i2);
    r1 = expand(r1);
    r2 = add(add(add(add(pow(x, i6), mul(i2, pow(x, i5))),
                mul(i3, pow(x, i4))), mul(i2, pow(x, i3))), pow(x, i2));
    std::cout << *r1 << std::endl;
    std::cout << *r2 << std::endl;
    assert(eq(r1, r2));

    r1 = pow(add(x, pow(x, i5)), i2);
    r1 = expand(r1);
    r2 = add(add(pow(x, i10), mul(i2, pow(x, i6))), pow(x, i2));
    std::cout << *r1 << std::endl;
    std::cout << *r2 << std::endl;
    assert(eq(r1, r2));

    r1 = mul(add(i2, x), add(i3, y));
    r1 = expand(r1);
    r2 = add(add(add(i6, mul(i2, y)), mul(i3, x)), mul(x, y));
    std::cout << *r1 << std::endl;
    std::cout << *r2 << std::endl;
    assert(eq(r1, r2));

    r1 = mul(i3, pow(i5, div(im1, i2)));
    r2 = mul(i4, pow(i5, div(im1, i2)));
    r2 = expand(pow(add(add(r1, r2), integer(1)), i2));
    assert(eq(r2, add(div(integer(54), i5), mul(integer(14), pow(i5, div(im1, i2))))));

    r1 = pow(add(mul(I, x), i2), i2);
    r1 = expand(r1);
    r2 = add(sub(mul(mul(I, x), i4), pow(x, i2)), i4);
    assert(eq(r1, r2));

    r1 = mul(add(sqrt(i3), one), add(sqrt(i3), i2));
    r1 = expand(r1);
    r2 = add(i5, mul(i3, sqrt(i3)));
    assert(eq(r1, r2));

    r1 = mul(add(mul(i2, x), sqrt(i2)), add(x, mul(i2, sqrt(i2))));
    r1 = expand(r1);
    r2 = add(mul(i2, mul(x, x)), add(mul(x, mul(sqrt(i2), i5)), i4));
    assert(eq(r1, r2));

    r1 = mul(sqrt(i2), add(mul(x, i2), mul(i2, sqrt(i2))));
    r1 = expand(r1);
    r2 = add(mul(i2, mul(sqrt(i2), x)), i4);
    assert(eq(r1, r2));

    r1 = pow(add(pow(add(one, sqrt(i2)), i2), one), i2);
    r1 = expand(r1);
    r2 = add(i24, mul(i16, sqrt(i2)));
    assert(eq(r1, r2));

    r1 = mul(add(mul(sqrt(i3), x), one), sub(mul(sqrt(i3), x), one));
    r1 = expand(r1);
    r2 = sub(mul(mul(i3, x), x), one);
    assert(eq(r1, r2));

    r1 = pow(add(i2, mul(y, x)), i2);
    r1 = expand(r1);
    r2 = add(i4, add(mul(mul(i4, x), y), pow(mul(x, y), i2)));
    assert(eq(r1, r2));

    // The following test that the expand method outputs canonical objects
    r1 = pow(add(y, mul(sqrt(i3), z)), i2);
    r1 = expand(mul(r1, add(r1, one)));
    std::cout << r1->__str__() << std::endl;

    r1 = pow(add(y, mul(sqrt(i3), z)), i3);
    r1 = expand(mul(r1, add(r1, one)));
    std::cout << r1->__str__() << std::endl;

    r1 = pow(mul(sqrt(i3), mul(y, add(one, pow(i3, div(one, i3))))), i3);
    r1 = expand(mul(r1, add(r1, one)));
    std::cout << r1->__str__() << std::endl;
}

void test_expand3()
{
    RCP<const Basic> x = rcp(new Symbol("x"));
    RCP<const Basic> y = rcp(new Symbol("y"));
    RCP<const Basic> z = rcp(new Symbol("z"));
    RCP<const Basic> w = rcp(new Symbol("w"));
    RCP<const Basic> i2 = rcp(new Integer(2));

    RCP<const Basic> e, f, r;

    e = pow(add(add(add(x, y), z), w), i2);
    f = mul(e, add(e, w));

    std::cout << *f << std::endl;

    auto t1 = std::chrono::high_resolution_clock::now();
    r = expand(f);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << *r << std::endl;
    std::cout
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
        << "ms" << std::endl;
    std::cout << "number of terms: "
        << rcp_dynamic_cast<const Add>(r)->dict_.size() << std::endl;

    RCP<const Number> rc1, rc2, c1;
    rc1 = Rational::from_two_ints(integer(2), integer(1));
    rc2 = Rational::from_two_ints(integer(3), integer(1));

    c1 = Complex::from_two_nums(*rc1, *rc2);
    e = pow(add(x, c1), integer(40));

    t1 = std::chrono::high_resolution_clock::now();
    r = expand(e);
    t2 = std::chrono::high_resolution_clock::now();

    std::cout << *r << std::endl;
    std::cout
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
        << "ms" << std::endl;
    std::cout << "number of terms: "
        << rcp_dynamic_cast<const Add>(r)->dict_.size() << std::endl;

    e = pow(c1, integer(-40));

    t1 = std::chrono::high_resolution_clock::now();
    r = expand(e);
    t2 = std::chrono::high_resolution_clock::now();

    std::cout << *r << std::endl;
    std::cout
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
        << "ms" << std::endl;
}

int main(int argc, char* argv[])
{
    print_stack_on_segfault();

    test_add();
    test_mul();
    test_pow();
    test_log();
    test_sub();
    test_div();
    test_multinomial();
    test_expand1();
    test_expand2();
    test_expand3();

    return 0;
}
