using size_t = decltype(sizeof(int));

#if !defined(CT_BENCHMARK_BASELINE_SIZE)
# define CT_BENCHMARK_BASELINE_SIZE 1024
#endif

namespace ebd {
namespace detail {

template <template <class _Tp, _Tp...> class _Base, class _Qu, class _Re>
struct __merge_seq;
template <template <class _Tp, _Tp...> class _Base, class _Tp, _Tp... _Qs, _Tp... _Rs>
struct __merge_seq<_Base, _Base<_Tp, _Qs...>, _Base<_Tp, _Rs...>> {
  static constexpr size_t QuotientSize = sizeof...(_Qs);
  using type = _Base<
      _Tp, 
      0 * QuotientSize + _Qs..., 1 * QuotientSize + _Qs..., 2 * QuotientSize + _Qs...,
      3 * QuotientSize + _Qs..., 4 * QuotientSize + _Qs..., 5 * QuotientSize + _Qs...,
      6 * QuotientSize + _Qs..., 7 * QuotientSize + _Qs..., 8 * QuotientSize + _Rs...>;
};

template <template <class _Tp, _Tp...> class _Base, class _Tp, size_t _Tgt, size_t _Cnt, _Tp... _Nx>
struct __make_small_seq {
  using type = typename __make_small_seq<_Base, _Tp, _Tgt, _Cnt + 1, _Nx..., _Cnt>::type;
};
template <template <class _Tp, _Tp...> class _Base, class _Tp, size_t _Tgt, _Tp... _Nx>
struct __make_small_seq<_Base, _Tp, _Tgt, _Tgt, _Nx...> {
  using type = _Base<_Tp, _Nx...>;
};

template <template <class _Tp, _Tp...> class _Base, class _Tp, size_t _Tgt, class _Seq, bool _IsSmall = (_Tgt < 8)>
struct __make_integer_seq_impl {
  using type = typename __merge_seq<
      _Base,
      typename __make_integer_seq_impl<_Base, _Tp, _Tgt / 8, _Seq>::type,
      typename __make_integer_seq_impl<_Base, _Tp, _Tgt % 8, _Seq>::type
    >::type;
};
template <template <class _Tp, _Tp...> class _Base, class _Tp, size_t _Tgt, class _Seq>
struct __make_integer_seq_impl<_Base, _Tp, _Tgt, _Seq, true> {
  using type = typename __make_small_seq<_Base, _Tp, _Tgt, 0>::type;
};

} // namespace detail


template <template <class _Tp, _Tp...> class _BaseType, class _Tp, _Tp _SequenceSize>
using __make_integer_sequence_impl = 
    typename detail::__make_integer_seq_impl<_BaseType, _Tp, _SequenceSize, _BaseType<_Tp>>::type;

template <class _Tp, _Tp... _Sequence>
struct integer_sequence {};

template <class _Tp, _Tp _SequenceSize>
using make_integer_sequence = __make_integer_sequence_impl<integer_sequence, _Tp, _SequenceSize>;

} // namespace ebd

int main() {
  ebd::make_integer_sequence<size_t, CT_BENCHMARK_BASELINE_SIZE> seq1;
  ebd::make_integer_sequence<int, CT_BENCHMARK_BASELINE_SIZE> seq2;
  ebd::make_integer_sequence<unsigned int, CT_BENCHMARK_BASELINE_SIZE> seq3;
  (void)seq1; (void)seq2; (void)seq3;
}

