#include <bits/stdc++.h>
using namespace std;
#define DEBUG
#define FOR(I, A, N) for (int I = A; I < N; ++I)
#define RFOR(I, A, N) for (int I = A; I >= N; --I)
#define qd(h) cout << #h << " = " << h << "\n";
namespace stlarp {
    template<typename... Args>
    void debug(Args&&... args) {
        #ifndef DEBUG 
        return; 
        #endif
        ((cout << args << " "), ...);
        cout << "\n";
    }
    int binarySearch(int minVal, int maxVal, function<bool(int)> condition) {
        int low = minVal, high = maxVal;   
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (condition(mid)) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return high;
    }
    vector<int> pre(const vector<int>& arr) {
        vector<int> prefix(arr.size() + 1, 0);
        for (size_t i = 0; i < arr.size(); ++i) {
            prefix[i + 1] = prefix[i] + arr[i];
        }
        return prefix;
    }
    template<typename T, typename LazyT, typename MergeFunc, typename UpdateFunc, typename LazyMergeFunc>
    class SegmentTree {
    public:
        int n;
        vector<T> tree;
        vector<LazyT> lazy;
        vector<bool> hasLazy;
        T identity;
        LazyT lazyIdentity;
        MergeFunc merge;
        UpdateFunc applyUpdate;
        LazyMergeFunc mergeLazy;
        SegmentTree(const vector<T>& arr, T identity, LazyT lazyIdentity, MergeFunc merge, UpdateFunc applyUpdate, LazyMergeFunc mergeLazy)
            : n(arr.size()), identity(identity), lazyIdentity(lazyIdentity), merge(merge), applyUpdate(applyUpdate), mergeLazy(mergeLazy) {
            tree.resize(4 * n);
            lazy.resize(4 * n, lazyIdentity);
            hasLazy.resize(4 * n, false);
            build(arr, 1, 0, n - 1);
        }
        void build(const vector<T>& arr, int node, int left, int right) {
            if (left == right) {
                tree[node] = arr[left];
                return;
            }
            int mid = left + (right - left) / 2;
            build(arr, node * 2, left, mid);
            build(arr, node * 2 + 1, mid + 1, right);
            tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
        }
        void push(int node, int left, int right) {
            if (hasLazy[node]) {
                tree[node] = applyUpdate(tree[node], lazy[node], left, right);
                if (left != right) {
                    lazy[node * 2] = mergeLazy(lazy[node * 2], lazy[node]);
                    hasLazy[node * 2] = true;
                    lazy[node * 2 + 1] = mergeLazy(lazy[node * 2 + 1], lazy[node]);
                    hasLazy[node * 2 + 1] = true;
                }
                lazy[node] = lazyIdentity;
                hasLazy[node] = false;
            }
        }
        void update(int ql, int qr, LazyT val) {
            update(1, 0, n - 1, ql, qr, val);
        }
        void update(int node, int left, int right, int ql, int qr, LazyT val) {
            push(node, left, right);
            if (ql > right || qr < left) return;
            if (ql <= left && right <= qr) {
                lazy[node] = mergeLazy(lazy[node], val);
                hasLazy[node] = true;
                push(node, left, right);
                return;
            }
            int mid = left + (right - left) / 2;
            update(node * 2, left, mid, ql, qr, val);
            update(node * 2 + 1, mid + 1, right, ql, qr, val);
            tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
        }
        T query(int ql, int qr) {
            return query(1, 0, n - 1, ql, qr);
        }
        T query(int node, int left, int right, int ql, int qr) {
            push(node, left, right);
            if (ql > right || qr < left) return identity;
            if (ql <= left && right <= qr) return tree[node];
            int mid = left + (right - left) / 2;
            return merge(query(node * 2, left, mid, ql, qr), query(node * 2 + 1, mid + 1, right, ql, qr));
        }
    };
    vector<int64_t> factorials = {1};
    void precomputeFactorials(int64_t count, int64_t MOD = 1e9+7) {
        factorials.resize(count + 1);
        factorials[0] = 1;
        for (int i = 1; i <= count; ++i) {
            factorials[i] = (factorials[i-1] * i) % MOD;
        }
    }
    int64_t binaryExponentiate(int64_t base, int64_t exp, int64_t MOD = 1e9+7) {
        int64_t curr = base % MOD;
        int64_t cum = 1;
        while (exp > 0) {
            if (exp % 2 == 1) {
                cum = (cum * curr) % MOD;
            }
            curr = (curr * curr) % MOD;
            exp /= 2;
        }
        return cum;
    }
    int interpolationSearch(int low, int high, function<int64_t(int)> func, bool minimize) {
        if (low >= high) {
            return low;
        }
        auto getGradient = [&](int x) -> int64_t {
            return func(x + 1) - func(x);
        };
        int left = low;
        int right = high - 1;
        while (left < right) {
            int64_t gradientLeft = getGradient(left);
            int64_t gradientRight = getGradient(right);
            int position;
            if ((gradientLeft < 0 && gradientRight > 0) || (gradientLeft > 0 && gradientRight < 0)) {
                double fraction = -static_cast<double>(gradientLeft) / (gradientRight - gradientLeft);
                position = left + static_cast<int>(round(fraction * (right - left)));
                if (position < left) {
                    position = left;
                }
                if (position > right) {
                    position = right;
                }
            } else {
                position = left + (right - left) / 2;
            }
            int64_t gradientMid = getGradient(position);
            if (gradientMid == 0) {
                left = position;
                break;
            }
            if (minimize) {
                if (gradientMid < 0) {
                    left = position + 1;
                } else {
                    right = position - 1;
                }
            } else {
                if (gradientMid > 0) {
                    left = position + 1;
                } else {
                    right = position - 1;
                }
            }
        }
        if (minimize) {
            return (func(left) <= func(left + 1)) ? left : left + 1;
        } else {
            return (func(left) >= func(left + 1)) ? left : left + 1;
        }
    }
    int ternarySearch(int low, int high, function<int64_t(int)> func, bool minimize) {
        int left = low;
        int right = high;
        while (right - left >= 3) {
            int mid1 = left + (right - left) / 3;
            int mid2 = right - (right - left) / 3;
            if (minimize) {
                if (func(mid1) < func(mid2)) {
                    right = mid2;
                } else {
                    left = mid1;
                }
            } else {
                if (func(mid1) > func(mid2)) {
                    right = mid2;
                } else {
                    left = mid1;
                }
            }
        }
        int bestIndex = left;
        for (int i = left + 1; i <= right; ++i) {
            if (minimize) {
                if (func(i) < func(bestIndex)) {
                    bestIndex = i;
                }
            } else {
                if (func(i) > func(bestIndex)) {
                    bestIndex = i;
                }
            }
        }
        return bestIndex;
    }
    class ModInt {
    public:
        int64_t value;
        int64_t MOD = 1e9 + 7;
        ModInt() : value(0) {}
        ModInt(int64_t val, int64_t mod = 1e9 + 7) {
            MOD = mod;
            value = val % MOD;
            if (value < 0) {
                value += MOD;
            }
        }
        ModInt pow(int64_t exponent) const {
            int64_t base = value;
            int64_t result = 1;
            while (exponent > 0) {
                if (exponent % 2 == 1) {
                    result = (result * base) % MOD;
                }
                base = (base * base) % MOD;
                exponent /= 2;
            }
            return ModInt(result, MOD);
        }
        ModInt inverse() const {
            return pow(MOD - 2);
        }
        ModInt& operator+=(const ModInt& other) {
            value += other.value;
            if (value >= MOD) {
                value -= MOD;
            }
            return *this;
        }
        ModInt& operator-=(const ModInt& other) {
            value -= other.value;
            if (value < 0) {
                value += MOD;
            }
            return *this;
        }
        ModInt& operator*=(const ModInt& other) {
            value = (value * other.value) % MOD;
            return *this;
        }
        ModInt& operator/=(const ModInt& other) {
            return *this *= other.inverse();
        }
        friend ModInt operator+(ModInt lhs, const ModInt& rhs) {
            lhs += rhs;
            return lhs;
        }
        friend ModInt operator-(ModInt lhs, const ModInt& rhs) {
            lhs -= rhs;
            return lhs;
        }
        friend ModInt operator*(ModInt lhs, const ModInt& rhs) {
            lhs *= rhs;
            return lhs;
        }
        friend ModInt operator/(ModInt lhs, const ModInt& rhs) {
            lhs /= rhs;
            return lhs;
        }
        bool operator==(const ModInt& other) const {
            return value == other.value;
        }
        bool operator!=(const ModInt& other) const {
            return value != other.value;
        }
        friend ostream& operator<<(ostream& os, const ModInt& modInt) {
            return os << modInt.value;
        }
        friend istream& operator>>(istream& is, ModInt& modInt) {
            int64_t inputVal;
            is >> inputVal;
            modInt = ModInt(inputVal, modInt.MOD);
            return is;
        }
    };
};
using namespace stlarp;
int main() {
	int a, b, c; cin >> a >> b >> c;
    debug("Sum =", a + b + c);
    qd(a+b+c)
}

/* 
 remember: test on small n
 don't make unnecessary assumptions
 if the problem is hard, prove and counterexample before coding
 CAN YOU BINARY SEARCH ON ANSWER??
 */