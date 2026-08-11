#include <bits/stdc++.h>
using namespace std;

#define TEMPLATE_NUMBER template<typename number, typename = enable_if_t<is_arithmetic_v<number>>

template<typename T>
struct next_type;

template<> struct next_type<int8_t>   { using type = int16_t; };
template<> struct next_type<int16_t>  { using type = int32_t; };
template<> struct next_type<int32_t>  { using type = int64_t; };
template<> struct next_type<int64_t>  { using type = __int128; };

template<> struct next_type<uint8_t>  { using type = uint16_t; };
template<> struct next_type<uint16_t> { using type = uint32_t; };
template<> struct next_type<uint32_t> { using type = uint64_t; };
template<> struct next_type<uint64_t> { using type = unsigned __int128; }; // not supported by all compilers

/*template<> struct next_type<float> { using type = float};
template<> structure next_type<double> { using type = double};
*/

template<typename integer = uint8_t>
class BigInt {
    private:
    bool negative;
    vector<integer> numbers;
TEMPLATE_NUMBER
     vector<integer> convert_base10(number num) {
        next_type<number> base = (next_type<number>)1 << (sizeof(number) * CHAR_BIT)
        vector<integer> result;
        while (num > 0) {
            result.push_back(num % base);
            num /= base;
        }
        return result;
     }

     static vector<integer> add(const vector<integer> &first, const vector<integer> &b) {
        vector<integer> a = first;
        next_type<integer> base = (next_type<integer>)1 << (sizeof(integer) * CHAR_BIT)
        for (int i = 0; i < a.size() || i < b.size(); ++i) {
            if (i >= a.size()) {
                a.push_back(0);
            }
            if (i >= b.size()) {
                break;
            }
            next_type<integer> result = (next_type<integer>)a[i] + b[i];
            if (result >= base) {
                a[i] = result - base;
                if (i + 1 >= a.size()) {
                    a.push_back(1);
                } else {
                    a[i + 1]++;
                }
            } else {
                a[i] = result;
            }
        }
        return a;
        
        }
        //assume we may have zeroes and therefore we compare all digits (because we may have leading zeroes)
        //we also assume that the numbers are positive
        bool compare(const vector<integer> &first, const vector<integer> &b) {
            
            if (b.size() > first.size()) {
                for (int i = b.size() - 1; i >= first.size(); --i) {
                    if (b[i] != 0) {
                        return true;
                    }
                }
            }
            
        
            for (int i = first.size() - 1; i >= 0; --i) {
                if (first[i] != b[i]) {
                    return first[i] < b[i];
                }
            }
            return false;
        }
      static vector<integer> subtract(const vector<integer> &first, const vector<integer> &b) {
        vector<integer> a = first;
        next_type<integer> base = (next_type<integer>)1 << (sizeof(integer) * CHAR_BIT)
        for (int i = 0; i < a.size() || i < b.size(); ++i) {
            if (i >= a.size()) {
                a.push_back(0);
            }
            if (i >= b.size()) {
                break;
            }
            std::make_signed_t<next_type<integer>> result = (std::make_signed_t<next_type<integer>>)a[i] - (std::make_signed_t<next_type<integer>>)b[i];
            if (result < 0) {
                a[i] = result + base;
                bool carried = false;
                for (int i = 0; i < a.size(); ++i) {
                    if (a[i] > 0) {
                        a[i]--;
                        carried = true;
                        break;
                }
                if (!carried) {
                    throw invalid_argument("Cannot subtract a larger number from a smaller number");
                }
            } else {
                a[i] = result;
            }
        }
        return a;
    }
} 
    //TODO: add karatsuba
    static vector<integer> multiply_schoolbook(const vector<integer> &first, const vector<integer> &b) {
        vector<integer> a = first;
        next_type<integer> base = (next_type<integer>)1 << (sizeof(integer) * CHAR_BIT)
        vector<integer> result(a.size() + b.size(), 0);
        for (int i = 0; i < a.size(); ++i) {
            next_type<integer> carry = 0;
            for (int j = 0; j < b.size() || carry > 0; ++j) {
                next_type<integer> current = result[i + j] + carry + (next_type<integer>)a[i] * (j < b.size() ? b[j] : 0);
                if (i + j >= result.size()) {
                    for (int k = result.size(); k <= i + j; ++k) {
                        result.push_back(0);
                    }
                }
                result[i + j] = current % base;
                carry = current / base;
            }
            if (carry > 0) {
                if (i + b.size() >= result.size()) {
                    for (int k = result.size(); k <= i + b.size(); ++k) {
                        result.push_back(0);
                    }
                }
                result[i + b.size()] += carry;
            }
        }
        return result;
    }
// 2nd vec is remainder
    static pair<vector<integer>, vector<integer>> divide_schoolbook(const vector<integer> &first, const vector<integer> &b) {
    vector<integer> a = first;
    next_type<integer> base = (next_type<integer>)1 << (sizeof(integer) * CHAR_BIT)
    vector<integer> result(a.size(), 0);
    vector<integer> temp;
        temp.push_back(a[i]);

    for (int i = a.size() - 1; i >= 0; --i) {
        next_type<integer> current = 0;
        if (temp.size() >= b.size() && compare(temp, b)) {
            //binary search on trial division
            //this can be optimized if we set leading bit to 1 (proved by kntuh)
            // but too lazy
            next_type<integer> left = 0;
            next_type<integer> right = base - 1;
            while (left <= right) {
                next_type<integer> mid = left + (right - left) / 2;
                vector<integer> midVec = convert_base10(mid);
                vector<integer> product = multiply_schoolbook(b, midVec);
                if (compare(product, temp)) {
                    right = mid - 1;
                } else {
                    current = mid;
                    left = mid + 1;
                }

            }
            
            vector<integer> currentVec = convert_base10(current);
            temp = subtract(temp, multiply_schoolbook(b, currentVec));
            result.push_back(current);
        
            
    }
    return {result, temp};
}
    static vector<integer> multiply_karatsuba(const vector<integer> &first, const vector<integer> &b) {
    vector<integer> result{first.size() + b.size(), 0};
    next_type<integer> base = (next_type<integer>)1 << (sizeof(integer) * CHAR_BIT)
    bool result = true;

        if (a.size() == 0 || b.size() == 0) {
        return {0};
    } else if (a.size() == 1 || b.size() == 1) {
        return multiply_schoolbook(a, b);
    } else if (a.size() < b.size()) {
        for (int i = 0; i < b.size() - a.size(); ++i) {
            a.push_back(0);
        }
    } else if (b.size() < a.size()) {
        for (int i = 0; i < a.size() - b.size(); ++i) {
            b.push_back(0);
        }
    }
    
    vector<integer> aLow(a.begin(), a.begin() + a.size() / 2);
    vector<integer> aHigh(a.begin() + a.size() / 2, a.end());
    vector<integer> bLow(b.begin(), b.begin() + b.size() / 2);
    vector<integer> bHigh(b.begin() + b.size() / 2, b.end());
    vector<integer> z0 = multiply_karatsuba(aLow, bLow);
    vector<integer> z1 = multiply_karatsuba(add(aLow, aHigh), add(bLow, bHigh));
    vector<integer> z2 = multiply_karatsuba(aHigh, bHigh);
    vector<integer> adbc = subtract(subtract(z1, z0), z2);
    int split = (a.size() / 2);
    for (int i = 0; i < split; ++i) {
        result[i] = z0[i];
    }
    for (int i = 0; i < z1.size(); ++i) {
        result[i + split] += adbc[i];
    }
    for (int i = 0; i < z2.size(); ++i) {
        result[i + 2 * split] += z2[i];
    }
    return result;
}
    static vector<integer> divide_newton(const vector<integer> &first, const vector<integer> &b) {
    
    }
    public:
    BigInt() {
        negative = false;
        numbers = {0};
    }
    TEMPLATE_NUMBER
    BigInt(number num) {
    
    if (number < 0) {
        negative = true;
        number *= -1;
    }
    next_type<number> base = (next_type<number>)1 << (sizeof(number) * CHAR_BIT)
    numbers = convert_base10(number);
    }
    TEMPLATE_NUMBER
    BigInt operator+(number num) {
        vector<integer> other = convert_base10(num);
        if ((num < 0) ^ negative ) {
            // use subtraction, if other number magntiude > this, inverse order and negative
            bool compareResult = compare(numbers, other);
            if (compareResult) {
                vector<integer> result = subtract(other, numbers);
                return BigInt(result, true);
            }
            vector<integer> result = subtract(numbers, other);
            return BigInt(result, false);
        }
        if (num < 0 && negative) {
            // add magnitudes, inverse sign
            return BigInt(add(numbers, other), true);
        }
        return BigInt(add(numbers, other), false);
        
        
        
    }
    BigInt operator-(number num) {
        vector<integer> other = convert_base10(num);
        return *this + (-num);
    }
    BigInt operator+=(number num) {
        *this = *this + num;
        return *this;
    }
    BigInt operator-=(number num) {
        *this = *this - num;
        return *this;
    }
    BigInt operator<(number num) {
        vector<integer> other = convert_base10(num);
        if (negative && num >= 0) {
            return true;
        }
        if (!negative && num < 0) {
            return false;
        }
        bool compareResult = compare(numbers, other);
        if (negative && num < 0) {
            return !compareResult;
        }
        return compareResult;
    }
    BigInt operator==(number num) {
        vector<integer> other = convert_base10(num);
        if (negative != (num < 0)) {
            return false;
        }
        return !compare(numbers, other) && !compare(other, numbers);
    }
    BigInt operator>(number num) {
        return !(*this < num) && !(*this == num);
    }
    BigInt operator<=(number num) {
        return (*this < num) || (*this == num);
    }
    BigInt operator>=(number num) {
        return !(*this < num);
    }
    BigInt operator!=(number num) {
        return !(*this == num);
    }
    BigInt operator-() {
        return BigInt(numbers, !negative);
    }
    BigInt operator+() { // just in case
        return *this;
    }
    BigInt operator++() {
        *this += 1;
        return *this;
    }
    BigInt operator--() {
        *this -= 1;
        return *this;
    }

    BigInt operator++(int) {
        BigInt temp = *this;
        *this += 1;
        return temp;
    }
    BigInt operator--(int) {
        BigInt temp = *this;
        *this -= 1;
        return temp;
    }
    
    BigInt operator*
}
#endif