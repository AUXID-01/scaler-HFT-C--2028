namespace utils {

template <typename T> struct Remove_Ref { using type = T; };
template <typename T> struct Remove_Ref<T &> { using type = T; };
template <typename T> struct Remove_Ref<T &&> { using type = T; };

template <typename T> T &&Forward(typename Remove_Ref<T>::type &obj) {

  return static_cast<T &&>(obj);
}
} // namespace utils
