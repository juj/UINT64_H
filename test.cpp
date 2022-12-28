#include "../UINT64.H"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t rnd64()
{
	if (rand() % 10 == 0) return rand()%16;
	return (uint64_t)rand() ^ ((uint64_t)rand() << 15) ^ ((uint64_t)rand() << 30) ^ ((uint64_t)rand() << 45) ^ ((uint64_t)rand() << 60);
}

uint32_t rnd32()
{
	return (uint32_t)rand() ^ ((uint32_t)rand() << 15) ^ ((uint32_t)rand() << 30);
}

uint64 to64(uint64_t val)
{
	return uint64(val>>32, val&0xFFFFFFFFu);
}

uint64_t toull(uint64 val)
{
	return uint64_t(((uint64_t)val.hi << 32) | val.lo);
}

void test_assignment_from_negative_int()
{
	uint64_t x = 0;
	int y = -5;
	x = y;

	uint64 X = 0;
	X = y;
	printf("test_assignment_from_negative_int: %s\n", X.to_str());

	assert(toull(X) == x);
}

void test_cmp_greater_than_negative_int()
{
	uint64_t x = 0;
	int y = -1;
	int res = (int)(x > y);

	uint64 X = 0;
	int RES = (int)(X > y);
	printf("test_cmp_greater_than_int: %d\n", RES);

	assert(RES == res);
}

typedef Bool (*test_U_func)(uint64 a);
Bool test_U_neg(uint64 a) { return toull(-a) == (uint64_t)-(int64_t)toull(a); }
Bool test_U_inv(uint64 a) { return toull(~a) == ~toull(a); }
Bool test_U_not(uint64 a) { return toull(!a) == !toull(a); }
Bool test_U_from_str(uint64 a)
{
	char str[256];
	sprintf(str, "%llu", toull(a));
	uint64 s = uint64::from_str(str);
	return toull(s) == toull(a);
}

Bool test_U_to_str(uint64 a)
{
	char str[256];
	sprintf(str, "%llu", toull(a));
	return !strcmp(a.to_str(), str);
}

Bool test_U_inc(uint64 a) { uint64_t b = toull(a); uint64 C = a++; uint64_t c = b++; return toull(a) == b && toull(C) == c; }
Bool test_U_dec(uint64 a) { uint64_t b = toull(a); uint64 C = a--; uint64_t c = b--; return toull(a) == b && toull(C) == c; }
Bool test_U_preinc(uint64 a) { uint64_t b = toull(a); uint64 C = ++a; uint64_t c = ++b; return toull(a) == b && toull(C) == c; }
Bool test_U_predec(uint64 a) { uint64_t b = toull(a); uint64 C = --a; uint64_t c = --b; return toull(a) == b && toull(C) == c; }

void test_U_on_random_inputs(test_U_func func, const char *name)
{
	printf("Testing %s\n", name);
	for (int i = 0; i < 1000000; ++i)
	{
		uint64 a = to64(rnd64());
		if (!func(a))
			printf("Failed on inputs: %llu\n", toull(a));
	}
}
typedef Bool (*test_UU_func)(uint64 a, uint64 b);
Bool test_UU_add(uint64 a, uint64 b) { return toull(a + b) == toull(a) + toull(b); }
Bool test_UU_sub(uint64 a, uint64 b) { return toull(a - b) == toull(a) - toull(b); }
Bool test_UU_mul(uint64 a, uint64 b) { return toull(a * b) == toull(a) * toull(b); }
Bool test_UU_div(uint64 a, uint64 b) { return b == 0 ? 1 : toull(a / b) == toull(a) / toull(b); }
Bool test_UU_rem(uint64 a, uint64 b) { return b == 0 ? 1 : toull(a % b) == toull(a) % toull(b); }
Bool test_UU_and(uint64 a, uint64 b) { return toull(a & b) == (toull(a) & toull(b)); }
Bool test_UU_or (uint64 a, uint64 b) { return toull(a | b) == (toull(a) | toull(b)); }
Bool test_UU_xor(uint64 a, uint64 b) { return toull(a ^ b) == (toull(a) ^ toull(b)); }
Bool test_UU_cmp_gt (uint64 a, uint64 b) { return (a >  b) == (Bool)(toull(a) >  toull(b)); }
Bool test_UU_cmp_gte(uint64 a, uint64 b) { return (a >= b) == (Bool)(toull(a) >= toull(b)); }
Bool test_UU_cmp_lt (uint64 a, uint64 b) { return (a <  b) == (Bool)(toull(a) <  toull(b)); }
Bool test_UU_cmp_lte(uint64 a, uint64 b) { return (a <= b) == (Bool)(toull(a) <= toull(b)); }
Bool test_UU_cmp_eq (uint64 a, uint64 b) { return (a == b) == (Bool)(toull(a) == toull(b)); }
Bool test_UU_cmp_neq(uint64 a, uint64 b) { return (a != b) == (Bool)(toull(a) != toull(b)); }
Bool test_UU_cmp_and(uint64 a, uint64 b) { return (a && b) == (Bool)(toull(a) && toull(b)); }
Bool test_UU_cmp_or (uint64 a, uint64 b) { return (a || b) == (Bool)(toull(a) || toull(b)); }

uint64 c;
Bool test_UU_add_assign(uint64 a, uint64 b) { c = a; c += b; return toull(c) == toull(a) + toull(b); }
Bool test_UU_sub_assign(uint64 a, uint64 b) { c = a; c -= b; return toull(c) == toull(a) - toull(b); }
Bool test_UU_mul_assign(uint64 a, uint64 b) { c = a; c *= b; return toull(c) == toull(a) * toull(b); }
Bool test_UU_div_assign(uint64 a, uint64 b) { if (b == 0) return 1; c = a; c /= b; return toull(c) == toull(a) / toull(b); }
Bool test_UU_rem_assign(uint64 a, uint64 b) { if (b == 0) return 1; c = a; c %= b; return toull(c) == toull(a) % toull(b); }
Bool test_UU_and_assign(uint64 a, uint64 b) { c = a; c &= b; return toull(c) == (toull(a) & toull(b)); }
Bool test_UU_or_assign (uint64 a, uint64 b) { c = a; c |= b; return toull(c) == (toull(a) | toull(b)); }
Bool test_UU_xor_assign(uint64 a, uint64 b) { c = a; c ^= b; return toull(c) == (toull(a) ^ toull(b)); }

typedef Bool (*test_Uu_func)(uint64 a, uint32 b);
Bool test_Uu_add(uint64 a, uint32 b) { return toull(a  + b) == (toull(a)  + toull(b)); }
Bool test_Uu_sub(uint64 a, uint32 b) { return toull(a  - b) == (toull(a)  - toull(b)); }
Bool test_Uu_mul(uint64 a, uint32 b) { return toull(a  * b) == (toull(a)  * toull(b)); }
Bool test_Uu_div(uint64 a, uint32 b) { return toull(a  / b) == (toull(a)  / toull(b)); }
Bool test_Uu_rem(uint64 a, uint32 b) { return toull(a  % b) == (toull(a)  % toull(b)); }
Bool test_Uu_and(uint64 a, uint32 b) { return toull(a  & b) == (toull(a)  & toull(b)); }
Bool test_Uu_or (uint64 a, uint32 b) { return toull(a  | b) == (toull(a)  | toull(b)); }
Bool test_Uu_xor(uint64 a, uint32 b) { return toull(a  ^ b) == (toull(a)  ^ toull(b)); }
//Bool test_Uu_shl(uint64 a, uint32 b) { return toull(a << b) == (toull(a) << toull(b)); }
//Bool test_Uu_shr(uint64 a, uint32 b) { return toull(a >> b) == (toull(a) >> toull(b)); }
Bool test_Uu_cmp_gt (uint64 a, uint32 b) { return (a >  b) == (Bool)(toull(a) >  toull(b)); }
Bool test_Uu_cmp_gte(uint64 a, uint32 b) { return (a >= b) == (Bool)(toull(a) >= toull(b)); }
Bool test_Uu_cmp_lt (uint64 a, uint32 b) { return (a <  b) == (Bool)(toull(a) <  toull(b)); }
Bool test_Uu_cmp_lte(uint64 a, uint32 b) { return (a <= b) == (Bool)(toull(a) <= toull(b)); }
Bool test_Uu_cmp_eq (uint64 a, uint32 b) { return (a == b) == (Bool)(toull(a) == toull(b)); }
Bool test_Uu_cmp_neq(uint64 a, uint32 b) { return (a != b) == (Bool)(toull(a) != toull(b)); }
Bool test_Uu_cmp_and(uint64 a, uint32 b) { return (a && b) == (Bool)(toull(a) && toull(b)); }
Bool test_Uu_cmp_or (uint64 a, uint32 b) { return (a || b) == (Bool)(toull(a) || toull(b)); }

Bool test_Uu_add_assign(uint64 a, uint32 b) { c = a; c += b; return toull(c) == toull(a) + toull(b); }
Bool test_Uu_sub_assign(uint64 a, uint32 b) { c = a; c -= b; return toull(c) == toull(a) - toull(b); }
Bool test_Uu_mul_assign(uint64 a, uint32 b) { c = a; c *= b; return toull(c) == toull(a) * toull(b); }
Bool test_Uu_div_assign(uint64 a, uint32 b) { c = a; c /= b; return toull(c) == toull(a) / toull(b); }
Bool test_Uu_rem_assign(uint64 a, uint32 b) { c = a; c %= b; return toull(c) == toull(a) % toull(b); }
Bool test_Uu_and_assign(uint64 a, uint32 b) { c = a; c &= b; return toull(c) == (toull(a) & toull(b)); }
Bool test_Uu_or_assign (uint64 a, uint32 b) { c = a; c |= b; return toull(c) == (toull(a) | toull(b)); }
Bool test_Uu_xor_assign(uint64 a, uint32 b) { c = a; c ^= b; return toull(c) == (toull(a) ^ toull(b)); }
//Bool test_Uu_shl_assign(uint64 a, uint32 b) { c = a; c <<= b; return toull(c) == (toull(a) << toull(b)); }
//Bool test_Uu_shr_assign(uint64 a, uint32 b) { c = a; c >>= b; return toull(c) == (toull(a) >> toull(b)); }

typedef Bool (*test_Us_func)(uint64 a, uint16 b);
Bool test_Us_mul(uint64 a, uint16 b) { return toull(a  * b) == (toull(a)  * toull(b)); }

void test_UU_on_random_inputs(test_UU_func func, const char *name)
{
	printf("Testing %s\n", name);
	for (int i = 0; i < 1000000; ++i)
	{
		uint64 a = to64(rnd64());
		uint64 b = to64(rnd64());
		if (!func(a, b))
			printf("Failed on inputs: %llu . %llu\n", toull(a), toull(b));
	}
}

void test_Uu_on_random_inputs(test_Uu_func func, const char *name)
{
	printf("Testing %s\n", name);
	for (int i = 0; i < 1000000; ++i)
	{
		uint64 a = to64(rnd64());
		uint32 b = rnd32();
		if (!func(a, b))
		{
			printf("Failed on inputs: %llu . %lu\n", toull(a), (unsigned long)b);
			int ret = func(a, b);
			printf("%d\n", ret);
		}
	}
}

void test_Us_on_random_inputs(test_Us_func func, const char *name)
{
	printf("Testing %s\n", name);
	for (int i = 0; i < 1000000; ++i)
	{
		uint64 a = to64(rnd64());
		uint16 b = (uint16)rnd32();
		if (!func(a, b))
		{
			printf("Failed on inputs: %llu . %lu\n", toull(a), (unsigned long)b);
			int ret = func(a, b);
			printf("%d\n", ret);
		}
	}
}

void test_shl()
{
	for(int i = -128; i < 128; ++i)
	{
		uint64_t one = 1ull;
		uint64 ONE(1);
		one = one << i;
		ONE = ONE << i;
		if (one != toull(ONE))
			printf("shift << %d: %llu vs %s\n", i, one, ONE.to_str());
	}
	for(int i = -128; i < 128; ++i)
	{
		uint64_t one = 1ull << 63;
		uint64 ONE(1);
		ONE = ONE << 63;
		one = one << i;
		ONE = ONE << i;
		if (one != toull(ONE))
			printf("shift << %d: %llu vs %s\n", i, one, ONE.to_str());
	}
}

void test_shr()
{
	for(int i = -128; i < 128; ++i)
	{
		uint64_t one = 1ull;
		uint64 ONE(1);
		one = one >> i;
		ONE = ONE >> i;
		if (one != toull(ONE))
			printf("shift >> %d: %llu vs %s\n", i, one, ONE.to_str());
	}
	for(int i = -128; i < 128; ++i)
	{
		uint64_t one = 1ull << 63;
		uint64 ONE(1);
		ONE = ONE << 63;
		one = one >> i;
		ONE = ONE >> i;
		if (one != toull(ONE))
			printf("shift >> %d: %llu vs %s\n", i, one, ONE.to_str());
	}
}

void test_to_str()
{
	uint64_t ull = 9000000000000000001ull;
	uint64 u = to64(ull);
	char *s = u.to_str();
	if (strcmp(s, "9000000000000000001"))
	{
		printf("9000000000000000001 = %s\n", s);
		assert(0);
	}
}

int main()
{
	test_shl();
	test_shr();

	test_to_str();

	test_assignment_from_negative_int();
	test_cmp_greater_than_negative_int();

	test_U_on_random_inputs(test_U_neg, "operator -");
	test_U_on_random_inputs(test_U_inv, "operator ~");
	test_U_on_random_inputs(test_U_not, "operator !");
	test_U_on_random_inputs(test_U_from_str, "uint64::from_str");
	test_U_on_random_inputs(test_U_to_str, "uint64::to_str");
	test_U_on_random_inputs(test_U_inc, "operator post ++");
	test_U_on_random_inputs(test_U_dec, "operator post --");
	test_U_on_random_inputs(test_U_preinc, "operator pre ++");
	test_U_on_random_inputs(test_U_predec, "operator pre --");

	test_UU_on_random_inputs(test_UU_add, "operator uint64 + uint64");
	test_UU_on_random_inputs(test_UU_sub, "operator uint64 - uint64");
	test_UU_on_random_inputs(test_UU_mul, "operator uint64 * uint64");
	test_UU_on_random_inputs(test_UU_div, "operator uint64 / uint64");
	test_UU_on_random_inputs(test_UU_rem, "operator uint64 % uint64");
	test_UU_on_random_inputs(test_UU_and, "operator uint64 & uint64");
	test_UU_on_random_inputs(test_UU_or , "operator uint64 | uint64");
	test_UU_on_random_inputs(test_UU_xor, "operator uint64 ^ uint64");
	test_UU_on_random_inputs(test_UU_cmp_gt,  "operator uint64 > uint64");
	test_UU_on_random_inputs(test_UU_cmp_gte, "operator uint64 >= uint64");
	test_UU_on_random_inputs(test_UU_cmp_lt,  "operator uint64 < uint64");
	test_UU_on_random_inputs(test_UU_cmp_lte, "operator uint64 <= uint64");
	test_UU_on_random_inputs(test_UU_cmp_eq,  "operator uint64 == uint64");
	test_UU_on_random_inputs(test_UU_cmp_neq, "operator uint64 != uint64");
	test_UU_on_random_inputs(test_UU_cmp_and, "operator uint64 && uint64");
	test_UU_on_random_inputs(test_UU_cmp_or,  "operator uint64 || uint64");
	test_UU_on_random_inputs(test_UU_add_assign, "operator uint64 += uint64");
	test_UU_on_random_inputs(test_UU_sub_assign, "operator uint64 -= uint64");
	test_UU_on_random_inputs(test_UU_mul_assign, "operator uint64 *= uint64");
	test_UU_on_random_inputs(test_UU_div_assign, "operator uint64 /= uint64");
	test_UU_on_random_inputs(test_UU_rem_assign, "operator uint64 %= uint64");
	test_UU_on_random_inputs(test_UU_and_assign, "operator uint64 &= uint64");
	test_UU_on_random_inputs(test_UU_or_assign , "operator uint64 |= uint64");
	test_UU_on_random_inputs(test_UU_xor_assign, "operator uint64 ^= uint64");

	test_Uu_on_random_inputs(test_Uu_add, "operator uint64 + uint32");
	test_Uu_on_random_inputs(test_Uu_sub, "operator uint64 - uint32");
	test_Uu_on_random_inputs(test_Uu_mul, "operator uint64 * uint32");
	test_Uu_on_random_inputs(test_Uu_div, "operator uint64 / uint32");
	test_Uu_on_random_inputs(test_Uu_rem, "operator uint64 % uint32");
	test_Uu_on_random_inputs(test_Uu_and, "operator uint64 & uint32");
	test_Uu_on_random_inputs(test_Uu_or , "operator uint64 | uint32");
	test_Uu_on_random_inputs(test_Uu_xor, "operator uint64 ^ uint32");
//	test_Uu_on_random_inputs(test_Uu_shl, "operator uint64 << uint32");
//	test_Uu_on_random_inputs(test_Uu_shr, "operator uint64 >> uint32");
	test_Uu_on_random_inputs(test_Uu_cmp_gt,  "operator uint64 > uint32");
	test_Uu_on_random_inputs(test_Uu_cmp_gte, "operator uint64 >= uint32");
	test_Uu_on_random_inputs(test_Uu_cmp_lt,  "operator uint64 < uint32");
	test_Uu_on_random_inputs(test_Uu_cmp_lte, "operator uint64 <= uint32");
	test_Uu_on_random_inputs(test_Uu_cmp_eq,  "operator uint64 == uint32");
	test_Uu_on_random_inputs(test_Uu_cmp_neq, "operator uint64 != uint32");
	test_Uu_on_random_inputs(test_Uu_cmp_and, "operator uint64 && uint32");
	test_Uu_on_random_inputs(test_Uu_cmp_or,  "operator uint64 || uint32");
	test_Uu_on_random_inputs(test_Uu_add_assign, "operator uint64 += uint32");
	test_Uu_on_random_inputs(test_Uu_sub_assign, "operator uint64 -= uint32");
	test_Uu_on_random_inputs(test_Uu_mul_assign, "operator uint64 *= uint32");
	test_Uu_on_random_inputs(test_Uu_div_assign, "operator uint64 /= uint32");
	test_Uu_on_random_inputs(test_Uu_rem_assign, "operator uint64 %= uint32");
	test_Uu_on_random_inputs(test_Uu_and_assign, "operator uint64 &= uint32");
	test_Uu_on_random_inputs(test_Uu_or_assign , "operator uint64 |= uint32");
	test_Uu_on_random_inputs(test_Uu_xor_assign, "operator uint64 ^= uint32");
//	test_Uu_on_random_inputs(test_Uu_shl_assign, "operator uint64 <<= uint32");
//	test_Uu_on_random_inputs(test_Uu_shr_assign, "operator uint64 >>= uint32");

	test_Us_on_random_inputs(test_Us_mul, "operator uint64 * uint16");
	return 0;
}
