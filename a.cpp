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
using namespace std;


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
vector<vector<bool> > unconcat(vector<bool> const & xs, int l) {
    int n = xs.size();
    assert (n % l == 0);
    vector<vector<bool> > yss(n/l, vector<bool>(l));
    repeat (i,n) yss[i/l][i%l] = xs[i];
    return yss;
}
vector<bool> flip(vector<bool> const & xs) {
    int n = xs.size();
    vector<bool> ys(n);
    repeat (i,n) ys[i] = not xs[i];
    return ys;
}
template <typename F>
vector<bool> select_best_with(map<vector<bool>,int> const & history, F f) {
    int score = -1;
    vector<bool> xs;
    for (auto it : history) {
        vector<bool> ys; int yscore; tie(ys, yscore) = f(it.first, it.second);
        if (score < yscore) {
            score = yscore;
            xs = ys;
        }
    }
    return xs;
}
vector<bool> select_best(map<vector<bool>,int> const & history) {
    return select_best_with(history, [](vector<bool> const & ys, int score) {
        return make_pair(ys, score);
    });
}

const int N = 5000;
const int K = 2000;
const int X = 100;
const int ES = N - 2*K;

int flipped_score(int score) {
    return ES + (ES - score);
}
vector<bool> select_flipped_best(map<vector<bool>,int> const & history) {
    return select_best_with(history, [](vector<bool> ys, int score) {
        ys = flip(ys);
        score = flipped_score(score);
        return make_pair(ys, score);
    });
}

int main() {
    map<vector<bool>,int> history;
    int query_count = 0;
    auto query = [&](vector<bool> const & xs) {
        query_count += 1;
        cout << decode(xs) << endl;
        cout.flush();
        int score; cin >> score;
        cerr << "query " << query_count << ":\t\"" << to_hashed(xs, 250) << "\" -> " << score << endl;
        cerr.flush();
        history[xs] = score;
        return score;
    };
    const int l = 200;
    const int x0 = 10;
    vector<vector<bool> > xss(N/l);
    while (query_count < X) {
        if (query_count < x0) {
            repeat (i,N/l) xss[i] = random_binary(l);
            query(concat(xss));
            if (query_count == x0) {
                vector<bool> ys = select_best(history);
                vector<bool> zs = select_flipped_best(history);
                if (history[ys] < flipped_score(history[zs])) {
                    query(flip(zs));
                }
                vector<bool> xs = select_best(history);
                cerr << "gacha done:\t\"" << to_hashed(xs, 250) << "\" -> " << history[xs] << endl;
                xss = unconcat(xs, l);
            }
        } else {
            int j = (query_count - x0) / (X/(N/l));
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
