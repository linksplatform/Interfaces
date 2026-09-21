#pragma once

#include <concepts>

namespace Platform::Interfaces {
  template <typename TSelf>
  concept CLinks = requires {
    typename TSelf::OptionsType;
    typename TSelf::LinkAddressType;
    typename TSelf::LinkType;
    typename TSelf::WriteHandlerType;
    typename TSelf::ReadHandlerType;
  } && requires(TSelf self, const typename TSelf::LinkType& restriction, const typename TSelf::LinkType& substitution, const typename TSelf::ReadHandlerType& readHandler, const typename TSelf::WriteHandlerType& writeHandler) {
    { self.Constants } -> std::convertible_to<const auto&>;
    { self.Count(restriction) } -> std::same_as<typename TSelf::LinkAddressType>;
    { self.Each(restriction, readHandler) } -> std::same_as<typename TSelf::LinkAddressType>;
    { self.Create(restriction, writeHandler) } -> std::same_as<typename TSelf::LinkAddressType>;
    { self.Update(restriction, substitution, writeHandler) } -> std::same_as<typename TSelf::LinkAddressType>;
    { self.Delete(restriction, writeHandler) } -> std::same_as<typename TSelf::LinkAddressType>;
  };
}  // namespace Platform::Interfaces