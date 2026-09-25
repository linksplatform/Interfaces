#include <Platform.Interfaces.h>
#include <gtest/gtest.h>

namespace Platform::Interfaces::Tests {
  namespace {
    struct ReadonlyListModel {
      int items[1]{};

      const int* begin() const { return items; }
      const int* end() const { return items + 1; }
      const int& operator[](std::size_t index) const { return items[index]; }
      std::size_t size() const { return static_cast<std::size_t>(end() - begin()); }
      bool empty() const { return begin() == end(); }
    };

    struct ReadonlySetModel {
      int items[1]{};

      const int* begin() const { return items; }
      const int* end() const { return items + 1; }
      const int* find(int item) const { return items[0] == item ? begin() : end(); }
      bool contains(int item) const { return find(item) != end(); }
      std::size_t size() const { return static_cast<std::size_t>(end() - begin()); }
      bool empty() const { return begin() == end(); }
    };

    struct ReadonlyDictionaryModel {
      std::tuple<int, int> items[1]{};

      const std::tuple<int, int>* begin() const { return items; }
      const std::tuple<int, int>* end() const { return items + 1; }
      const std::tuple<int, int>* find(int key) const { return std::get<0>(items[0]) == key ? begin() : end(); }
      bool contains(int key) const { return find(key) != end(); }
      std::size_t size() const { return static_cast<std::size_t>(end() - begin()); }
      bool empty() const { return begin() == end(); }
    };
  }  // namespace

  static_assert(CReadonlyList<const ReadonlyListModel>);
  static_assert(CReadonlyList<const ReadonlyListModel, int>);
  static_assert(!CList<const ReadonlyListModel>);
  static_assert(std::same_as<typename ReadonlyList<const ReadonlyListModel>::Item, int>);

  static_assert(CReadonlySet<const ReadonlySetModel>);
  static_assert(CReadonlySet<const ReadonlySetModel, int>);
  static_assert(!CSet<const ReadonlySetModel>);
  static_assert(std::same_as<typename ReadonlySet<const ReadonlySetModel>::Item, int>);

  static_assert(CReadonlyDictionary<const ReadonlyDictionaryModel>);
  static_assert(CReadonlyDictionary<const ReadonlyDictionaryModel, int>);
  static_assert(CReadonlyDictionary<const ReadonlyDictionaryModel, int, int>);
  static_assert(!CDictionary<const ReadonlyDictionaryModel>);
  static_assert(std::same_as<std::remove_reference_t<typename ReadonlyDictionary<const ReadonlyDictionaryModel>::Key>, int>);
  static_assert(std::same_as<std::remove_reference_t<typename ReadonlyDictionary<const ReadonlyDictionaryModel>::Value>, int>);

  TEST(CompileTests, Counter) {
    struct EmptyCounter1 : ICounter<int> {
      int Count() { return {}; }
    };
    static_assert(CCounter<EmptyCounter1, int>);

    struct EmptyCounter2 : public ICounter<int, int> {
      int Count(int) { return {}; }
    };
    static_assert(CCounter<EmptyCounter2, int, int>);

    {
      CCounter<int> auto counter = EmptyCounter1{};

      ASSERT_TRUE((CCounter<EmptyCounter1, int>));
      ASSERT_FALSE((CCounter<EmptyCounter1, float>));
    }

    {
      CCounter<int, int> auto counter = EmptyCounter2{};

      ASSERT_TRUE((CCounter<EmptyCounter2, int, int>));
      ASSERT_TRUE((CCounter<EmptyCounter2, int, float>));
      ASSERT_FALSE((CCounter<EmptyCounter2, float, int>));
      ASSERT_FALSE((CCounter<EmptyCounter2, float, float>));
    }
  }

  TEST(CompileTests, CriterionMatcher) {
    struct EmptyCriterionMatcher : public ICriterionMatcher<int> {
      bool IsMatched(int) { return {}; }
    };
    static_assert(CCriterionMatcher<EmptyCriterionMatcher, int>);

    {
      CCriterionMatcher<int> auto criterionMatcher = EmptyCriterionMatcher{};

      ASSERT_TRUE((CCriterionMatcher<EmptyCriterionMatcher, int>));
      ASSERT_TRUE((CCriterionMatcher<EmptyCriterionMatcher, float>));
    }
  }

  TEST(CompileTests, Factory) {
    struct EmptyIntFactory : public IFactory<int> {
      int Create() { return {}; }
    };
    static_assert(CFactory<EmptyIntFactory, int>);

    {
      CFactory<int> auto factory = EmptyIntFactory{};

      ASSERT_TRUE((CFactory<EmptyIntFactory, int>));
      ASSERT_FALSE((CFactory<EmptyIntFactory, float>));
    }
  }

  TEST(CompileTests, Properties) {
    struct EmptyProperties : public IProperties<int, int, int> {
      void SetValue(int, int, int) {}

      int GetValue(int, int) { return {}; }
    };
    static_assert(CProperties<EmptyProperties, int, int, int>);

    {
      CProperties<int, int, int> auto properties = EmptyProperties{};

      ASSERT_TRUE((CProperties<EmptyProperties, int, int, int>));
      ASSERT_TRUE((CProperties<EmptyProperties, int&, int, int>));
      ASSERT_TRUE((CProperties<EmptyProperties, const int&, int, int>));
    }
  }

  TEST(CompileTests, Provider) {
    struct EmptyProvider1 : public IProvider<int> {
      int Get() { return {}; }
    };
    static_assert(CProvider<EmptyProvider1, int>);

    struct EmptyProvider2 : public IProvider<int, int> {
      int Get(int) { return {}; }
    };
    static_assert(CProvider<EmptyProvider2, int, int>);

    {
      CProvider<int> auto provider = EmptyProvider1{};

      ASSERT_TRUE((CProvider<EmptyProvider1, int>));
    }

    {
      CProvider<int, int> auto provider = EmptyProvider2{};

      ASSERT_TRUE((CProvider<EmptyProvider2, int, int>));
      ASSERT_TRUE((CProvider<EmptyProvider2, int, float>));
    }
  }

  TEST(CompileTests, Setter) {
    struct EmptySetter1 : public ISetter<int> {
      void Set(int) {}
    };
    static_assert(CSetter<EmptySetter1, int>);

    struct EmptySetter2 : public ISetter<int, int> {
      void Set(int, int) {}
    };
    static_assert(CSetter<EmptySetter2, int, int>);

    {
      CSetter<int> auto provider = EmptySetter1{};

      ASSERT_TRUE((CSetter<EmptySetter1, int>));
    }

    {
      CSetter<int, int> auto provider = EmptySetter2{};

      ASSERT_TRUE((CSetter<EmptySetter2, int, int>));
      ASSERT_TRUE((CSetter<EmptySetter2, int, float>));
    }
  }

  TEST(CompileTests, Property) {
    struct EmptyProperty : public IProperty<int&, int> {
      void Set(int&, int) {}
      int Get(int&) { return {}; }
    };
    static_assert(CProperty<EmptyProperty, int&, int>);

    {
      CProperty<int&, int> auto provider = EmptyProperty{};

      ASSERT_TRUE((CProperty<EmptyProperty, int&, int>));
      ASSERT_TRUE((CProvider<EmptyProperty, int, int&>));
      ASSERT_TRUE((CSetter<EmptyProperty, int, int&>));
    }
  }

  TEST(CompileTests, Cli) {
    struct EmptyCli : public ICli {
      int Run(const std::vector<std::string>& args) override { return 0; }
    };
    static_assert(CCli<EmptyCli>);

    {
      CCli auto cli = EmptyCli{};

      ASSERT_TRUE((CCli<EmptyCli>));
    }
  }
}  // namespace Platform::Interfaces::Tests
