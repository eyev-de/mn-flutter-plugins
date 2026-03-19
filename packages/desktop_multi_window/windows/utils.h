

#ifndef DESKTOP_MULTI_WINDOW_UTILS_H
#define DESKTOP_MULTI_WINDOW_UTILS_H

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

// Safe value extraction helpers.
// Using std::get<T> with _HAS_EXCEPTIONS=0 (Flutter default) calls abort()
// on type mismatch. These helpers return a default value instead.

inline int64_t GetIntegerValue(const flutter::EncodableValue& value, int64_t fallback = 0) {
    if (auto* p = std::get_if<int32_t>(&value)) return *p;
    if (auto* p = std::get_if<int64_t>(&value)) return *p;
    return fallback;
}

inline int32_t GetInt32Value(const flutter::EncodableValue& value, int32_t fallback = 0) {
    if (auto* p = std::get_if<int32_t>(&value)) return *p;
    if (auto* p = std::get_if<int64_t>(&value)) return static_cast<int32_t>(*p);
    return fallback;
}

inline bool GetBoolValue(const flutter::EncodableValue& value, bool fallback = false) {
    if (auto* p = std::get_if<bool>(&value)) return *p;
    return fallback;
}

inline double GetDoubleValue(const flutter::EncodableValue& value, double fallback = 0.0) {
    if (auto* p = std::get_if<double>(&value)) return *p;
    if (auto* p = std::get_if<int32_t>(&value)) return static_cast<double>(*p);
    if (auto* p = std::get_if<int64_t>(&value)) return static_cast<double>(*p);
    return fallback;
}

inline std::string GetStringValue(const flutter::EncodableValue& value, const std::string& fallback = "") {
    if (auto* p = std::get_if<std::string>(&value)) return *p;
    return fallback;
}

inline const flutter::EncodableValue* ValueOrNull(const flutter::EncodableMap& map, const char* key) {
    auto it = map.find(flutter::EncodableValue(key));
    if (it == map.end()) {
        return nullptr;
    }
    return &(it->second);
}

// Safe .at() replacement - returns a static monostate value instead of throwing
inline const flutter::EncodableValue& SafeAt(const flutter::EncodableMap& map, const char* key) {
    static const flutter::EncodableValue kEmpty;
    auto it = map.find(flutter::EncodableValue(key));
    if (it == map.end()) {
        return kEmpty;
    }
    return it->second;
}

// Safe windowId extraction from arguments map
inline int64_t GetWindowId(const flutter::EncodableMap& map) {
    return GetIntegerValue(SafeAt(map, "windowId"));
}

inline void PrintEncodableValue(const flutter::EncodableValue& value, int indent = 0) {
    std::string indentStr(indent, ' ');
    std::visit([&](auto&& arg)
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::nullptr_t>) {
            std::cout << indentStr << "null";
        } else if constexpr (std::is_same_v<T, bool>) {
            std::cout << indentStr << (arg ? "true" : "false");
        } else if constexpr (std::is_same_v<T, int32_t>) {
            std::cout << indentStr << arg;
        } else if constexpr (std::is_same_v<T, int64_t>) {
            std::cout << indentStr << arg;
        } else if constexpr (std::is_same_v<T, double>) {
            std::cout << indentStr << arg;
        } else if constexpr (std::is_same_v<T, std::string>) {
            std::cout << indentStr << "\"" << arg << "\"";
        } else if constexpr (std::is_same_v<T, flutter::EncodableList>) {
            std::cout << indentStr << "[\n";
            for (const auto& elem : arg) {
                PrintEncodableValue(elem, indent + 2);
                std::cout << "\n";
            }
            std::cout << indentStr << "]";
        } else if constexpr (std::is_same_v<T, flutter::EncodableMap>) {
            std::cout << indentStr << "{\n";
            for (const auto& pair : arg) {
                PrintEncodableValue(pair.first, indent + 2);
                std::cout << ": ";
                PrintEncodableValue(pair.second, indent + 2);
                std::cout << "\n";
            }
            std::cout << indentStr << "}";
        } else {
            std::cout << indentStr << "Unknown type";
        } }, value);
}

#endif // DESKTOP_MULTI_WINDOW_UTILS_H
