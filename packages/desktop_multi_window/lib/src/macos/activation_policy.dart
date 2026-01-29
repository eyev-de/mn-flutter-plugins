/// Defines macOS application activation policies.
/// These values determine how the app appears in the Dock and app switcher.
enum MacOsActivationPolicy {
  /// Regular app with Dock icon, menu bar, and Cmd+Tab presence.
  regular(0),

  /// Accessory app without Dock icon but with Cmd+Tab presence.
  accessory(1),

  /// Prohibited - app doesn't appear in Dock or app switcher.
  prohibited(2);

  final int value;
  const MacOsActivationPolicy(this.value);
}