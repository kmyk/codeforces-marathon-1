#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <set>
#include <map>
#include <queue>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <random>
#include <cstdio>
#include <cassert>
#define repeat(i,n) for (int i = 0; (i) < (n); ++(i))
#define repeat_from(i,m,n) for (int i = (m); (i) < (n); ++(i))
#define repeat_reverse(i,n) for (int i = (n)-1; (i) >= 0; --(i))
#define repeat_from_reverse(i,m,n) for (int i = (n)-1; (i) >= (m); --(i))
#define whole(f,x,...) ([&](decltype((x)) y) { return (f)(begin(y), end(y), ## __VA_ARGS__); })(x)
#define dump(x)  cerr << #x << " = " << (x) << endl
#define debug(x) cerr << #x << " = " << (x) << " (L" << __LINE__ << ")" << " " << __FILE__ << endl
typedef long long ll;
template <class T> bool setmax(T & l, T const & r) { if (not (l < r)) return false; l = r; return true; }
template <class T> bool setmin(T & l, T const & r) { if (not (r < l)) return false; l = r; return true; }
template <typename F> auto compare_with(F const & f) { return [=](auto x, auto y) { return f(x) < f(y); }; }
using namespace std;
template <typename T, typename X> auto vectors(T a, X x) { return vector<T>(x, a); }
template <typename T, typename X, typename Y, typename... Zs> auto vectors(T a, X x, Y y, Zs... zs) { auto cont = vectors(a, y, zs...); return vector<decltype(cont)>(x, cont); }
template <class T> istream & operator >> (istream & in, vector<T> & a) { for (T & it : a) in >> it; return in; }
template <class T> ostream & operator << (ostream & out, vector<T> const & a) { bool i = false; for (T const & it : a) { if (i) out << ' '; else i = true; out << it; } return out; }


vector<bool> random_binary(int n) {
    static random_device device;
    static default_random_engine engine(device());
    uniform_int_distribution<int> dist(0, 1);
    vector<bool> xs(n); repeat (i,n) xs[i] = dist(engine);
    return xs;
}
string decode(vector<bool> const & xs) {
    int n = xs.size();
    string s(n, '\0');
    repeat (i,n) s[i] = '0' + xs[i];
    return s;
}
vector<bool> encode(string const & s) {
    int n = s.length();
    vector<bool> xs(n);
    repeat (i,n) xs[i] = s[i] - '0';
    return xs;
}
string to_hexadecimal(vector<bool> const & xs) {
    const char *table = "0123456789ABCDEF";
    int n = xs.size();
    assert (n % 4 == 0);
    string s;
    repeat (i,n/4) {
        int y = 0;
        repeat (j,4) {
            y <<= 1;
            if (xs[i*4+j]) y |= 1;
        }
        s += table[y];
    }
    return s;
}
string to_hashed(vector<bool> const & xs, int l) {
    int n = xs.size();
    string s = decode(xs);
    string t;
    repeat (i,n/l) {
        uint64_t y = hash<string>()(s.substr(i*l, l));
        char buf[8]; snprintf(buf, 8, "%8llx", y);
        t += buf;
    }
    return t;
}
vector<bool> concat(vector<vector<bool> > const & xss) {
    string s;
    for (auto xs : xss) s += decode(xs);
    return encode(s);
}
const int N = 5000;
const int K = 2000;
const int X = 100;
int main() {
    map<vector<bool>,int> history;
    auto query = [&](vector<bool> const & xs) {
        cout << decode(xs) << endl;
        cout.flush();
        int score; cin >> score;
        cerr << "\"" << to_hashed(xs, 250) << "\" -> " << score << endl;
        cerr.flush();
        history[xs] = score;
        return score;
    };
    const int l = 200;
    vector<vector<bool> > xss(N/l);
    repeat (i,N/l) xss[i] = random_binary(l);
    repeat (i,X) {
        if (i == 0) {
            query(concat(xss));
        } else {
            int j = i / (X/(N/l));
            vector<vector<bool> > yss = xss;
            yss[j] = random_binary(l);
            int score = query(concat(yss));
            if (history[concat(xss)] < score) {
                xss[j] = yss[j];
            }
        }
    }
    return 0;
}
