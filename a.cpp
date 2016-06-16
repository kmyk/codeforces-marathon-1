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
    vector<vector<bool> > yss;
    repeat (i,(n+l-1)/l) {
        yss.push_back(vector<bool>());
        repeat_from (j,i*l,min(n,i*l+l)) yss.back().push_back(xs[j]);
    }
    assert (concat(yss) == xs);
    return yss;
}
vector<bool> flip(vector<bool> const & xs) {
    int n = xs.size();
    vector<bool> ys(n);
    repeat (i,n) ys[i] = not xs[i];
    return ys;
}
vector<bool> select_best(map<vector<bool>,int> const & history) {
    int score = -1;
    vector<bool> xs;
    for (auto it : history) {
        vector<bool> ys; int yscore; tie(ys, yscore) = it;
        if (score < yscore) {
            score = yscore;
            xs = ys;
        }
    }
    return xs;
}
vector<bool> apply_decided(map<int,bool> const & decided, vector<bool> const & xs) {
    vector<bool> ys = xs;
    for (auto it : decided) {
        int i; bool y; tie(i, y) = it;
        ys[i] = y;
    }
    return ys;
}

const int N = 5000;
const int K = 2000;
const int X = 100;
const int ES = 2*K;

string visualize_decided(map<int,bool> const & decided) {
    string s(N, '?');
    for (auto it : decided) s[it.first] = it.second ? '1' : '0';
    return s;
}
int flipped_score(int score) {
    return ES + (ES - score);
}
int main() {
    vector<vector<bool> > history;
    map<vector<bool>,int> scores;
    map<vector<bool>,int> estimated;
    map<int,bool> decided;
    int query_count = 0;
    auto query = [&](vector<bool> const & xs) {
        query_count += 1;
        cout << decode(xs) << endl;
        cout.flush();
        int score; cin >> score;
        history.push_back(xs);
        scores[xs] = score;
        estimated[xs] = score;
        estimated[flip(xs)] = flipped_score(score);
        decided[score-1] = not xs[score-1];
        return score;
    };
    auto query_log = [&](function<void ()> f) {
        vector<bool> xs = history.back();
        cerr << "query " << query_count << ":\t\"" << to_hashed(xs, 250) << "\" -> " << scores[xs] << " ";
        f();
        cerr << endl;
        cerr.flush();
    };
    int flip_count = 0;
    const int l = 46;
    const int x0 = 1;
    while (query_count < X) {
        if (query_count < x0) {
            vector<bool> xs = random_binary(N);
            xs = apply_decided(decided, xs);
            query(xs);
            query_log([&]() { });
            if (query_count == x0) {
                vector<bool> xs = select_best(estimated);
                cerr << "resemara done:\t\"" << to_hashed(xs, 250) << "\" -> " << estimated[xs] << (scores.count(xs) ? " (decided)" : " (estimated)") << endl;
            }
        } else {
            vector<bool> xs = select_best(estimated);
            vector<bool> ys; {
                int j = query_count - x0;
                vector<vector<bool> > yss = unconcat(xs, l);
                yss[j] = flip(yss[j]);
                ys = concat(yss);
            }
            ys = apply_decided(decided, ys);
            query(ys);
            bool should_flip = estimated[xs] < estimated[ys];
            if (should_flip) flip_count += 1;
            query_log([&]() { cerr << (should_flip ? "(flipped)" : ""); });
        }
    }
    assert (decided.size() == X or scores[select_best(scores)] == N);
    cerr << "flipping has occured " << flip_count << " times" << endl;
    cerr << visualize_decided(decided) << endl;
    return 0;
}
