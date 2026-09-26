#pragma once

#include <concepts>

namespace Platform::Interfaces {
  /// <summary>
  /// <para>Requires link storage with counting, enumeration, creation, update and deletion operations.</para>
  /// <para>Требует хранилище связей с подсчётом, перебором, созданием, изменением и удалением.</para>
  /// </summary>
  /// <typeparam name="TSelf">
  /// <para>The type checked by this concept or described by this helper.</para>
  /// <para>Тип, проверяемый этим концептом или описываемый этим вспомогательным типом.</para>
  /// </typeparam>
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
