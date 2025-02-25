#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <queue>
#include <map>
#include <set>
#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>
#include <tuple>
#include <numeric>
#include <limits>

#define MULTI_TEST

template <typename T>
class SegmentTree {
public:
    // 修改标记类.
    class Tag {
    public:
        bool status;
        T lazy;

        // 无效 Tag 初始化, 作为默认构造函数.
        Tag() : status(false) {}

        // 有效 Tag 初始化, 根据需求具体实现.
        Tag(const T& value) : status(true), lazy(value) {}

        // 将有效的 Tag 应用到当前 Tag 上.
        void apply(const Tag& t) {
            if (status) {
                lazy = t.lazy;
            } else {
                *this = t;
            }
        }
    };

    // 结点信息类.
    class Info {
    public:
        long long l;
        long long r;
        T min;
        T max;
        T diff;

        // 无效 Info 初始化, 作为默认构造函数.
        Info() : l(-1), r(-1) {}

        // 有效 Info 单点初始化, 根据需求具体实现.
        Info(long long pos, const T& value) : l(pos), r(pos), min(value), max(value), diff(0) {}

        // 合并 2 个有效的 Info.
        friend Info operator+(const Info& lhs, const Info& rhs) {
            Info res;
            res.l = lhs.l;
            res.r = rhs.r;
            res.min = std::min(lhs.min, rhs.min);
            res.max = std::max(lhs.max, rhs.max);
            res.diff = std::max({lhs.diff, rhs.diff, rhs.max - lhs.min});
            return res;
        }

        // 将有效的 Tag 应用到当前有效的 Info 上.
        void apply(const Tag& t) {
            min = t.lazy;
            max = t.lazy;
            diff = 0;
        }
    };

    // 用 number 个 value 构造线段树.
    SegmentTree(long long number = 0, const T& value = T()) : n(number), info(4 * n, Info()), tag(4 * n, Tag()) {
        if (n >= 1) {
            build(0, 0, n - 1, value);
        }
    }

    // 用 std::vector<T> values 构造线段树.
    SegmentTree(const std::vector<T>& values) : n(values.size()), info(4 * n, Info()), tag(4 * n, Tag()) {
        if (n >= 1) {
            build(0, 0, n - 1, values);
        }
    }

    // 对区间 [ql, qr] 的信息按照修改标记 t 进行修改.
    void modify(long long ql, long long qr, const Tag& t) {
        if (ql < 0 || ql > n - 1 || qr < 0 || qr > n - 1 || ql > qr || !t.status) {
            return;
        }
        modify(0, ql, qr, t);
    }

    // 查询区间 [ql, qr] 的信息. 如果区间不合法返回无效 Info, 否则返回有效的 Info 表示结果.
    Info query(long long ql, long long qr) {
        if (ql < 0 || ql > n - 1 || qr < 0 || qr > n - 1 || ql > qr) {
            return Info();
        }
        return query(0, ql, qr);
    }

    // 查找区间 [ql, qr] 中第一个满足 p 的位置, 如果不存在返回 -1. 
    // p 是一个一元谓词, 传入一个 Info 返回一个 bool.
    // p 应满足 p(a + b) == p(a) || p(b).
    template <typename UnaryPred>
    long long find_first(long long ql, long long qr, UnaryPred p) {
        if (ql < 0 || ql > n - 1 || qr < 0 || qr > n - 1 || ql > qr) {
            return -1;
        }
        return find_first(0, ql, qr, p);
    }

    // 查找区间 [ql, qr] 中最后一个满足 p 的位置, 如果不存在返回 -1. 
    // p 是一个一元谓词, 传入一个 Info 返回一个 bool.
    // p 应满足 p(a + b) == p(a) || p(b).
    template <typename UnaryPred>
    long long find_last(long long ql, long long qr, UnaryPred p) {
        if (ql < 0 || ql > n - 1 || qr < 0 || qr > n - 1 || ql > qr) {
            return -1;
        }
        return find_last(0, ql, qr, p);
    }

    // 线段树的大小, 即元素个数.
    long long size() const {
        return n;
    }
    
private:
    long long n;
    std::vector<Info> info;
    std::vector<Tag> tag;

    void build(long long i, long long l, long long r, const T& value) {
        if (l == r) {
            info[i] = Info(l, value);
            return;
        }
        long long mid = (l + r) / 2;
        build(i * 2 + 1, l, mid, value);
        build(i * 2 + 2, mid + 1, r, value);
        info[i] = info[i * 2 + 1] + info[i * 2 + 2];
    }

    void build(long long i, long long l, long long r, const std::vector<T>& values) {
        if (l == r) {
            info[i] = Info(l, values[l]);
            return;
        }
        long long mid = (l + r) / 2;
        build(i * 2 + 1, l, mid, values);
        build(i * 2 + 2, mid + 1, r, values);
        info[i] = info[i * 2 + 1] + info[i * 2 + 2];
    }

    void push_down(long long i) {
        if (tag[i].status) {
            info[i * 2 + 1].apply(tag[i]);
            tag[i * 2 + 1].apply(tag[i]);
            info[i * 2 + 2].apply(tag[i]);
            tag[i * 2 + 2].apply(tag[i]);
            tag[i].status = false;
        }
    }

    void modify(long long i, long long ql, long long qr, const Tag& t) {
        long long l = info[i].l;
        long long r = info[i].r;
        if (ql <= l && r <= qr) {
            info[i].apply(t);
            tag[i].apply(t);
            return;
        }
        push_down(i);
        long long mid = (l + r) / 2;
        if (ql <= mid) {
            modify(i * 2 + 1, ql, qr, t);
        }
        if (qr >= mid + 1) {
            modify(i * 2 + 2, ql, qr, t);
        }
        info[i] = info[i * 2 + 1] + info[i * 2 + 2];
    }

    Info query(long long i, long long ql, long long qr) {
        long long l = info[i].l;
        long long r = info[i].r;
        if (ql <= l && r <= qr) {
            return info[i];
        }
        push_down(i);
        long long mid = (l + r) / 2;
        if (ql <= mid && qr >= mid + 1) {
            return query(i * 2 + 1, ql, qr) + query(i * 2 + 2, ql, qr);
        } else if (ql <= mid) {
            return query(i * 2 + 1, ql, qr);
        } else {
            // 此处 qr >= ql && ql >= mid + 1, 所以 qr >= mid + 1.
            return query(i * 2 + 2, ql, qr);
        }
    }

    template <typename UnaryPred>
    long long find_first(long long i, long long ql, long long qr, UnaryPred p) {
        long long l = info[i].l;
        long long r = info[i].r;
        if (ql <= l && r <= qr && !p(info[i])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        push_down(i);
        long long mid = (l + r) / 2;
        if (ql <= mid) {
            long long res = find_first(i * 2 + 1, ql, qr, p);
            if (res != -1) {
                return res;
            }
        }
        if (qr >= mid + 1) {
            long long res = find_first(i * 2 + 2, ql, qr, p);
            if (res != -1) {
                return res;
            }
        }
        return -1;
    }

    template <typename UnaryPred>
    long long find_last(long long i, long long ql, long long qr, UnaryPred p) {
        long long l = info[i].l;
        long long r = info[i].r;
        if (ql <= l && r <= qr && !p(info[i])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        push_down(i);
        long long mid = (l + r) / 2;
        if (qr >= mid + 1) {
            long long res = find_last(i * 2 + 2, ql, qr, p);
            if (res != -1) {
                return res;
            }
        }
        if (ql <= mid) {
            long long res = find_last(i * 2 + 1, ql, qr, p);
            if (res != -1) {
                return res;
            }
        }
        return -1;
    }
};

void solve() {
    int n, q;
    std::cin >> n >> q;
    std::vector<long long> a(n);
    std::vector<long long> a1(n);
    std::vector<long long> a2(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
        a1[i] = a[i] - i;
        a2[i] = -a[i] - i;
    }
    SegmentTree<long long> seg1(a1);
    SegmentTree<long long> seg2(a2);
    std::cout << std::max(seg1.query(0, n - 1).diff, seg2.query(0, n - 1).diff) << '\n';
    while (q--) {
        int p;
        long long x;
        std::cin >> p >> x;
        --p;
        seg1.modify(p, p, x - p);
        seg2.modify(p, p, -x - p);
        std::cout << std::max(seg1.query(0, n - 1).diff, seg2.query(0, n - 1).diff) << '\n';
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
#ifdef MULTI_TEST
    long long t;
    std::cin >> t;
    while (t--) {
        solve();
    }
#else
    solve();
#endif
    return 0;
}
